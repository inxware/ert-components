#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#
# Makefile fragment to build the common hardware abstraction layer files for EHS.

# Called by ../../Makefile
#

# @author: inx limited
#
# Predefined variables

#  OBJ - File extension for object files

#  NOREV - Defined if no revision number checking is to be performed
# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code
#  EHS_GUI_SUPPORT - defined if this target supports graphics

include $(EHS_COMMON_HAL_PATH)/include/deps.mk
#============================================================
# MANDATORY KERNEL HAL SUPPORT
#============================================================

include $(EHS_COMMON_HAL_PATH)/date/date.mk
include $(EHS_COMMON_HAL_PATH)/processing/processing.mk
include $(EHS_COMMON_HAL_PATH)/linkedlist/linkedlist.mk
include $(EHS_COMMON_HAL_PATH)/logger/logger.mk
include $(EHS_COMMON_HAL_PATH)/memory/mem.mk
include $(EHS_COMMON_HAL_PATH)/string/string.mk
include $(EHS_COMMON_HAL_PATH)/appmanager/appmanager.mk
include $(EHS_COMMON_HAL_PATH)/ota/ota.mk
include $(EHS_COMMON_HAL_PATH)/hashmap/hashmap.mk

#============================================================
# OPTIONAL COMMON COMPONENT HAL SUPPORT
#============================================================

#Default we include FILE SYSTEM SUPPORT
ifneq ($(EHS_FILESYSTEM_SUPPORTNONE),none)
include $(EHS_COMMON_HAL_PATH)/file/file.mk
endif

ifdef EHS_CONFIGS_SUPPORT
ifneq ($(EHS_CONFIGS_SUPPORT),none)
	include $(EHS_COMMON_HAL_PATH)/configs/configs.mk
endif
endif

ifdef EHS_GUI_SUPPORT
ifneq ($(EHS_GUI_SUPPORT), none) 
include $(EHS_COMMON_HAL_PATH)/graphics/graphics.mk
endif
endif

ifdef EHS_WEBKIT_SUPPORT
ifneq ($(EHS_WEBKIT_SUPPORT), none)
	include $(EHS_COMMON_HAL_PATH)/webkit/webkit.mk
endif
endif

ifdef EHS_LUA_SUPPORT
ifneq ($(EHS_LUA_SUPPORT), none)
	include $(EHS_COMMON_HAL_PATH)/lua/lua.mk
endif
endif

#####################
#Parsers
#####################
#Integrated Code
ifdef EHS_JSON_SUPPORT
ifneq ($(EHS_JSON_SUPPORT), none)
	include $(EHS_COMMON_HAL_PATH)/json/json.mk
endif
endif


#Note this includes the internal minimal XML parser used in Devman core.
#libxml2 is deselcted internally if not needed
include $(EHS_COMMON_HAL_PATH)/xml/xml.mk

#########################################
#Networking dependent components support
#########################################
ifdef EHS_NETWORKING_SUPPORT
ifneq ($(EHS_NETWORKING_SUPPORT), none)
	include $(EHS_COMMON_HAL_PATH)/url/url.mk
endif
endif

# Define devman mon type enum
DEFS+=EHS_DEVMAN_MON_CURL=1
DEFS+=EHS_DEVMAN_MON_MQTT=2
# include devman mon support if enabled
ifdef EHS_DEVMAN_MON_SUPPORT
ifneq ($(EHS_DEVMAN_MON_SUPPORT), none)
	include $(EHS_COMMON_HAL_PATH)/devmanmon/devmanmon.mk
endif
endif

##########################################
# Knit it all together 
##########################################
# THis is done earlier in platform.mk so all the target stuff can work
#INC_DIRS += $(EHS_COMMON_HAL_PATH)/include

OBJECTS+= hal.$(OBJ)

VPATH+= $(EHS_COMMON_HAL_PATH)
VPATH+= $(EHS_COMMON_HAL_PATH)/include
