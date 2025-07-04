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
# Set general architecture and OS version 
#################################################################################################################
# MUST SET the following for any component config: 
# EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
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

################################################################################################################
# Configure debug/production levels
################################################################################################################
EHS_DEBUGALL=yes

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################
EHS_NETWORKING_SUPPORT=all
# To enable full TCPIP networking toolbox ("netx" DCC=3)
EHS_COMPONENT_NETWORKING_SUPPORT=all

#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers
EHS_DEVMAN_SUPPORT=all

#unset EHS_DEVMAN_MON_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_MON_SUPPORT=yes

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
#Note - windows targets in component library use hyphens between components (randomly)
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
COMPONENT_VARIANT=gtk_gst

# Note: This is a host build so we don't ned it but will add it in case we fdo have any bits we may build for the target.
# For non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms  rather than built in ert-ccontriib-middleware).:
# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
#COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# To enable  IO features  (DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#This include RCUs, text displays, etc. We usually have this for arm linux so leaving this here
EHS_PERIPHERAL_DEVICE_SUPPORT=all

# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
# Set this to match one of the graphics types in EHS/target/graphics
EHS_GUI_SUPPORT=gtk

# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
EHS_AV_SUPPORT=gst

# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
EHS_VIDEO_SUPPORT=yes
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
EHS_MEDIA_SUPPORT=all
# To enable full TCPIP networking toolbox ("netx" DCC=3)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                   #
EHS_COMPONENT_NETWORKING_SUPPORT=all

################################### END OF TOOLBOX CONFIGURATION ###################################################

include ./target/devman-configs/inx-systems.com.mk
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes
#Packager
EHS_PACKAGER_TYPE=deb
