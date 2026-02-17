#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-waveshare-touchlcd-1_69
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-base_n8r8/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# COMPONENT_VARIANT - uartusb-N16R2 (16 MB Flash, 2 MB PSRAM, UART console on)
#COMPONENT_VARIANT=uartusb-n16r2

# SYSTEM_VARIANT optionally indicates specific target environment configurations
# See target/envbuildscripts/targetenv_hacks_*.sh scripts
SYSTEM_VARIANT=esp32s3-hrdcv2

# make sure to use libraries built with 32k cache for this target
TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1

# Watchdog (override base - use stubbed for this target)
EHS_WATCHDOG_SUPPORT=stubbed


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Enable debug mode
EHS_DEBUGALL=yes

# Runtime logger
EHS_RUNTIME_LOGGER_ENABLED=no

# Reboot after app load
EHS_TARGET_APPLOAD_RESTARTING_REBOOT=no


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# Wi-Fi Support
EHS_NETWORK_WIFI_SUPPORT=yes

# Device management
EHS_DEVMAN_SUPPORT=mqtt

# Memory management (uncomment to disable linked-list based allocator)
#EHS_MEMORY_MANAGMENT=none

#----- GUI Features -----
EHS_LVGL_DISPLAY_DRIVER=st7701
EHS_LVGL_TOUCH_DRIVER=gt911
CONFIG_LV_I2C=1


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# Default application to run
EHS_DEFAULT_APP=tutorials/hello_world


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

include ./target/devman-configs/esp32s3-base-inx-systems.com.mk


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# HRDCV2 product config ("2" means Rev. B)
DEFS += EHS_PLATFORM_ESP32S3_HRDCV2=2

# Enable nano printf support
DEFS += EHS_NANOPRINTF_SUPPORT=1

# Enable the serial command prompt interface
DEFS += EHS_ESP32_CMD_PROMPT_SUPPORT

# Main task stack size
DEFS += EHS_MAIN_ESP32_TASK_STACK_SIZE=16000

# Maximum wi-fi SSID records buffer size
DEFS += ESP32_MAX_AP_RECORDS=20

# Wi-Fi credentials (defaults if none saved)
DEFS += EHS_CONFIG_WIFI_SSID="\"SSIDXXXXX\""
DEFS += EHS_CONFIG_WIFI_PASSWORD="\"XXXXXXXX\""


################################### END OF CONFIGURATION ###################################################
