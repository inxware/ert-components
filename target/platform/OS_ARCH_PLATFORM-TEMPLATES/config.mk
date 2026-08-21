#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for XXXXXXXXXXXXXXXXXXXXXXXXX
# @author: XXXXXXXXXXXXXXXXXXXXXXXx


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

# (No parent config - this is a base configuration)
# OR include a parent config:
#include ./target/platform/<parent-target>/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# CPU and OS Type Selection
EHS_ARCH=amd64
EHS_OS=linux

# GNU-specific naming conventions (for contrib builds)
#EHS_GNU_ARCH=x86_64
#EHS_GNU_OS=linux-gnu
#EHS_TARGET_LIB_VARIANT=

# TOOLCHAIN_NAME is an optional alternative location to find the toolchain
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_
TOOLCHAIN_NAME=HOST
#CC_OVERRIDE=
#LINK_OVERRIDE=

# SYSTEM_VARIANT for conditional compilation for very specific features
# See target/envbuildscripts/targetenv_hacks_*.sh scripts
#SYSTEM_VARIANT=

# Apply target-specific hacks (esp32, arduino, etc.)
#INXWARE_TARGETENV_HACKS=

# Component variant for contributed libraries
# $(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME)
COMPONENT_VARIANT=base
#or COMPONENT_VARIANT=gtk_gst add -gtk_gst to the contrib middleware path

# For non-conformal paths to component libraries


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Debug/Production mode (yes/true = debug, empty = production)
EHS_DEBUGALL=yes

# Runtime logger
#EHS_RUNTIME_LOGGER_ENABLED=no

# TCPIP debug console
#EHS_DEBUG_TCPIP_CONSOLE=yes

# Serial console support
#EHS_SERIAL_CONSOLE_SUPPORT=yes

# Embedded targets: don't read main() arguments (saves memory)
#EHS_TARGET_NO_MAIN_ARGS=yes

# Reboot after app load
#EHS_TARGET_APPLOAD_RESTARTING_REBOOT=no


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# eRT1 SODL support
#ERT_SODL_VERSION=1

# Exclude unused xml parser (saves ~200k flash on embedded targets)
#EHS_EXCLUDE_XML_PARSER=yes

# Memory management (uncomment to disable linked-list based allocator)
#EHS_MEMORY_MANAGMENT=none

#----- Networking Features -----
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all
#EHS_COMPONENTS_NETWORK_URL_GET=none
#EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER=none
#EHS_COMPONENTS_NETWORK_TCPIP_SOCKET=yes

# Devman support (http, mqtt, none)
EHS_DEVMAN_SUPPORT=http

# MQTT support (esp_mqtt, lwip, aws_green_grass, etc.)
#EHS_MQTT_SUPPORT=

# LoRaWAN support
#EHS_LORAWAN_SUPPORT=yes

# Wi-Fi support
#EHS_NETWORK_WIFI_SUPPORT=yes

# Ethernet support
#EHS_NETWORK_ETHERNET_SUPPORT=yes

# BLE support (nimble, stubbed, etc.)
#EHS_NETWORK_BLE_SUPPORT=nimble

# OTA support (yes, none, stubbed)
#EHS_OTA_SUPPORT=

#----- GUI Features -----
##EHS_GUI_SUPPORT=gtk
##EHS_AV_SUPPORT=gst10
##EHS_VIDEO_SUPPORT=yes
##EHS_MEDIA_SUPPORT=all
# set EHS_DEBUG_AV for verbose debugg from the media sub system
#DEFS += EHS_DEBUG_AV for more verbose debugging of AV media susbsystems

# LVGL display and touch driver support
#EHS_LVGL_DISPLAY_DRIVER=
#EHS_LVGL_TOUCH_DRIVER=

# AV media support (gst, vlc)
#EHS_AV_SUPPORT=gst

# Video rendering support
#EHS_VIDEO_SUPPORT=yes

# Media support (rendering features)
#EHS_MEDIA_SUPPORT=all

#----- Peripheral Features -----
# IO features (GPIO, ADC, DAC, serial, user inputs)
EHS_PERIPHERAL_DEVICE_SUPPORT=all

# PWM support
#EHS_PERIPHERALS_PWM_SUPPORT=esp32

# UART/Modbus support
#EHS_UART_SUPPORT=yes
#EHS_MODBUS_SUPPORT=yes

# I2C support
#EHS_I2C_SUPPORT=yes

# RTC support
#EHS_RTC_SUPPORT=yes

# PID controller support
#EHS_PID_SUPPORT=esp32

# Scheduler support
#EHS_SCHEDULER_SUPPORT=1

# Watchdog support
#EHS_WATCHDOG_SUPPORT=

# Config file system support
#EHS_CONFIGS_SUPPORT=yes


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# Default application to run
#EHS_DEFAULT_APP=tutorials/hello_world

# Packager type (deb, etc.)
#EHS_PACKAGER_TYPE=deb

# Package naming
#ERT_PACKAGE_NAME=ehs
#ERT_NSIS_EXE_NAME=eRT


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

#include ./target/devman-configs/inx-systems.com.mk

#DEVMAN_SERVER_DOMAIN=
#DEVMAN_SERVER_PROTOCOL=https
#DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
#DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=no


#################################################################################################################
# Appland Deployment Configuration
#################################################################################################################

#EHS_APPLAND_INST_SUPPORT=yes
#EHS_APPLAND_INST_DEPLOY_NAME=
#EHS_APPLAND_INST_OS_NAME=


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################


# XML/libxml2 disabled
#DEFS += EHS_NO_LIBXML2_SUPPORT=1

# Networking stack
#DEFS += EHS_LWIP

# Console/debug buffer sizes
#DEFS += EHS_DEBUG_CONSOLE_BUFFER_SIZE=512
#DEFS += EHS_DEBUG_CONSOLE_THREAD_STACK_SIZE=4096

# set EHS_CONSOLE_QUEUE_STATS to log every console queue push (bytes pushed, %% of buffer
# used) via EHSH_LOG_INFO - use to size EHS_DEBUG_CONSOLE_BUFFER_SIZE per target/application
#DEFS += EHS_CONSOLE_QUEUE_STATS

# Task stack size
#DEFS += EHS_MAIN_ESP32_TASK_STACK_SIZE=10000

# Numeric type configurations
#DEFS += EHS_FLOAT_AS_FLOAT_TYPE=1
#DEFS += EHS_COORD_16_ENABLED

# UART configuration
#DEFS += EHS_TARGET_UART_COUNT=3

# File system workaround
#DEFS += EHS_TARGET_FILE_SKIP_STAT

# Nanoprintf support
#DEFS += EHS_NANOPRINTF_SUPPORT=1


#################################################################################################################
# Host OS Configuration Scripts (for first install)
#################################################################################################################

# See target/envtree/$EHS_OS-$EHS_ARCH/ for scripts available for configuring host OS on first boot.
#HOST_OS_CONFIG_SCRIPTS+= \
#    0450-update-Realtek-net-firmware \


################################### END OF CONFIGURATION ###################################################
