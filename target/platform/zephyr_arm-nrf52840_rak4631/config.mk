#---------------------------------------------------------------
# Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------

# @file config.mk
# eRT platform configuration for RAK Wireless RAK4631 (nRF52840, Cortex-M4F)
# Build method: Zephyr-as-master CMake build via 'make zephyr_cmake_gen' +
#               'make zephyr_build_docker'.  The toolchain.mk compiler
#               settings are retained for a potential future direct-compile
#               incremental path but are not used in the primary workflow.

#################################################################################################################
# Architecture, OS, Toolchain
#################################################################################################################

EHS_ARCH          = arm
EHS_OS            = zephyr
TOOLCHAIN_NAME    = arm-zephyr-eabi

EHS_MCU_TARGET    = yes

# EHS_TARGET_LIB_VARIANT selects the matching EHS-kernel build in ert-build-support:
#   arm-zephyr-cortexm4/kernel/libehs_ehrt1.a
# Build EHS-kernel for zephyr_arm_cortexm4_ehrt1 to produce this library.
EHS_TARGET_LIB_VARIANT = -cortexm4

# Use the pre-built kernel library (from EHS-kernel/zephyr_arm_cortexm4_ehrt1).
# This excludes target/os-arch/zephyr-arm/ehs_kernel_stub.c from the build.
EHS_BUILD_MONOLITHIC_KERNEL = yes


#################################################################################################################
# Zephyr-specific build parameters
#################################################################################################################

# Board identifier passed to 'west build -b <ERT_ZEPHYR_BOARD>'.
# Verify against your Zephyr version with: west boards | grep rak
ERT_ZEPHYR_BOARD = rak4631

# Zephyr release to check out via 'west init --mr <ERT_ZEPHYR_VERSION>'.
# Must be compatible with the Docker image in Dockerimagename.
# ci:v0.27.4 ships Python 3.12 and is validated against Zephyr v4.1.0.
ERT_ZEPHYR_VERSION = v4.1.0

# CPU-specific flags for nRF52840 (Cortex-M4F) — used if the direct-compile
# path is ever activated; ignored by the CMake-master path.
ERT_ZEPHYR_CPU_FLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

# Kconfig options written verbatim into the generated prj.conf.
# Space-separated; one CONFIG_* entry per token.
ERT_ZEPHYR_KCONFIG  = CONFIG_POSIX_API=y
ERT_ZEPHYR_KCONFIG += CONFIG_FPU=y
ERT_ZEPHYR_KCONFIG += CONFIG_REBOOT=y
ERT_ZEPHYR_KCONFIG += CONFIG_SERIAL=y
ERT_ZEPHYR_KCONFIG += CONFIG_UART_CONSOLE=y
ERT_ZEPHYR_KCONFIG += CONFIG_UART_INTERRUPT_DRIVEN=y
ERT_ZEPHYR_KCONFIG += CONFIG_GPIO=y
ERT_ZEPHYR_KCONFIG += CONFIG_PRINTK=y
ERT_ZEPHYR_KCONFIG += CONFIG_LOG=y
ERT_ZEPHYR_KCONFIG += CONFIG_LOG_BACKEND_UART=y
ERT_ZEPHYR_KCONFIG += CONFIG_HEAP_MEM_POOL_SIZE=8192


#################################################################################################################
# Feature flags
#################################################################################################################

EHS_GUI_SUPPORT              = none
EHS_AV_SUPPORT               = none
EHS_MEDIA_SUPPORT            = none
EHS_NETWORKING_SUPPORT       = none
EHS_ML_SUPPORT               = none
EHS_MV_SUPPORT               = none
EHS_DEBUG_TCPIP_CONSOLE      = stubbed
EHS_FILESYSTEM_SUPPORT       = posix
EHS_PERIPHERALS_GPIO_SUPPORT = zephyr
EHS_PERIPHERALS_ADC_DAC_SUPPORT = none
EHS_UART_SUPPORT             = zephyr

# libexpat is not available on Zephyr; exclude the XML stream component
EHS_EXCLUDE_XML_PARSER       = yes

# inx-application_info_getter uses POSIX opendir/readdir not available on Zephyr
EHS_SKIP_APPLICATION_INFO_GETTER = yes

ERT_SODL_VERSION = 1
EHS_DEBUGALL     = true

EHS_DEFAULT_APP  = tutorials/hello_world


#################################################################################################################
# Preprocessor definitions
#################################################################################################################

DEFS += EHS_MCU_TARGET=1
DEFS += ZEPHYR_NRF52840=1
DEFS += EHS_FLOAT_AS_FLOAT_TYPE=1

################################### END OF CONFIGURATION ###################################################
