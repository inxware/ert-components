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
EHS_ARCH=amd64
#todo2023 the following should be used when the x86_64 kernel build is fixed.
##EHS_ARCH=x86
EHS_OS=linux

# Optionally set the following if contrib build uses GNU-specific OS and ARCH naming conventions
# - Select the os-arch directory with these
#todo2023 the followig is what we should be using here, but the kernel is not built right for x86_64
##EHS_GNU_ARCH=x86_64
##EHS_GNU_OS=linux-gnu

#Select a specific version of contrib libraries and build support
#EHS_GNU_OS_VERSION=-1.0

#EHS_TOOLCHAIN_TYPE=gcc - leaving as default

# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_OS_ARCH
TOOLCHAIN_NAME=HOST
#OR target a specific toolchain: This target is using the same compiler as for the 32bit vlang build:
#TOOLCHAIN_PATH=./x86_64/XXXX

#Select a targe version for Debian. This may affect linking options to host libraries and the packager
EHS_DEBIAN_VERSION=11
# SET THIS ONLY IF YOU ALSO WANT TO USE THE HOST'S /usr/include and library paths for depedencies to pick up the rigt architecture
EHS_HOST_DEBIAN_BUILD=x86

# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).
# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features
#SYSTEM_VARIANT=

# Contributed library dependencies variant
COMPONENT_VARIANT=gtk_gst

# For non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms rather than built in ert-contrib-middleware):
# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
#COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

#todo2023 - we need to add this to everyting until we decide if we will have a debug and non-debug kernel. (I guess we ultimately wan the latter to mimise sizes.)
EHS_DEBUGALL=true


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- Networking Features -----
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all

#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http

#----- GUI Features -----
EHS_GUI_SUPPORT=gtk
EHS_AV_SUPPORT=gst10
EHS_VIDEO_SUPPORT=yes
EHS_MEDIA_SUPPORT=all
EHS_TOOLKIT_DEPRECATED=yes

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_PERIPHERALS_GPIO_SUPPORT=stubbed


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_PACKAGER_TYPE=deb


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

include ./target/devman-configs/inx-systems.com.mk
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# (No legacy DEFS for this target)


# LoRaWAN modem support (WIO-E5 via UART/serial)
EHS_LORAWAN_SUPPORT=wio_e5

################################### END OF CONFIGURATION ###################################################
