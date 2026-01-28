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



# Network config interface target - this is not really how we want to do it lets 
DEFS+=EHS_NETWORK_CONFIG_TYPE_STUBBED=1
DEFS+=EHS_NETWORK_CONFIG_TYPE_ESP32=2
DEFS+=EHS_INTERFACE_CONFIG_TYPE_STUBBED=1
DEFS+=EHS_INTERFACE_CONFIG_TYPE_ESP32=2
ifdef EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT
ifneq ($(EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT),none)
	# Setup some types for the config support? Do we really need this, shouldn't this be done by the target_XXXX.c support for the os-arch automatically
	# We will default to a stubbed build
	ifeq ($(EHS_NETWORK_CONFIG_TYPE_SUPPORT),esp32)
		DEFS+=EHS_NETWORK_CONFIG_SUPPORT=EHS_NETWORK_CONFIG_TYPE_ESP32
	else
		DEFS+=EHS_NETWORK_CONFIG_SUPPORT=EHS_NETWORK_CONFIG_TYPE_STUBBED
	endif
	ifeq ($(EHS_INTERFACE_CONFIG_TYPE_SUPPORT),esp32)
		DEFS += EHS_INTERFACE_CONFIG_SUPPORT=EHS_INTERFACE_CONFIG_TYPE_ESP32
	else
		DEFS += EHS_INTERFACE_CONFIG_SUPPORT=EHS_INTERFACE_CONFIG_TYPE_STUBBED
	endif
	DEFS += EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT
	OBJECTS += inx-network_config.$(OBJ)
	OBJECTS += inx-interface_manager.$(OBJ)
endif
else
# default to stubbing this function if not defined so need to add object
	DEFS += EHS_NETWORK_CONFIG_SUPPORT=EHS_NETWORK_CONFIG_TYPE_STUBBED
	OBJECTS += inx-network_config.$(OBJ)
	DEFS += EHS_INTERFACE_CONFIG_SUPPORT=EHS_INTERFACE_CONFIG_TYPE_STUBBED
	OBJECTS += inx-interface_manager.$(OBJ)
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
