#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_amd64
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
export EHS_ARCH=amd64
export EHS_OS=linux

# TOOLCHAIN_NAME is an optional alternative location to find the toolchain. 
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_ 
export TOOLCHAIN_NAME=HOST

################################################################################################################
# Configure debug/production levels
################################################################################################################
#DEBUG OPTIONS
# Set ALL debug use this:
EHS_DEBUGALL=true
ifdef EHS_DEBUGALL
# Or use one of the more fine-grained debug congurations
# Or enable only stdout & serial console logging
DEFS += EHS_RUNTIME_LOGGER_ENABLED
#enable TCPIP debugger connections (Do not enable for secure production builds)
DEFS += EHS_DEBUG_TCPIP_CONSOLE
export EHS_DEBUG=yes
endif

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

# Set this to reflect the type of communication task (e.g. tcp_server_common, ...)
#todo2022 - check disabling Network Support builds (with the EHS_COMMS_TASK enabled -as we gett link errors from mismatched builds here)
export EHS_NETWORKING_SUPPORT=all
# To enable full TCPIP networking toolbox ("netx" DCC=3)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                   #
export EHS_COMPONENT_NETWORKING_SUPPORT=all

#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers
export EHS_DEVMAN_SUPPORT=all
#unset EHS_DEVMAN_MON_SUPPORT to disable the OS-level Devman monitoring features 
export EHS_DEVMAN_MON_SUPPORT=yes 
################################### END OF TOOLBOX CONFIGURATION ###################################################
