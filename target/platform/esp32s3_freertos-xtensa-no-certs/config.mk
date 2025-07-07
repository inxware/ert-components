#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for esp32_freertos-xtensor-base
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version
#################################################################################################################

include ./target/platform/esp32s3_freertos-xtensa-base-debug/config.mk

# libraries built with 16k cache
TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1-16k

# Configure wifi interface for esp32s3 device
# to set specific wifi credentials replace 'ssid' and 'password'
#DEFS += EHS_CONFIG_WIFI_SSID="\"Wireless-INX\""
#DEFS += EHS_CONFIG_WIFI_PASSWORD="\"HelloInx101\""

# Enable the serial command prompt interface
DEFS += EHS_ESP32_CMD_PROMPT_SUPPORT

EHS_WIFI_SUPPORT=yes
# Maximum wi-fi SSID records buffer size
DEFS += ESP32_MAX_AP_RECORDS=40

# OTA is not supported for the community at the moment
EHS_OTA_SUPPORT=stubbed

# Enable linked-list based memory allocator managment in the hal_mem
#EHS_MEMORY_MANAGMENT=yes

EHS_DEFAULT_APP=tutorials/hello_world

#include ./target/devman-configs/devman.inxware.io.mk

# appland deployment config

include ./target/devman-configs/esp32s3-base-inx-systems.com.mk

#Add Information for the appland presentation

EHS_APPLAND_INST_SUPPORT=yes
EHS_APPLAND_INST_DEPLOY_NAME=Xtensa-ESP32-S3-Community
EHS_APPLAND_INST_OS_NAME=xtensa-esp32-s3

################################### END OF TOOLBOX CONFIGURATION ###################################################
