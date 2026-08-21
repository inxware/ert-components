#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# Default feature configuration for Zephyr ARM targets
# Features default to minimal/stubbed unless the platform config enables them

# Filesystem: Zephyr provides a POSIX-compatible FS API when CONFIG_POSIX_API is enabled
EHS_FILESYSTEM_SUPPORT ?= posix

# No GPIO support by default - platform config should enable specific driver
EHS_PERIPHERALS_GPIO_SUPPORT ?= stubbed

# System console: use Zephyr shell/UART console
EHS_SYSTEM_CONSOLE ?= yes

# No Wi-Fi by default - platform config.mk sets EHS_WIFI_SUPPORT=zephyr_nrf70
# (currently the only backend) to opt in on boards with an nRF70-series chip
# wired up. The actual Kconfig list lives in wifi_nrf70.mk, not the platform
# config, so it's defined once and shared across every nRF70-equipped board.
EHS_WIFI_SUPPORT ?= none
ifneq ($(EHS_WIFI_SUPPORT),none)
    include $(EHS_TARGET_OS_HW_PATH)/wifi_$(EHS_WIFI_SUPPORT:zephyr_%=%).mk
endif

# No cellular by default - platform config.mk sets EHS_CELLULAR_SUPPORT=zephyr_nrf91
# on boards with an nRF91-series modem. Same arrangement as Wi-Fi above: the
# Kconfig list lives in cellular_nrf91.mk so it is defined once and shared
# across every nRF91 board, and the platform config carries only the selector.
#
# Wi-Fi and cellular are MUTUALLY EXCLUSIVE on nRF91 (measured RAM overflow of
# 57,076 bytes with both). The guard that enforces it lives in
# target/Component-HAL/cellular/cellular.mk, which runs for any target that
# enables the eRT cellular subsystem.
EHS_CELLULAR_SUPPORT ?= none
ifneq ($(filter-out none stubbed,$(EHS_CELLULAR_SUPPORT)),)
    include $(EHS_TARGET_OS_HW_PATH)/cellular_$(EHS_CELLULAR_SUPPORT:zephyr_%=%).mk
endif

# Default packager for this os-arch. A platform config.mk may override it with a
# plain '=' -- this file is included after the platform's own config (see
# target/platform/platform.mk, "include $(EHS_TARGET_OS_HW_PATH)/config.mk"),
# so '?=' here yields to the platform. 'make targetenv_package' dispatches on it.
EHS_PACKAGER_TYPE ?= none
