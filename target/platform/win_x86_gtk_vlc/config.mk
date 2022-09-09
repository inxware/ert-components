#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for win_x86_gtk_vlc
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

export EHS_GNU_ARCH=i586## this will pick up the glibc2.9 library
export EHS_GNU_OS=mingw32msvc## make sure we have no spaces after !!
export KERNEL_VERSION=#None

# ehs is more generic and doesn't use special libc magic.
export EHS_ARCH=x86#
export EHS_OS=mingw# does this work for 

# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
export SYSTEM_VARIANT=windesktop


################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:

# Some debug options
#EHS_DEBUGALL=true
ifdef EHS_DEBUGALL
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_DEBUG_AV
export EHS_DEBUGALL=yes

endif

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

export EHS_NETWORKING_SUPPORT=all# This needs to be enabled for the core EHS - there is a HAL dependency
#@todo we need an entry here for the URL components etc.
export EHS_COMPONENT_NETWORKING_SUPPORT=all
#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers
export EHS_DEVMAN_SUPPORT=all#@todo these conditions need to be used for module inclusion instead of just EHS_COMPONENT_NETWORKING_SUPPORT 
#unset EHS_DEVMAN_MON_SUPPORT to disable the OS-level Devman monitoring features 
export EHS_DEVMAN_MON_SUPPORT=yes


################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
#Note - windows targets in componentlibrary use hyphens between components (randomly)
export COMPONENT_VARIANT=gtk-vlc

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################
# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
export EHS_GUI_SUPPORT=gtk
# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
export  EHS_AV_SUPPORT=vlc
# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
EHS_VIDEO_SUPPORT=yes
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
export  EHS_MEDIA_SUPPORT=all# This is for the media handling stuff


# Set this to reflect the type of communication task (e.g. tcp_server_common, ...)
export EHS_COMMS_TASK=tcp_server_common
#@todo this should just go to the bdcsockets and winsockets .mk files
# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
# Keyboard and stuff..
export EHS_PERIPHERAL_DEVICE_SUPPORT=all

################################### END OF TOOLBOX CONFIGURATION ###################################################
