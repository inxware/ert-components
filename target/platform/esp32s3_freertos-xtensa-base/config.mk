#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for esp32_freertos-xtensor-base
# @author: inx limited

# set pre-build env
export EHS_TARGETENV_PREBUILD_LITTLE_FS_APP_PARTITION=yes
#export EHS_TARGETENV_PREBUILD_<other>=yes

#################################################################################################################
# Set general architecture and OS version
#################################################################################################################

# MUST SET the following for any component config: 
# CPU and OS Type Selection 
EHS_ARCH=xtensa
EHS_OS=esp32s3_freertos

# TOOLCHAIN_NAME is an optional alternative location to find the toolchain. 
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_ 
#TOOLCHAIN_NAME=xtensa-esp32s3-elf-4.4.4
TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1

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

# Note the following might not remain lwip - we could use esp32's native mqtt component instead of ert#s lwip based one (TBC if this is difficult
DEFS += EHS_NO_LIBXML2_SUPPORT=1
EHS_MQTT_SUPPORT=esp32
EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_GUI_SUPPORT=lvgl
# Note the specifc Component-HAL support for IO is the same for allesp32 so is defined in the os-arch

# we may also no use any lwip stuff sirectly in EHS
DEFS += EHS_LWIP
DEFS += EHS_DEBUG_CONSOLE_BUFFER_SIZE=256
# This should be dine int the base_xxx.h file now: DEFS += EHS_TARGET_FP_SUPPORT=1
DEFS += EHS_STRING_LENGTH_MAX=256

# LoRaWAN support
DEFS += EHS_LORAWAN_SUPPORT
EHS_LORAWAN_SUPPORT=yes

# Wi-Fi Support
DEFS += EHS_WIFI_SUPPORT
EHS_WIFI_SUPPORT=yes

# enable eRT1 support
ERT_SODL_VERSION=1

# exclude unused xml parser which saves around 200k of the flash on esp32s3 device
EHS_EXCLUDE_XML_PARSER=yes

EHS_PID_SUPPORT=esp32
EHS_SCHEDULER_SUPPORT=1

EHS_OTA_SUPPORT=yes

EHS_TARGET_NO_MAIN_ARGS=yes
#DEFS += EHS_RUNTIME_LOGGER_ENABLED
#DEFS += EHS_DEBUG_TCPIP_CONSOLE

#Application Selection
EHS_DEFAULT_APP=hardware-test-apps/HRDCv2


################################### END OF TOOLBOX CONFIGURATION ###################################################
