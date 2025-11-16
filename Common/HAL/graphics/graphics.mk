#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#
# Makefile fragment to build the graphics kernel files for EHS.

# Called by ../kernel.mk
#

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

#  NOREV - Defined if no revision number checking is to be performed
# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

##EHS_COMMON_KERNEL_GRAPHICS_PATH=$(EHS_COMMON_KERNEL_PATH)/graphics # this is common support code in the HAL.
##INC_DIRS+=$(EHS_COMMON_KERNEL_GRAPHICS_PATH)

include $(EHS_COMMON_HAL_PATH)/graphics/deps.mk

ifneq ($(EHS_RENDER_MODE),B)

include $(EHS_COMMON_HAL_PATH)/graphics/png/png.mk

include $(EHS_COMMON_HAL_PATH)/graphics/jpeg/jpg.mk

ifdef EHS_GRAPHICS_SVG
include $(EHS_COMMON_HAL_PATH)/graphics/svg/svg.mk
endif

endif #($(EHS_RENDER_MODE),B)

#tell the code we are in business
DEFS+=EHS_GUI_SUPPORT

#GUI code objects to build and link
ifndef EHS_DONT_USE_BASIC_FONTS
	OBJECTS+= font.$(OBJ)
else
	DEFS+=EHS_DONT_USE_BASIC_FONTS
endif

OBJECTS+= graphics.$(OBJ)
OBJECTS+= widget.$(OBJ)

ifneq ($(EHS_RENDER_MODE),B)
OBJECTS+= html.$(OBJ)
OBJECTS+= widget_textbox.$(OBJ)
OBJECTS+= widget_image.$(OBJ)
OBJECTS+= widget_image_png.$(OBJ)
OBJECTS+= widget_image_jpg.$(OBJ) 
OBJECTS+= widget_image_gif.$(OBJ)
OBJECTS+= widget_patch.$(OBJ)
endif
OBJECTS+= widget_viewport.$(OBJ)
#OBJECTS+=widget_video_port.$(OBJ)
#OBJECTS+=widget_image_svg.$(OBJ)  //@tod will this work??

# add extended ui widgets
OBJECTS+= widget_ui.$(OBJ)

VPATH+= $(EHS_COMMON_HAL_PATH)/graphics $(EHS_COMMON_HAL_PATH)/graphics/png

INC_DIRS += $(EHS_COMMON_HAL_PATH)/graphics
INC_DIRS += $(EHS_COMMON_HAL_PATH)/graphics/png

