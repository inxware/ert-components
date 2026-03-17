#---------------------------------------------------------------
# Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# Makefile fragment to build target-specific component and support code.
# Called by ../../platform/<platform-type>

# Expected variables:
#   OBJECTS      - list of object files added by this makefile
#   INC_DIRS     - target-specific include paths
#   VPATH        - where to look for source code
#   LIB_DIRS     - library search paths
#   LIB          - libraries to link
#   EHS_TARGET_OS_HW_PATH - path to the current directory (set by platform makefile)

EHS_XCORE=yes

# -----------------------------------------------------------------------------
# Source code from this os-arch directory
# -----------------------------------------------------------------------------
# TODO review if already have standards for this in the GNU system. Worth using the same even if duplicated.
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/xcore_freertos-xcore/
VPATH    += $(EHS_TARGETS_ROOT_PATH)/os-arch/xcore_freertos-xcore/

# -----------------------------------------------------------------------------
# XMOS SDK include paths (pre-built headers in ert-contrib-middleware)
# COMPONENT_BASE_TECHNOLOGIES is set in platform config.mk, e.g.:
#   COMPONENT_BASE_TECHNOLOGIES=xcore_freertos-xcore-xtc-15.x
# EHS_COMPONENT_SUPPORT_INCLUDE resolves to:
#   ../ert-contrib-middleware/target_libs/<COMPONENT_BASE_TECHNOLOGIES>/build/include/
# -----------------------------------------------------------------------------

INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)FreeRTOS/
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)FreeRTOS/portable/
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)rtos_support/
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)xcore/

# Library search path for pre-built .a files
LIB_DIRS += $(EHS_COMPONENT_SUPPORT_LIBS)

# -----------------------------------------------------------------------------
# Always-on OS/arch defines
# -----------------------------------------------------------------------------

EHS_COMMS_TASK=tcp_server_common
EHS_XCORE_SUPPORT=1
DEFS += EHS_XCORE_SUPPORT=1

-include TARGET.cfg
DEFS += 'TARGET_OS_VERSION_STRING="$(shell head -c -1 ./Releases/version_strings | tr '\n' '.')\x20:$(TARGET)"'

# -----------------------------------------------------------------------------
# Default peripheral HAL selections (overridable in platform config.mk)
# -----------------------------------------------------------------------------

ifndef EHS_PERIPHERALS_GPIO_SUPPORT
    EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
endif

ifndef EHS_PERIPHERALS_ADC_DAC_SUPPORT
    EHS_PERIPHERALS_ADC_DAC_SUPPORT=stubbed
endif

ifndef EHS_MQTT_SUPPORT
    EHS_MQTT_SUPPORT=stubbed
endif

# -----------------------------------------------------------------------------
# Core object files — platform-independent OS/arch glue
# Equivalent files must exist in this directory (to be ported from zephyr/esp32
# equivalents as the XMOS bring-up progresses).
# -----------------------------------------------------------------------------

OBJECTS += target_main.$(OBJ)
OBJECTS += targetos_init.$(OBJ)
OBJECTS += target_process.$(OBJ)
OBJECTS += target_time.$(OBJ)
OBJECTS += target_math.$(OBJ)
OBJECTS += target_sys_stat.$(OBJ)

ifneq ($(EHS_FILESYSTEM_SUPPORT),stubbed)
    OBJECTS += target_file.$(OBJ)
endif

ifdef EHS_GUI_SUPPORT
    ifneq ($(EHS_GUI_SUPPORT),none)
        OBJECTS += target_display.$(OBJ)
    endif
endif

# -----------------------------------------------------------------------------
# XMOS SDK libraries (pre-built, in ert-contrib-middleware target_libs)
# Core FreeRTOS/RTOS support — always required.
# Peripheral libs are added conditionally by Component-HAL makefiles.
# -----------------------------------------------------------------------------

LIB += freertos
LIB += rtos_support
LIB += c
