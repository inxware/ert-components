
# Default dependencies and feature support for Windows targets.

# NOTE: block below kept as ifneq/ifndef — outer ifneq tests EHS_PERIPHERALS_GPIO_SUPPORT
# but inner ifndef sets EHS_COMMS_API_SUPPORT (mismatched variables, likely a pre-existing bug).
# Review before converting.
ifneq ($(EHS_PERIPHERALS_GPIO_SUPPORT),none)
ifndef  EHS_COMMS_API_SUPPORT
export EHS_COMMS_API_SUPPORT=winsock
endif
endif

# Default network features for Windows targets
EHS_COMPONENTS_NETWORK_URL_GET       ?= enabled

EHS_IMAGEPROCESSING_APRILTAG_SUPPORT ?= enabled

EHS_PERIPHERALS_UART_SUPPORT         ?= win32

# New peripheral HAL defaults — stub on Windows targets
EHS_WATCHDOG_SUPPORT      ?= stubbed
EHS_UPS_SUPPORT           ?= stubbed
EHS_BUZZER_SUPPORT        ?= stubbed
EHS_SD_SELECT_SUPPORT     ?= stubbed
EHS_USB_POWER_SUPPORT     ?= stubbed
EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT ?= stubbed
EHS_RS485_CONFIG_SUPPORT  ?= stubbed

# Default packager for this os-arch. A platform config.mk may override it with a
# plain '=' -- this file is included after the platform's own config (see
# target/platform/platform.mk, "include $(EHS_TARGET_OS_HW_PATH)/config.mk"),
# so '?=' here yields to the platform. 'make targetenv_package' dispatches on it.
EHS_PACKAGER_TYPE ?= nsis
