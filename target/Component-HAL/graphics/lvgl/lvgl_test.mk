LVGL_DIR_NAME ?= lvgl
LVGL_DIR=$(EHS_TARGET_COMPONENT_HAL_PATH)/graphics/lvgl
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/lvgl.mk
#include $(LVGL_DIR)/lv_drivers/lv_drivers.mk

#DEFS+=EHS_GUI_SUPPORT_MODE_B

LVGL_OBJS = $(CSRCS:.c=.$(OBJ))
OBJECTS+=$(LVGL_OBJS)
# VPATH+=/usr/include/SDL2/
# INC_DIRS+=/usr/include/SDL2/
# LIB_DIRS+=/lib/x86_64-linux-gnu/
# LIB+=SDL2
LIB+=m

OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_helpers.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/EVE_commands.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/FT81x.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/disp_driver.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_tft/disp_spi.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_touch/touch_driver.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_touch/FT81x.$(OBJ)
OBJECTS+=$(LVGL_DIR)/lvgl_esp32_drivers/lvgl_touch/tp_spi.$(OBJ)
INC_DIRS+=$(LVGL_DIR)/lvgl_esp32_drivers/

INC_DIRS+=$(LVGL_DIR)
