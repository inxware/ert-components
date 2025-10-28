/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_mqtt.h.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_HAL_MQTT_H
#define EHS_HAL_MQTT_H

/*****************************************************************************/
/* Included files */
#include "globals.h"

#ifndef EHS_MQTT_CLIENT_INSTANCE_MAX
#define EHS_MQTT_CLIENT_INSTANCE_MAX 1
// a default maximum number mqtt clinet instance/loop
#endif

#ifndef INX_MQTT_CERT_FILENAME_LENGTH
#define INX_MQTT_CERT_FILENAME_LENGTH 32
#endif

#ifndef EHS_MQTT_SUBS_RECV_BUFF_MAX_LENGTH
#define EHS_MQTT_SUBS_RECV_BUFF_MAX_LENGTH EHS_STRING_LENGTH_MAX
#endif

typedef enum
{
    MQTT_STATE_INIT,
    MQTT_STATE_IDLE,
    MQTT_STATE_DNS_LOOKUP,
    MQTT_STATE_DO_CONNECT,
    MQTT_STATE_DO_TLS_HANDSHAKE,
    MQTT_STATE_WAIT_FOR_CONNECTION,
    MQTT_STATE_CONNECTED,
    MQTT_STATE_RECV_DATA,
    MQTT_STATE_DO_PUBLISH,
    MQTT_STATE_DO_DISCONNECT,
    MQTT_STATE_DISCONNECTED,
    MQTT_STATE_DISCONNECTED_NEW,
    MQTT_STATE_INVALID
} MQTT_State_t;

typedef enum
{
    MQTT_QOS_0 = 0,
    MQTT_QOS_1 = 1,
    MQTT_QOS_2 = 2
} mqtt_qos_t;

typedef struct inx_mqtt_client_state_mine
{
    char host[EHS_STRING_LENGTH_MAX];
    ehs_uint16 port;
    char clientid[EHS_STRING_LENGTH_MAX];
    char username[EHS_STRING_LENGTH_MAX];
    char password[EHS_STRING_LENGTH_MAX];
    ehs_bool tls;
    ehs_char clientCertFileName[INX_MQTT_CERT_FILENAME_LENGTH];
    ehs_char clientKeyFileName[INX_MQTT_CERT_FILENAME_LENGTH];
    ehs_char rootCAFileName[INX_MQTT_CERT_FILENAME_LENGTH];
    ehs_bool connect;
    ehs_bool initialised;
    void* pFIdata;
    ehs_bool always_read; // allow for reading data from this struct even if pFIdata is NULL
    struct inx_mqtt_client_state_mine* pNext;
    struct inx_mqtt_client_state_mine* pPrev;

} inx_mqtt_client_state_type_mine;

typedef struct inx_mqtt_subscribe_state
{
    ehs_bool needProcessing;
    ehs_bool needSubscribe;
    ehs_uint8 qos;
    ehs_char topic[EHS_STRING_LENGTH_MAX];
    void* pFIdata;
    ehs_bool (*pMqttSubscribeCallback)(struct inx_mqtt_subscribe_state* subscribe_state, char* payload, ehs_sint32 payloadSize);
    struct inx_mqtt_subscribe_state* pNext;
    struct inx_mqtt_subscribe_state* pPrev;
} inx_mqtt_subscribe_state_type;

typedef struct inx_mqtt_publish_state
{
    ehs_bool needProcessing;
    ehs_uint8 qos;
    ehs_char topic[EHS_STRING_LENGTH_MAX];
    ehs_char message[EHS_STRING_LENGTH_MAX];
    void* pFIdata;
    ehs_bool always_read; // allow for reading data from this struct even if pFIdata is NULL
    struct inx_mqtt_publish_state* pNext;
    struct inx_mqtt_publish_state* pPrev;
} inx_mqtt_publish_state_type;

typedef enum
{
    MQTT_DEVMAN_MON_INIT,
    MQTT_DEVMAN_MON_CONNECT,
    MQTT_DEVMAN_MON_CONNECTING,
    MQTT_DEVMAN_MON_PUB_SUB_INIT,
    MQTT_DEVMAN_MON_RUNNING

} EhsMqttDevmanMonState_t;

typedef struct
{
    void* (*pMqttDevmanMonLoop)(void*);
    void (*pMqttDevmanMonErrorCallback)(const ehs_char* msg);
    EhsMqttDevmanMonState_t state;
    volatile ehs_bool mqtt_client_connected;
    inx_mqtt_client_state_type_mine mqtt_client_state;

} EhsMqttDevmanMon_t;

/* Counter used for limiting number of mqtt clinet instance */
extern ehs_uint32 gMqttClientInstanceCount;

/* API for the generic mqtt */

 MQTT_State_t mqttGetGlobalState();
 void mqttSetGlobalState(const MQTT_State_t newState);
 void mqttSetTopicNames(const char* clientId);

 void* EhsMqttClientLoop(void* args);

 ehs_uint32 EhsMqttClientInstanceCount();

 void EhsMQTTConnectEvent(ehs_bool connect);
 void EhsMQTTConnectPoll(ehs_bool* connect, char** host, ehs_uint16* pPort, ehs_bool* pTLS, char** clientid, char** username, char** password, char** clientCertFileName, char** clientKeyFileName, char** rootCAFileName);
 void EhsMQTTReportError(const ehs_char* err_msg);

 ehs_bool EhsMQTTSubscribeEvent(char* topic, char* payload, ehs_sint32 payloadSize);
 ehs_bool EhsMQTTSubscribeWritePoll(char* buffer, ehs_bool* subscribe, ehs_uint8* qos);

 ehs_bool EhsMQTTPublishWritePoll(ehs_char* topic, ehs_char* payload, ehs_uint8* qos);

#if EHS_DEVMAN_SUPPORT == EHS_DEVMAN_MQTT
/* API for the mqtt client used by devmon service */
 EhsMqttDevmanMon_t* EhsMqttDevmanMonSupport();
 inx_mqtt_subscribe_state_type* EhsMqttDevmanMonSubscribeNeedProcessing();
 inx_mqtt_subscribe_state_type* EhsMqttDevmanMonSubscribeGetWidgetById(const char* topic);
 inx_mqtt_publish_state_type* EhsMqttDevmanMonPublishNeedProcessing();
 #endif

#endif /* EHS_HAL_MQTT_H */
