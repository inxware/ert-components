#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#


# @file config.mk 
# inxware ERT configuration file for linux_android_arm
# @author: inx limited

# This target siimply builds a standard linux version of EHS with android toolchain.
# It does not manage any events or resources such as graphics, so is unlikely to be useful
# other than to test compilation of standard code. It would run an exe using sysecec I expect.

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config to find external libraries.
#This should be the same as $ARCH in Component/OS Support build scripts
export EHS_GNU_ARCH=armv7a
#This should be the same as $OS in Component/OS Support build scripts
export EHS_GNU_OS=linux-android
#If a specific clibrary (not the toolchains) is used the give the version string here: (should be the same as INX_GLIBC_VERSION)

export CC_OVERRIDE=arm-linux-androideabi-gcc
export LINK_OVERRIDE=arm-linux-androideabi-gcc
export CC_SWITCHES+=-fpic
# ehs is more generic
export EHS_ARCH=arm
export EHS_OS=linux-android

################################################################################################################
# Configure debug/production levels
################################################################################################################

export EHS_DEBUGALL=yes
# Or use one of the more fine-grained debug congurations
# Or enable only stdout & serial console logging
DEFS += EHS_RUNTIME_LOGGER_ENABLED

# There are some #ifdeferies in some general code specific to Android rather than duplicating code 
export EHS_ANDROID=yes
DEFS += EHS_BSD EHS_ANDROID

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

#tcpip debug is still enabled
export EHS_NETWORKING_SUPPORT=all

#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers

export EHS_DEVMAN_SUPPORT=all
#unset EHS_DEVMAN_MON_SUPPORT to disable the OS-level Devman monitoring features 
export EHS_DEVMAN_MON_SUPPORT=yes


################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# Set this to match one of the graphics types in EHS/target/graphics/
# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
export EHS_GUI_SUPPORT=OpenGLE1_1

# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
export EHS_VIDEO_SUPPORT=yes

# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
export EHS_MEDIA_SUPPORT=all

# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
export EHS_AV_SUPPORT=android

# set EHS_DEBUG_AV for verbose debugg from the media sub system
# for more verbose debugging of AV media susbsystems
DEFS += EHS_DEBUG_AV

# To enable full TCPIP networking toolbox ("netx" DCC=3)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                   #
export EHS_COMPONENT_NETWORKING_SUPPORT=all

# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#This include RCUs, text displays, etc.
export EHS_PERIPHERAL_DEVICE_SUPPORT=all

################################### END OF TOOLBOX CONFIGURATION ###################################################

