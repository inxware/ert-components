#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# HW independent linux build environment parameters 

#Arduino doesn't support LWIP by default
#ifndef EHS_COMMS_API_SUPPORT
#  EHS_COMMS_API_SUPPORT=lwip
#endif

#EHS_NETWORKING_SUPPORT=disabled
#EHS_NETWORKING_HTTP_SOCKET=stub
#EHS_NETWORKING_HTTP_CLIENT=stub
#EHS_NETWORKING_HTTP_SERVER=stub
#EHS_NETWORKING_MQTT_CLIENT=stub
#EHS_COMMS_API=NONE

include $(EHS_TARGETS_ROOT_PATH)/os-arch/arduino_ALL/config.mk

# This will be changed with Arduino specific build in the future
#ifndef EHS_COMMS_API_SUPPORT
#	export  EHS_COMMS_API_SUPPORT=bsdsockets
#    DEFS += $(EHS_COMMS_API_SUPPORT)
#endif

EHS_MCU_TARGET=1

EHS_COMPONENTS_SYSTEMEXEC_SUPPORT=none
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/arduino_ALL/
VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/arduino_ALL/

#VPATH += $(EHS_TARGET_OS_HW_PATH)/HAL
#VPATH += $(EHS_TARGET_OS_HW_PATH)/Components

INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/HAL
DEFS += JSMN_PARENT_LINKS
#These are the core mandatory target apecific porting components needed to run eRT. 
OBJECTS += target_logger.$(OBJ)
OBJECTS += target_math.$(OBJ) 
ifneq ($(EHS_FILESYSTEM_SUPPORT),stubbed)
OBJECTS += target_file.$(OBJ)
else
DEFS += INX_SODL_IN_FLASH
OBJECTS += target_file_flash.$(OBJ)
endif
OBJECTS += target_process.$(OBJ) 
OBJECTS += targetos_init.$(OBJ)
OBJECTS += target_main.$(OBJ) 
OBJECTS += target_time.$(OBJ)

ifdef EHS_COMPONENT_NETWORKING_SUPPORT
ifneq ($(EHS_COMPONENT_NETWORKING_SUPPORT),stubbed)
OBJECTS += target_network.$(OBJ)
endif

ifdef EHS_DEBUG_TCPIP_CONSOLE
OBJECTS += target_console_server.$(OBJ)
endif
endif

# These are Arduino dependencies
LIB += libs
LIB += misc


#TODO2025  - Move to the component HAL
# Include Arduino LSM6DS3 library source
VPATH += /home/inxware/Arduino/libraries/Arduino_LSM6DS3/src
OBJECTS += target_accel_gyro.$(OBJ)
OBJECTS += LSM6DS3.$(OBJ)
OBJECTS += target_display.$(OBJ)
