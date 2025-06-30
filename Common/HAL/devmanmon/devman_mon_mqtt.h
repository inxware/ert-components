
/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/*
 * @file devman_mon_mqtt.h
 *
 * @author: inx limited
 *
 */

#ifndef COMMON_HAL_DEVMAN_MON_MQTT_H_
#define COMMON_HAL_DEVMAN_MON_MQTT_H_

#include "hal_mqtt.h"

/* Used for setting period for pinging server */
#define EHS_MQTT_DEVMAN_MON_PING_PERIOD_MS 20 * 1000
#define EHS_MQTT_DEVMAN_MON_PING_ENABLED EHS_FALSE

#define EHS_MQTT_UNKNOWN_TOPIC_ID -1

/* MQTT Devman mon subscriptions */
#define EHS_MQTT_SUB_TOPICS 2 /* Make sure to increase this when adding new topic */
/* EHS_MQTT_SUB_TOPIC_START_OTA */
#define EHS_MQTT_SUB_TOPIC_START_OTA_ID 0
#define EHS_MQTT_SUB_TOPIC_START_OTA "firmware/startota/%s"
/* EHS_MQTT_SUB_TOPIC_POST_CHUNK_OTA */
#define EHS_MQTT_SUB_TOPIC_POST_CHUNK_OTA_ID 1
#define EHS_MQTT_SUB_TOPIC_POST_CHUNK_OTA "firmwarechunk/post/%s"

/* MQTT Devman mon publish */
#define EHS_MQTT_PUB_TOPICS 2 /* Make sure to increase this when adding new topic */
/* EHS_MQTT_PUB_TOPIC_GET_CHUNK_OTA */
#define EHS_MQTT_PUB_TOPIC_GET_CHUNK_OTA_ID 0
#define EHS_MQTT_PUB_TOPIC_GET_CHUNK_OTA "firmwarechunk/get/group1/%s"
/* EHS_MQTT_PUB_TOPIC_DIDCONNECT */
#define EHS_MQTT_PUB_TOPIC_DIDCONNECT_ID 1
#define EHS_MQTT_PUB_TOPIC_DIDCONNECT "connectivity/group1/%s/didconnect"

/* Initalise devman mon mqtt handler and returns its pointer */
EhsMqttDevmanMon_t* EhsMqttDevmanMonSupportInit();

/* Returns instance of devman mon mqtt clinet */
inx_mqtt_client_state_type_mine* EhsMqttDevmanMonMqttClient();

/* Getter and setter for devman mon mqtt state */
EhsMqttDevmanMonState_t EhsGetMqttDevmanMonState();
void EhsSetMqttDevmanMonState(EhsMqttDevmanMonState_t state);

/* Returns true when Devman mon mqtt client is connected */
ehs_bool EhsGetMqttDevmanMonConnected();

/* Does any required actions when mqtt is identified as disconnected */
void EhsMqttDevmanMonHandleDisconnected();

/* Does any required actions when mqtt is identified as connected */
void EhsMqttDevmanMonHandleConnected();

/* Devman mon mqtt subscription functions */
inx_mqtt_subscribe_state_type* EhsMqttDevmanMonSubscriptions(ehs_uint16 index);

/* Devman mon mqtt subscription callback function. It is implemented in devman_mon.c where the incoming payload needs handling. */
ehs_bool EhsMqttDevmanMonSubscriptionCallback(struct inx_mqtt_subscribe_state* pState, char* payload, ehs_sint32 payloadSize);

/* Returns subscription topic id for devman mon specific topic string. It uses clinet ID as format argument where requred. */
ehs_sint16 EhsSubscriptionTopicID(const ehs_char* topic);

/* Register substate of the Device Manager (Not sure what kind of state we are talking here - overall hardware status?)*/
void EhsMqttDevmanMonRegisterSub(inx_mqtt_subscribe_state_type* subscribe_state, const char* topic, const char* clientId);
void EhsMqttDevmanMonRegisterPub(inx_mqtt_publish_state_type* publish_state, const char* topic, const char* clientId);

/* Devman mon mqtt publish functions */
inx_mqtt_publish_state_type* EhsMqttDevmanMonPublications(ehs_uint16 index);

/* Used for publishing a payload string (must be null terminated!) for a defined topic ID.*/
void EhsMqttDevmanMonPublish(ehs_uint8 topic_id, const char* payload);

#endif /* COMMON_HAL_DEVMAN_MON_MQTT_H_ */