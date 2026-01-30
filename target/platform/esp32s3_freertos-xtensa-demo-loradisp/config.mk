#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-demo-loradisp
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

# SYSTEM_VARIANT optionally indicates specific target environment configurations
# See target/envbuildscripts/targetenv_hacks_*.sh scripts
SYSTEM_VARIANT=esp32s3-hrdcv2


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Enable the serial command prompt interface
EHS_SERIAL_CONSOLE_SUPPORT=yes

# Do NOT restart after loading the app
EHS_TARGET_APPLOAD_RESTARTING_REBOOT=yes


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# Wi-Fi Support
EHS_NETWORK_WIFI_SUPPORT=yes

# Device management
EHS_DEVMAN_SUPPORT=mqtt


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# Default application to run
EHS_DEFAULT_APP=events/microelectronics-uk-2025/lorawan-demo


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

include ./target/devman-configs/esp32s3-base-inx-systems.com.mk


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# Disable esp logger
DEFS += EHS_ESP32_DISABLE_LOGS=1

# Main task stack size
DEFS += EHS_MAIN_ESP32_TASK_STACK_SIZE=16000

# Enable nano printf support for isr safe vsnprintf
DEFS += EHS_NANOPRINTF_SUPPORT=1

# Maximum wi-fi SSID records buffer size
DEFS += ESP32_MAX_AP_RECORDS=20

# Wi-Fi credentials (defaults if none saved)
DEFS += EHS_CONFIG_WIFI_SSID="\"SSIDXXXXX\""
DEFS += EHS_CONFIG_WIFI_PASSWORD="\"XXXXXXXX\""


################################### END OF CONFIGURATION ###################################################
