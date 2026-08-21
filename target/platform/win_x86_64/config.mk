#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for win_x86_gtk_gst
# @author: inx limited


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
EHS_ARCH=x86#
EHS_OS=mingw

EHS_GNU_ARCH=x86_64
EHS_GNU_OS=mingw32posix

TOOLCHAIN_NAME=HOST
# Unversioned on purpose: the gcc version belongs to the Docker image
# (Dockerimagename), not to this config. Both jammy (gcc 10) and noble
# (gcc 13) provide this driver name, so the image can move without
# touching seven platform configs.
CC_OVERRIDE=x86_64-w64-mingw32-gcc-posix
# Must be set too: target/os-arch/mingw-x86/toolchain.mk defaults LINK_OVERRIDE
# to i686-w64-mingw32-gcc, so overriding only CC compiles 64-bit objects and
# then links them with the 32-bit linker.
LINK_OVERRIDE=x86_64-w64-mingw32-gcc-posix

# COMPONENT_VARIANT=gtk-gst - lets use the normal one as used by LVGL that has the MQTT stuff in it


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Debug/Production mode
EHS_DEBUGALL=true


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- Networking Features -----
EHS_NETWORKING_SUPPORT=all# This needs to be enabled for the core EHS - there is a HAL dependency
EHS_COMPONENT_NETWORKING_SUPPORT=all

#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http

#----- GUI Features -----
#EHS_GUI_SUPPORT=none
#EHS_AV_SUPPORT=gst - not with i686 mingw
#EHS_AV_SUPPORT=none
#EHS_VIDEO_SUPPORT=none
#EHS_MEDIA_SUPPORT=all

#----- Peripheral Features -----
#EHS_PERIPHERAL_DEVICE_SUPPORT=all


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# The 32-bit win_x86 sets no COMPONENT_VARIANT, so platform.mk:244 derives
# i686-mingw32posix-HOST -- a directory that has never existed, which is why
# that target cannot compile hal_url.h (curl/curl.h: No such file). Giving the
# 64-bit base target a real variant fixes it here rather than inheriting the
# fault. "base" is networking + TLS + AWS CRT, no GUI or media.
COMPONENT_VARIANT=base
SYSTEM_VARIANT=windesktop

# Used for specifing name of the eRT package/executable
ERT_PACKAGE_NAME=ehs
# Used for specifing user facing name of installed application (windows installer)
ERT_NSIS_EXE_NAME=eRT


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=none
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=no


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# (No legacy DEFS for this target)


# LoRaWAN modem support (WIO-E5 via UART/serial)
EHS_LORAWAN_SUPPORT=wio_e5

################################### END OF CONFIGURATION ###################################################
