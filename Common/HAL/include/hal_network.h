/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/* @file hal_network.h
 *
 * @author: inx limited
 *
 */

#ifndef HAL_NETWORK_H
#define HAL_NETWORK_H

#include "target_tcp.h"

//include the following to fill in gaps in mingw headers
#ifdef EHS_MINGW
typedef int socklen_t;
#endif

#ifdef EHS_MINGW
#define EHS_MQTT_PUBLISH_EXPORT __declspec(dllexport)
#else
#define EHS_MQTT_PUBLISH_EXPORT // nothing
#endif
/* Function used for polling the MQTT client process on various platforms*/
EHS_MQTT_PUBLISH_EXPORT ehs_bool EhsMQTTPublishWritePoll(ehs_char *topic, ehs_char* payload, ehs_uint8* qos);



#endif
