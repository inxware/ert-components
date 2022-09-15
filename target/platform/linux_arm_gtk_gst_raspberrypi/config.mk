#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_arm_gtk_gst_raspberrypi
# @author: inx limited


#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
EHS_GNU_ARCH=arm#this must map onto the component library
EHS_GNU_OS=linux-gnu
EHS_GNU_OS_VERSION=""#arm-none-linux-gnueabi-4.6.1
#Optional if different clib build is required - dangerous!
KERNEL_VERSION=linux/2.6.35.9

# ehs is more generic
EHS_ARCH=arm
EHS_OS=linux

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

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#DEBUG OPTIONS
ifdef EHS_DEBUGALL
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
EHS_COMPONENT_NETWORKING_SUPPORT=all

#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers
EHS_DEVMAN_SUPPORT=all
#unset EHS_DEVMAN_MON_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_MON_SUPPORT=yes#@todo this should be the same as above?
#todo there should be a better conversion of 'all' into each devman required - maybe scrap EHS_DEVMAN_SUPPORT?


################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
# Set this to match one of the graphics types in EHS/target/graphics
EHS_GUI_SUPPORT=gtk
# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
EHS_AV_SUPPORT=gst
# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
EHS_VIDEO_SUPPORT=yes
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
EHS_MEDIA_SUPPORT=all


################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
COMPONENT_VARIANT=

# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=arm-linux-gnu-arm-none-linux-gnueabi-4.6.1-gtk_gst_raspberrypi


################################### END OF TOOLBOX CONFIGURATION ###################################################
