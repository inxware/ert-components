#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for win_x86_gtk_gst
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################


# ehs is more generic and doesn't use special libc magic.
SYSTEM_VARIANT=windesktop
EHS_ARCH=x86#
EHS_OS=mingw
TOOLCHAIN_NAME=HOST
EHS_GNU_ARCH=i686
EHS_GNU_OS=mingw32posix
CC_OVERRIDE=i686-w64-mingw32-gcc-10-posix 

# Used for specifing name of the eRT package/executable
ERT_PACKAGE_NAME=ehs
# Used for specifing user facing name of installed application (windows installer)
ERT_NSIS_EXE_NAME=eRT

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
# Some debug options
EHS_DEBUGALL=true

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

EHS_NETWORKING_SUPPORT=all# This needs to be enabled for the core EHS - there is a HAL dependency
#@todo we need an entry here for the URL components etc.
EHS_COMPONENT_NETWORKING_SUPPORT=all


#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_SUPPORT=http

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
#Note - windows targets in componentlibrary use hyphens between components (randomly)
# COMPONENT_VARIANT=gtk-gst - lets use the normal one as used by LVGL that has the MQTT stuff in it

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################
# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
#EHS_GUI_SUPPORT=none
# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
#EHS_AV_SUPPORT=gst - not with i686 mingw
#EHS_AV_SUPPORT=none
#
# uncomment this variable if the platform requires media manager support (e.g. SMIL, DLNA).
#EHS_VIDEO_SUPPORT=none
# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
# EHS_MEDIA_SUPPORT=all
# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
# Keyboard and stuff..
#EHS_PERIPHERAL_DEVICE_SUPPORT=all
################################### END OF TOOLBOX CONFIGURATION ###################################################

DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=none
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=no
