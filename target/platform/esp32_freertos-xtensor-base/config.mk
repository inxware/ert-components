#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32_freertos-xtensor-base (original ESP32, not S3)
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

# (No parent config - this is a base configuration)


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# CPU and OS Type Selection
EHS_ARCH=xtensa
EHS_OS=esp32_freertos

# TOOLCHAIN_NAME - original ESP32 toolchain
TOOLCHAIN_NAME=xtensa-esp32-elf-4.4.1

# Export ESP32 platform flag
export EHS_ESP32=yes


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Enable debug mode
EHS_DEBUGALL=yes


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- File System -----
EHS_FILESYSTEM_SUPPORT=posix
#EHS_FILESYSTEM_SUPPORT=stubbed

#----- Networking Features -----
EHS_COMPONENT_NETWORKING_SUPPORT=nocurl
EHS_MQTT_SUPPORT=esp_mqtt-441

# Network/Interface config support
EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT=yes
#EHS_HAL_INTERFACE_CONFIG_SUPPORT=EHS_HAL_INTERFACE_CONFIG_ESP32
EHS_HAL_INTERFACE_CONFIG_SUPPORT=EHS_HAL_INTERFACE_CONFIG_STUBBED
#EHS_HAL_NETWORK_CONFIG_SUPPORT=EHS_HAL_NETWORK_CONFIG_ESP32
EHS_HAL_NETWORK_CONFIG_SUPPORT=EHS_HAL_NETWORK_CONFIG_STUBBED

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# Default application to run
EHS_DEFAULT_APP=NONE


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

include ./target/devman-configs/inx-systems.com.mk


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# Platform identification
DEFS += EHS_ESP32

# File system workarounds for ESP32 (not S3) - feof linked in 4.4.1 libraries
DEFS += EHS_FEOF_MISSING_WORKAROUND
DEFS += EHS_TARGET_FILE_SKIP_STAT

# XML/libxml2 disabled
DEFS += EHS_NO_LIBXML2_SUPPORT=1

# Networking stack
DEFS += EHS_LWIP

# Console/debug buffer sizes
DEFS += EHS_DEBUG_CONSOLE_BUFFER_SIZE=256


################################### END OF CONFIGURATION ###################################################
