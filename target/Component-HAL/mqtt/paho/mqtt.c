/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file mqtt.c  (mqtt/paho)
 * @brief Eclipse Paho MQTT async client HAL.
 *
 * Uses MQTTAsync (libpaho-mqtt3as — async + SSL) from libpaho-mqtt-c.
 * Available as a standard apt package on Ubuntu 22.04, Debian 12/13,
 * and Raspberry Pi OS — no ert-contrib-middleware build step required.
 *
 * Compatible with any standards-compliant MQTT 3.1.1 broker, including
 * AWS IoT Core (port 8883 mTLS), HiveMQ, Mosquitto, and Azure IoT Hub.
 *
 * AWS IoT Core configuration
 * --------------------------
 * In the mqtt_client function block set:
 *   Host        = <thing-id>.iot.<region>.amazonaws.com
 *   Port        = 8883
 *   TLS         = enabled
 *   CA cert     = AmazonRootCA1.pem  (filename relative to app data dir)
 *   Client cert = <thing>.cert.pem
 *   Client key  = <thing>.private.key
 *
 * Port 8883 uses standard TLS without ALPN and works with this HAL.
 * Port 443 (requiring ALPN "x-amzn-mqtt-ca") is not needed for device
 * connectivity and is not supported here.
 */

#include "globals.h"
#include "hal-api.h"
#include "hal_mqtt.h"

#include <MQTTAsync.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>

/* -------------------------------------------------------------------------
 * Internal thread state
 * ------------------------------------------------------------------------- */

typedef enum
{
    PAHO_STATE_INIT,          /* no client created, waiting for connect=true   */
    PAHO_STATE_CONNECTING,    /* MQTTAsync_connect() called, awaiting callback  */
    PAHO_STATE_CONNECTED,     /* broker acknowledged CONNACK                   */
    PAHO_STATE_DISCONNECTING, /* MQTTAsync_disconnect() called                 */
    PAHO_STATE_DISCONNECTED,  /* clean disconnect confirmed, will go to INIT   */
    PAHO_STATE_INTERRUPTED    /* connection lost, will trigger a reconnect     */
} PahoState_t;

/* -------------------------------------------------------------------------
 * Per-loop context
 * ------------------------------------------------------------------------- */

typedef struct
{
    MQTTAsync   client;
    PahoState_t state;
    pthread_mutex_t lock;
} PahoCtx_t;

/* -------------------------------------------------------------------------
 * Callbacks — called from Paho's internal network thread
 * ------------------------------------------------------------------------- */

static void on_connect_success(void *context, MQTTAsync_successData *response)
{
    (void)response;
    PahoCtx_t *ctx = (PahoCtx_t *)context;
    pthread_mutex_lock(&ctx->lock);
    ctx->state = PAHO_STATE_CONNECTED;
    EhsMQTTConnectEvent(EHS_TRUE);
    pthread_mutex_unlock(&ctx->lock);
}

static void on_connect_failure(void *context, MQTTAsync_failureData *response)
{
    PahoCtx_t *ctx = (PahoCtx_t *)context;
    ehs_char buf[128];
    EhsSprintf(buf, "Paho connect failed (rc=%d)", response ? response->code : -1);
    EhsMQTTReportError(buf);
    pthread_mutex_lock(&ctx->lock);
    ctx->state = PAHO_STATE_DISCONNECTED;
    pthread_mutex_unlock(&ctx->lock);
}

static void on_disconnect_complete(void *context, MQTTAsync_successData *response)
{
    (void)response;
    PahoCtx_t *ctx = (PahoCtx_t *)context;
    pthread_mutex_lock(&ctx->lock);
    ctx->state = PAHO_STATE_DISCONNECTED;
    EhsMQTTConnectEvent(EHS_FALSE);
    pthread_mutex_unlock(&ctx->lock);
}

static void on_connection_lost(void *context, char *cause)
{
    (void)cause;
    PahoCtx_t *ctx = (PahoCtx_t *)context;
    pthread_mutex_lock(&ctx->lock);
    ctx->state = PAHO_STATE_INTERRUPTED;
    EhsMQTTConnectEvent(EHS_FALSE);
    pthread_mutex_unlock(&ctx->lock);
}

