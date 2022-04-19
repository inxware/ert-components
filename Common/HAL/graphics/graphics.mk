#
# Makefile fragment to build the graphics kernel files for EHS.

# Called by ../kernel.mk
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

#  NOREV - Defined if no revision number checking is to be performed
# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

##EHS_COMMON_KERNEL_GRAPHICS_PATH=$(EHS_COMMON_KERNEL_PATH)/graphics # this is common support code in the HAL.
##INC_DIRS+=$(EHS_COMMON_KERNEL_GRAPHICS_PATH)

include $(EHS_COMMON_HAL_PATH)/graphics/png/png.mk

include $(EHS_COMMON_HAL_PATH)/graphics/jpeg/jpg.mk

include $(EHS_COMMON_HAL_PATH)/graphics/deps.mk

ifdef EHS_GRAPHICS_SVG
include $(EHS_COMMON_HAL_PATH)/graphics/svg/svg.mk
endif




OBJECTS+= font.$(OBJ)
OBJECTS+= graphics.$(OBJ)
OBJECTS+= html.$(OBJ)
OBJECTS+= widget.$(OBJ)
OBJECTS+= widget_image.$(OBJ)
OBJECTS+= widget_image_png.$(OBJ)
OBJECTS+= widget_textbox.$(OBJ)
OBJECTS+= widget_patch.$(OBJ)
OBJECTS+= widget_image_jpg.$(OBJ) 
OBJECTS+=widget_image_gif.$(OBJ)
OBJECTS+=widget_viewport.$(OBJ)
#OBJECTS+=widget_video_port.$(OBJ)
#OBJECTS+=widget_image_svg.$(OBJ)  //@tod will this work??

VPATH+= $(EHS_COMMON_HAL_PATH)/graphics $(EHS_COMMON_HAL_PATH)/graphics/png

INC_DIRS += $(EHS_COMMON_HAL_PATH)/graphics
INC_DIRS += $(EHS_COMMON_HAL_PATH)/graphics/png

