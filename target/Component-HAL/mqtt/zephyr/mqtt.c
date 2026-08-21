/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file mqtt.c  (mqtt/zephyr)
 * @brief MQTT HAL over Zephyr's native socket MQTT library (CONFIG_MQTT_LIB).
 *
 * Serves every Zephyr target rather than one board: the library sits on plain
 * BSD sockets, so it rides whatever provides them. On nRF91 that is the modem
 * (CONFIG_NET_SOCKETS_OFFLOAD — the IP stack is inside the modem and the socket
 * calls are forwarded to it); on an nRF70 Wi-Fi target it is Zephyr's own
 * native stack. Neither case needs anything from this file.
 *
 * WHY NOT ONE OF THE EXISTING BACKENDS
 * ------------------------------------
 *   * paho     — links libpaho-mqtt3as, an external .so cross-built into the
 *                Docker image. No such build exists for arm-zephyr-eabi, and
 *                Paho spawns its own network thread through pthreads.
 *   * lwip     — lwIP's own MQTT app. Zephyr does not use lwIP.
 *   * esp_mqtt — ESP-IDF's esp_mqtt_client.
 * Zephyr's library is already in-tree, needs no contrib-middleware step, and
 * costs one Kconfig line.
 *
 * THREADING — AND WHY THERE IS NO MUTEX HERE
 * ------------------------------------------
 * Unlike the Paho backend, this one is single-threaded by construction.
 * Zephyr's MQTT library never spawns a thread: it decodes inbound packets
 * inside mqtt_input(), which WE call, so mqtt_evt_handler() below runs on this
 * loop's own thread. Nothing is shared across threads, so nothing needs
 * locking. Do not "add a mutex for safety" — it would only mask a change that
 * broke this invariant.
 *
 * The flip side is that the connection only progresses while this loop runs.
 * mqtt_live() must be called regularly or the broker drops us on keepalive, so
 * the poll timeout below doubles as the loop's tick and must stay well under
 * CONFIG_MQTT_KEEPALIVE.
 *
 * TLS
 * ---
 * Not supported here: CONFIG_MQTT_LIB_TLS pulls in mbedTLS, which on nRF9151
 * is a large fraction of a flash budget already at ~50%, and the nRF91 modem
 * offers native TLS through a completely different API (TLS credentials
 * installed into the modem by security tag, not PEM files on the filesystem)
 * that the mqtt_client function block's cert-FILENAME parameters cannot
 * express. A connect requesting TLS is refused with a clear message rather
 * than silently downgraded to plaintext — a silent downgrade would put
 * credentials on the wire in the clear.
 *
 * SINGLE INSTANCE
 * ---------------
 * One static context. EHS_MQTT_CLIENT_INSTANCE_MAX defaults to 1 and the
 * function block enforces it at init, and EHS_DEVMAN_SUPPORT is not configured
 * on any Zephyr target, so the second caller that would need a second context
 * does not currently exist. A second concurrent loop is refused loudly instead
 * of quietly sharing one client's buffers.
 */

#include "globals.h"
#include "hal-api.h"
#include "hal_mqtt.h"

#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>

#include <errno.h>
#include <string.h>

/* -------------------------------------------------------------------------
 * Buffer sizing
 *
 * All three are overridable from a platform config.mk via DEFS. The defaults
 * are MCU-sized, not desktop-sized.
 *
 * rx_buf holds one inbound packet's FIXED HEADER AND TOPIC only — the payload
 * is not copied into it, it is drawn separately with mqtt_read_publish_payload
 * (see the MQTT_EVT_PUBLISH note in mqtt.h). So rx_buf is sized by the longest
 * topic in use, not by the largest message, and 512 is generous for that.
 *
 * tx_buf must hold a whole outbound PUBLISH, header plus payload, because
 * mqtt_publish() serialises into it in one go. So tx_buf is the real cap on
 * publish size, and a publish that does not fit is rejected by the library
 * with -ENOMEM rather than truncated.
 * ------------------------------------------------------------------------- */

#ifndef EHS_MQTT_RX_BUF_SIZE
#define EHS_MQTT_RX_BUF_SIZE 512
#endif

#ifndef EHS_MQTT_TX_BUF_SIZE
#define EHS_MQTT_TX_BUF_SIZE 1024
#endif

/* Staging buffer for a received payload before it is handed to the FB layer.
 * A payload longer than this is drained and dropped rather than delivered in
 * pieces: EhsMQTTSubscribeEvent has no continuation concept, so half a message
 * would reach the application indistinguishable from a whole one. */
