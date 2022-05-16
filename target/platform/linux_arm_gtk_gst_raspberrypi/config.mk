#
# config.mk - Configuration properties of the current platform
# 
# Called by ../../../Makefile
#

# @author: inx limited, Pierre Drezet
# @version: $Revision: 43 $
# @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
# 
# Copyright (c) inx limited, 2007. All rights reserved.
#
#

#DEBUG OPTIONS
#EHS_DEBUGALL=true
ifdef EHS_DEBUGALL
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_DEBUG_AV
export EHS_DEBUG=yes
endif

################################################################################################################
# Define the specific variant of the architecture and OS - this selects different component support library sets
################################################################################################################

# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
export SYSTEM_VARIANT=RASPBERRYPI

#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
export COMPONENT_VARIANT=

# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
export COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=arm-linux-gnu-arm-none-linux-gnueabi-4.6.1-gtk_gst_raspberrypi

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
export EHS_GNU_ARCH=arm#this must map onto the component library
export EHS_GNU_OS=linux-gnu
#export EHS_GNU_OS_VERSION=
export EHS_GNU_OS_VERSION=""#arm-none-linux-gnueabi-4.6.1
#Optional if different clib build is required - dangerous!
export KERNEL_VERSION=linux/2.6.35.9

# ehs is more generic
export EHS_ARCH=arm
export EHS_OS=linux

#Toolchain information
export TOOLCHAIN_NAME=gcc-linaro-arm-linux-gnueabihf-raspbian
export CC_OVERRIDE=arm-linux-gnueabihf-gcc 
export LD_SWITCHES+=-fno-use-linker-plugin

# uncomment this variable if the platform requires graphics/video support
#
# Set this to match one of the graphics types in EHS/target/graphics
#EHS_GUI=none
export EHS_GUI_SUPPORT=gtk
#IS_RGBA=yes - delete this it is not used ..

#
# uncomment this variable if the platform requires audio / video support
export EHS_AV_SUPPORT=gst

#
# uncomment this variable if the platform requires media manager support (e.g. SMIL, DLNA).
export EHS_VIDEO_SUPPORT=yes
export EHS_MEDIA_SUPPORT=all

#
# uncomment this variable if the platform requires NETWORKING e.g. devman plugins 

export EHS_NETWORKING_SUPPORT=all
export EHS_COMPONENT_NETWORKING_SUPPORT=all

#
# uncomment this variable if the platform requires devman monitor support
export EHS_DEVMAN_SUPPORT=all
export EHS_DEVMAN_MON_SUPPORT=yes#@todo this should be the same as above?
#todo there should be a better conversion of 'all' into each devman required - maybe scrap EHS_DEVMAN_SUPPORT?

#
# uncomment this variable if the platform needs to support deprecated toolkit
##export EHS_TOOLKIT_DEPRECATED=yes


#This include RCUs, text displays, etc.
export EHS_PERIPHERAL_DEVICE_SUPPORT=all

