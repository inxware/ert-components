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
export EHS_GNU_ARCH=x86_64
export EHS_GNU_OS=linux-gnu
export EHS_GNU_OS_VERSION=-clang10ubuntu18#use toolchain clib environment
export COMPONENT_VARIANT=_aws-iot

# EHS Section 
# ehs is more generic
export EHS_ARCH=x86
export EHS_OS=linux_clang

export CC_OVERRIDE=clang
export LINK_OVERRIDE=clang
# - for creating archives.

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#DEBUG OPTIONS
#EHS_DEBUGALL=true
ifdef EHS_DEBUGALL
# Or use one of the more fine-grained debug congurations
# Or enable only stdout & serial console logging
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_DEBUG_AV
export EHS_DEBUG=yes
endif
DEFS += EHS_DEBUG_TCPIP_CONSOLE

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################ 

export EHS_NETWORKING_SUPPORT=all

export EHS_COMPONENT_NETWORKING_SUPPORT=all
#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers
export EHS_DEVMAN_SUPPORT=all
#unset EHS_DEVMAN_MON_SUPPORT to disable the OS-level Devman monitoring features 
export EHS_DEVMAN_MON_SUPPORT=yes 

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
export COMPONENT_VARIANT=aws-iot



export EHS_MQTT_SUPPORT=greengrass

DEFS += EHS_SKIP_APPMANAGER
################################### END OF TOOLBOX CONFIGURATION ###################################################
