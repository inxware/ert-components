#---------------------------------------------------------------
# Copyright (C) 2025 inx limited, UK - All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit:
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------

# This is for Raspberry Pi - it is aimed at being used with inxware 'hello_world' demo app.
# We're assuming Pi 4B or 5 below. Other flavours may work but haven't been tested.
#
# Architecture and OS
EHS_ARCH=arm64
EHS_OS=linux
EHS_GNU_ARCH=arm64
EHS_GNU_OS=linux-gnu
#
# This determines the /usr/<arch> when using the host's toolchain
EHS_HOST_DEBIAN_BUILD=arm64

# This will indicate what ert-contrib-middleware is used and toolchains if not using a host toolchain
# For the PoC, the clang Debian 11 will do...
#
EHS_GNU_OS_VERSION=-clang11_debian11

# Configure some library version choices and Debian packager specifics
EHS_DEBIAN_VERSION=12
DEFS += EHS_DEBIAN_VERSION=12
EHS_PACKAGER_TYPE=deb

# Use host toolchain + Qt headers and static objects from Docker
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

# Enable eRT1 support (binary format SODL files)
ERT_SODL_VERSION=1

# Debugging
EHS_DEBUGALL=true
DEFS += EHS_LOG_LEVEL_VERBOSE
DEFS += EHS_LOG_TO_STDIO
# DEFS += EHSL_MODULE_ID=EHSH_LOG_MODULE_LOGGER

# To enable UI support ("ui", DCC=4) set EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support
# for your target:
#
# EHS_GUI_SUPPORT=gtk
EHS_GUI_SUPPORT=qt

# Qt uses iterative event loop pattern in `target_main.c`. This means we poll `EhsMainLoop()` from a Qt timer to enable EHS
# to progress its state, rather than the more traditional (blocking) call into `EhsLoop()`.
#
EHS_MAIN_LOOP_ITERATIVE=yes
DEFS += EHS_MAIN_LOOP_ITERATIVE

# Graphics configuration - delegate all rendering to Qt and automatically plumb between the widgets in '*.gui' and their
# respective QObject (from '*.qml'. This requres the EHS widget name (string) to match the `objectName` in the QML.
#
EHS_GUI_SUPPORT_MODE_B_QT=yes
DEFS += EHS_GUI_SUPPORT_MODE_B_QT
# EHS_GUI_SUPPORT_MODE_B=yes
# DEFS += EHS_GUI_SUPPORT_MODE_B

# Mock GPIO widgets in Qt UI - enables desktop development of GPIO apps before hardware is available.
# Uncomment to enable virtual GPIO LED/switch widgets rendered in the Qt window.
#
# EHS_MOCK_GPIO_QT=yes
# DEFS += EHS_MOCK_GPIO_QT

# In the Arduino targets, they have their own `target_network.c` implementation which moves networking into a separate
# thread, so it doesn't block the main thread. For our Qt PoC, we simply disable the TCPIP console.
#
EHS_DEBUG_TCPIP_CONSOLE=stubbed

# To enable AV media support ("media", DCC=5) set EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target.
#
EHS_AV_SUPPORT=devmanonly

# This  is set to include the rendering features in eRT. It is nearly always set, so should be removed (default on) and
# specific platform exceptions set instead.
#
EHS_MEDIA_SUPPORT=all

# Enable machine vision support for testing C++ integration, or disable with `stubbed`.
#
EHS_MV_SUPPORT=stubbed
