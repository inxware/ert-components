#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_arm_gtk_gst
# @author: inx limited

#Target Platform Uses
# Arm linux (e.g. Armbian running on Debian 7-9). Generic Vanilla version.
# All contributed middlware is provided from Rasberry PI target in contrib-middleware? (TODO - check this)


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

# Optionally set the following if contrib build uses GNU-specific OS and ARCH naming conventions
# - Select the os-arch directory with these
EHS_GNU_ARCH=arm
EHS_GNU_OS=linux-gnu
EHS_GNU_OS_VERSION=glibc-2.12.2

#todo do we really need this?
KERNEL_VERSION=linux/2.6.35.9

# TOOLCHAIN_NAME is an optional alternative location to find the toolchain.
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_
TOOLCHAIN_NAME=arm-none-linux-gnueabi-4.4.6

# and the CC compiler file name is
CC_OVERRIDE=arm-none-linux-gnueabi-gcc

#OR target a specific toolchain: This target is using the same compiler as for the 32bit vlang build:
#TOOLCHAIN_PATH=./x86_64/XXXX

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

# Debug/Production mode
EHS_DEBUGALL=yes


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
EHS_AV_SUPPORT=gst
EHS_VIDEO_SUPPORT=yes
EHS_MEDIA_SUPPORT=all

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_PACKAGER_TYPE=deb


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

# - we might need this? DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# (No legacy DEFS for this target)


################################### END OF CONFIGURATION ###################################################
