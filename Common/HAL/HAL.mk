#
# Makefile fragment to build the common hardware abstraction layer files for EHS.

# Called by ../../Makefile
#

# @author: inx limited
# @version: $Revision: 43 $
# @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
# 
# Copyright (c) inx limited, 2007. All rights reserved.
#
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
DEFS+=EHS_GUI_SUPPORT 
include $(EHS_COMMON_HAL_PATH)/graphics/graphics.mk
endif
endif

ifdef EHS_WEBKIT_SUPPORT
ifneq ($(EHS_WEBKIT_SUPPORT), none)
DEFS+=EHS_WEBKIT_SUPPORT 
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
#ifdef EHS_DEVMAN_SUPPORT #@todo we need the ORs for other dependencies here
include $(EHS_COMMON_HAL_PATH)/xml/xml.mk
#else 
#ifdef EHS_MEDIA_SUPPORT#Horrible Logic @todo create a ifnot already inclusion construct
#include $(EHS_COMMON_HAL_PATH)/xml/xml.mk
#endif
#endif

#########################################
#Networking dependent components support
#########################################
ifdef EHS_NETWORKING_SUPPORT
DEFS+=EHS_NETWORKING_SUPPORT 
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
