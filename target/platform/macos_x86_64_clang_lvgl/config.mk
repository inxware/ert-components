#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for macos_x86_64_clang_lvgl
# Host (no Docker) build using Apple clang on macOS Intel, with LVGL+SDL2 GUI.
# @author: inx limited
#
# Prerequisites (one-time setup):
#   ./target/envbuildscripts/prepdeps.sh   (installs SDL2 via Homebrew)


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
EHS_GUI_SUPPORT=lvgl
EHS_TOOLKIT_DEPRECATED=yes
EHS_PERIPHERALS_GPIO_SUPPORT=gui

#----- Peripheral -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all


#################################################################################################################
# Device Management Credentials
#################################################################################################################

include ./target/devman-configs/inx-systems.com.mk


################################### END OF CONFIGURATION ###################################################
