#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-n8r2_waveshare_wearable
# THIS IS for ESP32-S3-XXXXX-N8R2 - 8MB PSRAM devices
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

# (No parent config - this is a standalone configuration for N8R2 wearable)


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# CPU and OS Type Selection
EHS_ARCH=xtensa
EHS_OS=esp32s3_freertos

# TOOLCHAIN_NAME - libraries built with 32k cache
TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1

# COMPONENT_VARIANT - N8R2 = 8 MB Flash (N8) and 2 MB PSRAM (R2)
COMPONENT_VARIANT=n8r2

# Apply esp32 specific targetenv hacks
INXWARE_TARGETENV_HACKS=esp32

#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Debug/Production mode (empty = production)
EHS_DEBUGALL=

# Runtime logger
EHS_RUNTIME_LOGGER_ENABLED=no

# esp32 doesn't read main() function arguments - saves memory
EHS_TARGET_NO_MAIN_ARGS=yes

# Reboot after app load
EHS_TARGET_APPLOAD_RESTARTING_REBOOT=no

# Interface config type
EHS_INTERFACE_CONFIG_TYPE_SUPPORT=esp32


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# eRT1 SODL support
ERT_SODL_VERSION=1

# Exclude unused xml parser (saves ~200k flash on esp32s3)
EHS_EXCLUDE_XML_PARSER=yes

# Memory management (uncomment to disable linked-list based allocator)
#EHS_MEMORY_MANAGMENT=none

#----- Networking Features -----
EHS_COMPONENT_NETWORKING_SUPPORT=no-curl
EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER=none
EHS_MQTT_SUPPORT=esp_mqtt

# Network support disabled
EHS_NETWORK_ETHERNET_SUPPORT=none
EHS_NETWORK_BLE_SUPPORT=none

# Wi-Fi Support (uncomment to enable)
#EHS_NETWORK_WIFI_SUPPORT=yes

# LoRaWAN support
##EHS_LORAWAN_SUPPORT=wio_e5
EHS_LORAWAN_SUPPORT=none

#----- GUI Features -----
##EHS_GUI_SUPPORT=lvgl
EHS_GUI_SUPPORT=stubbed
EHS_LVGL_DISPLAY_DRIVER=ft81x
EHS_LVGL_TOUCH_DRIVER=ft81x

#----- Peripheral Features -----
##EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_PERIPHERAL_DEVICE_SUPPORT=none
##EHS_PERIPHERALS_PWM_SUPPORT=esp32
EHS_PERIPHERALS_PWM_SUPPORT=none

##EHS_UART_SUPPORT=yes
EHS_UART_SUPPORT=none

##EHS_MODBUS_SUPPORT=yes
EHS_MODBUS_SUPPORT=none



# PID controller (uncomment to enable ISR mode for ESP32)
#EHS_PID_SUPPORT=esp32

# Scheduler support
##EHS_SCHEDULER_SUPPORT=1
EHS_SCHEDULER_SUPPORT=0

# Watchdog support
##EHS_WATCHDOG_SUPPORT=ESP32S3

# File system config support
##EHS_CONFIGS_SUPPORT=yes

# OTA support
##EHS_OTA_SUPPORT=yes


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# Default application to run
##EHS_DEFAULT_APP=demos/UI_for_Waveshare
#EHS_DEFAULT_APP=tutorials/hello_world

#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

include ./target/devman-configs/esp32s3-base-inx-systems.com.mk


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################


# Wi-Fi credentials (defaults if none saved)
DEFS += EHS_CONFIG_WIFI_SSID="\"SSIDXXXXX\""
DEFS += EHS_CONFIG_WIFI_PASSWORD="\"XXXXXXXX\""


################################### END OF CONFIGURATION ###################################################
