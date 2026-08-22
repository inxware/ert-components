#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-hrdcv2B-ehs-caravan-inx-devman-debug
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-hrdcv2B-debug/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# Libraries built with 32k cache
TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- Networking Features -----
# Wi-Fi support
EHS_NETWORK_WIFI_SUPPORT=yes

#----- Console Features -----
# Enable the serial command prompt interface
EHS_SERIAL_CONSOLE_SUPPORT=yes


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

#EHS_DEFAULT_APP=customer-apps/ElectricHeatingSystems/Caravan-MVP-07.01.24-2-WorkingWithDC-CT/
EHS_DEFAULT_APP=customer-apps/ElectricHeatingSystems/Caravan-MVP-07.01.24-2-WorkingWithDC-CT-Willerbys/Caravan-MVP-07.01.24-2-WorkingWithDC-CT


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

# This target connects to inx-systems mqtt broker
include ./target/devman-configs/esp32s3-hrdcv2-inx-systems.com.mk


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# Task stack size
DEFS += EHS_MAIN_ESP32_TASK_STACK_SIZE=12000

# ADC channel setup
DEFS += ADC_PT100_CHANNEL1=0
DEFS += ADC_PT100_CHANNEL2=1
DEFS += ADC_10V=2
DEFS += ADC_20MA=3
DEFS += ADC_THERMOCOUPLE1=4
DEFS += ADC_THERMOCOUPLE2=5


################################### END OF CONFIGURATION ###################################################
