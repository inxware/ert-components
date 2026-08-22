#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-hrdcv2C-debug-wifi
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-hrdcv2C-debug/config.mk


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
EHS_NETWORK_ETHERNET_SUPPORT=yes
EHS_NETWORK_WIFI_SUPPORT=yes

#----- Console Features -----
EHS_SERIAL_CONSOLE_SUPPORT=yes


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

#EHS_DEFAULT_APP=customer-apps/ElectricHeatingSystems/Caravan-MVP-07.01.24-2-WorkingWithDC-CT/
#EHS_DEFAULT_APP=customer-apps/ElectricHeatingSystems/Caravan-MVP-07.01.24-2-WorkingWithDC-CT-Willerbys/Caravan-MVP-07.01.24-2-WorkingWithDC-CT
#EHS_DEFAULT_APP=customer-apps/ElectricHeatingSystems/Caravan-MVP-07.01.24-2-WorkingWithDC-CT-Willerbys-NewBoard


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# Task stack size
DEFS += EHS_MAIN_ESP32_TASK_STACK_SIZE=12000


# Wi-Fi credentials (uncomment to set)
#DEFS += EHS_CONFIG_WIFI_SSID="\"Wireless-INX\""
#DEFS += EHS_CONFIG_WIFI_PASSWORD="\"HelloInx101\""

# ADC channel setup (uncomment if needed)
#DEFS += ADC_PT100_CHANNEL1=0
#DEFS += ADC_PT100_CHANNEL2=1
#DEFS += ADC_10V=2
#DEFS += ADC_20MA=3
#DEFS += ADC_THERMOCOUPLE1=4
#DEFS += ADC_THERMOCOUPLE2=5


################################### END OF CONFIGURATION ###################################################
