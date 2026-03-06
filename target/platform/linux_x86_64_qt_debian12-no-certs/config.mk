#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_x86_64_qt_debian12-no-certs
# @author: inx limited

# Linux PC (x86_64) with Qt6, Debian 12.
# Based on linux_x86_64_clang_gg_debian11 and linux_arm64_qt_debian12-no-certs.


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

# (No parent config - this is a base configuration)


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# CPU and OS Type Selection
EHS_ARCH=x86
EHS_OS=linux
EHS_GNU_ARCH=x86_64
EHS_GNU_OS=linux-gnu

# This determines the /usr/<arch> when using the host's toolchain
EHS_HOST_DEBIAN_BUILD=x86

# This will indicate what ert-contrib-middleware is used and toolchains if not using a host toolchain
# Using gcc-based middleware for host gcc toolchain
#EHS_GNU_OS_VERSION=-clang11_debian11
EHS_GNU_OS_VERSION=-clang10_clang10
#EHS_GNU_OS_VERSION=-clang11_unknown

# Configure some library version choices and Debian packager specifics
EHS_DEBIAN_VERSION=12

# Use host toolchain + Qt headers and static objects from Docker
TOOLCHAIN_NAME=HOST

# Select the os-arch directory with these - using gcc (host default)
EHS_TOOLCHAIN_TYPE=gcc

# Contributed library dependencies variant (adds _base to the middleware path)
COMPONENT_VARIANT=base

# Enable eRT1 support (binary format SODL files)
ERT_SODL_VERSION=1


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Debug/Production mode
EHS_DEBUGALL=true

# Graphics subsystem log level
EHS_LOG_LEVEL_GRAPHICS=EHSH_LOG_LEVEL_ALL


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- Networking Features -----
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all
EHS_MQTT_SUPPORT=aws_green_grass

# Unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http

#----- GUI Features -----
# Qt uses iterative event loop pattern in `target_main.c`. This means we poll `EhsMainLoop()` from a Qt
# timer to enable EHS to progress its state, rather than the more traditional (blocking) call into `EhsLoop()`.
EHS_MAIN_LOOP_ITERATIVE=yes

EHS_GUI_SUPPORT=qt
# Set this as a modifier for Qt - not a whole new way of working.
EHS_GUI_SUPPORT_QT6=yes
#TODO2026 - we probably want Qt6 to be the default and the extra flag would be for Qt5.

# To enable AV media support set EHS_AV_SUPPORT to {gst,vlc} depending on target support.
EHS_AV_SUPPORT=devmanonly
EHS_MEDIA_SUPPORT=all

# In the Arduino targets, networking moves into a separate thread so it doesn't block the main thread.
# For this Qt target, simply disable the TCPIP console.
EHS_DEBUG_TCPIP_CONSOLE=stubbed

#----- Machine Vision / ML Features -----
# Enable machine vision support for testing C++ integration, or disable with `stubbed`.
EHS_MV_SUPPORT=stubbed

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
EHS_PERIPHERALS_ADC_DAC_SUPPORT=stubbed

# EHS_MOCK_GPIO_QT: Mock GPIO widgets in Qt UI for desktop development before hardware is available.
# Uncomment to enable virtual GPIO LED/switch widgets rendered in the Qt window.
# EHS_MOCK_GPIO_QT=yes
# DEFS += EHS_MOCK_GPIO_QT


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

#EHS_DEFAULT_APP=tutorials/hello_world
#EHS_DEFAULT_APP=demos/simple-qt-socket_webserver

EHS_PACKAGER_TYPE=deb


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

# (No devman config for this target - no-certs variant)


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

DEFS += EHS_DEBIAN_VERSION=12

DEFS += EHS_LOG_LEVEL_VERBOSE
DEFS += EHS_LOG_TO_STDIO

# Force 'unknown' source files to output logging at the logger's logging level
DEFS += EHSL_MODULE_ID=EHSH_LOG_MODULE_LOGGER

# Enable QML singleton scanning support in the object table builder.
# When defined, ertqt_add_singleton_scan() is compiled in and callers can register QML expressions
# (e.g. "Rooms") to be evaluated via QQmlExpression so that objects inside pragma Singleton files
# appear in the object table. Requires QQmlExpression / QQmlContext headers (included automatically).
DEFS += ERTQT_SINGLETON_SCAN


################################### END OF CONFIGURATION ###################################################
