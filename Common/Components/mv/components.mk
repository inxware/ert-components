#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#
# Makefile fragment to build the camera component code for EHS.

# Called by ../components.mk
#

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

include $(EHS_COMMON_COMPONENTS_PATH)/mv/deps.mk
INC_PATH+= $(EHS_COMPONENT_SUPPORT_INCLUDE)/

OBJECTS += inx-camera.$(OBJ)

OBJECTS += inx-frame_crop.$(OBJ)

OBJECTS += inx-frame_resize.$(OBJ)

OBJECTS += inx-frame_fmt.$(OBJ)

OBJECTS += mv_components.$(OBJ)

OBJECTS += inx-frame_read_file.$(OBJ)

OBJECTS += inx-frame_show.$(OBJ)

OBJECTS += inx-apriltag.$(OBJ)

OBJECTS += inx-mv_objdet_annotate.$(OBJ)

VPATH+= $(EHS_COMMON_COMPONENTS_PATH)/mv
