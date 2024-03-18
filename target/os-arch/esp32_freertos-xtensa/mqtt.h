/**
 * @file
 * MQTT client
 */

// see https://www.nongnu.org/lwip/2_0_x/group__mqtt.html
// from  https://github.com/ARMmbed/lwip/blob/master/src/apps/mqtt/mqtt.c

/*
 * Copyright (c) 2016 Erik Andersson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Erik Andersson
 *
 */
#ifndef LWIP_HDR_APPS_MQTT_CLIENT_H
#define LWIP_HDR_APPS_MQTT_CLIENT_H



/************************************************************************************************************/




typedef enum
{
    INX_NETWORK_CONNECTION_MODE_OFF,
    INX_NETWORK_CONNECTION_MODE_READ,
    INX_NETWORK_CONNECTION_MODE_WRITE
} inxNetworkConnectionMode_t;

typedef enum
{
    NETWORK_STATE_INIT,
    NETWORK_STATE_WAIT_FOR_PHY,
    NETWORK_STATE_START_DHCP,
    NETWORK_STATE_WAITING_FOR_DHCP,
    NETWORK_STATE_CONNECTED,
    NETWORK_STATE_DO_DISCONNECT
} NETWORK_State_t;



typedef enum
{
    INX_RTC_PART_YEAR,
    INX_RTC_PART_MONTH,
    INX_RTC_PART_DAY,
    INX_RTC_PART_HOUR,
    INX_RTC_PART_MINUTE,
    INX_RTC_PART_SECOND
} inxRTCPartEnum_t;


/************************************************************************************************************/
typedef struct mqtt_client_t mqtt_client_t;

/** @ingroup mqtt
 * Default MQTT port */
#define MQTT_PORT     1883
#define MQTT_PORT_TLS 8883

typedef enum
{
    MQTT_STATE_INIT,
    MQTT_STATE_IDLE,
    MQTT_STATE_DNS_LOOKUP,
    MQTT_STATE_DO_CONNECT,
    MQTT_STATE_DO_TLS_HANDSHAKE,
    MQTT_STATE_WAIT_FOR_CONNECTION,
    MQTT_STATE_CONNECTED,
    MQTT_STATE_DO_PUBLISH,
    MQTT_STATE_DO_DISCONNECT,
    MQTT_STATE_DISCONNECTED,
    MQTT_STATE_INVALID
} MQTT_State_t;


/*---------------------------------------------------------------------------------------------- */
/* Connection with server */

/**
 * @ingroup mqtt
 * Client information and connection parameters */
struct mqtt_connect_client_info_t
{
    /** Client identifier, must be set by caller */
    const char *client_id;
    /** User name and password, set to NULL if not used */
    const char* client_user;
    const char* client_pass;
    /** keep alive time in seconds, 0 to disable keep alive functionality*/
    unsigned int keep_alive;
    /** will topic, set to NULL if will is not to be used,
        will_msg, will_qos and will retain are then ignored */
    const char* will_topic;
    const char* will_msg;
    unsigned char will_qos;
    unsigned char will_retain;
};

/**
 * @ingroup mqtt
 * Connection status codes */
typedef enum
{
    MQTT_CONNECT_ACCEPTED                 = 0,
    MQTT_CONNECT_REFUSED_PROTOCOL_VERSION = 1,
    MQTT_CONNECT_REFUSED_IDENTIFIER       = 2,
    MQTT_CONNECT_REFUSED_SERVER           = 3,
    MQTT_CONNECT_REFUSED_USERNAME_PASS    = 4,
    MQTT_CONNECT_REFUSED_NOT_AUTHORIZED_  = 5,
    MQTT_CONNECT_DISCONNECTED             = 256,
    MQTT_CONNECT_TIMEOUT                  = 257
} mqtt_connection_status_t;

typedef enum
{
    MQTT_QOS_0 = 0,
    MQTT_QOS_1 = 1,
    MQTT_QOS_2 = 2
} mqtt_qos_t;

/**
 * @ingroup mqtt
 * Function prototype for mqtt connection status callback. Called when
 * client has connected to the server after initiating a mqtt connection attempt by
 * calling mqtt_connect() or when connection is closed by server or an error
 *
 * @param client MQTT client itself
 * @param arg Additional argument to pass to the callback function
 * @param status Connect result code or disconnection notification @see mqtt_connection_status_t
 *
 */
typedef void (*mqtt_connection_cb_t)(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);

/**
 * @ingroup mqtt
 * Data callback flags */
enum
{
    /** Flag set when last fragment of data arrives in data callback */
    MQTT_DATA_FLAG_LAST = 1
};

#endif /* LWIP_HDR_APPS_MQTT_CLIENT_H */




