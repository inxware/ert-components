/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
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

/* Network config error code IDs */
#define EHS_NETWORK_CONFIG_NO_ERROR_ID 0
#define EHS_NETWORK_CONFIG_FAILED_STATIC_ID 1
#define EHS_NETWORK_CONFIG_FAILED_DHCP_ID 2
#define EHS_NETWORK_CONFIG_FAILED_DNS1_ID 3
#define EHS_NETWORK_CONFIG_FAILED_NOT_SUPPORTED_ID 3
#define EHS_NETWORK_CONFIG_INVALID_PARAM_ID 4

typedef struct EhsNetworkConfigData
{
    ehs_uint16 mode;
    const ehs_char* address;
    const ehs_char* gateway;
    const ehs_char* mask;
    const ehs_char* dns;
    ehs_bool save;
} EhsNetworkConfigDataType;

#define EHS_NETWORK_INTERFACE_NAME_LENGTH 20
typedef struct EhsNetworkInterfaceConfigData
{
    ehs_bool b_wifi_enable;
    //ehs_char s_wifi_if_name[EHS_NETWORK_INTERFACE_NAME_LENGTH];
    ehs_bool b_eth_enable;
    //ehs_char s_eth_if_name[EHS_NETWORK_INTERFACE_NAME_LENGTH];
    ehs_bool save;
} EhsNetworkInterfaceConfigDataType;

/* Returns true when the eRT target network is connected */
ehs_bool EhsNetworkIsConnected();

/* Configures the target network. Returns error code ID. */
ehs_sint32 EhsNetworkConfigure(const EhsNetworkConfigDataType* config);

/* Configures the target network interface. Returns error code ID */
ehs_sint32 EhsNetworkInterfaceConfigure(const EhsNetworkInterfaceConfigDataType* config);

ehs_bool EhsNetworkInterfaceWifiIsEnabled();

ehs_bool EhsNetworkInterfaceEthIsEnabled();

/* Ethernet Specific functions called generally */

ehs_bool ehs_eth_init();
ehs_bool eth_deinit();

#endif
