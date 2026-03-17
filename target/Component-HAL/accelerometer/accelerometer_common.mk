# Accelerometer HAL common makefile.
# Included by component-hal.mk when EHS_ACCELEROMETER_SUPPORT is set and not 'none'.
# Selects the implementation directory from the support variable, adds paths,
# compiles target_accelerometer.c, then includes the implementation's target_accelerometer.mk.

EHS_COMMON_ACCELEROMETER_HAL_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/accelerometer
EHS_TARGET_ACCELEROMETER_HAL_PATH=$(EHS_COMMON_ACCELEROMETER_HAL_PATH)/$(EHS_ACCELEROMETER_SUPPORT)

INC_DIRS += $(EHS_COMMON_ACCELEROMETER_HAL_PATH)
INC_DIRS += $(EHS_TARGET_ACCELEROMETER_HAL_PATH)
VPATH    += $(EHS_TARGET_ACCELEROMETER_HAL_PATH)

OBJECTS  += target_accelerometer.$(OBJ)

include $(EHS_TARGET_ACCELEROMETER_HAL_PATH)/target_accelerometer.mk
