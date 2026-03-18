# Default dependencies and feature support for Linux arm32 targets.

EHS_IMAGEPROCESSING_APRILTAG_SUPPORT ?= enabled
EHS_PERIPHERALS_UART_SUPPORT         ?= linux

# New peripheral HAL defaults — stub on generic linux-arm unless overridden by platform config.mk
EHS_WATCHDOG_SUPPORT      ?= stubbed
EHS_UPS_SUPPORT           ?= stubbed
EHS_BUZZER_SUPPORT        ?= stubbed
EHS_SD_SELECT_SUPPORT     ?= stubbed
EHS_USB_POWER_SUPPORT     ?= stubbed
EHS_ACCELEROMETER_SUPPORT ?= stubbed
EHS_RS485_CONFIG_SUPPORT  ?= stubbed
