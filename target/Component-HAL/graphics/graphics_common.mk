

EHS_TARGET_GRAPHICS_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/graphics/$(EHS_GUI_SUPPORT)
DEFS+=EHS_GUI_SUPPORT

# include sourcecode path of this dir in build
include $(EHS_TARGET_GRAPHICS_PATH)/deps.mk
VPATH+=$(EHS_TARGET_GRAPHICS_PATH)
INC_DIRS+=$(EHS_TARGET_GRAPHICS_PATH)

# include sourcecode from components dir in build
include $(EHS_TARGET_GRAPHICS_PATH)/Components/deps.mk
VPATH+= $(EHS_TARGET_GRAPHICS_PATH)/Components
INC_DIRS += $(EHS_TARGET_GRAPHICS_PATH)/Components

include $(EHS_TARGET_GRAPHICS_PATH)/graphics.mk
include $(EHS_TARGET_GRAPHICS_PATH)/Components/components.mk