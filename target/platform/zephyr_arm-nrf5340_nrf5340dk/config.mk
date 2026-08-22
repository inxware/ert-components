#---------------------------------------------------------------
# Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------

# @file config.mk
# eRT platform configuration for Nordic nRF5340 DK (cpuapp core, Cortex-M33)
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

# EHS_TARGET_LIB_VARIANT selects the matching EHS-kernel build in ert-build-support:
#   arm-zephyr-cortexm33/kernel/libehs_ehrt1.a
# Build EHS-kernel for zephyr_arm_cortexm33_ehrt1 to produce this library.
EHS_TARGET_LIB_VARIANT = -cortexm33

EHS_MCU_TARGET    = yes



#################################################################################################################
# Zephyr-specific build parameters
#################################################################################################################

# Board identifier passed to 'west build -b <ERT_ZEPHYR_BOARD>'.
# nRF5340 DK application core (Cortex-M33, 512 KB RAM, 1 MB Flash).
# Zephyr 3.5+ uses the qualified name with slashes; older Zephyr used
# nrf5340dk_nrf5340_cpuapp (underscores).  Adjust if using an older SDK.
ERT_ZEPHYR_BOARD = nrf5340dk/nrf5340/cpuapp

# Zephyr release to check out via 'west init --mr <ERT_ZEPHYR_VERSION>'.
# Must be compatible with the Docker image in Dockerimagename.
# ci:v0.27.4 ships Python 3.12 and is validated against Zephyr v4.1.0.
ERT_ZEPHYR_VERSION = v4.1.0

# CPU-specific flags for nRF5340 application core (Cortex-M33 + FPU) —
# used if the direct-compile path is ever activated; ignored by the CMake-master path.
ERT_ZEPHYR_CPU_FLAGS = -mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16

# Kconfig options written verbatim into the generated prj.conf.
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
ERT_ZEPHYR_KCONFIG += CONFIG_HEAP_MEM_POOL_SIZE=16384


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
DEFS += ZEPHYR_NRF5340=1
DEFS += EHS_FLOAT_AS_FLOAT_TYPE=1

################################### END OF CONFIGURATION ###################################################
