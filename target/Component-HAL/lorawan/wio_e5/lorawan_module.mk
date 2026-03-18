# LoRaWAN module: Seeed WIO-E5 (AT-command modem over UART)
#
# The WIO-E5 communicates via AT commands over a UART port.
# EHS_PERIPHERALS_UART_SUPPORT must be set to a real UART implementation.

ifndef EHS_PERIPHERALS_UART_SUPPORT
$(error LoRaWAN wio_e5 requires a UART HAL: set EHS_PERIPHERALS_UART_SUPPORT in your platform config.mk)
endif
ifeq ($(EHS_PERIPHERALS_UART_SUPPORT),none)
$(error LoRaWAN wio_e5 requires a UART HAL: EHS_PERIPHERALS_UART_SUPPORT cannot be 'none')
endif
ifeq ($(EHS_PERIPHERALS_UART_SUPPORT),stubbed)
$(warning LoRaWAN wio_e5: EHS_PERIPHERALS_UART_SUPPORT=stubbed — modem will not communicate at runtime)
endif

OBJECTS += lorawan-wio_e5.$(OBJ)