#ifndef EHS_MQTT_PAYLOAD_BUF_SIZE
#define EHS_MQTT_PAYLOAD_BUF_SIZE 512
#endif

/* Loop tick. Also the poll() timeout, so it is how long the thread blocks when
 * the socket is idle rather than a busy-wait interval. Must be comfortably
 * below CONFIG_MQTT_KEEPALIVE (seconds) so mqtt_live() gets to send PINGREQ in
 * time; 100 ms against a 60 s keepalive has three orders of magnitude spare. */
#define EHS_MQTT_POLL_TIMEOUT_MS 100

/* Bound on how long a connect may sit in CONNECTING before we give up. The
 * library has no connect timeout of its own — without this a broker that
 * completes the TCP handshake and then never sends CONNACK (a firewall
 * black-holing the response, typically) leaves the loop stuck forever with no
 * error reported to the application. */
#define EHS_MQTT_CONNACK_TIMEOUT_MS 15000

/* -------------------------------------------------------------------------
 * Loop state
 * ------------------------------------------------------------------------- */

typedef enum
{
    ZMQTT_STATE_INIT,          /* no socket, waiting for connect=true         */
    ZMQTT_STATE_CONNECTING,    /* mqtt_connect() issued, awaiting CONNACK     */
    ZMQTT_STATE_CONNECTED,     /* CONNACK received with result 0              */
    ZMQTT_STATE_DISCONNECTED   /* teardown needed, then back to INIT          */
} ZmqttState_t;

static struct mqtt_client   s_client;
static struct sockaddr_storage s_broker;
static ehs_uint8            s_rx_buf[EHS_MQTT_RX_BUF_SIZE];
static ehs_uint8            s_tx_buf[EHS_MQTT_TX_BUF_SIZE];
static ehs_uint8            s_payload_buf[EHS_MQTT_PAYLOAD_BUF_SIZE];

/* mqtt_utf8 bodies for the credentials. The library keeps POINTERS into these
 * for the life of the connection rather than copying, so they must outlive the
 * mqtt_connect() call — hence file scope, not the stack of the INIT case. */
static struct mqtt_utf8     s_username;
static struct mqtt_utf8     s_password;

static ZmqttState_t         s_state = ZMQTT_STATE_INIT;
static ehs_bool             s_loop_active = EHS_FALSE;
static ehs_uint32           s_connect_elapsed_ms = 0u;

/* Packet identifier source for QoS 1/2 PUBLISH and for SUBSCRIBE/UNSUBSCRIBE.
 * mqtt_publish() does not allocate one. A plain counter rather than a random
 * value: 0 is not a legal identifier, and monotonic ids make a packet capture
 * readable when a QoS 1 exchange has to be traced. */
static ehs_uint16           s_next_message_id = 1u;

static ehs_uint16 zmqtt_message_id(void)
{
    ehs_uint16 id = s_next_message_id;

    s_next_message_id++;
    if (s_next_message_id == 0u)
    {
        s_next_message_id = 1u;   /* wrap past the reserved 0 */
    }
    return id;
}

/* Global MQTT state, part of the HAL contract (hal_mqtt.h). */
static MQTT_State_t         s_global_state = MQTT_STATE_INIT;

MQTT_State_t mqttGetGlobalState(void)
{
    return s_global_state;
}

void mqttSetGlobalState(const MQTT_State_t newState)
{
    s_global_state = newState;
}

/* -------------------------------------------------------------------------
 * Payload delivery
 * ------------------------------------------------------------------------- */

/**
 * Drain a received PUBLISH payload and hand it to the function block layer.
 *
 * mqtt_read_publish_payload() may return short reads, so this loops until the
 * announced length has been consumed. Draining is not optional even when the
 * message is being dropped: bytes left unread stay in the socket and the next
 * mqtt_input() would decode them as a packet header, desynchronising the
 * stream for good.
 */
