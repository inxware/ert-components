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
#  EHS_TARGET_VIDEO_PATH - path to the current directory (set by platform makefile)

# include sourcecode from this dir in build

OBJECTS += target_dtv.$(OBJ)
OBJECTS += targetvideo_init.$(OBJ)

VPATH+=: STAPI_DIR..
OBJECTS += libEhsStapi.so