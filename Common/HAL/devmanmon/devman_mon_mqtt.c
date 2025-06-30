#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

#include "hal-api.h"
#include "devman_mon_mqtt.h"
#include "hal_logger.h"
#include "hal_string.h"

#ifndef EHS_CONTROLLER_MODEL
#define EHS_CONTROLLER_MODEL "HH-J3-02-00"
#endif

/* list of devman mon subscriptions */
inx_mqtt_subscribe_state_type EhsMqttDevmanMonSub[EHS_MQTT_SUB_TOPICS] = {0};

/* list of devman mon publications */
inx_mqtt_publish_state_type EhsMqttDevmanMonPub[EHS_MQTT_PUB_TOPICS] = {0};

// MQTT Devman mon clinet object
EhsMqttDevmanMon_t EhsMqttDevmanMon = {0};

// Ping timer value
EhsTickType gEhsMqttDevmanMonPingTimer = 0;

// Keeps a count of connect/disconnect
ehs_uint32 gMqttConnectionsCounter = 0;
// Holds the connection state
ehs_bool gMqttConnected = EHS_FALSE;

/* local function prototypes */
void EhsMqttDevmanMonConnectPayload(char* payload, ehs_uint32 max_size, ehs_uint32 nMqttConnections);
void EhsMqttDevmanMonHandlePing(ehs_bool enabled);

/****************************************************/
/* Implement global functions defined in hal_mqtt.h */
/****************************************************/

EhsMqttDevmanMon_t* EhsMqttDevmanMonSupport()
{
    return &EhsMqttDevmanMon;
}

inx_mqtt_subscribe_state_type* EhsMqttDevmanMonSubscribeNeedProcessing()
{
    for(ehs_uint8 i = 0; i < EHS_MQTT_SUB_TOPICS; i++)
    {
        inx_mqtt_subscribe_state_type* subscribe_state = &(EhsMqttDevmanMonSub[i]);
        ehs_bool needProcessing;
        EhsTPMutex_lock(EhsTPMutex_subMQTT);
        needProcessing = subscribe_state->needProcessing;
        EhsTPMutex_unlock(EhsTPMutex_subMQTT);
        if(needProcessing==EHS_TRUE){
            return subscribe_state;
        }
    }
    return NULL;
}

inx_mqtt_subscribe_state_type* EhsMqttDevmanMonSubscribeGetWidgetById(const char* topic)
{
    for(ehs_uint8 i = 0; i < EHS_MQTT_SUB_TOPICS; i++){
        inx_mqtt_subscribe_state_type* subscribe_state = &(EhsMqttDevmanMonSub[i]);
        if (EhsStrcmp(subscribe_state->topic, topic) == 0){
            return subscribe_state;
        }
    }
    return NULL;
}

inx_mqtt_publish_state_type* EhsMqttDevmanMonPublishNeedProcessing()
{
    for(ehs_uint8 i = 0; i < EHS_MQTT_PUB_TOPICS; i++)
    {
        inx_mqtt_publish_state_type* publish_state = &(EhsMqttDevmanMonPub[i]);
        ehs_bool needProcessing;
        EhsTPMutex_lock(EhsTPMutex_pubMQTT);
        needProcessing = publish_state->needProcessing;
        EhsTPMutex_unlock(EhsTPMutex_pubMQTT);
        if(needProcessing==EHS_TRUE){
            return publish_state;
        }
    }
    return NULL;
}

/****************************************************/
/* Define functions used within hal/devmanmon scope */
/****************************************************/

void EhsMqttDevmanMonError(const ehs_char* msg)
{
    EHSH_LOG_ERROR("%s \n", msg);
}

EhsMqttDevmanMon_t* EhsMqttDevmanMonSupportInit()
{
    EhsMqttDevmanMon.mqtt_client_state.always_read = EHS_TRUE;
    EhsMqttDevmanMon.mqtt_client_state.pFIdata = NULL;
    EhsMqttDevmanMon.state = MQTT_DEVMAN_MON_INIT;
    EhsMqttDevmanMon.pMqttDevmanMonErrorCallback = EhsMqttDevmanMonError;

    return &EhsMqttDevmanMon;
}

