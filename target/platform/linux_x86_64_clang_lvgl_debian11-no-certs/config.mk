#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_x86_gtk_gst_debian11
# @author: inx limited

#Target Platform Uses
# For 64 bit linux running on Debian 11. Generic Vanilla version.
# All contributed middlware is provided by the container container inxware/inx-debian11


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

#It seems the Debian 10 greengrass build works fine with debian 11 so we are using this for now.
EHS_GNU_OS_VERSION=-clang10_clang10

EHS_TOOLCHAIN_TYPE=clang
TOOLCHAIN_NAME=HOST
EHS_HOST_DEBIAN_BUILD=x86_64
#IMPORTANT - ELF files built with llvm make object files in a bit format that the gcc ld linker doesn't recognize.
# So we must use the llvm linker instead and remove all the gcc -Wl directoves to linker options..
#LINK_OVERRIDE=ld.lld
#clang linker is the ld.ldd called via clang so it gets all the magic incantations.
LINK_OVERRIDE=clang
EHS_DEBIAN_VERSION=11

# Contributed library dependencies variant
#COMPONENT_VARIANT=gtk_gst - we don't need this because we use the host's dev libs
COMPONENT_VARIANT=base

# For non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms rather than built in ert-contrib-middleware):
# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
#COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

#todo2023 - we need to add this to everyting until we decide if we will have a debug and non-debug kernel. (I guess we ultimately wan the latter to mimise sizes.)
EHS_DEBUGALL=yes


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- Networking Features -----
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all
EHS_MQTT_SUPPORT=aws_green_grass

#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http

#----- GUI Features -----
EHS_GUI_SUPPORT=lvgl
EHS_AV_SUPPORT=ffmpeg
EHS_VIDEO_SUPPORT=yes
EHS_MEDIA_SUPPORT=all
EHS_TOOLKIT_DEPRECATED=yes

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_PERIPHERALS_ADC_DAC_SUPPORT=stubbed
EHS_PERIPHERALS_GPIO_SUPPORT=gui
EHS_PID_SUPPORT=stubbed
#EHS_PID_SUPPORT=gnu
EHS_SCHEDULER_SUPPORT=1
EHS_MODBUS_SUPPORT=stubbed


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_DEFAULT_APP=tutorials/hello_world
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

# (No legacy DEFS for this target)


# LoRaWAN modem support (WIO-E5 via UART/serial)
EHS_LORAWAN_SUPPORT=wio_e5

################################### END OF CONFIGURATION ###################################################
