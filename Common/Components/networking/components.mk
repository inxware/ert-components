#
# Makefile fragment to build the networking component code for EHS.

# Called by ../components.mk
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

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code



include $(EHS_COMMON_COMPONENTS_PATH)/networking/deps.mk
INC_PATH+= $(EHS_COMPONENT_SUPPORT_INCLUDE)/

ifdef EHS_DEVMAN_SUPPORT
OBJECTS += devman_player.$(OBJ)
endif

OBJECTS += networking_components.$(OBJ) url_get.$(OBJ) 

VPATH+= $(EHS_COMMON_COMPONENTS_PATH)/networking
