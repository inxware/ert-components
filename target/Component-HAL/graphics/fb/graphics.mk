#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#
# Makefile fragment to build the graphics/video-specific code for EHS.

# Called by ../../platform/<platform-type>
#

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_GRAPHICS_PATH - path to the current directory (set by platform makefile)

# include sourcecode from this dir in build
include $(EHS_TARGET_GRAPHICS_PATH)/deps.mk
include $(EHS_TARGET_GRAPHICS_PATH)/Components/components.mk
include $(EHS_TARGET_GRAPHICS_PATH)/Components/deps.mk
VPATH+=: $(EHS_TARGET_GRAPHICS_PATH)
INC_DIRS += $(EHS_TARGET_GRAPHICS_PATH)

# include sourcecode from components dir in build
include $(EHS_TARGET_GRAPHICS_PATH)/Components/deps.mk
VPATH+=: $(EHS_TARGET_GRAPHICS_PATH)/Components
INC_DIRS += $(EHS_TARGET_GRAPHICS_PATH)/Components

OBJECTS += target_data.$(OBJ)
ifndef EHS_DONT_USE_BASIC_FONTS
OBJECTS += loadfont.$(OBJ)
endif
OBJECTS += target_viewport.$(OBJ) 
OBJECTS += targetgfx_init.$(OBJ)
#OBJECTS += dtv_diag.$(OBJ)
#OBJECTS += dtv_remote.$(OBJ)
