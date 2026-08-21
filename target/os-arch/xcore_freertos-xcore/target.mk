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
# XMOS SDK include paths — OPTION B (source tree direct, temporary)
#
# The fwk_rtos SDK is not designed to export pre-built headers as a developer
# library (Option C, using cmake install, requires hacking the cmake build
# system in ways it is not intended to support).  The correct long-term
# approach is Phase 2 (make targetenv_xmos_docker) which uses xcommon_cmake
# to consume fwk_rtos natively — headers are never exported to target_libs.
#
# For Phase 1 compilation we point directly at the fwk_rtos source tree.
# FreeRTOS-SMP-Kernel must be cloned into the submodule path by
# build-xcore-freertos-ehs.sh before running make all_docker.
#
# If the SDK source tree layout changes, update these paths here.
# EHS_COMPONENT_SUPPORT_INCLUDE paths are retained below (commented) for
# reference — they would be used if a pre-built header install existed.
# -----------------------------------------------------------------------------

XMOS_SDK_DIR := $(EHS_ROOT_PATH)/../ert-contrib-middleware/contrib/xmos-sdk
FREERTOS_KERNEL_DIR := $(XMOS_SDK_DIR)/fwk_rtos/modules/FreeRTOS/FreeRTOS-SMP-Kernel

# FreeRTOS.h, task.h, timers.h, queue.h etc.
INC_DIRS += $(FREERTOS_KERNEL_DIR)/include
# XCORE-AI portable layer (portmacro.h etc.)
INC_DIRS += $(FREERTOS_KERNEL_DIR)/portable/ThirdParty/xClang/XCOREAI
# rtos_support API (rtos_printf, etc.) and its private impl headers
INC_DIRS += $(XMOS_SDK_DIR)/fwk_rtos/modules/rtos_support/api
INC_DIRS += $(XMOS_SDK_DIR)/fwk_rtos/modules/rtos_support/src
# fwk_core utility headers (xcore_utils.h etc.) pulled in by rtos_support
INC_DIRS += $(XMOS_SDK_DIR)/fwk_core/modules/utils/api
# RTOS OSAL API
INC_DIRS += $(XMOS_SDK_DIR)/fwk_rtos/modules/osal/api
INC_DIRS += $(XMOS_SDK_DIR)/fwk_rtos/modules/osal/FreeRTOS

# FreeRTOSConfig.h — provided in this os-arch directory.
# (already on INC_DIRS via the xcore_freertos-xcore/ path added above)

# -----------------------------------------------------------------------------
# Comms HAL header path — TRANSITIONAL
#
# hal_network.h unconditionally #include "target_tcp.h", so the header must
# resolve even when EHS_COMMS_API_SUPPORT=none (the platform default until
# networking is implemented). The freertos_plus_tcp Component-HAL is the
# architectural home for xcore networking (fwk_rtos uses FreeRTOS-Plus-TCP
# via FreeRTOS_Sockets.h), but currently contains only a stub target_tcp.h.
# Add it to INC_DIRS unconditionally; remove this once EHS_COMMS_API_SUPPORT
# is set to freertos_plus_tcp (at which point ehs.mk adds the path itself).
# -----------------------------------------------------------------------------
INC_DIRS += $(EHS_TARGET_COMPONENT_HAL_PATH)/comms/freertos_plus_tcp

# Pre-built header paths (Option C, not yet usable — kept for reference):
# INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)
# INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)FreeRTOS/
# INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)FreeRTOS/portable/
# INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)rtos_support/
# INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)xcore/

# Library search path — not used in Phase 1 (archive build).
# Phase 2 (xcommon_cmake) resolves SDK .a files via fwk_rtos dependencies.
# LIB_DIRS += $(EHS_COMPONENT_SUPPORT_LIBS)

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

# Per-target serial-console HAL — backs Common/Ehs/serial_console.c.
# Contract: Common/HAL/include/hal_serial.h.
ifdef EHS_SERIAL_CONSOLE_SUPPORT
ifneq ($(EHS_SERIAL_CONSOLE_SUPPORT),none)
OBJECTS += target_serial.$(OBJ)
endif
endif

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
# These are expanded to -l flags by toolchain.mk via $(foreach i,$(LIB),-l$i).
# -----------------------------------------------------------------------------

LIB += freertos
LIB += rtos_support
LIB += c
