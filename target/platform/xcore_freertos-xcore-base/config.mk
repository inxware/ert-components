#---------------------------------------------------------------
# Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for xcore_freertos-xcore-base
# Base platform for XMOS xcore.ai targets running FreeRTOS via fwk_rtos.
# Derive board-specific platforms from this file.


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
#################################################################################################################

EHS_ARCH=xcore
EHS_OS=xcore_freertos

# XTC Tools version — matches the Docker image and ert-contrib-middleware target_libs directory name.
# Update this when upgrading the XTC Tools version used to build the SDK libraries.
TOOLCHAIN_NAME=xtc-tools-15.x

# Pre-built SDK library variant in ert-contrib-middleware/target_libs/
COMPONENT_BASE_TECHNOLOGIES=xcore_freertos-xcore-xtc-15.x

# Board target — overridden per-board in derived platform configs.
# This is passed to xcc as -target <XMOS_BOARD_TARGET>.
# Use a board name (e.g. XCORE-AI-EXPLORER) or a path to a .xn file.
XMOS_BOARD_TARGET=XCORE-AI-EXPLORER


#################################################################################################################
# Debug and Startup Modifiers
#################################################################################################################

EHS_DEBUGALL=yes


#################################################################################################################
# Feature Configuration
#################################################################################################################

#----- Filesystem -----
EHS_FILESYSTEM_SUPPORT=posix

#----- Networking -----
EHS_COMPONENT_NETWORKING_SUPPORT=nocurl
EHS_COMMS_API_SUPPORT=lwip

#----- Peripheral Features -----
# Set to xmos_gpio when the Component-HAL implementation is available
EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
EHS_PERIPHERALS_ADC_DAC_SUPPORT=stubbed

#----- GUI / AV / ML -----
EHS_GUI_SUPPORT=none
EHS_AV_SUPPORT=none
EHS_ML_SUPPORT=none
EHS_MV_SUPPORT=none


#################################################################################################################
# Application and Packaging
#################################################################################################################

EHS_DEFAULT_APP=NONE
ERT_SODL_VERSION=1


#################################################################################################################
# Device Management Credentials
#################################################################################################################

include ./target/devman-configs/inx-systems.com.mk


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
#################################################################################################################

DEFS += EHS_XCORE=1
DEFS += EHS_NO_LIBXML2_SUPPORT=1
DEFS += EHS_LWIP
DEFS += EHS_DEBUG_CONSOLE_BUFFER_SIZE=256


################################### END OF CONFIGURATION ###################################################
