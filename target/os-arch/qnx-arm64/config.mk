########################################################################
#  Default dependencies and feature support for QNX Neutrino ARM64.
#  QNX-wide defaults are set in qnx_ALL/config.mk; only ARM64-specific
#  overrides belong here.
########################################################################

EHS_PERIPHERALS_GPIO_SUPPORT       ?= stubbed
EHS_PERIPHERAL_DEVICE_SUPPORT      ?= all

EHS_WATCHDOG_SUPPORT               ?= stubbed
EHS_UPS_SUPPORT                    ?= stubbed
EHS_BUZZER_SUPPORT                 ?= stubbed
EHS_SD_SELECT_SUPPORT              ?= stubbed
EHS_USB_POWER_SUPPORT              ?= stubbed
EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT ?= stubbed
EHS_RS485_CONFIG_SUPPORT           ?= stubbed

# Default packager for this os-arch. A platform config.mk may override it with a
# plain '=' -- this file is included after the platform's own config (see
# target/platform/platform.mk, "include $(EHS_TARGET_OS_HW_PATH)/config.mk"),
# so '?=' here yields to the platform. 'make targetenv_package' dispatches on it.
EHS_PACKAGER_TYPE ?= none
