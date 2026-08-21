#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license.
#---------------------------------------------------------------#
#
# Stubbed graphics HAL — selected via EHS_GUI_SUPPORT=stubbed.
#
# Provides the minimum surface (target_viewport.* + targetgfx_init.*) to satisfy
# linkage from Common/HAL/graphics/* and Common/Components/gui/* on platforms
# that have no display attached. Every entry point is a no-op so the FB graph
# can still construct viewport widgets at boot without crashing — they just
# render to nowhere.
#
# Falls into the EHS_RENDER_MODE=A branch in graphics_common.mk (Mode A — inx
# widgets, no LVGL/QT). If a future caller happens to reach a render primitive
# that isn't stubbed here, add it as a no-op below rather than re-enabling a
# real backend.

EHS_PERIPHERALS_GUI_KEYBOARD ?= no
EHS_PERIPHERALS_RCU          ?= no

# include sourcecode from this dir in build
include $(EHS_TARGET_GRAPHICS_PATH)/deps.mk
VPATH    += $(EHS_TARGET_GRAPHICS_PATH)
INC_DIRS += $(EHS_TARGET_GRAPHICS_PATH)

# include sourcecode from components dir in build
include $(EHS_TARGET_GRAPHICS_PATH)/Components/deps.mk
VPATH    += $(EHS_TARGET_GRAPHICS_PATH)/Components
INC_DIRS += $(EHS_TARGET_GRAPHICS_PATH)/Components

OBJECTS += target_viewport.$(OBJ)
OBJECTS += targetgfx_init.$(OBJ)
