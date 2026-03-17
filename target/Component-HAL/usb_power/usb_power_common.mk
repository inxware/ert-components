# USB power HAL common makefile.
# Included by component-hal.mk when EHS_USB_POWER_SUPPORT is set and not 'none'.
# Selects the implementation directory from the support variable, adds paths,
# compiles target_usb_power.c, then includes the implementation's target_usb_power.mk.

EHS_COMMON_USB_POWER_HAL_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/usb_power
EHS_TARGET_USB_POWER_HAL_PATH=$(EHS_COMMON_USB_POWER_HAL_PATH)/$(EHS_USB_POWER_SUPPORT)

INC_DIRS += $(EHS_COMMON_USB_POWER_HAL_PATH)
INC_DIRS += $(EHS_TARGET_USB_POWER_HAL_PATH)
VPATH    += $(EHS_TARGET_USB_POWER_HAL_PATH)

OBJECTS  += target_usb_power.$(OBJ)

include $(EHS_TARGET_USB_POWER_HAL_PATH)/target_usb_power.mk
