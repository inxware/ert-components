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

include ./target/platform/esp32s3_freertos-xtensa-base/config.mk

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
## uartusb-N8R2 is the module variant of esp32-s3 module.
##  It means default UART console on with 16 MB Flash (N16) and 2 MB PSRAM (R2)
COMPONENT_VARIANT=uartusb-n16r2

# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).
#SYSTEM_VARIANT=esp32s3-hrdcv2

#todo we need to change the defaults to the os-arch so we don't undu base stuff and get a multi-def warning/
EHS_WATCHDOG_SUPPORT=stubbed


# make sure to use libraries built with 32k cache for this target (DO WE REALLY NEED THIS?)
#TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1

# Enable debug mode
EHS_DEBUGALL=yes

EHS_RUNTIME_LOGGER_ENABLED=no

# Disable linked-list based memory allocator managment in the hal_mem
#EHS_MEMORY_MANAGMENT=none
EHS_NETWORK_ETHERNET_SUPPORT=none
EHS_NETWORK_BLE_SUPPORT=none

#We shouldn't need this here - we know it's an ESP32 already from the os arch
EHS_NETWORK_CONFIG_TYPE_SUPPORT=esp32
# this target connects to inx-systems mqtt broker
include ./target/devman-configs/esp32s3-hrdcv2-inx-systems.com.mk


EHS_NETWORK_WIFI_SUPPORT=yes
EHS_DEVMAN_SUPPORT=mqtt

EHS_TARGET_APPLOAD_RESTARTING_REBOOT=no

EHS_LVGL_DISPLAY_DRIVER=gc9a01
#What does leaving this blank mean???
EHS_LVGL_TOUCH_DRIVER=
# App to run
#EHS_DEFAULT_APP=tutorials/hello_world
EHS_DEFAULT_APP=demos/UI_for_Waveshare

#Hacks that need to be removed or moved to targetconfig.h
################################################################################################
# Enable the serial command prompt interface
DEFS += EHS_ESP32_CMD_PROMPT_SUPPORT
# Disable esp logger  why???
DEFS += EHS_ESP32_DISABLE_LOGS=1
DEFS += EHS_CONFIG_WIFI_SSID="\"Wireless-XXX\""
DEFS += EHS_CONFIG_WIFI_PASSWORD="\"XXXXXX""
# Maximum wi-fi SSID records buffer size
DEFS += ESP32_MAX_AP_RECORDS=20
#This needs to be done properly and say what it actually does not how it does something
DEFS += EHS_NANOPRINTF_SUPPORT=1
#The following should be done using the TARGETVARIANT config variable.
# HRDCV2 product config
## "2" means Rev. B
DEFS += EHS_PLATFORM_ESP32S3_HRDCV2=2
#This was 1200 
#DEFS += EHS_MAIN_ESP32_TASK_STACK_SIZE=12000
DEFS += EHS_MAIN_ESP32_TASK_STACK_SIZE=16000


################################### END OF TOOLBOX CONFIGURATION ###################################################
