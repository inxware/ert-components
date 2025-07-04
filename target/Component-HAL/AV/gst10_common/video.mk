#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#
#
# Makefile fragment to build the graphics/video-specific code for EHS.

# Called by ../../platform/<platform-type>
# Supports linux/GTK should also support win32 x86 if the dll/.so is added.

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

# Expected variables


VPATH+=: $(EHS_TARGET_AV_PATH)/../gst10_common
#INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)# don't need this we'll reference in the .h
INC_DIRS += $(EHS_TARGET_AV_PATH)/../gst10_common

include $(EHS_TARGET_AV_PATH)/../gst10_common/deps.mk

#todo2022 we should be using a global contrib middleware support path not conditional everywhere.
ifneq ($(EHS_HOST_DEBIAN_BUILD),)
INC_DIRS += /usr/include/gstreamer-1.0
else
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/gstreamer-1.0
endif
OBJECTS += target_dtv.$(OBJ) targetvideo_init.$(OBJ)
