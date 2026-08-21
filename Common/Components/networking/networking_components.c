/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file networking_components.c
 * Contains the static definition of the function block reference table for EHS
 *
 * @author: inx limited
 *
 */

#include "blockref_table.h"

// Components moved from user directory
#ifdef EHS_COMMS_API_SUPPORT
#include "inx-inx-netsocket.h"
#include "inx-netsocketrsrvr.h"
#endif

#ifdef EHS_MQTT_SUPPORT
#include "inx-mqtt_publish.h"
#include "inx-mqtt_subscribe.h"
#include "inx-mqtt_publish_typed.h"
#include "inx-mqtt_subscribe_typed.h"
#include "inx-mqtt_client.h"
#endif

#ifdef EHS_NETWORK_WIFI_SUPPORT
#include "inx-wifi_station.h"
#endif

#ifdef EHS_NETWORK_BLE_SUPPORT
#include "inx-ble_service.h"
#endif//EHS_NETWORK_BLE_SUPPORT

#ifdef EHS_NETWORK_CELLULAR_SUPPORT
#include "inx-cell_modem_manager.h"
#include "inx-cell_status.h"
#include "inx-cell_power_manager.h"
#include "inx-sim_manager.h"
#include "inx-softsim_prov.h"
#include "inx-esim_profile_mgr.h"
#include "inx-sgp32_download.h"
#include "inx-esim_factory_prov.h"
#endif//EHS_NETWORK_CELLULAR_SUPPORT

#include "devman_player.h"
#include "url_get.h"
#include "inx-network_config.h"
#include "inx-interface_manager.h"

#ifdef EHS_COMPONENT_NETWORKING_SUPPORT

EHS_C_CPP_EXPORT const EhsBlockRefType EhsBlockRefTable_networking[] =
{
    /* devman_player.h */
#ifdef EHS_DEVMAN_SUPPORT
#ifdef EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DevmanPlayer,EHS_FB_ID_DevmanPlayer, DevmanPlayer),
#endif
#endif // EHS_DEVMAN_SUPPORT

#ifdef EHS_COMPONENTS_NETWORK_URL_GET
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_UrlGet,EHS_FB_ID_UrlGet,UrlGet),
#endif

#ifdef EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_network_config,INXWARE_FB_ID_network_config,network_config),
#endif

// Components moved from user directory
#if defined(EHS_NETWORKING_SUPPORT) || defined(EHS_COMPONENTS_NETWORK_TCPIP_SOCKET)
#ifdef EHS_COMMS_API_SUPPORT
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_netsocketsrvr,INXWARE_FB_ID_netsocketsrvr ,netsocketsrvr),
	EHS_BLOCKREF_ENTRY(INXWARE_FB_NAME_netSocket,INXWARE_FB_ID_netSocket ,netSocket),
#endif // EHS_COMMS_API_SUPPORT
#endif // EHS_NETWORKING_SUPPORT or EHS_COMPONENTS_NETWORK_TCPIP_SOCKET

#ifdef EHS_MQTT_SUPPORT
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_mqtt_client ,INXWARE_FB_ID_mqtt_client,mqtt_client),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_mqtt_subscribe,INXWARE_FB_ID_mqtt_subscribe ,mqtt_subscribe),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_mqtt_publish,INXWARE_FB_ID_mqtt_publish ,mqtt_publish),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_mqtt_subscribe_typed,INXWARE_FB_ID_mqtt_subscribe_typed ,mqtt_subscribe_typed),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_mqtt_publish_typed,INXWARE_FB_ID_mqtt_publish_typed ,mqtt_publish_typed),
#endif

#ifdef EHS_NETWORK_WIFI_SUPPORT
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_wifi_station,INXWARE_FB_ID_wifi_station,wifi_station),
#endif
#ifdef EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_interface_manager,INXWARE_FB_ID_interface_manager,interface_manager),
#endif
#ifdef EHS_NETWORK_BLE_SUPPORT
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_ble_service,INXWARE_FB_ID_ble_service,ble_service),
#endif
#ifdef EHS_NETWORK_CELLULAR_SUPPORT
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_cell_modem_manager,INXWARE_FB_ID_cell_modem_manager,cell_modem_manager),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_cell_status,INXWARE_FB_ID_cell_status,cell_status),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_cell_power_manager,INXWARE_FB_ID_cell_power_manager,cell_power_manager),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_sim_manager,INXWARE_FB_ID_sim_manager,sim_manager),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_softsim_prov,INXWARE_FB_ID_softsim_prov,softsim_prov),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_esim_profile_mgr,INXWARE_FB_ID_esim_profile_mgr,esim_profile_mgr),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_sgp32_download,INXWARE_FB_ID_sgp32_download,sgp32_download),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_esim_factory_prov,INXWARE_FB_ID_esim_factory_prov,esim_factory_prov),
#endif
	{0}
};

#endif /* EHS_COMPONENT_NETWORKING_SUPPORT */
