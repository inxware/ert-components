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
EHS_ARCH=amd64
EHS_OS=linux

EHS_GNU_ARCH=amd64

#EHS_GNU_OS_VERSION=-debian-9.4
EHS_GNU_OS_VERSION=-clang10_debian10

# TOOLCHAIN_NAME is an optional alternative location to find the toolchain. 
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_ 
TOOLCHAIN_NAME=HOST
# SET THIS ONLY IF YOU ALSO WANT TO USE THE HOST'S /usr/include and library paths for depedencies 
EHS_HOST_DEBIAN_BUILD=x86

# Note: This is a host build so we don't ned it but will add it in case we fdo have any bits we may build for the target.
COMPONENT_VARIANT=gtk_gst

################################################################################################################
# Configure debug/production levels
################################################################################################################
#DEBUG OPTIONS
# Set ALL debug use this:
EHS_DEBUGALL=true

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

EHS_NETWORKING_SUPPORT=all
# To enable full TCPIP networking toolbox ("netx" DCC=3)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                   #
EHS_COMPONENT_NETWORKING_SUPPORT=all

#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers
EHS_DEVMAN_SUPPORT=all
#unset EHS_DEVMAN_MON_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_MON_SUPPORT=yes 

#We don't have GPIO on any amd64 devices I don't think?
#If we want GPIO then we need the peripherals toolbox
EHS_PERIPHERAL_DEVICE_SUPPORT=all
# Assume these generic devices don't have GPIO (#ifndef in case we use this as a base for other builds)
#ifndef EHS_PERIPHERALS_GPIO_SUPPORT
EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
EHS_OTA_SUPPORT=stubbed
#endif
################################### END OF TOOLBOX CONFIGURATION ###################################################

EHS_DEFAULT_APP=NONE
