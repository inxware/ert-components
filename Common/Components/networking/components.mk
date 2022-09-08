#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
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
OBJECTS += devman_player.$(OBJ)
endif

OBJECTS += networking_components.$(OBJ) url_get.$(OBJ) 

VPATH+= $(EHS_COMMON_COMPONENTS_PATH)/networking
