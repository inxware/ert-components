#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_DEVMAN_MON_PATH - path to the current directory (set by platform makefile)
# Make sure any code switches for this are active

# include sourcecode from this dir in build
include $(EHS_COMMON_HAL_PATH)/devmanmon/deps.mk
VPATH+=$(EHS_COMMON_HAL_PATH)/devmanmon

# If devman mqtt client TLS enabled we need to set some different MQTT configs
ifeq ($(DEVMAN_SERVER_PROTOCOL),mqtts)
    DEFS += EHS_DEVMAN_MQTT_CLIENT_TLS=1
endif
ifeq ($(DEVMAN_SERVER_PROTOCOL),mqtt)
    # #ifdef just look for TLS enabled with EHS_DEVMAN_MQTT_CLIENT_TLS
endif

# Set the devman mon type
ifdef EHS_DEVMAN_SUPPORT
export EHS_DEVMAN_SUPPORT
endif

# Set the devman mon type and set up dependencies.
# Include MQTT components if using MQTT option and/or set XML format requirements for HTTP
OBJECTS += inx-devman_interface.$(OBJ)
OBJECTS += devman_mon.$(OBJ)
ifeq ($(EHS_DEVMAN_SUPPORT),mqtt)
    DEFS += EHS_DEVMAN_SUPPORT=EHS_DEVMAN_MQTT
	OBJECTS += devman_mon_mqtt.$(OBJ)
	OBJECTS += devman_mon_ota.$(OBJ)
else ifeq ($(EHS_DEVMAN_SUPPORT),http)
	DEFS += EHS_DEVMAN_SUPPORT=EHS_DEVMAN_HTTP
	EHS_INCLUDE_XML_SUPPORT=yes
else ifeq ($(EHS_DEVMAN_SUPPORT),stubbed)
	DEFS += EHS_DEVMAN_SUPPORT=EHS_DEVMAN_STUBBED
endif

