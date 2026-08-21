#---------------------------------------------------------------
# Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# Default feature configuration for all xcore FreeRTOS targets.
# Platform config.mk files can override any of these.

# Filesystem: XMOS FreeRTOS provides FatFS via fwk_rtos
ifndef EHS_FILESYSTEM_SUPPORT
    EHS_FILESYSTEM_SUPPORT=posix
endif

# No GPIO by default — set to xmos_gpio or stubbed in platform config.mk
ifndef EHS_PERIPHERALS_GPIO_SUPPORT
    EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
endif

# No ADC/DAC by default
ifndef EHS_PERIPHERALS_ADC_DAC_SUPPORT
    EHS_PERIPHERALS_ADC_DAC_SUPPORT=stubbed
endif

# No display by default
ifndef EHS_GUI_SUPPORT
    EHS_GUI_SUPPORT=none
endif

# No AV by default
ifndef EHS_AV_SUPPORT
    EHS_AV_SUPPORT=none
endif

# No ML acceleration by default
ifndef EHS_ML_SUPPORT
    EHS_ML_SUPPORT=none
endif

# Networking: lwip is provided by fwk_rtos
ifndef EHS_COMMS_API_SUPPORT
    EHS_COMMS_API_SUPPORT=lwip
endif
## TODO: This needs moving to the LWIP make files if it is actually needed:
DEFS += EHS_LWIP=1

# Default packager for this os-arch. A platform config.mk may override it with a
# plain '=' -- this file is included after the platform's own config (see
# target/platform/platform.mk, "include $(EHS_TARGET_OS_HW_PATH)/config.mk"),
# so '?=' here yields to the platform. 'make targetenv_package' dispatches on it.
EHS_PACKAGER_TYPE ?= xmos