static int on_message_arrived(void *context, char *topicName, int topicLen,
                               MQTTAsync_message *message)
{
    (void)context;
    /* topicLen == 0 means topicName is null-terminated */
    if (topicLen == 0) topicLen = (int)strlen(topicName);

    if (topicLen < EHS_STRING_LENGTH_MAX &&
        message->payloadlen < EHS_MQTT_SUBS_RECV_BUFF_MAX_LENGTH)
    {
        ehs_char topic[EHS_STRING_LENGTH_MAX];
        memcpy(topic, topicName, topicLen);
        topic[topicLen] = '\0';
        EhsMQTTSubscribeEvent(topic, (char *)message->payload, message->payloadlen);
    }

    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    return 1; /* 1 = message ownership taken */
}

/* -------------------------------------------------------------------------
 * Helper: build Paho server URI and create client
 * ------------------------------------------------------------------------- */

static int paho_create_client(PahoCtx_t *ctx,
                               const char *host, ehs_uint16 port,
                               ehs_bool tls, const char *clientid)
{
    ehs_char uri[EHS_STRING_LENGTH_MAX];
    EhsSprintf(uri, "%s://%s:%u", tls ? "ssl" : "tcp", host, (unsigned)port);

    int rc = MQTTAsync_create(&ctx->client, uri, clientid,
                               MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if (rc != MQTTASYNC_SUCCESS)
    {
        ehs_char buf[128];
        EhsSprintf(buf, "Paho MQTTAsync_create failed (rc=%d)", rc);
        EhsMQTTReportError(buf);
        ctx->client = NULL;
        return rc;
    }

    MQTTAsync_setCallbacks(ctx->client, ctx,
                           on_connection_lost,
                           on_message_arrived,
                           NULL /* deliveryComplete */);
    return MQTTASYNC_SUCCESS;
}

/* -------------------------------------------------------------------------
 * Helper: build and issue MQTTAsync_connect
 * ------------------------------------------------------------------------- */

static int paho_do_connect(PahoCtx_t *ctx,
                            ehs_bool tls,
                            const char *username, const char *password,
                            const char *clientCert, const char *clientKey,
                            const char *rootCA)
{
    MQTTAsync_SSLOptions ssl = MQTTAsync_SSLOptions_initializer;
    MQTTAsync_connectOptions opts = MQTTAsync_connectOptions_initializer;

    opts.keepAliveInterval  = 30;
    opts.cleansession       = 1;
    opts.onSuccess          = on_connect_success;
    opts.onFailure          = on_connect_failure;
    opts.context            = ctx;
    opts.connectTimeout     = 10;

    if (username && *username)  opts.username = username;
    if (password && *password)  opts.password = password;

    if (tls)
    {
        if (rootCA   && *rootCA)   ssl.trustStore  = rootCA;
        if (clientCert && *clientCert) ssl.keyStore = clientCert;
        if (clientKey  && *clientKey)  ssl.privateKey = clientKey;
        ssl.enableServerCertAuth = (rootCA && *rootCA) ? 1 : 0;
        opts.ssl = &ssl;
    }

    int rc = MQTTAsync_connect(ctx->client, &opts);
    if (rc != MQTTASYNC_SUCCESS)
    {
        ehs_char buf[128];
        EhsSprintf(buf, "Paho MQTTAsync_connect failed (rc=%d)", rc);
        EhsMQTTReportError(buf);
    }
    return rc;
}

/* -------------------------------------------------------------------------
 * Helper: issue disconnect
 * ------------------------------------------------------------------------- */

static void paho_do_disconnect(PahoCtx_t *ctx)
{
    MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
    opts.onSuccess  = on_disconnect_complete;
    opts.context    = ctx;
    opts.timeout    = 5000; /* ms */
    if (MQTTAsync_disconnect(ctx->client, &opts) != MQTTASYNC_SUCCESS)
    {
        /* If disconnect call itself fails, jump straight to disconnected */
        pthread_mutex_lock(&ctx->lock);
        ctx->state = PAHO_STATE_DISCONNECTED;
        EhsMQTTConnectEvent(EHS_FALSE);
        pthread_mutex_unlock(&ctx->lock);
    }
}

/* -------------------------------------------------------------------------
 * Helper: destroy client and reset state to INIT
 * ------------------------------------------------------------------------- */

static void paho_destroy_client(PahoCtx_t *ctx)
{
    if (ctx->client)
    {
        MQTTAsync_destroy(&ctx->client);
        ctx->client = NULL;
    }
    pthread_mutex_lock(&ctx->lock);
    ctx->state = PAHO_STATE_INIT;
    pthread_mutex_unlock(&ctx->lock);
}

/* -------------------------------------------------------------------------
 * EhsMqttClientLoop — thread entry point, mirrors aws_green_grass/mqtt.c
 * ------------------------------------------------------------------------- */

void *EhsMqttClientLoop(void *args)
{
    EhsMqttDevmanMon_t *pEhsMqttDevmanMon = (EhsMqttDevmanMon_t *)args;

    /* Connection parameters from EhsMQTTConnectPoll */
    ehs_bool  connect = EHS_FALSE;
    char     *host    = NULL;
    ehs_uint16 port   = 0;
    ehs_bool  tls     = EHS_FALSE;
    char     *clientid            = NULL;
    char     *username            = NULL;
    char     *password            = NULL;
    char     *clientCertFileName  = NULL;
    char     *clientKeyFileName   = NULL;
    char     *rootCAFileName      = NULL;

    /* Canonical (absolute) paths built from the app data directory */
    ehs_char appDataPath[EHS_STRING_LENGTH_MAX];
    ehs_char certPath[EHS_STRING_LENGTH_MAX];
    ehs_char keyPath[EHS_STRING_LENGTH_MAX];
    ehs_char caPath[EHS_STRING_LENGTH_MAX];

    PahoCtx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.state  = PAHO_STATE_INIT;
    ctx.client = NULL;
    pthread_mutex_init(&ctx.lock, NULL);

    ehs_bool run = EHS_TRUE;
    ehs_bool isDevmanMon = (pEhsMqttDevmanMon != NULL) ? EHS_TRUE : EHS_FALSE;

    while (run)
    {
        /* Run devman monitor loop if configured */
        if (pEhsMqttDevmanMon && pEhsMqttDevmanMon->pMqttDevmanMonLoop)
            pEhsMqttDevmanMon->pMqttDevmanMonLoop(NULL);

        EhsMQTTConnectPoll(&connect, &host, &port, &tls, &clientid,
                           &username, &password,
                           &clientCertFileName, &clientKeyFileName, &rootCAFileName);

        PahoState_t state;
        pthread_mutex_lock(&ctx.lock);
        state = ctx.state;
        pthread_mutex_unlock(&ctx.lock);

        switch (state)
        {
        /* ---- INIT: waiting for connect=true ---- */
        case PAHO_STATE_INIT:
            if (connect)
            {
                /* Validate mandatory parameters */
                if (!clientid || !*clientid)
                    { EhsMQTTReportError("clientId not specified"); break; }
                if (!host || !*host)
                    { EhsMQTTReportError("host not specified"); break; }
                if (port == 0)
                    { EhsMQTTReportError("port must be > 0"); break; }

                /* Resolve certificate paths relative to the app data directory */
                if (isDevmanMon)
                    EhsTF_tryCanonicPath(appDataPath, EHS_RUNTIME_DEVMAN_DIR, "core/certs", EHS_TRUE);
                else
                    EhsHMetagetCurrentAppDir(appDataPath);

                certPath[0] = keyPath[0] = caPath[0] = '\0';

                if (tls)
                {
                    if (clientCertFileName && *clientCertFileName)
                    {
                        EhsStrcat(certPath, appDataPath);
                        EhsStrcat(certPath, EHS_TD_FILES_SEPARATOR_STR);
                        EhsStrcat(certPath, clientCertFileName);
                        if (!EhsTF_exists(certPath))
                            { EhsMQTTReportError("client cert not found"); break; }
                    }
                    if (clientKeyFileName && *clientKeyFileName)
                    {
                        EhsStrcat(keyPath, appDataPath);
                        EhsStrcat(keyPath, EHS_TD_FILES_SEPARATOR_STR);
                        EhsStrcat(keyPath, clientKeyFileName);
                        if (!EhsTF_exists(keyPath))
                            { EhsMQTTReportError("client key not found"); break; }
                    }
                    if (rootCAFileName && *rootCAFileName)
                    {
                        EhsStrcat(caPath, appDataPath);
                        EhsStrcat(caPath, EHS_TD_FILES_SEPARATOR_STR);
                        EhsStrcat(caPath, rootCAFileName);
                        if (!EhsTF_exists(caPath))
                            { EhsMQTTReportError("CA cert not found"); break; }
                    }
                }

                if (paho_create_client(&ctx, host, port, tls, clientid) != MQTTASYNC_SUCCESS)
                    break;

                pthread_mutex_lock(&ctx.lock);
                ctx.state = PAHO_STATE_CONNECTING;
                pthread_mutex_unlock(&ctx.lock);

                if (paho_do_connect(&ctx, tls,
                                    username, password,
                                    *certPath ? certPath : NULL,
                                    *keyPath  ? keyPath  : NULL,
                                    *caPath   ? caPath   : NULL) != MQTTASYNC_SUCCESS)
                {
                    paho_destroy_client(&ctx); /* resets to INIT */
                }
            }
            break;

        /* ---- CONNECTING / DISCONNECTING: waiting for callback ---- */
        case PAHO_STATE_CONNECTING:
        case PAHO_STATE_DISCONNECTING:
            break;

        /* ---- CONNECTED: publish, subscribe, or disconnect ---- */
        case PAHO_STATE_CONNECTED:
            if (connect)
            {
                ehs_char topic[EHS_STRING_LENGTH_MAX];
                ehs_char message[EHS_STRING_LENGTH_MAX];
                ehs_uint8 qos;
                ehs_bool  doSubscribe;

                if (EhsMQTTPublishWritePoll(topic, message, &qos) == EHS_TRUE)
                {
                    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
                    pubmsg.payload    = (void *)message;
                    pubmsg.payloadlen = (int)EhsStrlen(message);
                    pubmsg.qos        = (int)qos;
                    pubmsg.retained   = 0;
                    MQTTAsync_sendMessage(ctx.client, topic, &pubmsg, NULL);
                }

                if (EhsMQTTSubscribeWritePoll(topic, &doSubscribe, &qos) == EHS_TRUE)
                {
                    MQTTAsync_responseOptions resp = MQTTAsync_responseOptions_initializer;
                    if (doSubscribe == EHS_TRUE)
                        MQTTAsync_subscribe(ctx.client, topic, (int)qos, &resp);
                    else
                        MQTTAsync_unsubscribe(ctx.client, topic, &resp);
                }
            }
            else
            {
                /* Disconnect requested by function block */
                pthread_mutex_lock(&ctx.lock);
                ctx.state = PAHO_STATE_DISCONNECTING;
                pthread_mutex_unlock(&ctx.lock);
                paho_do_disconnect(&ctx);
            }
            break;

        /* ---- INTERRUPTED: connection lost, disconnect cleanly ---- */
        case PAHO_STATE_INTERRUPTED:
            pthread_mutex_lock(&ctx.lock);
            ctx.state = PAHO_STATE_DISCONNECTING;
            pthread_mutex_unlock(&ctx.lock);
            paho_do_disconnect(&ctx);
            break;

        /* ---- DISCONNECTED: clean up, return to INIT ---- */
        case PAHO_STATE_DISCONNECTED:
            paho_destroy_client(&ctx); /* sets state = INIT */
            if (isDevmanMon == EHS_FALSE && *bNewSodlFlagRef == EHS_TRUE)
                run = EHS_FALSE;
            break;
        }

        EhsSleep(EHS_TIME_ms(100));

        /* SODL reload: request a graceful exit */
        if (isDevmanMon == EHS_FALSE && *bNewSodlFlagRef == EHS_TRUE)
        {
            pthread_mutex_lock(&ctx.lock);
            state = ctx.state;
            pthread_mutex_unlock(&ctx.lock);
            switch (state)
            {
            case PAHO_STATE_CONNECTED:
                pthread_mutex_lock(&ctx.lock);
                ctx.state = PAHO_STATE_DISCONNECTING;
                pthread_mutex_unlock(&ctx.lock);
                paho_do_disconnect(&ctx);
                break;
            case PAHO_STATE_INIT:
            case PAHO_STATE_DISCONNECTED:
                run = EHS_FALSE;
                break;
            default:
                break;
            }
        }
    }

    /* Final cleanup — ensure client is destroyed before thread exits */
    if (ctx.client)
        MQTTAsync_destroy(&ctx.client);
    pthread_mutex_destroy(&ctx.lock);

    return NULL;
}
