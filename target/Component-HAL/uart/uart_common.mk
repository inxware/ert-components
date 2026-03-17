# UART HAL common makefile.
# Included by component-hal.mk when EHS_PERIPHERALS_UART_SUPPORT is set and not 'none'.
# Selects the implementation directory from the support variable, adds paths, compiles
# target_uart.c, then includes the implementation's target_uart.mk for any extras
# (extra libraries, additional include paths, etc.).
#
# 'stubbed' is treated as a peer implementation like any other — there is no special
# handling for it here.  The stubbed/ directory provides no-op stubs that satisfy the
# linker so the build succeeds on platforms without real UART hardware.

EHS_COMMON_UART_HAL_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/uart
EHS_TARGET_UART_HAL_PATH=$(EHS_COMMON_UART_HAL_PATH)/$(EHS_PERIPHERALS_UART_SUPPORT)

INC_DIRS += $(EHS_COMMON_UART_HAL_PATH)
INC_DIRS += $(EHS_TARGET_UART_HAL_PATH)
VPATH    += $(EHS_TARGET_UART_HAL_PATH)

OBJECTS  += target_uart.$(OBJ)

# Include implementation-specific extras (additional libs, include paths, defines).
include $(EHS_TARGET_UART_HAL_PATH)/target_uart.mk
