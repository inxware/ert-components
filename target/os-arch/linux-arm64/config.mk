########################################################################
#  Default dependencies and feature support for Linux arm64 targets.
########################################################################

EHS_PERIPHERALS_GPIO_SUPPORT      ?= stubbed
EHS_PERIPHERALS_UART_SUPPORT      ?= linux
EHS_PERIPHERAL_DEVICE_SUPPORT     ?= all
EHS_IMAGEPROCESSING_APRILTAG_SUPPORT ?= enabled

# New peripheral HAL defaults — stub on generic linux-arm64 unless overridden by platform config.mk
EHS_WATCHDOG_SUPPORT      ?= stubbed
EHS_UPS_SUPPORT           ?= stubbed
EHS_BUZZER_SUPPORT        ?= stubbed
EHS_SD_SELECT_SUPPORT     ?= stubbed
EHS_USB_POWER_SUPPORT     ?= stubbed
EHS_ACCELEROMETER_SUPPORT ?= stubbed
EHS_RS485_CONFIG_SUPPORT  ?= stubbed
