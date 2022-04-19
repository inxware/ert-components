#
# Makefile fragment to build the graphics/video-specific code for EHS.

# Called by ../../platform/<platform-type>
# Supports linux/GTK should also support win32 x86 if the dll/.so is added.

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


VPATH+=: $(EHS_TARGET_AV_PATH)/../gst10_common
#INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)# don't need this we'll reference in the .h
INC_DIRS += $(EHS_TARGET_AV_PATH)/../gst10_common

include $(EHS_TARGET_AV_PATH)/../gst10_common/deps.mk

ifeq ($(EHS_NATIVE_DEBIAN_BUILD),yes)
INC_DIRS += /usr/include/gstreamer-1.0
else
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/gstreamer-1.0
endif

OBJECTS += target_dtv.$(OBJ) targetvideo_init.$(OBJ)
