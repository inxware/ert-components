#default dependencies and feature support for linux android arm targets.

EHS_IMAGEPROCESSING_APRILTAG_SUPPORT ?= enabled
EHS_PERIPHERALS_UART_SUPPORT         ?= stubbed

# Default packager for this os-arch. A platform config.mk may override it with a
# plain '=' -- this file is included after the platform's own config (see
# target/platform/platform.mk, "include $(EHS_TARGET_OS_HW_PATH)/config.mk"),
# so '?=' here yields to the platform. 'make targetenv_package' dispatches on it.
EHS_PACKAGER_TYPE ?= apk
