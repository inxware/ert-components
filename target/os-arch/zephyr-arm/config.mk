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
