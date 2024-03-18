#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_arm_gtk_vlc
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
EHS_GNU_ARCH=arm-none#this must map onto the component library
EHS_GNU_OS=linux-gnueabi
#arm-linux-gnu-glibc-2.12.1-ti-blaze-ubuntu-10_10
EHS_GNU_OS_VERSION=arm-none-linux-gnueabi-4.4.6
#Optional if different clib build is required - dangerous!
#EHS_GNU_CLIB_ARCH_OVERRIDE=

CC_OVERRIDE=arm-none-linux-gnueabi-gcc
TOOLCHAIN_NAME=arm-none-linux-gnueabi-4.4.6
#gcc-4.2.4-i686-pc-linux-gnu

# ehs is more generic
EHS_ARCH=arm
EHS_OS=linux

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
EHS_DEBUGALL=yes

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

EHS_NETWORKING_SUPPORT=all
# To enable  basic networking toolbox ("netx" DCC=2)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                          #
EHS_COMPONENT_NETWORKING_SUPPORT=all
#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers
EHS_DEVMAN_SUPPORT=all
#unset EHS_DEVMAN_MON_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_MON_SUPPORT=yes #@todo this should be the same as above?

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################

#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
#Note - windows targets in component library use hyphens between components (randomly)
COMPONENT_VARIANT=gtk_vlc

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
EHS_GUI_SUPPORT=yes
EHS_GUI_SUPPORT=gtk

# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#This include RCUs, text displays, etc.
EHS_PERIPHERAL_DEVICE_SUPPORT=all

################################### END OF TOOLBOX CONFIGURATION ###################################################
