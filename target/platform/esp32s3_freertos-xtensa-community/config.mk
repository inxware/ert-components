#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-community
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-base-debug/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# Do not use 16k cache because it's highly likely that the community target uses Wi-Fi
TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1
# To use 16k cache version uncomment the following line
#TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1-16k


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Enable the serial command prompt interface
EHS_SERIAL_CONSOLE_SUPPORT=yes

# Memory management (uncomment to disable linked-list based allocator)
#EHS_MEMORY_MANAGMENT=none


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# Wi-Fi Support
EHS_NETWORK_WIFI_SUPPORT=yes

# OTA is not supported for the community at the moment
EHS_OTA_SUPPORT=stubbed


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# Default application to run
EHS_DEFAULT_APP=demos/UI_for_Waveshare
#EHS_DEFAULT_APP=tutorials/hello_world

# Appland deployment config
EHS_APPLAND_INST_SUPPORT=yes
EHS_APPLAND_INST_DEPLOY_NAME=Xtensa-ESP32-S3-Community
EHS_APPLAND_INST_OS_NAME=xtensa-esp32-s3


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

include ./target/devman-configs/devman.inxware.io.mk


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# Wi-Fi credentials (uncomment and set to configure)
#DEFS += EHS_CONFIG_WIFI_SSID="\"SSIDXXXXX\""
#DEFS += EHS_CONFIG_WIFI_PASSWORD="\"XXXXXXXX\""


################################### END OF CONFIGURATION ###################################################
