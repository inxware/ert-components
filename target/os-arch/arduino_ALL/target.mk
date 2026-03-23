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

# Make sure we build the MCU Profile and include the standard MCU threads for peripheral support
EHS_MCU_TARGET=1

EHS_COMPONENTS_SYSTEMEXEC_SUPPORT=none

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
# Not sure what is in here..
LIB += misc


# Add this path explicitly so that any derived osarches will find things in here if needed.
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/arduino_ALL/
VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/arduino_ALL/

# TODO Should be done properly
DEFS += JSMN_PARENT_LINKS

OBJECTS += target_display.$(OBJ)



