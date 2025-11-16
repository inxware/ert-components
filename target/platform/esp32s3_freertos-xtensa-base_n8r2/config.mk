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

## THIS IS for ESP32-S3-XXXXX-N8R2

# MUST SET the following for any component config: 
# CPU and OS Type Selection 
EHS_ARCH=xtensa
EHS_OS=esp32s3_freertos

# TOOLCHAIN_NAME is an optional alternative location to find the toolchain. 
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_ 
#TOOLCHAIN_NAME=xtensa-esp32s3-elf-4.4.4
# libraries built with 32k cache
TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1
# libraries built with 16k cache
#TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1-16k

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
## N8R2 is the module variant of esp32-s3 module.
##  It means 8 MB Flash (N8) and 2 MB PSRAM (R2)
COMPONENT_VARIANT=n8r2

# apply esp32 specific hacks
INXWARE_TARGETENV_HACKS=esp32

################################################################################################################
# Configure debug/production levels
################################################################################################################

#todo2022 DEFS+ should be done conditionally on EHS_ESP32 in the os-arch/xtensa target.mk
export EHS_ESP32=yes
DEFS += EHS_ESP32

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# Note the following might not remain lwip - we could use esp32's native mqtt component instead of ert#s lwip based one (TBC if this is difficult
#todo sort this XML macro stuff out
DEFS += EHS_NO_LIBXML2_SUPPORT=1
export EHS_NO_LIBXML2_SUPPORT=1 

EHS_MQTT_SUPPORT=esp_mqtt
EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_GUI_SUPPORT=lvgl

# Set LVGL related driver support
EHS_LVGL_DISPLAY_DRIVER=ft81x
EHS_LVGL_TOUCH_DRIVER=ft81x

# Note the specifc Component-HAL support for IO is the same for allesp32 so is defined in the os-arch

#TODO2024 All these need turning into make variables and not pre-processor ones:
# we may also no use any lwip stuff sirectly in EHS
DEFS += EHS_LWIP
DEFS += EHS_DEBUG_CONSOLE_BUFFER_SIZE=256
DEFS += EHS_DEBUG_CONSOLE_THREAD_STACK_SIZE=2048
# ehs_float as float, not as double
DEFS += EHS_FLOAT_AS_FLOAT_TYPE=1
# ehs_coord is using ehs_sint16 for graphics type e.g. EhsGraphicsRectangleClass
DEFS += EHS_COORD_16_ENABLED

# network components setting
EHS_COMPONENT_NETWORKING_SUPPORT=no-curl
# exclude components which contain curl
EHS_COMPONENTS_NETWORK_URL_GET=none
EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER=none
# include tcp/ip clinet & server socket fb components
EHS_COMPONENTS_NETWORK_TCPIP_SOCKET=yes

# This should be dine int the base_xxx.h file now: DEFS += EHS_TARGET_FP_SUPPORT=1
#DEFS += EHS_STRING_LENGTH_MAX=256

# Make sure debug mode is disabled 
EHS_DEBUGALL=

# esp32 doesn't read main() function arguments. This saves some memory by not
# creating string buffers used for reading the args
EHS_TARGET_NO_MAIN_ARGS=yes

# Disable linked-list based memory allocator managment in the hal_mem
#EHS_MEMORY_MANAGMENT=none

# LoRaWAN support
EHS_LORAWAN_SUPPORT=yes

# Wi-Fi Support
# DEFS += EHS_NETWORK_WIFI_SUPPORT
# EHS_NETWORK_WIFI_SUPPORT

# enable eRT1 support
ERT_SODL_VERSION=1

# exclude unused xml parser which saves around 200k of the flash on esp32s3 device
EHS_EXCLUDE_XML_PARSER=yes

#This enables the ISR mode PID controller in ESP32 targets
EHS_PID_SUPPORT=esp32

EHS_SCHEDULER_SUPPORT=1

#Add Built-in Devman OTA SUPPORT (or is this a function block?)
EHS_OTA_SUPPORT=yes


#EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
#EHS_OTA_SUPPORT=stubbed
EHS_UART_SUPPORT=yes

EHS_MODBUS_SUPPORT=yes
DEFS += EHS_TARGET_UART_COUNT=3

EHS_PERIPHERALS_PWM_SUPPORT=esp32

# basic file system confing support
EHS_CONFIGS_SUPPORT=yes

EHS_WATCHDOG_SUPPORT = ESP32S3

EHS_TARGET_APPLOAD_RESTARTING_REBOOT=yes

EHS_INTERFACE_CONFIG_TYPE_SUPPORT=esp32

#Application Selection
EHS_DEFAULT_APP=tutorials/hello_world

include ./target/devman-configs/esp32s3-base-inx-systems.com.mk

################################### END OF TOOLBOX CONFIGURATION ###################################################

DEFS += EHS_CONFIG_WIFI_SSID="\"Wireless-INX\""
DEFS += EHS_CONFIG_WIFI_PASSWORD="\"HelloInx101\""