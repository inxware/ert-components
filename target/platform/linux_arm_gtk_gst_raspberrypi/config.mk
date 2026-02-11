#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_arm_gtk_gst_raspberrypi
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
EHS_ARCH=arm
EHS_OS=linux

EHS_GNU_ARCH=arm#this must map onto the component library
EHS_GNU_OS=linux-gnu
EHS_GNU_OS_VERSION=""#arm-none-linux-gnueabi-4.6.1
#Optional if different clib build is required - dangerous!
#KERNEL_VERSION=linux/2.6.35.9

#Toolchain information
# TOOLCHAIN_NAME is an optional alternative location to find the toolchain.
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_
TOOLCHAIN_NAME=gcc-linaro-arm-linux-gnueabihf-raspbian
CC_OVERRIDE=arm-linux-gnueabihf-gcc
LD_SWITCHES+=-fno-use-linker-plugin

# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).
# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features
SYSTEM_VARIANT=RASPBERRYPI

# Contributed library dependencies variant
COMPONENT_VARIANT=

# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=arm-linux-gnu-arm-none-linux-gnueabi-4.6.1-gtk_gst_raspberrypi


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Debug/Production mode
#EHS_DEBUGALL=1


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- Networking Features -----
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all

#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http#@todo this should be the same as above?

#----- GUI Features -----
EHS_GUI_SUPPORT=gtk
EHS_AV_SUPPORT=gst
EHS_VIDEO_SUPPORT=yes
EHS_MEDIA_SUPPORT=all


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# (No application/packaging settings for this target)


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

#DEVMAN_SERVER_DOMAIN=devman.inx-systems.com
#DEVMAN_SERVER_PROTOCOL=https

include ./target/devman-configs/inx-systems.com.mk


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

#Anomolous stuff apparently just for this platform
LIB += crypto
LIB += selinux
LIB += pcre


################################### END OF CONFIGURATION ###################################################
