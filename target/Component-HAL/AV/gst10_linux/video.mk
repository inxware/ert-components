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

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_AV_PATH - path to the current directory (set by platform makefile)
#INC_DIRS += $(EHS_TARGET_AV_PATH)/../../../../vlc_source/include
#INC_DIRS += $(EHS_TARGET_AV_PATH)/../../../../vlc_source/include/src

# include sourcecode from this dir in build


include $(EHS_TARGET_AV_PATH)/../gst10_common/video.mk


export INC_DIRS += /usr/lib/x86_64-linux-gnu/gstreamer-1.0/include

#INC_DIRS += /usr/lib/x86_64-linux-gnu/  /include/
#INC_DIRS += /usr/lib/x86_64-linux-gnu/

LIB+=gstreamer-1.0
#LIB+=gstinterfaces-1.0

OBJECTS += target_os_dtv.$(OBJ)
