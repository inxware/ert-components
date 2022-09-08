#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_x86_gtk
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
export EHS_GNU_ARCH=i686
export EHS_GNU_OS=linux-gnu
export EHS_GNU_OS_VERSION=i686-pc-linux-gnu-4.4.6#use toolchain clib environment
export KERNEL_VERSION=linux/2.6.35.9

# EHS Section 
# ehs is more generic
export EHS_ARCH=x86
export EHS_OS=linux
# TOOLCHAIN_NAME is an optional alternative location to find the toolchain. 
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_ 
export TOOLCHAIN_NAME=i686-pc-linux-gnu-4.4.6
export CC_OVERRIDE=i686-pc-linux-gnu-gcc
export EHS_DEBIAN_VERSION=8


################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#DEBUG OPTIONS
#EHS_DEBUGALL=true
ifdef EHS_DEBUGALL
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_DEBUG_AV
export EHS_DEBUG=yes
endif

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

export EHS_NETWORKING_SUPPORT=all
# To enable full TCPIP networking toolbox ("netx" DCC=3)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target
export EHS_COMPONENT_NETWORKING_SUPPORT=all
#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers
export EHS_DEVMAN_SUPPORT=all
#unset EHS_DEVMAN_MON_SUPPORT to disable the OS-level Devman monitoring features 
export EHS_DEVMAN_MON_SUPPORT=yes
#todo there should be a better conversion of 'all' into each devman required - maybe scrap EHS_DEVMAN_SUPPORT?
################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
export COMPONENT_VARIANT=gtk_vlc

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# Set this to match one of the graphics types in EHS/target/graphics
# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
export EHS_GUI_SUPPORT=gtk
# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target 
export EHS_AV_SUPPORT=vlc
# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
export EHS_VIDEO_SUPPORT=yes
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
export EHS_MEDIA_SUPPORT=all

#This include RCUs, text displays, etc.
# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )      
export EHS_PERIPHERAL_DEVICE_SUPPORT=all

################################### END OF TOOLBOX CONFIGURATION ###################################################
