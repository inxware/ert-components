# accel_gyro HAL common makefile.
# Included by component-hal.mk when EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT is set and not 'none'.
# Selects the HAL implementation directory from the support variable, adds paths,
# compiles target_accel_gyro.c, then includes the implementation's target_accel_gyro.mk.

EHS_COMMON_ACCEL_GYRO_HAL_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/accel_gyro
EHS_TARGET_ACCEL_GYRO_HAL_PATH=$(EHS_COMMON_ACCEL_GYRO_HAL_PATH)/$(EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT)

INC_DIRS += $(EHS_COMMON_ACCEL_GYRO_HAL_PATH)
INC_DIRS += $(EHS_TARGET_ACCEL_GYRO_HAL_PATH)
VPATH    += $(EHS_TARGET_ACCEL_GYRO_HAL_PATH)

OBJECTS  += target_accel_gyro.$(OBJ)

include $(EHS_TARGET_ACCEL_GYRO_HAL_PATH)/target_accel_gyro.mk
