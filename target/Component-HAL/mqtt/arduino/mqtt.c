#include "globals.h"
#include "hal_mqtt.h"
#include "hal-api.h"

#include <ArduinoMqttClient.h>
#if defined(ARDUINO_NANO_RP2040_CONNECT) || defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_NICLA_VISION) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_GIGA) || defined(ARDUINO_OPTA)
  #include <WiFi.h>
#elif defined(ARDUINO_PORTENTA_C33)
  #include <WiFiC3.h>
#elif defined(ARDUINO_UNOR4_WIFI)
  #include <WiFiS3.h>
#endif

#ifndef CONFIG_MQTT_MAX_TOPIC_LENGTH
#define CONFIG_MQTT_MAX_TOPIC_LENGTH (EHS_STRING_LENGTH_MAX)
#endif
#ifndef INX_MQTT_MAX_PAYLOAD_SIZE
#define INX_MQTT_MAX_PAYLOAD_SIZE (EHS_STRING_LENGTH_MAX)
#endif
#define INX_MQTT_MAX_ERROR_MSG_BUFFER_SIZE (EHS_STRING_LENGTH_MAX)

#define EHS_MQTT_ARDUINO_DEBUG 1
#if EHS_MQTT_ARDUINO_DEBUG == 1
#define EHS_ARDUINO_MQTT_LOG(...) EhsStdioPrintf(__VA_ARGS__)
#else
#define EHS_ARDUINO_MQTT_LOG(...)
#endif

#define EHS_MQTT_APP_STATE_IDLE -1

MQTT_State_t MQTT_state = MQTT_STATE_IDLE;
// we can share the payload buffer as it's used in a the same loop
ehs_char* gMqttPayload = NULL;
ehs_char* gPubTopicName = NULL;
char* gSubTopicName = NULL;

ehs_bool gUseTLS = EHS_FALSE;
char* host = NULL;
char* clientid = NULL;
char* username = NULL;
char* password = NULL;
ehs_uint16 port = 1883;

char *clientCertFileName = NULL;
char *clientKeyFileName = NULL;
char *rootCAFileName = NULL;
ehs_bool mqtt_init = EHS_FALSE;

// @TODO - this is how tls support is enabled. Not working yet!
// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate 
//    flashed in the WiFi module.
WiFiClient* wifiClient = NULL;
MqttClient* mqttClient = NULL;

int gMqttAppState = EHS_MQTT_APP_STATE_IDLE;

void mqttSetGlobalState(const MQTT_State_t newState)
{
    MQTT_state = newState;
}

void EhsMqttClientLoopInit()
{
    EHS_ARDUINO_MQTT_LOG("Initialise MQTT\n");
    if(gMqttPayload == NULL){
        gMqttPayload = (ehs_char*)malloc(sizeof(ehs_char) * INX_MQTT_MAX_PAYLOAD_SIZE);
    }
    if(gPubTopicName == NULL){
        gPubTopicName = (ehs_char*)malloc(sizeof(ehs_char) * CONFIG_MQTT_MAX_TOPIC_LENGTH);
    }
    if(gSubTopicName == NULL){
        gSubTopicName = (char*)malloc(sizeof(char) * CONFIG_MQTT_MAX_TOPIC_LENGTH);
    }
    if(mqttClient == NULL && wifiClient == NULL){
        // Do we care for freeing this memory at any point ?
        wifiClient = new WiFiClient;
        mqttClient = new MqttClient( wifiClient );
    }
}

void onMqttMessage(int messageSize)
{
    if(messageSize > 0){
        int size = (messageSize>=INX_MQTT_MAX_PAYLOAD_SIZE) ? INX_MQTT_MAX_PAYLOAD_SIZE-1 : messageSize;
        EhsTPMutex_lock(EhsTPMutex_socketClient);
        const char* topic = mqttClient->messageTopic().c_str();
        int ret = mqttClient->read(gMqttPayload,size);
        EhsTPMutex_unlock(EhsTPMutex_socketClient);
        gMqttPayload[size]=0; // null terminate (just in case)
        //EHS_ARDUINO_MQTT_LOG("r=%d t=%s s=%d m=%s\n",ret,topic,size,gMqttPayload);
        EhsMQTTSubscribeEvent(topic, gMqttPayload, size);
    }
}

