#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for XXXXXXXXXXXXXXXXXXXXXXXXX
# @author: XXXXXXXXXXXXXXXXXXXXXXXx


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
EHS_OS=linux

# TOOLCHAIN_NAME is an optional alternative location to find the toolchain.
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_
TOOLCHAIN_NAME=HOST

# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).
# SYSTEM_VARIANT=

# Contributed library dependencies variant
COMPONENT_VARIANT=gtk_gst

# For non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms rather than built in ert-contrib-middleware):
#COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Debug/Production mode
EHS_DEBUGALL=yes
# Or use one of the more fine-grained debug congurations
# Or enable only stdout & serial console logging
#DEFS += EHS_RUNTIME_LOGGER_ENABLED
#enable TCPIP debugger connections (Do not enable for secure production builds)


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- Networking Features -----
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all

#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http

# To enable basic networking toolbox ("netx" DCC=2)
#EHS_COMPONENTBASIC_NETWORKING_SUPPORT=all    #note this is not yet implemented

#----- GUI Features -----
##EHS_GUI_SUPPORT=gtk
##EHS_AV_SUPPORT=gst10
##EHS_VIDEO_SUPPORT=yes
##EHS_MEDIA_SUPPORT=all
# set EHS_DEBUG_AV for verbose debugg from the media sub system
#DEFS += EHS_DEBUG_AV for more verbose debugging of AV media susbsystems

##EHS_TOOLKIT_DEPRECATED=yes

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# (No application/packaging settings for this target)


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

# place to incude overrides for variables suhc as devman servers URLs that might not be defined as required in the target hack files.
# XXXXX=


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# (No legacy DEFS for this target)

# see target/envtree/$EHS_OS-$EHS_ARCH/ for scripts available for configurating host OS's on first boot.
# HOST_OS_CONFIG_SCRIPTS+= \
#    0450-update-Realtek-net-firmware \


################################### END OF CONFIGURATION ###################################################
