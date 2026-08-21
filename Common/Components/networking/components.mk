#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#
# Makefile fragment to build the networking component code for EHS.

# Called by ../components.mk
#

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

include $(EHS_COMMON_COMPONENTS_PATH)/networking/deps.mk
INC_PATH+= $(EHS_COMPONENT_SUPPORT_INCLUDE)/

# Only add the devman player if we are supporting HTTP devman
ifdef EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER
ifneq ($(EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER),none)
ifdef EHS_DEVMAN_SUPPORT
ifneq ($(EHS_DEVMAN_SUPPORT),none)
	ifneq ($(EHS_DEVMAN_SUPPORT),http)
	     $(error "EHS_DEVMAN_SUPPORT must be set to EHS_DEVMAN_HTTP to use the devman player")
	else
		DEFS += EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER
		OBJECTS += devman_player.$(OBJ)
	endif
else 
$(error "EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER is set without EHS_DEVMAN_SUPPORT set to http")
endif
else
$(error "EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER is set without EHS_DEVMAN_SUPPORT set to http")
endif
endif
endif


# NEnumeratiosn for different platform's networking and interface configuration options
# Stubbed means network config and functions may exist but will not do anything
DEFS += EHS_HAL_NETWORK_CONFIG_STUBBED=1
#This currently implies the W5500 chip vai SPI 1.
DEFS += EHS_HAL_NETWORK_CONFIG_ESP32=2

# Stubbed means interface config and functions may exist but will not do anything
DEFS += EHS_HAL_INTERFACE_CONFIG_STUBBED=1
#This would for example onfigure the WiFiSSID and credentials or allow enable/disable of interfaces.
DEFS += EHS_HAL_INTERFACE_CONFIG_ESP32=2


ifdef EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT
	ifneq ($(EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT),none)
		DEFS += EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT
		OBJECTS += inx-network_config.$(OBJ)	
		# Note this object might get included again because we also use it in the interface config console
	endif
endif


# Enable the TCPIP configuration HAL
ifdef EHS_HAL_NETWORK_CONFIG_SUPPORT
ifneq ($(EHS_HAL_NETWORK_CONFIG_SUPPORT),none)
	DEFS+=EHS_HAL_NETWORK_CONFIG_SUPPORT=$(EHS_HAL_NETWORK_CONFIG_SUPPORT)
# No object for this is as it is implemented in target_main.c currently?
endif
endif

# Enable the Interface (e.g. enable/disable WiFI credentials or Ethernet baud) configuration HAL
ifdef EHS_HAL_INTERFACE_CONFIG_SUPPORT
ifneq ($(EHS_HAL_INTERFACE_CONFIG_SUPPORT),none)	
    OBJECTS += inx-interface_manager.$(OBJ)
    DEFS += EHS_HAL_INTERFACE_CONFIG_SUPPORT=$(EHS_HAL_INTERFACE_CONFIG_SUPPORT)
endif
endif



ifdef EHS_COMPONENTS_NETWORK_URL_GET
ifneq ($(EHS_COMPONENTS_NETWORK_URL_GET),none)
	DEFS += EHS_COMPONENTS_NETWORK_URL_GET
	OBJECTS += url_get.$(OBJ) 
endif
endif

OBJECTS += networking_components.$(OBJ)
# Components moved from user/ directory
ifdef EHS_MQTT_SUPPORT
ifneq ($(EHS_MQTT_SUPPORT),none)
ifneq ($(EHS_MQTT_SUPPORT),)
	DEFS += EHS_MQTT_SUPPORT
	OBJECTS += inx-mqtt_client.$(OBJ)
	OBJECTS += inx-mqtt_publish.$(OBJ)
	OBJECTS += inx-mqtt_subscribe.$(OBJ)
	OBJECTS += inx-mqtt_publish_typed.$(OBJ)
	OBJECTS += inx-mqtt_subscribe_typed.$(OBJ)
endif
endif
endif

# Network socket support
ifneq ($(EHS_NETWORKING_SUPPORT)$(EHS_COMPONENTS_NETWORK_TCPIP_SOCKET),)
ifneq ($(EHS_NETWORKING_SUPPORT),none)
ifneq ($(EHS_COMPONENTS_NETWORK_TCPIP_SOCKET),none)
	OBJECTS += inx-inx-netsocket.$(OBJ)
	OBJECTS += inx-netsocketrsrvr.$(OBJ)
	DEFS += EHS_COMPONENTS_NETWORK_TCPIP_SOCKET
endif
endif
endif

# WiFi station support
ifdef EHS_NETWORK_WIFI_SUPPORT
ifneq ($(EHS_NETWORK_WIFI_SUPPORT),none)
ifneq ($(EHS_NETWORK_WIFI_SUPPORT),)
	OBJECTS += inx-wifi_station.$(OBJ)
endif
endif
endif

# Cellular (LTE-M / NB-IoT) support — the shared connect state machine and the
# Cell_* / EhsSim_* unified API. Platform-independent; the target backend that
# implements the doCellular* contract is selected by EHS_CELLULAR_SUPPORT in
# target/Component-HAL/cellular/cellular.mk.
ifdef EHS_NETWORK_CELLULAR_SUPPORT
ifneq ($(EHS_NETWORK_CELLULAR_SUPPORT),none)
ifneq ($(EHS_NETWORK_CELLULAR_SUPPORT),)
	OBJECTS += inx-cellular_link.$(OBJ)
	# Function blocks: Networking -> Cellular (and -> SIM Provisioning)
	OBJECTS += inx-cell_modem_manager.$(OBJ)
	OBJECTS += inx-cell_status.$(OBJ)
	OBJECTS += inx-cell_power_manager.$(OBJ)
	OBJECTS += inx-sim_manager.$(OBJ)
	OBJECTS += inx-softsim_prov.$(OBJ)
	OBJECTS += inx-esim_profile_mgr.$(OBJ)
	OBJECTS += inx-sgp32_download.$(OBJ)
	OBJECTS += inx-esim_factory_prov.$(OBJ)
endif
endif
endif

# BLE service support
ifdef EHS_NETWORK_BLE_SUPPORT
ifneq ($(EHS_NETWORK_BLE_SUPPORT),none)
ifneq ($(EHS_NETWORK_BLE_SUPPORT),)
	DEFS += EHS_NETWORK_BLE_SUPPORT
	OBJECTS += inx-ble_service.$(OBJ)
	# Add BLE HAL include path for glue layer
	INC_DIRS += $(EHS_TARGET_COMPONENT_HAL_PATH)/ble/$(EHS_NETWORK_BLE_SUPPORT)
endif
endif
endif

VPATH+= $(EHS_COMMON_COMPONENTS_PATH)/networking
