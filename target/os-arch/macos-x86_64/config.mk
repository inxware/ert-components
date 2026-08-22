########################################################################
#  Default dependencies and feature support for macOS x86_64 targets.
########################################################################

# POSIX serial (termios) is available on macOS
EHS_PERIPHERALS_UART_SUPPORT  ?= linux

# Hardware peripherals not available on a Mac desktop — stub them all
EHS_PERIPHERALS_GPIO_SUPPORT        ?= stubbed
EHS_WATCHDOG_SUPPORT                ?= stubbed
EHS_UPS_SUPPORT                     ?= stubbed
EHS_BUZZER_SUPPORT                  ?= stubbed
EHS_SD_SELECT_SUPPORT               ?= stubbed
EHS_USB_POWER_SUPPORT               ?= stubbed
EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT  ?= stubbed
EHS_RS485_CONFIG_SUPPORT            ?= stubbed
EHS_IMAGEPROCESSING_APRILTAG_SUPPORT ?= enabled

# Networking defaults
EHS_NETWORKING_SUPPORT           ?= http
EHS_COMPONENTS_NETWORK_URL_GET   ?= enabled

# Default packager for this os-arch. A platform config.mk may override it with a
# plain '=' -- this file is included after the platform's own config (see
# target/platform/platform.mk, "include $(EHS_TARGET_OS_HW_PATH)/config.mk"),
# so '?=' here yields to the platform. 'make targetenv_package' dispatches on it.
EHS_PACKAGER_TYPE ?= none
