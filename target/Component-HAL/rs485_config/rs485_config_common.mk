# RS-485 config HAL common makefile.
# Included by component-hal.mk when EHS_RS485_CONFIG_SUPPORT is set and not 'none'.
# Selects the implementation directory from the support variable, adds paths,
# compiles target_rs485_config.c, then includes the implementation's target_rs485_config.mk.

EHS_COMMON_RS485_CONFIG_HAL_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/rs485_config
EHS_TARGET_RS485_CONFIG_HAL_PATH=$(EHS_COMMON_RS485_CONFIG_HAL_PATH)/$(EHS_RS485_CONFIG_SUPPORT)

INC_DIRS += $(EHS_COMMON_RS485_CONFIG_HAL_PATH)
INC_DIRS += $(EHS_TARGET_RS485_CONFIG_HAL_PATH)
VPATH    += $(EHS_TARGET_RS485_CONFIG_HAL_PATH)

OBJECTS  += target_rs485_config.$(OBJ)

include $(EHS_TARGET_RS485_CONFIG_HAL_PATH)/target_rs485_config.mk
