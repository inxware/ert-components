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
ifneq ($(EHS_FILESYSTEM_SUPPORT),none)
ifndef EHS_FILESYSTEM_SUPPORT
	EHS_FILESYSTEM_SUPPORT=posix
endif
endif

# No GPIO support by default - platform config should enable specific driver
ifneq ($(EHS_PERIPHERALS_GPIO_SUPPORT),none)
ifndef EHS_PERIPHERALS_GPIO_SUPPORT
	EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
endif
endif

# System console: use Zephyr shell/UART console
ifneq ($(EHS_SYSTEM_CONSOLE),none)
ifndef EHS_SYSTEM_CONSOLE
	EHS_SYSTEM_CONSOLE=yes
endif
endif