static void zmqtt_deliver_publish(struct mqtt_client *client,
                                  const struct mqtt_publish_param *pub)
{
    ehs_char topic[EHS_STRING_LENGTH_MAX];
    ehs_uint32 topic_len = pub->message.topic.topic.size;
    ehs_uint32 payload_len = pub->message.payload.len;
    ehs_uint32 got = 0u;
    ehs_bool   too_long = (payload_len >= (ehs_uint32)sizeof(s_payload_buf)) ? EHS_TRUE : EHS_FALSE;

    /* Read the payload out of the socket whatever we intend to do with it. */
    while (got < payload_len)
    {
        ehs_uint32 space = (ehs_uint32)sizeof(s_payload_buf) - (too_long ? 0u : got);
        ehs_uint32 want  = payload_len - got;
        int rc;

        if (want > space)
        {
            want = space;
        }

        /* When discarding, keep reusing the front of the buffer as a sink. */
        rc = mqtt_read_publish_payload(client,
                                       too_long ? s_payload_buf : &s_payload_buf[got],
                                       (size_t)want);
        if (rc <= 0)
        {
            EhsMQTTReportError("MQTT payload read failed");
            return;
        }
        got += (ehs_uint32)rc;
    }

    if (too_long)
    {
        ehs_char msg[96];
        EhsSprintf(msg, "MQTT payload %u bytes exceeds buffer %u - dropped",
                   (unsigned)payload_len, (unsigned)sizeof(s_payload_buf));
        EhsMQTTReportError(msg);
        return;
    }

    if (topic_len >= (ehs_uint32)sizeof(topic))
    {
        EhsMQTTReportError("MQTT topic too long - message dropped");
        return;
    }

    /* EhsMemcpy's source parameter is non-const, so the cast is unavoidable. */
    EhsMemcpy(topic, (void *)(uintptr_t)pub->message.topic.topic.utf8, topic_len);
    topic[topic_len] = '\0';

    s_payload_buf[payload_len] = '\0';

    EhsMQTTSubscribeEvent(topic, (char *)s_payload_buf, (ehs_sint32)payload_len);
}

/* -------------------------------------------------------------------------
 * Library event callback — runs on THIS loop's thread, from mqtt_input()
 * ------------------------------------------------------------------------- */

static void zmqtt_evt_handler(struct mqtt_client *client, const struct mqtt_evt *evt)
{
    switch (evt->type)
    {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0)
        {
            ehs_char msg[64];
            EhsSprintf(msg, "MQTT CONNACK refused (rc=%d)", evt->result);
            EhsMQTTReportError(msg);
            s_state = ZMQTT_STATE_DISCONNECTED;
        }
        else
        {
            s_state = ZMQTT_STATE_CONNECTED;
            mqttSetGlobalState(MQTT_STATE_CONNECTED);
            EhsMQTTConnectEvent(EHS_TRUE);
        }
        break;

    case MQTT_EVT_DISCONNECT:
        /* The client reference is invalid past this point (see mqtt.h) — do
         * not touch `client` here, only flag the teardown for the loop. */
        s_state = ZMQTT_STATE_DISCONNECTED;
        break;

    case MQTT_EVT_PUBLISH:
    {
        const struct mqtt_publish_param *pub = &evt->param.publish;
        ehs_uint8 qos = pub->message.topic.qos;

        zmqtt_deliver_publish(client, pub);

        /* Acknowledge AFTER the payload has been drained. */
        if (qos == MQTT_QOS_1_AT_LEAST_ONCE)
        {
            struct mqtt_puback_param ack = { .message_id = pub->message_id };
            (void)mqtt_publish_qos1_ack(client, &ack);
        }
        else if (qos == MQTT_QOS_2_EXACTLY_ONCE)
        {
            struct mqtt_pubrec_param rec = { .message_id = pub->message_id };
            (void)mqtt_publish_qos2_receive(client, &rec);
        }
        break;
    }

    case MQTT_EVT_PUBREL:
    {
        /* Inbound QoS 2, second leg: confirm the release. */
        struct mqtt_pubcomp_param cmp = { .message_id = evt->param.pubrel.message_id };
        (void)mqtt_publish_qos2_complete(client, &cmp);
        break;
    }

    case MQTT_EVT_PUBACK:
    case MQTT_EVT_PUBREC:
    case MQTT_EVT_PUBCOMP:
    case MQTT_EVT_SUBACK:
    case MQTT_EVT_UNSUBACK:
    case MQTT_EVT_PINGRESP:
    default:
        /* Nothing the function block layer exposes hangs off these. */
        break;
    }
}

/* -------------------------------------------------------------------------
 * Broker address resolution
 * ------------------------------------------------------------------------- */

/**
 * Resolve host:port into s_broker. Handles both a dotted-quad literal and a
 * name — zsock_getaddrinfo() covers both, and on nRF91 it is offloaded to the
 * modem, which is the only DNS resolver available there (Zephyr's own resolver
 * is compiled out along with NET_NATIVE).
 */
