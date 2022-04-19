#
# config.mk - Configuration properties of the current platform
#
# Called by ../../../Makefile
#

# @author: inx limited
# @version: $Revision: 43 $
# @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
#
# Copyright (c) inx limited, 2007. All rights reserved.
#
#
#

################################################################################################################
# Define the specific variant of the architecture and OS - this selects different component support library sets
################################################################################################################

# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features
export SYSTEM_VARIANT=nxp_arm

#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
export COMPONENT_VARIANT=

# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
#export COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=

#################################################################################################################
# Set general architecture and OS version
#################################################################################################################


#export EXCLUDE_EHS_COMMON=1
export HEATROD_CONTROLLER_PROJECT=1

export EHS_ARCH=arm
export EHS_OS=nxp
export EHS_SKIP_GNULIBRARIES=1
export EHS_COMMS_TASK=tcp_server_common
export EHS_COMMS_API_SUPPORT=lwip

export TOOLCHAIN_NAME=nxp-arm
export CC_OVERRIDE=arm-none-eabi-gcc
export EHS_CLIB_OVERRIDE_PATH=nxp-arm
export EHS_PERIPHERAL_DEVICE_SUPPORT=all
export EHS_PERIPHERALS_GPIO=yes
#DEFS += EHS_COMMS_API_SUPPORT=lwip
DEFS += EHS_LWIP
DEFS += EHS_DEBUG_TCPIP_CONSOLE
DEFS += EHS_COMMS_TASK=tcp_server_common
DEFS += EHS_TARGET_EVENT_MODEL=1
DEFS += EHS_SKIP_GNULIBRARIES=1
DEFS += INX_HEATROD_IOT_BOARD_VARIANT=INX_HR_HRi
DEFS += CONFIG_MQTT_LOCATION=CONFIG_MQTT_LOCATION_INX
DEFS += CONFIG_LIFESTYLE=INX_LIFESTYLE_EHS
INCLUDE_DIRECTORIES += $(EHS_COMMON_HAL_PATH)/include
INCLUDE_DIRECTORIES += $(EHS_COMMON_KERNEL_PATH)
INCLUDE_DIRECTORIES += $(EHS_COMMON_COMPONENTS_PATH)/core
INCLUDE_DIRECTORIES += $(EHS_TARGETS_ROOT_PATH)/platform/nxp_arm_inx_hri_ehs_debug
INCLUDE_DIRECTORIES += $(EHS_TARGETS_ROOT_PATH)/Component-HAL/comms/lwip
INCLUDE_DIRECTORIES += $(EHS_TARGETS_ROOT_PATH)/Component-HAL/comms/tcp_server_common