/* Used by Mqtt for observing the application status */
void EhsTgtNetworkAppStatus_MQTT(ehs_uint32 status)
{
    switch(status)
    {
        case EHS_APP_LOAD_RESTARTING : {
            // Tearing down the app, make sure to shutdown
            EhsTPMutex_lock(EhsTPMutex_socketClient);
            gMqttAppState = EHS_APP_LOAD_RESTARTING;
            EhsTPMutex_unlock(EhsTPMutex_socketClient);
            break;
        }
        default: {
            break;
        }
    }
}

/* Report error */
void EhsTgtMqttReportError(String err_msg)
{
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    int error_no = mqttClient->connectError();
    EhsTPMutex_unlock(EhsTPMutex_socketClient);
    String errorString = err_msg;
    errorString += " (err=";
    errorString += error_no;
    errorString += ")";
    EhsMQTTReportError(errorString.c_str());
    errorString += "\n";
    EHS_ARDUINO_MQTT_LOG(errorString.c_str());
}

void* EhsMqttClientLoop(void* args)
{
    ehs_bool connect = EHS_FALSE;

    if(mqtt_init == EHS_FALSE){
        EhsMqttClientLoopInit();
        mqtt_init = EHS_TRUE;
    }
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    if (gMqttAppState == EHS_APP_LOAD_RESTARTING){
        EHS_ARDUINO_MQTT_LOG("MQTT - EHS_APP_LOAD_RESTARTING\n");
        mqttSetGlobalState(MQTT_STATE_DISCONNECTED_NEW);
        gMqttAppState = EHS_MQTT_APP_STATE_IDLE; // clear
    }
    EhsTPMutex_unlock(EhsTPMutex_socketClient);

    switch (MQTT_state)
    {
        case MQTT_STATE_IDLE:
        {
            EHS_ARDUINO_MQTT_LOG("MQTT_STATE_IDLE\n");
            mqttSetGlobalState(MQTT_STATE_INIT);
            break;
        }
        case MQTT_STATE_INIT:
        {
            EhsMQTTConnectPoll(&connect, &host, &port, &gUseTLS, &clientid, &username, &password, &clientCertFileName, &clientKeyFileName, &rootCAFileName);
            if (connect == EHS_TRUE)
            {
                EHS_ARDUINO_MQTT_LOG("Init MQTT %s %i\n", host, port);
                EhsTPMutex_lock(EhsTPMutex_socketClient);
                //mqttClient->setKeepAliveInterval(unsigned long interval);
                //mqttClient->setConnectionTimeout(unsigned long timeout);
                mqttClient->setCleanSession(false);
                mqttClient->onMessage(onMqttMessage);
                if(gUseTLS == EHS_TRUE){
                    EHS_ARDUINO_MQTT_LOG("Using tls mqtt is not supported!\n");
                }
                if(clientid){
                    EHS_ARDUINO_MQTT_LOG("Mqtt clinet id=%s\n",clientid);
                    mqttClient->setId(clientid);
                }
                if((username && username[0] != '\0') || (password && password[0] != '\0')){
                    EHS_ARDUINO_MQTT_LOG("Mqtt set custom credentials\n");
                    mqttClient->setUsernamePassword(username, password);
                }
                EhsTPMutex_unlock(EhsTPMutex_socketClient);
                mqttSetGlobalState(MQTT_STATE_DO_CONNECT);
            }
            break;
        }
        case MQTT_STATE_DO_CONNECT:
        {
            EhsMQTTConnectPoll(&connect, &host, &port, &gUseTLS, &clientid, &username, &password, &clientCertFileName, &clientKeyFileName, &rootCAFileName);
            if (connect == EHS_TRUE)
            {
                bool connected = false;
                EHS_ARDUINO_MQTT_LOG("Mqtt connecting ...\n");
                EhsTPMutex_lock(EhsTPMutex_socketClient);
                // start client
                if (gMqttAppState == EHS_APP_LOAD_RESTARTING){
                    // if restart requested, do no continue as connect blocks
                    EhsTPMutex_unlock(EhsTPMutex_socketClient);
                    break;
                }
                if (mqttClient->connect(host, port) && mqttClient->connected()) {
                    mqttSetGlobalState(MQTT_STATE_CONNECTED);
                    connected = true;
                }
                EhsTPMutex_unlock(EhsTPMutex_socketClient);
                if(connected){
                    EhsMQTTConnectEvent(true);
                    EHS_ARDUINO_MQTT_LOG("Mqtt connected!\n");
                } else {
                    // report error
                    EhsTgtMqttReportError("Mqtt failed to connect");
                }
            }
            break;
        }
        case MQTT_STATE_CONNECTED:
        {
            EhsMQTTConnectPoll(&connect, &host, &port, &gUseTLS, &clientid, &username, &password, &clientCertFileName, &clientKeyFileName, &rootCAFileName);
            if (connect == EHS_FALSE)
            {
                // Go to DO_DISCONNECT, reset gMqttConnectionAttempts to 0.
                EHS_ARDUINO_MQTT_LOG("Disconnect requested!\n");
                mqttSetGlobalState(MQTT_STATE_DO_DISCONNECT);
                break;
            }
            // check if connected
            EhsTPMutex_lock(EhsTPMutex_socketClient);
            bool connected = mqttClient->connected();
            bool restarting = (gMqttAppState == EHS_APP_LOAD_RESTARTING);
            if(connected) mqttClient->poll();
            EhsTPMutex_unlock(EhsTPMutex_socketClient);
            if(restarting) break;
            if(!connected){
                EhsTgtMqttReportError("Mqtt lost connection");
                mqttSetGlobalState(MQTT_STATE_DO_DISCONNECT);
                break;
            }
            ehs_uint8 qos_sub = 0;
            ehs_bool subscribe = EHS_FALSE;
            if(EhsMQTTSubscribeWritePoll(gSubTopicName, &subscribe, &qos_sub) == EHS_TRUE)
            {
                if (subscribe)
                {
                    // Subscribe
                    EHS_ARDUINO_MQTT_LOG("Mqtt subscribe=%s, qos=%d)\n", gSubTopicName, (int)qos_sub);
                    EhsTPMutex_lock(EhsTPMutex_socketClient);
                    mqttClient->subscribe(gSubTopicName, qos_sub);
                    EhsTPMutex_unlock(EhsTPMutex_socketClient);
                }
                else
                {
                    // Unsubscribe
                    EHS_ARDUINO_MQTT_LOG("Mqtt unsubscribe=%s\n", gSubTopicName);
                    EhsTPMutex_lock(EhsTPMutex_socketClient);
                    mqttClient->unsubscribe(gSubTopicName);
                    EhsTPMutex_unlock(EhsTPMutex_socketClient);
                }
            }
            ehs_uint8 qos_pub = 0;
            if(EhsMQTTPublishWritePoll(gPubTopicName, gMqttPayload, &qos_pub) == EHS_TRUE)
            {
                // Publish the MQTT message to a topic if it exists. Determine the number of times of publish.
                if (gMqttPayload[0] != 0) {
                    // Publish
                    bool retained = false, dup = false;
                    int qos = (int)qos_pub;
                    int len = (int)EhsStrlen(gMqttPayload);
                    EhsTPMutex_lock(EhsTPMutex_socketClient);
                    mqttClient->beginMessage(gPubTopicName, len, retained, qos, dup);
                    mqttClient->print(gMqttPayload);
                    mqttClient->endMessage();
                    EhsTPMutex_unlock(EhsTPMutex_socketClient);
                }
            }
            break;
        }
        case MQTT_STATE_DO_DISCONNECT:
        {
            EHS_ARDUINO_MQTT_LOG("MQTT_STATE_DO_DISCONNECT\n");
            // DESTROY clinet
            EhsTPMutex_lock(EhsTPMutex_socketClient);
            mqttClient->setCleanSession(false);
            mqttClient->stop();
            EhsTPMutex_unlock(EhsTPMutex_socketClient);
            EhsMQTTConnectEvent(false);
            mqttSetGlobalState(MQTT_STATE_INIT);
            break;
        }
        case MQTT_STATE_DISCONNECTED_NEW:
        {
            EHS_ARDUINO_MQTT_LOG("MQTT_STATE_DISCONNECTED_NEW\n");
            EhsTPMutex_lock(EhsTPMutex_socketClient);
            mqttClient->setCleanSession(true);
            mqttClient->stop();
            EhsTPMutex_unlock(EhsTPMutex_socketClient);
            mqttSetGlobalState(MQTT_STATE_IDLE);
            break;
        }
        default:
        {
            break;
        }
    }
    return NULL;
}