inx_mqtt_client_state_type_mine* EhsMqttDevmanMonMqttClient()
{
    return &(EhsMqttDevmanMon.mqtt_client_state);
}

EhsMqttDevmanMonState_t EhsGetMqttDevmanMonState()
{
    return EhsMqttDevmanMon.state;
}

void EhsSetMqttDevmanMonState(EhsMqttDevmanMonState_t state)
{
    EhsMqttDevmanMon.state = state;
}

ehs_bool EhsGetMqttDevmanMonConnected()
{
    return EhsMqttDevmanMon.mqtt_client_connected;
}

void EhsMqttDevmanMonHandleDisconnected()
{
    // toggle connection state
    if(gMqttConnected == EHS_TRUE){
        printf("* DEVMAN MON MQTT DISCONNECTED * \n");
        gMqttConnected = EHS_FALSE;
    }
}

void EhsMqttDevmanMonHandleConnected()
{
    // toggle connection state
    if(gMqttConnected == EHS_FALSE){
        gEhsMqttDevmanMonPingTimer = 0;
        gMqttConnectionsCounter++;
        char payload[EHS_STRING_LENGTH_MAX] = { 0 };
        EhsMqttDevmanMonConnectPayload(payload, EHS_STRING_LENGTH_MAX, gMqttConnectionsCounter);
        EhsMqttDevmanMonPublish(EHS_MQTT_PUB_TOPIC_DIDCONNECT_ID, payload);
        gMqttConnected = EHS_TRUE;
        printf("* DEVMAN MON MQTT CONNECTED (%d) * \n", gMqttConnectionsCounter);
    }
    // run ping handler if enabled
    EhsMqttDevmanMonHandlePing(EHS_MQTT_DEVMAN_MON_PING_ENABLED);
}

/* Devman mon mqtt subscription functions */
inx_mqtt_subscribe_state_type* EhsMqttDevmanMonSubscriptions(ehs_uint16 index)
{
    if(index < EHS_MQTT_SUB_TOPICS){
        return &(EhsMqttDevmanMonSub[index]);
    }
    return NULL;
}

void EhsMqttDevmanMonRegisterSub(inx_mqtt_subscribe_state_type* subscribe_state, const char* topic, const char* clientId)
{
    if(subscribe_state == NULL){
        EHSH_LOG_ERROR("Failed to register devman mon subscription \n");
        return;
    }
    subscribe_state->pFIdata = NULL;
    subscribe_state->qos = 0;
    subscribe_state->needSubscribe = EHS_TRUE;
    subscribe_state->needProcessing = EHS_TRUE;
    subscribe_state->pMqttSubscribeCallback = EhsMqttDevmanMonSubscriptionCallback;
    if(clientId != NULL && clientId[0] != '\0'){
        EhsSprintf(subscribe_state->topic, topic, clientId);
    }else{
        EhsStrcpy(subscribe_state->topic, topic);
    }
}

ehs_sint16 EhsSubscriptionTopicID(const ehs_char* topic)
{
    if(topic != NULL){
        ehs_uint32 topiclen = EhsStrlen(topic);
        if(topic != NULL && topiclen > 0 && topiclen < EHS_STRING_LENGTH_MAX-1){
            const char* clientId = EhsMqttDevmanMon.mqtt_client_state.clientid;
            char expected_topic[EHS_STRING_LENGTH_MAX+20] = { 0 };// Adding 20 because their is a fixed constant string prefix that can be added to the clientId
            // check topic
            EhsSprintf(expected_topic, EHS_MQTT_SUB_TOPIC_START_OTA, clientId);
            if (EhsStrcmp(expected_topic, topic) == 0) {
                return EHS_MQTT_SUB_TOPIC_START_OTA_ID;
            }
            // check topic
            EhsSprintf(expected_topic, EHS_MQTT_SUB_TOPIC_POST_CHUNK_OTA, clientId);
            if (EhsStrcmp(expected_topic, topic) == 0) {
                return EHS_MQTT_SUB_TOPIC_POST_CHUNK_OTA_ID;
            }
        }
    }
    return EHS_MQTT_UNKNOWN_TOPIC_ID;
}

