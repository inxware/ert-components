# UPS HAL common makefile.
# Included by component-hal.mk when EHS_UPS_SUPPORT is set and not 'none'.
# Selects the implementation directory from the support variable, adds paths,
# compiles target_ups.c, then includes the implementation's target_ups.mk.

EHS_COMMON_UPS_HAL_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/ups
EHS_TARGET_UPS_HAL_PATH=$(EHS_COMMON_UPS_HAL_PATH)/$(EHS_UPS_SUPPORT)

INC_DIRS += $(EHS_COMMON_UPS_HAL_PATH)
INC_DIRS += $(EHS_TARGET_UPS_HAL_PATH)
VPATH    += $(EHS_TARGET_UPS_HAL_PATH)

OBJECTS  += target_ups.$(OBJ)

include $(EHS_TARGET_UPS_HAL_PATH)/target_ups.mk
