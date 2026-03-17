# Buzzer HAL common makefile.
# Included by component-hal.mk when EHS_BUZZER_SUPPORT is set and not 'none'.
# Selects the implementation directory from the support variable, adds paths,
# compiles target_buzzer.c, then includes the implementation's target_buzzer.mk.

EHS_COMMON_BUZZER_HAL_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/buzzer
EHS_TARGET_BUZZER_HAL_PATH=$(EHS_COMMON_BUZZER_HAL_PATH)/$(EHS_BUZZER_SUPPORT)

INC_DIRS += $(EHS_COMMON_BUZZER_HAL_PATH)
INC_DIRS += $(EHS_TARGET_BUZZER_HAL_PATH)
VPATH    += $(EHS_TARGET_BUZZER_HAL_PATH)

OBJECTS  += target_buzzer.$(OBJ)

include $(EHS_TARGET_BUZZER_HAL_PATH)/target_buzzer.mk