/* Devman mon mqtt publish functions */
inx_mqtt_publish_state_type* EhsMqttDevmanMonPublications(ehs_uint16 index)
{
    if(index < EHS_MQTT_PUB_TOPICS){
        return &(EhsMqttDevmanMonPub[index]);
    }
    return NULL;
}

void EhsMqttDevmanMonPublish(ehs_uint8 topic_id, const char* payload)
{
    if(topic_id < EHS_MQTT_PUB_TOPICS){
        inx_mqtt_publish_state_type* publish_state = &(EhsMqttDevmanMonPub[topic_id]);
        EhsTPMutex_lock(EhsTPMutex_fbIO);
        EhsStrcpy(publish_state->message, payload);
        publish_state->needProcessing = EHS_TRUE;
        EhsTPMutex_unlock(EhsTPMutex_fbIO);
    }
}

void EhsMqttDevmanMonRegisterPub(inx_mqtt_publish_state_type* publish_state, const char* topic, const char* clientId)
{
    if(publish_state == NULL){
        EHSH_LOG_ERROR("Failed to register devman mon publication \n");
        return;
    }
    publish_state->pFIdata = NULL;
    publish_state->qos = 0;
    publish_state->needProcessing = EHS_FALSE;
    publish_state->always_read = EHS_TRUE;
    publish_state->message[0]='\0';
    if(clientId != NULL && clientId[0] != '\0'){
        EhsSprintf(publish_state->topic, topic, clientId);
    }else{
        EhsStrcpy(publish_state->topic, topic);
    }
}

void EhsMqttDevmanMonConnectPayload(char* payload, ehs_uint32 max_size, ehs_uint32 nMqttConnections)
{
    if(payload == NULL){
        return;
    }
    ehs_uint32 ram_total = EhsHMetaGetRAMTotal();
    ehs_uint32 perc_mem = (ram_total) ? (EhsHMetaGetMiscAppRAMUsed_kB()*100)/ram_total : 0;
    EhsSnprintf(payload, max_size,
                /* payload format */
                "{\"c\":%d,"
                //"\"total_physmem\":%3.1f,"
                //"\"device_system_space_total_KB\":%d,"
                //"\"device_user_space_total_KB\":%d,"
                //"\"device_user_space_used_KB\":%d,"
                "\"percmem_ehs\":%d,"
                //"\"dhcp\":%d,"
                //"\"ubr\":%d,"
                //"\"dns1\":\"%u.%u.%u.%u\","
                //"\"dns2\":\"%u.%u.%u.%u\","
                //"\"netmask\":\"%u.%u.%u.%u\","
                //"\"gateway\":\"%u.%u.%u.%u\","
                "\"percproc_ehs\":\"100\","
                "\"ehs_version\":\"%s\","
                "\"controllerModel\":\"%s\","
                "\"sdl_version\":\"0.0.0\","
                "\"ipaddr\":\"%s\"}",
                /* payload data */
                nMqttConnections,
                perc_mem,
                EhsHMetaGetVersion(),
                EHS_CONTROLLER_MODEL,
                EhsHMetaGetIPAddr()
    );
}

void EhsMqttDevmanMonHandlePing(ehs_bool enabled)
{
    if(enabled == EHS_FALSE){
        return;
    }
    // check if it's time to ping server
    if(gEhsMqttDevmanMonPingTimer == 0 || ((EhsTgtTimer_tickTous(EHS_CURRENT_TIME)-EhsTgtTimer_tickTous(gEhsMqttDevmanMonPingTimer))/1000) > EHS_MQTT_DEVMAN_MON_PING_PERIOD_MS){
        printf("* DEVMAN MON MQTT PING * \n");
        // @TODO - what topic do we ping to ?
        //char payload[EHS_STRING_LENGTH_MAX] = { 0 };
        //EhsMqttDevmanMonPingPayload(payload, EHS_STRING_LENGTH_MAX, 1 /* todo - count disconnect */);
        //EhsMqttDevmanMonPublish(EHS_MQTT_PUB_TOPIC_DIDCONNECT_ID, payload);
        gEhsMqttDevmanMonPingTimer = EHS_CURRENT_TIME;
    }
}