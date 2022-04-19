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
#export SYSTEM_VARIANT=

#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
export COMPONENT_VARIANT=base

# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
#export COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
export EHS_GNU_ARCH=i686-pc
export EHS_GNU_OS=linux-gnu
export EHS_GNU_OS_VERSION=4.4.6
#export EHS_GNU_OS_VERSION=i686-pc-linux-gnu-4.4.6
#use toolchain clib environment
#Optional if different clib build is required - dangerous!
#export EHS_GNU_CLIB_ARCH_OVERRIDE=i686
export KERNEL_VERSION=linux/2.6.35.9


# The folllowing  overrdies are not needed
#export TOOLCHAIN_NAME=i686-pc-linux-gnu-4.4.6
#export CC_OVERRIDE=i686-pc-linux-gnu-gcc


# EHS Section 
# ehs is more generic
export EHS_ARCH=x86
export EHS_OS=linux


# uncomment this variable if the platform requires graphics/video support
#EHS_GUI_SUPPORT=yes
#
# Set this to match one of the graphics types in EHS/target/graphics
#EHS_GUI=none
#export EHS_GUI_SUPPORT=gtk
#IS_RGBA=yes - delete this it is not used ..

#
# uncomment this variable if the platform requires audio / video support
export  EHS_AV_SUPPORT=devmanonly
# @todo this should be the same as media support?

#
# uncomment this variable if the platform requires media manager support (e.g. SMIL, DLNA).
#EHS_VIDEO_SUPPORT=yes
#EHS_VIDEO=none
export  EHS_MEDIA_SUPPORT=all

#
# uncomment this variable if the platform requires NETWORKING e.g. devman plugins 

export EHS_NETWORKING_SUPPORT=all
export EHS_COMPONENT_NETWORKING_SUPPORT=all

#
# uncomment this variable if the platform requires devman monitor support
export EHS_DEVMAN_SUPPORT=all
export EHS_DEVMAN_MON_SUPPORT=yes 
#todo there should be a better conversion of 'all' into each devman required - maybe scrap EHS_DEVMAN_SUPPORT?

#
# uncomment this variable if the platform needs to support deprecated toolkit
##export EHS_TOOLKIT_DEPRECATED=yes

#
# uncomment this variable if the platform requires networking support
#
# Set this to reflect the Comms API (e.g. BSD or Winsock)
export  EHS_COMMS_API_SUPPORT=bsdsockets

#
# Set this to reflect the type of communication task (e.g. tcp_server_common, ...)
export EHS_COMMS_TASK=tcp_server_common
#@todo this should just go to the bdcsockets and winsockets .mk files

#This include RCUs, text displays, etc.
##export EHS_PERIPHERAL_DEVICE_SUPPORT=all