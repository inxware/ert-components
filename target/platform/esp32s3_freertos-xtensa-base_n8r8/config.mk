#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for esp32s3_freertos-xtensa-base_n8r2
# THIS IS for ESP32-S3-XXXXX-N8R2 (8 MB Flash, 2 MB PSRAM)
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

# (No parent config - this is a base configuration for N8R2 variant)


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# CPU and OS Type Selection
EHS_ARCH=xtensa
EHS_OS=esp32s3_freertos

# TOOLCHAIN_NAME - libraries built with 32k cache
TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1
# libraries built with 16k cache (alternative)
#TOOLCHAIN_NAME=xtensa-esp32s3-elf-5.1-16k

# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory
# N8R8 = 8 MB Flash (N8) and 8 MB PSRAM (R8) or more (R16)
COMPONENT_VARIANT=n8r8

# Apply esp32 specific targetenv hacks
INXWARE_TARGETENV_HACKS=esp32

# Export ESP32 platform flag
export EHS_ESP32=yes
export EHS_NO_LIBXML2_SUPPORT=1


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
EHS_TARGET_APPLOAD_RESTARTING_REBOOT=yes


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
EHS_COMPONENTS_NETWORK_URL_GET=none
EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER=none
EHS_COMPONENTS_NETWORK_TCPIP_SOCKET=yes
EHS_MQTT_SUPPORT=esp_mqtt

# LoRaWAN support
EHS_LORAWAN_SUPPORT=yes

# Wi-Fi Support (uncomment to enable)
#EHS_NETWORK_WIFI_SUPPORT=yes

# BLE support (uncomment to enable)
EHS_NETWORK_BLE_SUPPORT=nimble

#----- GUI Features -----
EHS_GUI_SUPPORT=lvgl
EHS_LVGL_DISPLAY_DRIVER=ft81x
EHS_LVGL_TOUCH_DRIVER=ft81x

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_PERIPHERALS_PWM_SUPPORT=esp32
EHS_UART_SUPPORT=yes
EHS_MODBUS_SUPPORT=yes

# PID controller (ISR mode for ESP32)
EHS_PID_SUPPORT=esp32

# Scheduler support
EHS_SCHEDULER_SUPPORT=1

# Watchdog support
EHS_WATCHDOG_SUPPORT=ESP32S3

# File system config support
EHS_CONFIGS_SUPPORT=yes

# OTA support
EHS_OTA_SUPPORT=yes

# Serial Console Support
EHS_SERIAL_CONSOLE_SUPPORT=yes

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

# Platform identification
#DEFS += EHS_ESP32

# XML/libxml2 disabled
DEFS += EHS_NO_LIBXML2_SUPPORT=1

# Networking stack
DEFS += EHS_LWIP

# Console/debug buffer sizes
DEFS += EHS_DEBUG_CONSOLE_BUFFER_SIZE=256
DEFS += EHS_DEBUG_CONSOLE_THREAD_STACK_SIZE=2048

# Numeric type configurations
DEFS += EHS_FLOAT_AS_FLOAT_TYPE=1
DEFS += EHS_COORD_16_ENABLED

# UART configuration
DEFS += EHS_TARGET_UART_COUNT=3



################################### END OF CONFIGURATION ###################################################
