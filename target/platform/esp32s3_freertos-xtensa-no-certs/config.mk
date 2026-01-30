#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-no-certs
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

# libraries built with 16k cache (uncomment to use)
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

# OTA support (uncomment to enable stubbed version)
#EHS_OTA_SUPPORT=stubbed


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# Default application to run
EHS_DEFAULT_APP=tutorials/hello_world

# Appland deployment config
EHS_APPLAND_INST_SUPPORT=yes
EHS_APPLAND_INST_DEPLOY_NAME=Xtensa-ESP32-S3-Community
EHS_APPLAND_INST_OS_NAME=xtensa-esp32-s3


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

# No devman credentials included for this target (no-certs)
#include ./target/devman-configs/devman.inxware.io.mk


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# Disable esp logger
DEFS += EHS_ESP32_DISABLE_LOGS=1

# Maximum wi-fi SSID records buffer size
DEFS += ESP32_MAX_AP_RECORDS=40

# Wi-Fi credentials (uncomment and set to configure)
#DEFS += EHS_CONFIG_WIFI_SSID="\"SSIDXXXXX\""
#DEFS += EHS_CONFIG_WIFI_PASSWORD="\"XXXXXXXX\""


################################### END OF CONFIGURATION ###################################################