static ehs_bool zmqtt_resolve(const char *host, ehs_uint16 port)
{
    struct zsock_addrinfo hints;
    struct zsock_addrinfo *res = NULL;
    struct sockaddr_in *sin = (struct sockaddr_in *)&s_broker;
    int rc;

    EhsMemset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;      /* IPv6 is off on the cellular target */
    hints.ai_socktype = SOCK_STREAM;

    rc = zsock_getaddrinfo(host, NULL, &hints, &res);
    if ((rc != 0) || (res == NULL))
    {
        ehs_char msg[128];
        EhsSprintf(msg, "MQTT DNS lookup of '%s' failed (rc=%d)", host, rc);
        EhsMQTTReportError(msg);
        return EHS_FALSE;
    }

    EhsMemset(&s_broker, 0, sizeof(s_broker));
    sin->sin_family = AF_INET;
    sin->sin_port   = htons(port);
    sin->sin_addr   = ((struct sockaddr_in *)res->ai_addr)->sin_addr;

    zsock_freeaddrinfo(res);
    return EHS_TRUE;
}

/* -------------------------------------------------------------------------
 * Connect / teardown
 * ------------------------------------------------------------------------- */

static ehs_bool zmqtt_start_connect(const char *host, ehs_uint16 port,
                                    const char *clientid,
                                    const char *username, const char *password)
{
    int rc;

    if (!zmqtt_resolve(host, port))
    {
        return EHS_FALSE;
    }

    mqtt_client_init(&s_client);

    s_client.broker           = &s_broker;
    s_client.evt_cb           = zmqtt_evt_handler;
    s_client.client_id.utf8   = (const ehs_uint8 *)clientid;
    s_client.client_id.size   = (ehs_uint32)EhsStrlen(clientid);
    s_client.protocol_version = MQTT_VERSION_3_1_1;
    s_client.rx_buf           = s_rx_buf;
    s_client.rx_buf_size      = (ehs_uint32)sizeof(s_rx_buf);
    s_client.tx_buf           = s_tx_buf;
    s_client.tx_buf_size      = (ehs_uint32)sizeof(s_tx_buf);
    s_client.transport.type   = MQTT_TRANSPORT_NON_SECURE;

    /* A password without a user name is not representable in MQTT 3.1.1, so
     * the password is only offered when a user name is also present. */
    if ((username != NULL) && (*username != '\0'))
    {
        s_username.utf8    = (const ehs_uint8 *)username;
        s_username.size    = (ehs_uint32)EhsStrlen(username);
        s_client.user_name = &s_username;

        if ((password != NULL) && (*password != '\0'))
        {
            s_password.utf8   = (const ehs_uint8 *)password;
            s_password.size   = (ehs_uint32)EhsStrlen(password);
            s_client.password = &s_password;
        }
    }

    rc = mqtt_connect(&s_client);
    if (rc != 0)
    {
        ehs_char msg[64];
        EhsSprintf(msg, "mqtt_connect failed (rc=%d)", rc);
        EhsMQTTReportError(msg);
        return EHS_FALSE;
    }

    s_connect_elapsed_ms = 0u;
    return EHS_TRUE;
}

/**
 * Service the socket: block up to timeout_ms for inbound data, decode whatever
 * arrived, then let the library run its keepalive clock.
 *
 * Returns EHS_FALSE if the link is gone and the caller should tear down.
 */
static ehs_bool zmqtt_service(int timeout_ms)
{
    struct zsock_pollfd fds[1];
    int rc;

    fds[0].fd      = s_client.transport.tcp.sock;
    fds[0].events  = ZSOCK_POLLIN;
    fds[0].revents = 0;

    rc = zsock_poll(fds, 1, timeout_ms);
    if (rc < 0)
    {
        EhsMQTTReportError("MQTT socket poll failed");
        return EHS_FALSE;
    }

    if ((rc > 0) && ((fds[0].revents & ZSOCK_POLLIN) != 0))
    {
        rc = mqtt_input(&s_client);
        if (rc != 0)
        {
            /* MQTT_EVT_DISCONNECT has usually already fired here; reporting
             * would double up on an error the application has seen. */
            return EHS_FALSE;
        }
    }

    if ((rc >= 0) && ((fds[0].revents & (ZSOCK_POLLERR | ZSOCK_POLLHUP | ZSOCK_POLLNVAL)) != 0))
    {
        return EHS_FALSE;
    }

    /* Sends PINGREQ when keepalive is due. Nothing else drives it. */
    rc = mqtt_live(&s_client);
    if ((rc != 0) && (rc != -EAGAIN))
    {
        return EHS_FALSE;
    }

    return EHS_TRUE;
}

