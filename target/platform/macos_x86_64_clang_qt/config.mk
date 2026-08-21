#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for macos_x86_64_clang_qt
# Host (no Docker) build using Apple clang on macOS Intel, with Qt6/QML GUI.
# @author: inx limited
#
# Prerequisites (one-time setup):
#   brew install qt
#   (or run: ./target/envbuildscripts/prepdeps.sh)


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
#################################################################################################################

EHS_ARCH=x86_64
EHS_OS=macos

EHS_GNU_ARCH=x86_64
EHS_GNU_OS=darwin

EHS_TOOLCHAIN_TYPE=clang
TOOLCHAIN_NAME=HOST

EHS_HOST_MACOS_BUILD=x86_64

COMPONENT_VARIANT=base


#################################################################################################################
# Debug and Startup Modifiers
#################################################################################################################

EHS_DEBUGALL=true

EHS_LOG_LEVEL_GRAPHICS=EHSH_LOG_LEVEL_ALL


#################################################################################################################
# macOS Compatibility Definitions
#################################################################################################################

DEFS += EHS_MACOS
DEFS += EHS_BSD
DEFS += EHS_USE_ORDINARY_NANOSLEEP
DEFS += _DARWIN_C_SOURCE


#################################################################################################################
# Feature Configuration
#################################################################################################################

#----- Networking -----
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all
EHS_DEVMAN_SUPPORT=http

#----- GUI -----
# Qt drives the event loop from main(); EhsMainLoopSingle() is called from a Qt
# timer callback rather than from a blocking EhsMain() call. Same pattern as
# linux_x86_64_qt_debian12-no-certs.
EHS_MAIN_LOOP_ITERATIVE=yes

EHS_GUI_SUPPORT=qt
EHS_GUI_SUPPORT_QT6=yes

EHS_AV_SUPPORT=devmanonly
EHS_MEDIA_SUPPORT=all

# Disable the TCP/IP console — the Qt main thread can't block on console I/O.
EHS_DEBUG_TCPIP_CONSOLE=yes

#----- Machine Vision -----
EHS_MV_SUPPORT=stubbed

#----- Peripheral -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
EHS_PERIPHERALS_ADC_DAC_SUPPORT=stubbed


#################################################################################################################
# Device Management Credentials
#################################################################################################################

include ./target/devman-configs/inx-systems.com.mk


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
#################################################################################################################

DEFS += EHS_LOG_LEVEL_VERBOSE
DEFS += EHS_LOG_TO_STDIO

# Force 'unknown' source files to log at the logger's level
DEFS += EHSL_MODULE_ID=EHSH_LOG_MODULE_LOGGER

DEFS += ERTQT_SINGLETON_SCAN


################################### END OF CONFIGURATION ###################################################
