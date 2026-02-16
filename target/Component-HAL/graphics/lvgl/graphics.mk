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
OBJECTS+=target_viewport_style.$(OBJ)
OBJECTS+=target_viewport.$(OBJ) 
OBJECTS+=targetgfx_init.$(OBJ)

# Make the dependency list: note this depends on how the component libraries are generated 
# Permutations on static dynamic linking .

LVGL_DIR_NAME ?= lvgl
LVGL_DIR=$(EHS_TARGET_GRAPHICS_PATH)
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/lvgl.mk
include $(LVGL_DIR)/inx_lv_fonts/inx_lv_fonts.mk
include $(LVGL_DIR)/lv_drivers/lv_drivers.mk

#EHS_GUI_SUPPORT=lvgl

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

#TODO - this #if should be on the GRAPHICS_SUPPORT=lvgl?
ifdef EHS_ESP32
# Display drivers
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),gc9a01)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/GC9A01.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_GC9A01
endif
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),ft81x)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/EVE_commands.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/FT81x.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_FT81X
endif
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),ili9341)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/ili9341.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_ILI9341
endif
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),st7789)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/st7789.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_ST7789
endif
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),ssd1306)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/ssd1306.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_SSD1306
endif
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),sh1107)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/sh1107.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_SH1107
endif
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),ili9488)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/ili9488.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_ILI9488
endif
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),ili9486)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/ili9486.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_ILI9486
endif
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),ili9481)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/ili9481.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_ILI9481
endif
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),st7735s)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/st7735s.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_ST7735S
endif
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),st7796s)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/st7796s.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_ST7796S
endif
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),hx8357)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/hx8357.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_HX8357
endif
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),il3820)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/il3820.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_IL3820
endif
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),jd79653a)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/jd79653a.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_JD79653A
endif
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),uc8151d)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/uc8151d.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_UC8151D
endif
ifeq ($(EHS_LVGL_DISPLAY_DRIVER),ra8875)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/ra8875.$(OBJ)
DEFS += CONFIG_LV_TFT_DISPLAY_CONTROLLER_RA8875
endif
# Touch drivers
ifeq ($(EHS_LVGL_TOUCH_DRIVER),ft81x)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_touch/FT81x.$(OBJ)
DEFS += CONFIG_LV_TOUCH_CONTROLLER_FT81X
endif
ifeq ($(EHS_LVGL_TOUCH_DRIVER),xpt2046)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_touch/xpt2046.$(OBJ)
DEFS += CONFIG_LV_TOUCH_CONTROLLER_XPT2046
endif
ifeq ($(EHS_LVGL_TOUCH_DRIVER),ft6x06)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_touch/ft6x36.$(OBJ)
DEFS += LVGL_USE_FT6X06
endif
ifeq ($(EHS_LVGL_TOUCH_DRIVER),stmpe610)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_touch/stmpe610.$(OBJ)
DEFS += LVGL_USE_STMPE610
endif
ifeq ($(EHS_LVGL_TOUCH_DRIVER),adcraw)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_touch/adcraw.$(OBJ)
DEFS += LVGL_USE_ADC_RAW
endif
ifeq ($(EHS_LVGL_TOUCH_DRIVER),ra8875)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_touch/ra8875_touch.$(OBJ)
DEFS += LVGL_USE_RA8875_TOUCH
endif
ifeq ($(EHS_LVGL_TOUCH_DRIVER),cst816t)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_touch/cst816t.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_i2c/i2c_manager.$(OBJ)
DEFS += CONFIG_LV_TOUCH_CONTROLLER_CST816T
DEFS += CONFIG_I2C_MANAGER_0_FREQ_HZ=100000
DEFS += CONFIG_I2C_MANAGER_1_FREQ_HZ=100000
DEFS += CONFIG_LV_I2C
endif
# Common files
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_helpers.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/disp_driver.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/disp_spi.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/esp_lcd_backlight.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_touch/touch_driver.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_touch/tp_spi.$(OBJ)
INC_DIRS+=$(LVGL_DIR)/lvgl_esp32_drivers/
else
# asuming linux or windows (give it at least 1G)
DEFS += LV_MEM_SIZE=1000000000
# this is required for compiling LV_USE_FS_STDIO used by image decoders
DEFS += _GNU_SOURCE
endif

ifeq ($(SDL_FULLSCREEN),yes)
DEFS += SDL_FULLSCREEN=1
endif