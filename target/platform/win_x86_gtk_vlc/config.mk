#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for win_x86_gtk_vlc
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

EHS_GNU_ARCH=i686## this will pick up the glibc2.9 library
EHS_GNU_OS=mingw32posix## make sure we have no spaces after !!
KERNEL_VERSION=#None

# ehs is more generic and doesn't use special libc magic.
EHS_ARCH=x86#
EHS_OS=mingw# does this work for 

# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
SYSTEM_VARIANT=windesktop

# Used for specifing name of the eRT package/executable
ERT_PACKAGE_NAME=ehs
# Used for specifing user facing name of installed application (windows installer)
ERT_NSIS_EXE_NAME=eRT

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:

# Some debug options
#EHS_DEBUGALL=true

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
COMPONENT_VARIANT=gtk-vlc

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################
# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
EHS_GUI_SUPPORT=gtk
# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
 EHS_AV_SUPPORT=vlc
# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
EHS_VIDEO_SUPPORT=yes
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
 EHS_MEDIA_SUPPORT=all# This is for the media handling stuff


# Set this to reflect the type of communication task (e.g. tcp_server_common, ...)
EHS_COMMS_TASK=tcp_server_common
#@todo this should just go to the bdcsockets and winsockets .mk files
# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
# Keyboard and stuff..
EHS_PERIPHERAL_DEVICE_SUPPORT=all

################################### END OF TOOLBOX CONFIGURATION ###################################################

DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=none
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=no