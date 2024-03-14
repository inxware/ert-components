#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#
# Makefile fragment to build the graphics/video-specific code for EHS.

# Called by ../../platform/<platform-type>
#

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_GRAPHICS_PATH - path to the current directory (set by platform makefile)


EHS_PERIPHERALS_GUI_KEYBOARD=yes
EHS_PERIPHERALS_RCU=yes

# include sourcecode from this dir in build
include $(EHS_TARGET_GRAPHICS_PATH)/deps.mk
VPATH+=$(EHS_TARGET_GRAPHICS_PATH)
INC_DIRS+=$(EHS_TARGET_GRAPHICS_PATH)

# include sourcecode from components dir in build
include $(EHS_TARGET_GRAPHICS_PATH)/Components/deps.mk
VPATH+= $(EHS_TARGET_GRAPHICS_PATH)/Components
INC_DIRS += $(EHS_TARGET_GRAPHICS_PATH)/Components
# No extra lib paths - all are in the target_libs dir


# PPP: Moved to Video Diectory OBJECTS += target_dtv.$(OBJ)
OBJECTS+=target_viewport.$(OBJ) 
OBJECTS+=targetgfx_init.$(OBJ)

# Make the dependency list: note this depends on how the component libraries are generated 
# Permutations on static dynamic linking .

LVGL_DIR_NAME ?= lvgl
LVGL_DIR=$(EHS_TARGET_GRAPHICS_PATH)
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/lvgl.mk
include $(LVGL_DIR)/lv_drivers/lv_drivers.mk

DEFS+=EHS_GUI_SUPPORT_MODE_B
DEFS+=EHS_GUI_SUPPORT_MODE_B_LVGL

DEFS+=PRINTF_SUPPORT_FLOAT=1

LVGL_OBJS = $(CSRCS:.c=.$(OBJ))
OBJECTS+=$(LVGL_OBJS)

ifeq ($(EHS_OS), linux)
	VPATH+=/usr/include/SDL2/
	INC_DIRS+=/usr/include/SDL2/
	ifeq ($(EHS_ARCH), amd64)
		LIB_DIRS+=/lib/x86_64-linux-gnu/
	endif
	LIB+=SDL2
else
	ifeq ($(EHS_OS), mingw)
		LIB+=SDL2
	endif
endif

LIB+=m

ifdef EHS_ESP32
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_helpers.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/EVE_commands.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/FT81x.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/disp_driver.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/disp_spi.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/esp_lcd_backlight.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_touch/touch_driver.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_touch/FT81x.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_touch/tp_spi.$(OBJ)
INC_DIRS+=$(LVGL_DIR)/lvgl_esp32_drivers/
else
# asuming linux or windows (give it at least 1G)
DEFS += LV_MEM_SIZE=1000000000
DEFS += _GNU_SOURCE # this is required for compiling LV_USE_FS_STDIO used by image decoders
endif
