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

ifdef EHS_DEVMAN_SUPPORT
ifeq ($(EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER),none)
DEFS += EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER__NONE
else
OBJECTS += devman_player.$(OBJ)
endif
endif

# Network config interface target - this is not really how we want to do it lets 
ifeq ($(EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT),none)
	DEFS += EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT__NONE
else 
# Setup some types for the config support? Do we really need this, shouldn't this be done by the target_XXXX.c support for the os-arch automatically
# We will default to a stubbed build
	DEFS+=EHS_NETWORK_CONFIG_TYPE_STUBBED=1
	DEFS+=EHS_NETWORK_CONFIG_TYPE_ESP32=2
	ifeq ($(EHS_NETWORK_CONFIG_TYPE_SUPPORT),esp32)
		DEFS+=EHS_NETWORK_CONFIG_SUPPORT=EHS_NETWORK_CONFIG_TYPE_ESP32
	else
		DEFS+=EHS_NETWORK_CONFIG_SUPPORT=EHS_NETWORK_CONFIG_TYPE_STUBBED
	endif
	OBJECTS += inx-network_config.$(OBJ)
endif

ifeq ($(EHS_COMPONENTS_NETWORK_URL_GET),none)
DEFS += EHS_COMPONENTS_NETWORK_URL_GET__NONE
else
	DEFS += EHS_COMPONENTS_NETWORK_URL_GET
	OBJECTS += url_get.$(OBJ) 
endif

OBJECTS += networking_components.$(OBJ)

VPATH+= $(EHS_COMMON_COMPONENTS_PATH)/networking
