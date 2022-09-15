#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_x86_64_clang_host
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
# EHS Section 
# ehs is more generic
EHS_ARCH=x86
EHS_OS=linux

#todo2022 - the following line shouldn't be needed. but we need to change the paths in the ert-contrib-middleware to not include the double clic/toolchain ..
#COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=x86_64-linux-gnu_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base
# TOOLCHAIN_NAME is an optional alternative location to find the toolchain. 
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_ 
TOOLCHAIN_NAME=HOST

################################################################################################################
# Configure debug/production levels
################################################################################################################

EHS_DEBUGALL=true
# Or use one of the more fine-grained debug congurations
# Or enable only stdout & serial console logging
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_DEBUG_AV
EHS_DEBUGALL=yes
endif

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

EHS_NETWORKING_SUPPORT=all
# To enable  basic networking toolbox ("netx" DCC=2)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target  
EHS_COMPONENT_NETWORKING_SUPPORT=all
#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers
EHS_DEVMAN_SUPPORT=all
#unset EHS_DEVMAN_MON_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_MON_SUPPORT=yes 
#todo there should be a better conversion of 'all' into each devman required - maybe scrap EHS_DEVMAN_SUPPORT?

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################

#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
COMPONENT_VARIANT=aws-iot

EHS_MQTT_SUPPORT=greengrass
################################### END OF TOOLBOX CONFIGURATION ###################################################

