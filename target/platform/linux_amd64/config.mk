#
# config.mk - Configuration properties of the current platform
# 
# Called by ../../../Makefile
#

# @author: inx limited
# @version: $Revision: 43 $
# @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
# 
# Copyright (c) inx limited, 2007. All rights reserved.
#
#
#
# uncomment this variable if the platform requires graphics support

#DEBUG OPTIONS
EHS_DEBUGALL=true
ifdef EHS_DEBUGALL
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_DEBUG_TCPIP_CONSOLE
export EHS_DEBUG=yes
endif

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
export EHS_ARCH=amd64
export EHS_OS=linux

export EHS_HOST_DEBIAN_BUILD=yes
#export EHS_GNU_ARCH=amd64
#export EHS_GNU_OS=linux-gnu


#export EHS_GNU_OS_VERSION=#use toolchain clib environment
#Optional if different clib build is required - dangerous!
#export EHS_GNU_CLIB_ARCH_OVERRIDE=i686
#export KERNEL_VERSION=linux/x.x

export TOOLCHAIN_NAME=HOST
#export CC_OVERRIDE= gcc
#some hosts have very old ar's installed separately to gcc's ar
#export LINK_OVERRIDE=gcc-ar
#


#
# Set this to reflect the type of communication task (e.g. tcp_server_common, ...)
#todo2022 - check disabling Network Support builds (with the EHS_COMMS_TASK enabled -as we gett link errors from mismatched builds here)
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


