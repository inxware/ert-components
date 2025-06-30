/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/
 
/** @file target_file.h
 * The target-specific declarations required to support the HAL for file services
 * are defined here. This file should only be included by hal_file.h
 *
 * @author: inx limited
 *
 */
/**
 * @page Verification Verification report
 * @section hal_file_h
 * @anchor hal_file_h
 * @subsection misra MISRA compliance:
 * Demonstrated MISRA compliant on
 * Last modified on $Date$
 *
 * This file contained the following derogations to the MISRA standard.
 * - Rule 20.9 "Stdio.h shall not be used in production code". Acceptable to use stdio.h on this platform
 */


#ifndef EHS_TARGET_NETWORK_H
#define EHS_TARGET_NETWORK_H

#include "globals.h"


void EhsTgtNetworkSetAppStatus(ehs_uint32 status);
/* Initalise network and start LP netwrok thread */
ehs_bool EhsTgtNetworkInit();
ehs_bool EhsTgtNetworkStartWifi(const ehs_char* ssid, const ehs_char* pass);

/* returns true if the network is connected */
ehs_bool EhsTgtNetworkConnected();

/* reads wifi ssid from fs and outputs it to a buffer */
void EhsTgtNetworkGetSSID(ehs_char* ssid_buff, ehs_uint32 buff_size);
/* reads wifi pass from fs and outputs it to a buffer */
void EhsTgtNetworkGetPass(ehs_char* pass_buff, ehs_uint32 buff_size);

/* sets wifi ssid */
void EhsTgtNetworkSetSSID(const ehs_char* ssid_buff, ehs_bool save);
/* sets wifi pass */
void EhsTgtNetworkSetPass(const ehs_char* pass_buff, ehs_bool save);

/* outputs ip address to a buffer */
void EhsTgtNetworkGetIPv4(ehs_char* ip_buff, ehs_uint32 buff_size);
/* outputs mac address to a buffer */
void EhsTgtNetworkGetMAC(ehs_char* mac_buff, ehs_uint32 buff_size);

#ifdef EHS_MQTT_SUPPORT
/* Used by Mqtt for observing the application status */
void EhsTgtNetworkAppStatus_MQTT(ehs_uint32 status);
#endif

#endif // EHS_TARGET_NETWORK_H
