#
# Makefile fragment to build the graphics/video-specific code for EHS.

# Called by ../../platform/<platform-type>
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
OBJECTS += loadfont.$(OBJ)
OBJECTS += target_viewport.$(OBJ) 
OBJECTS += targetgfx_init.$(OBJ)
#OBJECTS += dtv_diag.$(OBJ)
#OBJECTS += dtv_remote.$(OBJ)