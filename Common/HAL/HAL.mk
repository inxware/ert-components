#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
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

include $(EHS_COMMON_HAL_PATH)/file/file.mk
include $(EHS_COMMON_HAL_PATH)/date/date.mk
include $(EHS_COMMON_HAL_PATH)/processing/processing.mk
include $(EHS_COMMON_HAL_PATH)/linkedlist/linkedlist.mk
include $(EHS_COMMON_HAL_PATH)/logger/logger.mk
include $(EHS_COMMON_HAL_PATH)/memory/mem.mk
include $(EHS_COMMON_HAL_PATH)/string/string.mk
include $(EHS_COMMON_HAL_PATH)/appmanager/appmanager.mk

#============================================================
# OPTIONAL COMMON COMPONENT HAL SUPPORT
#============================================================

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
include $(EHS_COMMON_HAL_PATH)/lua/lua.mk
endif

#####################
#Parsers
#####################
#Integrated Code
ifdef EHS_JSON_SUPPORT
include $(EHS_COMMON_HAL_PATH)/json/json.mk
endif
#Requires Library support

#Minimal XML - no library needed 
ifdef EHS_DEVMAN_SUPPORT 
#  || defined (EHS_MEDIA_SUPPORT)  && !defined(EHS_SKIP_GNULIBRARIES)
    include $(EHS_COMMON_HAL_PATH)/xml/xml.mk
else ifdef EHS_INCLUDE_XML_SUPPORT
    include $(EHS_COMMON_HAL_PATH)/xml/xml.mk
endif

#########################################
#Networking dependent components support
#########################################
ifdef EHS_NETWORKING_SUPPORT
include $(EHS_COMMON_HAL_PATH)/url/url.mk
endif

ifdef EHS_DEVMAN_MON_SUPPORT
include $(EHS_COMMON_HAL_PATH)/devmanmon/devmanmon.mk
endif

##########################################
# Knit it all together 
##########################################

INC_DIRS += $(EHS_COMMON_HAL_PATH)/include

OBJECTS+= hal.$(OBJ)

VPATH+= $(EHS_COMMON_HAL_PATH)
VPATH+= $(EHS_COMMON_HAL_PATH)/include
