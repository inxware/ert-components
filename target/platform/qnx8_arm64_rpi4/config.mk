#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration for qnx8_arm64_rpi4
# QNX Neutrino RTOS 8.0 on Raspberry Pi 4 (Cortex-A72 / AArch64)
# Reference runtime: QNX SDP 8.0 with official Raspberry Pi 4 BSP.


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
#################################################################################################################

# CPU and OS type — selects target/os-arch/qnx-arm64/ via platform.mk
EHS_ARCH = arm64
EHS_OS   = qnx

# GNU canonical triple components used for ert-contrib-middleware path selection
# Full triple: aarch64-unknown-nto-qnx8.0.0
EHS_GNU_ARCH = aarch64
EHS_GNU_OS   = nto-qnx8.0.0

# QNX SDP 8.0 cross-tools are on $PATH inside the Docker container
# (added to PATH via ENV QNX_HOST in the Dockerfile)
TOOLCHAIN_NAME = HOST

# Contributed library variant — 'base' until QNX-specific middleware is built
COMPONENT_VARIANT = base


#################################################################################################################
# Debug and Startup Modifiers
#################################################################################################################

EHS_DEBUGALL = true

# TCPIP console conflicts with QNX networking setup on initial bring-up
EHS_DEBUG_TCPIP_CONSOLE = stubbed


#################################################################################################################
# Feature Configuration
#################################################################################################################

ERT_SODL_VERSION = 1

EHS_NETWORKING_SUPPORT           = all
EHS_COMPONENT_NETWORKING_SUPPORT = all

# No GUI for initial reference bring-up — enable once display stack is confirmed
# EHS_GUI_SUPPORT = ...

EHS_MV_SUPPORT = none
EHS_ML_SUPPORT = none
# EHS_ML_SUPPORT = ...

EHS_PERIPHERAL_DEVICE_SUPPORT    = all
EHS_PERIPHERALS_GPIO_SUPPORT     = stubbed
EHS_PERIPHERALS_UART_SUPPORT     = qnx


#################################################################################################################
# Application and Packaging
#################################################################################################################

EHS_DEFAULT_APP = demos/simple_io/hello_world

# QNX deployables are not .deb or .apk.
# A simple tarball via targetenv is the starting point; a QNX IFS image
# (using mkifs) can be added here once the BSP boot flow is established.
# EHS_PACKAGER_TYPE = ...


#################################################################################################################
# Preprocessor Definitions
#################################################################################################################

DEFS += EHS_LOG_LEVEL_VERBOSE
DEFS += EHS_LOG_TO_STDIO

# Force 'unknown' source files to output logging at the logger's level
DEFS += EHSL_MODULE_ID=EHSH_LOG_MODULE_LOGGER

################################### END OF CONFIGURATION ###################################################
