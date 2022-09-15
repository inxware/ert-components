#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_x86_64_clang
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
# EHS Section 
# ehs is more generic
EHS_ARCH=x86
EHS_OS=linux_clang
CC_OVERRIDE=clang
EHS_GNU_ARCH=x86_64
EHS_GNU_OS=linux-gnu
#use toolchain clib environment
EHS_GNU_OS_VERSION=-clang10_clang10

LINK_OVERRIDE=clang
TOOLCHAIN_NAME=HOST

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#DEBUG OPTIONS
EHS_DEBUGALL=true

################################################################################################################
# Enable or disable non-component networking support (e.g. socket debugging or Devman or none)
################################################################################################################ 

EHS_NETWORKING_SUPPORT=all

EHS_COMPONENT_NETWORKING_SUPPORT=all
#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers
EHS_DEVMAN_SUPPORT=all
#unset EHS_DEVMAN_MON_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_MON_SUPPORT=yes 

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components

COMPONENT_VARIANT=base

EHS_MQTT_SUPPORT=greengrass

DEFS += EHS_SKIP_APPMANAGER
################################### END OF TOOLBOX CONFIGURATION ###################################################
