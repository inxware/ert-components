#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_amd64_gtk_gst
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
export EHS_ARCH=amd64
export EHS_OS=linux

export EHS_GNU_OS_VERSION=-debian-9.4

# TOOLCHAIN_NAME is an optional alternative location to find the toolchain. 
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_ 
export TOOLCHAIN_NAME=HOST
# SET THIS ONLY IF YOU ALSO WANT TO USE THE HOST'S /usr/include and library paths for depedencies 
export EHS_HOST_DEBIAN_BUILD=yes

################################################################################################################
# Configure debug/production levels
################################################################################################################
#DEBUG OPTIONS
# Set ALL debug use this:
EHS_DEBUGALL=true

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

export EHS_NETWORKING_SUPPORT=all
# To enable full TCPIP networking toolbox ("netx" DCC=3)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                   #
export EHS_COMPONENT_NETWORKING_SUPPORT=all

#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers
export EHS_DEVMAN_SUPPORT=all
#unset EHS_DEVMAN_MON_SUPPORT to disable the OS-level Devman monitoring features 
export EHS_DEVMAN_MON_SUPPORT=yes

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################

#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
#Note - windows targets in componentlibrary use hyphens between components (randomly)
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
export COMPONENT_VARIANT=gtk_gst
# Note: This is a host build so we don't ned it but will add it in case we fdo have any bits we may build for the target.

# For non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms  rather than built in ert-ccontriib-middleware).:
# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
#export COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################
#@todo this should just go to the bdcsockets and winsockets .mk files
#This include RCUs, text displays, etc.
# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
export EHS_PERIPHERAL_DEVICE_SUPPORT=all

# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
# Set this to match one of the graphics types in EHS/target/graphics
export EHS_GUI_SUPPORT=gtk

# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
export EHS_AV_SUPPORT=gst10

# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
export EHS_VIDEO_SUPPORT=yes
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
export EHS_MEDIA_SUPPORT=all

# The following toolbox contains legacy components that are no longer supported in the main toolsboxes and can b relegacted here in case               #
# backward compatability with previous apps  is required. Note this requires the toolbox hash checks to be  disabled                                                        #
export EHS_TOOLKIT_DEPRECATED=yes

################################### END OF TOOLBOX CONFIGURATION ###################################################
