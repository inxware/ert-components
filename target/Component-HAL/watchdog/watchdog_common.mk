# Watchdog HAL common makefile.
# Included by component-hal.mk when EHS_WATCHDOG_SUPPORT is set and not 'none'.
# Selects the implementation directory from the support variable, adds paths,
# compiles target_watchdog.c, then includes the implementation's target_watchdog.mk.
#
# Note: the legacy target_watchdog.mk at this directory level is still included
# by component-hal.mk for the old EhsTWatchdog* inline stubs used elsewhere.
# This watchdog_common.mk adds the new EhsTWatchdog*2 function implementations.

EHS_COMMON_WATCHDOG_HAL_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/watchdog
EHS_TARGET_WATCHDOG_HAL_PATH=$(EHS_COMMON_WATCHDOG_HAL_PATH)/$(EHS_WATCHDOG_SUPPORT)

INC_DIRS += $(EHS_COMMON_WATCHDOG_HAL_PATH)
INC_DIRS += $(EHS_TARGET_WATCHDOG_HAL_PATH)
VPATH    += $(EHS_TARGET_WATCHDOG_HAL_PATH)

OBJECTS  += target_watchdog.$(OBJ)

include $(EHS_TARGET_WATCHDOG_HAL_PATH)/target_watchdog.mk
