#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-ehs-sysboiler-debug
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-base/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# SYSTEM_VARIANT for conditional compilation for very specific features
# See target/envbuildscripts/targetenv_hacks_*.sh scripts
SYSTEM_VARIANT=esp32s3-hrdcv2

# Libraries built with 32k cache
TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

EHS_DEBUGALL=yes
EHS_RUNTIME_LOGGER_ENABLED=no

# Serial console support
EHS_SERIAL_CONSOLE_SUPPORT=yes


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# HRDCV2 product config ("2" means Rev. B)
DEFS += EHS_PLATFORM_ESP32S3_HRDCV2=2

#----- Networking Features -----
EHS_NETWORK_WIFI_SUPPORT=yes

# Devman support
EHS_DEVMAN_SUPPORT=mqtt

#----- Peripheral Features -----
# TODO: need to change defaults in os-arch so we don't undo base stuff and get multi-def warning
EHS_WATCHDOG_SUPPORT=stubbed

#----- GUI Features -----
# Disable GUI for this target
undefine EHS_GUI_SUPPORT


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

#EHS_DEFAULT_APP=customer-apps/ElectricHeatingSystems/System-Boiler-with-Hot-Water/
EHS_DEFAULT_APP=customer-apps/ElectricHeatingSystems/System-Boiler-with-Hot-Water-Simplified/


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

include ./target/devman-configs/esp32s3-hrdcv2-inx-systems.com.mk


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# Task stack size
DEFS += EHS_MAIN_ESP32_TASK_STACK_SIZE=16000


# Wi-Fi credentials (uncomment to set)
# TODO2025: Need to know if this clobbers saved files or are defaults if none are saved
#DEFS += EHS_CONFIG_WIFI_SSID="\"Wireless-INX\""
#DEFS += EHS_CONFIG_WIFI_PASSWORD="\"HelloInx101\""

# ADC channel setup
DEFS += ADC_PT100_CHANNEL1=0
DEFS += ADC_PT100_CHANNEL2=1
DEFS += ADC_10V=2
DEFS += ADC_20MA=3
DEFS += ADC_THERMOCOUPLE1=4
DEFS += ADC_THERMOCOUPLE2=5

# Enable nano printf support for ISR safe vsnprintf
DEFS += EHS_NANOPRINTF_SUPPORT=1


################################### END OF CONFIGURATION ###################################################
