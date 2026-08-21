#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# Platform configuration for Zephyr on Nordic nRF52840 (Cortex-M4F)
# Board: nrf52840dk_nrf52840

EHS_ARCH=arm
EHS_OS=zephyr
TOOLCHAIN_NAME=arm-zephyr-eabi
# Selects the kernel archive: support_libs/target_libs/arm-zephyr-cortexm4/.
# Without it the key truncates to `arm-zephyr`, which has no directory, and the
# link has no libehs to find. nRF52840 is Cortex-M4F (see ZEPHYR_CPU_FLAGS below).
EHS_TARGET_LIB_VARIANT = -cortexm4
EHS_MCU_TARGET=yes

# Zephyr build output directory (set by Docker or user environment)
# Should point to the west build output containing Makefile.export
ZEPHYR_BUILD_DIR ?= $(EHS_ROOT_PATH)/../zephyr-build

# CPU-specific flags for nRF52840 (ARM Cortex-M4F)
ZEPHYR_CPU_FLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

# Minimal feature set for initial bring-up
EHS_GUI_SUPPORT=none
EHS_AV_SUPPORT=none
EHS_MEDIA_SUPPORT=none
EHS_NETWORKING_SUPPORT=none
EHS_ML_SUPPORT=none
EHS_MV_SUPPORT=none
EHS_DEBUG_TCPIP_CONSOLE=stubbed

# Filesystem: Zephyr provides POSIX-like FS when CONFIG_FILE_SYSTEM is set
EHS_FILESYSTEM_SUPPORT=posix

# No GPIO/peripheral support initially - can be enabled per-project
EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
EHS_PERIPHERALS_ADC_DAC_SUPPORT=none

COMPONENT_VARIANT=base
ERT_SODL_VERSION=1
EHS_DEBUGALL=true

# Default application
# EHS_DEFAULT_APP=tutorials/hello_world

# Additional defines for Zephyr
DEFS += EHS_MCU_TARGET=1
DEFS += ZEPHYR_NRF52840=1
