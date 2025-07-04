#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#
# Makefile fragment to build the graphics/video-specific code for EHS.

# Called by ../../platform/<platform-type>
# Supports linux/GTK should also support win32 x86 if the dll/.so is added.

# @author: inx limited
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


include $(EHS_TARGET_AV_PATH)/../gst_common/video.mk


LIB+=gstreamer-0.10
LIB+=gstinterfaces-0.10

OBJECTS += target_os_dtv.$(OBJ)
