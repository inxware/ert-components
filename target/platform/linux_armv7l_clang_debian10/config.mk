#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_armv7l_clang
# @author: inx limited

#Target Platform Uses
# This is the horrific arm / raspberry pi config with scraped components and uses a clang compiler
# so that is can potentially integrate green grass components built with the same compiler
# It should be refactored to used docker and Debians arm cross compile environment for Rasberrian
# All contributed middlware is provided by a munge of things we want rid of ???


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
EHS_ARCH=arm
EHS_OS=linux

EHS_GNU_ARCH=armv7l
EHS_GNU_OS=linux-gnu

EHS_TOOLCHAIN_TYPE=clang
TOOLCHAIN_NAME=HOST

# SET THIS ONLY IF YOU ALSO WANT TO USE THE HOST'S /usr/include and library paths for depedencies
EHS_HOST_DEBIAN_BUILD=armhf

#EHS_TARGET_LIB_VARIANT=-clang10ubuntu18
EHS_DEBIAN_VERSION=10

#Experimental hacks:
CFLAGS += -Wno-error=deprecated-declarations

# Contributed library dependencies variant
COMPONENT_VARIANT=base

# For non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms rather than built in ert-contrib-middleware):
# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).


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
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all

#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http

#----- GUI Features -----
EHS_AV_SUPPORT=devmanonly
#EHS_VIDEO_SUPPORT=no
EHS_MEDIA_SUPPORT=all
EHS_TOOLKIT_DEPRECATED=yes


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# (No application/packaging settings for this target)


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
