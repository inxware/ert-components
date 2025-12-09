#---------------------------------------------------------------
# Copyright (C) 2025 inx limited, UK - All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit:
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------

# This is for Raspberry Pi - it is aimed at being used with inxware 'hello_world' demo app

# Architecture and OS
EHS_ARCH=arm64
EHS_OS=linux
EHS_GNU_ARCH=arm64
EHS_GNU_OS=linux-gnu

# This determines the /usr/<arch> when using the host's toolchain
EHS_HOST_DEBIAN_BUILD=arm64

# This will indicate what ert-contrib-middleware is used and toolchains if not using a host toolchain
EHS_GNU_OS_VERSION=-clang11_debian11

# This determines some library version choices and for the Debian packager
EHS_DEBIAN_VERSION=12
DEFS += EHS_DEBIAN_VERSION=12

EHS_PACKAGER_TYPE=deb

# Use host toolchain + Qt from Docker
TOOLCHAIN_NAME=HOST

# Select the os-arch directory with these
EHS_TOOLCHAIN_TYPE=clang
LINK_OVERRIDE=clang

# Networking (standard)
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all
# EHS_MQTT_SUPPORT=aws_green_grass

# Unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http

# We should try Wi-Fi by default in Raspberry Pi devices
DEFS += EHS_USE_WIFI_INTERFACE=1

# Peripheral support
EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_PERIPHERALS_GPIO_SUPPORT=sysfs_linux_arm
EHS_PERIPHERALS_ADC_DAC_SUPPORT=SPI_A6_LTC241X

# Component variant
COMPONENT_VARIANT=base

# Application Selection
EHS_DEFAULT_APP=tutorials/hello_world_qt

# Enable eRT1 support
ERT_SODL_VERSION=1

# Debugging
EHS_DEBUGALL=true
EHS_RUNTIME_LOGGER_ENABLED=yes
DEFS += EHS_LOG_LEVEL_VERBOSE
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_LOG_TO_STDIO
DEFS += EHSL_MODULE_ID=EHSH_LOG_MODULE_LOGGER

# Toolboxes and supporting middleware options...

# To enable UI support ("ui", DCC=4) set EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target
# EHS_GUI_SUPPORT=gtk
EHS_GUI_SUPPORT=qt

# Graphics configuration
EHS_GUI_SUPPORT_MODE_B_QT=yes
DEFS += EHS_GUI_SUPPORT_MODE_B_QT=yes

# Qt uses iterative event loop pattern - TODO
EHS_MAIN_LOOP_ITERATIVE=yes
DEFS += EHS_MAIN_LOOP_ITERATIVE=yes

# To enable AV media support ("media", DCC=5) set EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target
EHS_AV_SUPPORT=devmanonly

# This  is set to include the rendering features in eRT. It is nearly always set, so should be removed (default on) and specific platform exceptions set instead
EHS_MEDIA_SUPPORT=all

# Enable machine vision support for testing C++ integration
EHS_MV_SUPPORT=stubbed
