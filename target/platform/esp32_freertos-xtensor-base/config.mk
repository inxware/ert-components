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

# MUST SET the following for any component config: 
# EHS Section 
# ehs is more generic
EHS_ARCH=xtensa
EHS_OS=esp32_freertos
# TOOLCHAIN_NAME is an optional alternative location to find the toolchain. 
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_ 
TOOLCHAIN_NAME=xtensa-esp32-elf-4.4.1

################################################################################################################
# Configure debug/production levels
################################################################################################################

#todo2022 DEFS+ should be done conditionally on EHS_ESP32 in the os-arch/xtensa target.mk
export EHS_ESP32=yes
DEFS += EHS_ESP32
EHS_DEBUGALL=yes

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################
#todo  why is the stubbed file system so difficult compared to others/ We should move the the Component HAL and yse that method as more consistent?
EHS_FILESYSTEM_SUPPORT=stubbed
DEFS += EHS_TARGET_FILE_SKIP_STAT

EHS_COMPONENT_NETWORKING_SUPPORT=nocurl
# Note the following might not remain lwip - we could use esp32's native mqtt component instead of ert#s lwip based one (TBC if this is difficult
DEFS += EHS_NO_LIBXML2_SUPPORT=1
EHS_MQTT_SUPPORT=esp_mqtt-441
EHS_PERIPHERAL_DEVICE_SUPPORT=all
# Note the specifc Component-HAL support for IO is the same for allesp32 so is defined in the os-arch

# we may also no use any lwip stuff sirectly in EHS
DEFS += EHS_LWIP
DEFS += EHS_DEBUG_CONSOLE_BUFFER_SIZE=256
################################### END OF TOOLBOX CONFIGURATION ###################################################

#DEVMAN_SERVER_DOMAIN=devman.inx-systems.com
#DEVMAN_SERVER_PROTOCOL=https

include ./target/devman-configs/inx-systems.com.mk

EHS_DEFAULT_APP=NONE
