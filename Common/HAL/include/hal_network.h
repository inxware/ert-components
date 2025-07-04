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

typedef struct EhsNetworkConfigData
{
    ehs_uint16 mode;
    const ehs_char* address;
    const ehs_char* gateway;
    const ehs_char* mask;
    const ehs_char* dns;
    ehs_bool save;
} EhsNetworkConfigDataType;

/* Returns true when the eRT target network is connected */
ehs_bool EhsNetworkIsConnected();

/* Configures the traget network interface. Returns error code ID. */
ehs_sint32 EhsNetworkConfigureInterface(const EhsNetworkConfigDataType* config);

#endif
