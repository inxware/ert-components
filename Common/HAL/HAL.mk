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
# Some #defines that are used broadly
#============================================================

#============================================================
# Logging
# Per-module log level overrides.
# Set in config.mk as make variables, e.g.:
#   EHS_LOG_LEVEL_GRAPHICS=0x07
# The value is a bitmask: ERROR=0x01, WARNING=0x02, INFO=0x04, ENTER=0x08, EXIT=0x10

# Override the default level for any modules that are not set explicitly.
ifdef EHSH_LOG_DEFAULT_LEVEL
    DEFS += EHSH_LOG_DEFAULT_LEVEL=$(EHSH_LOG_DEFAULT_LEVEL)
endif

# If the build want no verbose logging despite any explicit settings then knoble this.
ifdef EHSH_LOG_MAX_GLOBAL_LEVEL
    DEFS += EHSH_LOG_MAX_GLOBAL_LEVEL=$(EHSH_LOG_MAX_GLOBAL_LEVEL)
endif

ifdef EHS_LOG_LEVEL_UNDEFINED
    DEFS += EHS_LOG_LEVEL_UNDEFINED=$(EHS_LOG_LEVEL_UNDEFINED)
endif
ifdef EHS_LOG_LEVEL_KERNEL
    DEFS += EHS_LOG_LEVEL_KERNEL=$(EHS_LOG_LEVEL_KERNEL)
endif
ifdef EHS_LOG_LEVEL_GRAPHICS
    DEFS += EHS_LOG_LEVEL_GRAPHICS=$(EHS_LOG_LEVEL_GRAPHICS)
endif
ifdef EHS_LOG_LEVEL_LOGGER
    DEFS += EHS_LOG_LEVEL_LOGGER=$(EHS_LOG_LEVEL_LOGGER)
endif
ifdef EHS_LOG_LEVEL_HALMEMORY
    DEFS += EHS_LOG_LEVEL_HALMEMORY=$(EHS_LOG_LEVEL_HALMEMORY)
endif
ifdef EHS_LOG_LEVEL_HALPROCESS
    DEFS += EHS_LOG_LEVEL_HALPROCESS=$(EHS_LOG_LEVEL_HALPROCESS)
endif
ifdef EHS_LOG_LEVEL_HALSTRING
    DEFS += EHS_LOG_LEVEL_HALSTRING=$(EHS_LOG_LEVEL_HALSTRING)
endif
ifdef EHS_LOG_LEVEL_TGTVIEWPORT
    DEFS += EHS_LOG_LEVEL_TGTVIEWPORT=$(EHS_LOG_LEVEL_TGTVIEWPORT)
endif
ifdef EHS_LOG_LEVEL_NETWORK
    DEFS += EHS_LOG_LEVEL_NETWORK=$(EHS_LOG_LEVEL_NETWORK)
endif
ifdef EHS_LOG_LEVEL_DEVMAN
    DEFS += EHS_LOG_LEVEL_DEVMAN=$(EHS_LOG_LEVEL_DEVMAN)
endif
ifdef EHS_LOG_LEVEL_FILE
    DEFS += EHS_LOG_LEVEL_FILE=$(EHS_LOG_LEVEL_FILE)
endif

#============================================================
# OPTIONAL COMMON COMPONENT HAL SUPPORT
#============================================================

#Default we include FILE SYSTEM SUPPORT
ifdef EHS_FILESYSTEM_SUPPORT
ifneq ($(EHS_FILESYSTEM_SUPPORT),none)
include $(EHS_COMMON_HAL_PATH)/file/file.mk
endif
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

# include devman mon support if enabled
ifdef EHS_DEVMAN_SUPPORT
ifneq ($(EHS_DEVMAN_SUPPORT), none)
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
