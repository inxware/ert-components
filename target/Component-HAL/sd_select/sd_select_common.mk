# SD select HAL common makefile.
# Included by component-hal.mk when EHS_SD_SELECT_SUPPORT is set and not 'none'.
# Selects the implementation directory from the support variable, adds paths,
# compiles target_sd_select.c, then includes the implementation's target_sd_select.mk.

EHS_COMMON_SD_SELECT_HAL_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/sd_select
EHS_TARGET_SD_SELECT_HAL_PATH=$(EHS_COMMON_SD_SELECT_HAL_PATH)/$(EHS_SD_SELECT_SUPPORT)

INC_DIRS += $(EHS_COMMON_SD_SELECT_HAL_PATH)
INC_DIRS += $(EHS_TARGET_SD_SELECT_HAL_PATH)
VPATH    += $(EHS_TARGET_SD_SELECT_HAL_PATH)

OBJECTS  += target_sd_select.$(OBJ)

include $(EHS_TARGET_SD_SELECT_HAL_PATH)/target_sd_select.mk
