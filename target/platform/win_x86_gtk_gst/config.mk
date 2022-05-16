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

# Some debug options
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
export SYSTEM_VARIANT=windesktop

#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
#Note - windows targets in componentlibrary use hyphens between components (randomly)
export COMPONENT_VARIANT=gtk-gst

# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
#export COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################


#export CLIB_USEHOST
export EHS_GNU_ARCH=i586## this will pick up the glibc2.9 library
export EHS_GNU_OS=mingw32msvc## make sure we have no spaces after !!
#by not defining a toolchain, we leave the toolchain path blank and the mingw_x86 toolchain.mk file selectsthe system installed version of mingw
export KERNEL_VERSION=#None

# ehs is more generic and doesn't use special libc magic.
export EHS_ARCH=x86#
#export EHS_OS=win#
export EHS_OS=mingw# does this work for 

#export TOOLCHAIN_NAME=#i686-pc-linux-gnu-4.4.6
#export CC_OVERRIDE=#i686-pc-linux-gnu-gcc

#
# uncomment this variable if the platform requires graphics/video support
#EHS_GUI_SUPPORT=yes
#
# Set this to match one of the graphics types in EHS/target/graphics
#EHS_GUI=none
export EHS_GUI_SUPPORT=gtk
#IS_RGBA=yes - delete this it is not used ..

#
# uncomment this variable if the platform requires audio / video support
export  EHS_AV_SUPPORT=gst

#
# uncomment this variable if the platform requires media manager support (e.g. SMIL, DLNA).
EHS_VIDEO_SUPPORT=yes
#EHS_VIDEO=none
export  EHS_MEDIA_SUPPORT=all# This is for the media handling stuff

#
# uncomment this variable if the platform requires NETWORKING e.g. devman plugins 

export EHS_NETWORKING_SUPPORT=all# This needs to be enabled for the core EHS - there is a HAL dependency
#@todo we need an entry here for the URL components etc.
export EHS_COMPONENT_NETWORKING_SUPPORT=all

#
# uncomment this variable if the platform requires devman monitor support
export EHS_DEVMAN_SUPPORT=all#@todo these conditions need to be used for module inclusion instead of just EHS_COMPONENT_NETWORKING_SUPPORT 
export EHS_DEVMAN_MON_SUPPORT=yes
#todo there should be a better conversion of 'all' into each devman required - maybe scrap EHS_DEVMAN_SUPPORT?

#
# uncomment this variable if the platform needs to support deprecated toolkit
##export EHS_TOOLKIT_DEPRECATED=yes
#@todo remove all TK_DEPRECATE symbols - no longer used

#@todo this should just go to the bdcsockets and winsockets .mk files

# Keyboard and stuff..
export EHS_PERIPHERAL_DEVICE_SUPPORT=all
