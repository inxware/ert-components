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

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_AV_PATH - path to the current directory (set by platform makefile)
###INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/ffmpeg - TODO
# include sourcecode from this dir in build

VPATH+=: $(EHS_TARGET_AV_PATH)/../ffmpeg_common
INC_DIRS += $(EHS_TARGET_AV_PATH)/../ffmpeg_common

include $(EHS_TARGET_AV_PATH)/../ffmpeg_common/deps.mk


OBJECTS += target_dtv.$(OBJ) targetvideo_init.$(OBJ)