/**
 * Close the connection and return the state machine to INIT.
 *
 * mqtt_abort() rather than mqtt_disconnect() when the link is already known
 * broken: mqtt_disconnect() tries to WRITE a DISCONNECT packet, which on a
 * dead socket blocks until it errors out and leaves the fd open either way.
 */
static void zmqtt_teardown(ehs_bool graceful)
{
    if (graceful == EHS_TRUE)
    {
        (void)mqtt_disconnect(&s_client);
    }
    else
    {
        (void)mqtt_abort(&s_client);
    }

    s_state = ZMQTT_STATE_INIT;
    mqttSetGlobalState(MQTT_STATE_DISCONNECTED);
    EhsMQTTConnectEvent(EHS_FALSE);
}

/* -------------------------------------------------------------------------
 * EhsMqttClientLoop — thread entry point
 * ------------------------------------------------------------------------- */

void *EhsMqttClientLoop(void *args)
{
    /* args carries the devman monitor context on targets that have one. No
     * Zephyr target configures EHS_DEVMAN_SUPPORT, so it is always NULL here;
     * asserting that is cheaper than carrying an untestable code path. */
    (void)args;

    ehs_bool   connect = EHS_FALSE;
    char      *host = NULL;
    ehs_uint16 port = 0;
    ehs_bool   tls = EHS_FALSE;
    char      *clientid = NULL;
    char      *username = NULL;
    char      *password = NULL;
    char      *clientCertFileName = NULL;
    char      *clientKeyFileName = NULL;
    char      *rootCAFileName = NULL;

    ehs_bool   run = EHS_TRUE;
    ehs_bool   tls_refused = EHS_FALSE;

    if (s_loop_active == EHS_TRUE)
    {
        EhsMQTTReportError("MQTT: only one client loop is supported on Zephyr");
        return NULL;
    }
    s_loop_active = EHS_TRUE;

    s_state = ZMQTT_STATE_INIT;
    mqttSetGlobalState(MQTT_STATE_IDLE);

    while (run == EHS_TRUE)
    {
        EhsMQTTConnectPoll(&connect, &host, &port, &tls, &clientid,
                           &username, &password,
                           &clientCertFileName, &clientKeyFileName, &rootCAFileName);

        switch (s_state)
        {
        /* ---- INIT: idle until the function block asks for a connection ---- */
        case ZMQTT_STATE_INIT:
            if (connect == EHS_TRUE)
            {
                if ((clientid == NULL) || (*clientid == '\0'))
                {
                    EhsMQTTReportError("MQTT clientId not specified");
                    break;
                }
                if ((host == NULL) || (*host == '\0'))
                {
                    EhsMQTTReportError("MQTT host not specified");
                    break;
                }
                if (port == 0u)
                {
                    EhsMQTTReportError("MQTT port must be > 0");
                    break;
                }
                if (tls == EHS_TRUE)
                {
                    /* Refuse rather than downgrade — see the TLS note at the
                     * top of this file. Reported once per connect request, not
                     * once per 100 ms tick. */
                    if (tls_refused == EHS_FALSE)
                    {
                        EhsMQTTReportError("MQTT TLS is not supported on this target "
                                           "- use port 1883 without TLS");
                        tls_refused = EHS_TRUE;
                    }
                    break;
                }

                mqttSetGlobalState(MQTT_STATE_DNS_LOOKUP);

                if (zmqtt_start_connect(host, port, clientid, username, password) == EHS_TRUE)
                {
                    s_state = ZMQTT_STATE_CONNECTING;
                    mqttSetGlobalState(MQTT_STATE_WAIT_FOR_CONNECTION);
                }
                /* else: stay in INIT and retry on the next tick */
            }
            else
            {
                tls_refused = EHS_FALSE;   /* re-arm for the next request */
            }
            break;

        /* ---- CONNECTING: pump the socket until CONNACK or timeout ---- */
        case ZMQTT_STATE_CONNECTING:
            if (zmqtt_service(EHS_MQTT_POLL_TIMEOUT_MS) == EHS_FALSE)
            {
                zmqtt_teardown(EHS_FALSE);
                break;
            }

            /* Only meaningful while still CONNECTING — the callback may have
             * moved us on during the zmqtt_service() above. */
            if (s_state == ZMQTT_STATE_CONNECTING)
            {
                s_connect_elapsed_ms += (ehs_uint32)EHS_MQTT_POLL_TIMEOUT_MS;
                if (s_connect_elapsed_ms >= (ehs_uint32)EHS_MQTT_CONNACK_TIMEOUT_MS)
                {
                    EhsMQTTReportError("MQTT timed out waiting for CONNACK");
                    zmqtt_teardown(EHS_FALSE);
                }
                else if (connect == EHS_FALSE)
                {
                    /* Request withdrawn mid-handshake. */
                    zmqtt_teardown(EHS_FALSE);
                }
            }
            break;

        /* ---- CONNECTED: publish, (un)subscribe, service, or disconnect ---- */
        case ZMQTT_STATE_CONNECTED:
            if (connect == EHS_TRUE)
            {
                ehs_char  topic[EHS_STRING_LENGTH_MAX];
                ehs_char  message[EHS_STRING_LENGTH_MAX];
                ehs_uint8 qos = 0u;
                ehs_bool  doSubscribe = EHS_FALSE;

                if (EhsMQTTPublishWritePoll(topic, message, &qos) == EHS_TRUE)
                {
                    struct mqtt_publish_param pub;

                    EhsMemset(&pub, 0, sizeof(pub));
                    pub.message.topic.topic.utf8 = (const ehs_uint8 *)topic;
                    pub.message.topic.topic.size = (ehs_uint32)EhsStrlen(topic);
                    pub.message.topic.qos        = qos;
                    pub.message.payload.data     = (ehs_uint8 *)message;
                    pub.message.payload.len      = (ehs_uint32)EhsStrlen(message);
                    pub.message_id               = zmqtt_message_id();

                    if (mqtt_publish(&s_client, &pub) != 0)
                    {
                        EhsMQTTReportError("MQTT publish failed");
                    }
                }

                if (EhsMQTTSubscribeWritePoll(topic, &doSubscribe, &qos) == EHS_TRUE)
                {
                    struct mqtt_topic          t;
                    struct mqtt_subscription_list list;

                    t.topic.utf8 = (const ehs_uint8 *)topic;
                    t.topic.size = (ehs_uint32)EhsStrlen(topic);
                    t.qos        = qos;

                    list.list       = &t;
                    list.list_count = 1u;
                    list.message_id = zmqtt_message_id();

                    if (doSubscribe == EHS_TRUE)
                    {
                        if (mqtt_subscribe(&s_client, &list) != 0)
                        {
                            EhsMQTTReportError("MQTT subscribe failed");
                        }
                    }
                    else
                    {
                        if (mqtt_unsubscribe(&s_client, &list) != 0)
                        {
                            EhsMQTTReportError("MQTT unsubscribe failed");
                        }
                    }
                }

                if (zmqtt_service(EHS_MQTT_POLL_TIMEOUT_MS) == EHS_FALSE)
                {
                    zmqtt_teardown(EHS_FALSE);
                }
            }
            else
            {
                zmqtt_teardown(EHS_TRUE);
            }
            break;

        /* ---- DISCONNECTED: the callback flagged a teardown ---- */
        case ZMQTT_STATE_DISCONNECTED:
        default:
            zmqtt_teardown(EHS_FALSE);
            break;
        }

        /* zmqtt_service() already blocks for the tick when there is a socket;
         * sleeping again would double the loop period and, more to the point,
         * halve the responsiveness of publish. Only sleep when idle. */
        if (s_state == ZMQTT_STATE_INIT)
        {
            EhsSleep(EHS_TIME_ms(EHS_MQTT_POLL_TIMEOUT_MS));
        }

        /* SODL reload: the application is going away, so close cleanly and let
         * the thread exit. Matches the Paho backend's contract. */
        if (*bNewSodlFlagRef == EHS_TRUE)
        {
            if (s_state != ZMQTT_STATE_INIT)
            {
                zmqtt_teardown(s_state == ZMQTT_STATE_CONNECTED ? EHS_TRUE : EHS_FALSE);
            }
            run = EHS_FALSE;
        }
    }

    s_loop_active = EHS_FALSE;
    mqttSetGlobalState(MQTT_STATE_IDLE);
    return NULL;
}
