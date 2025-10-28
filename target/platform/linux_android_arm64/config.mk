#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_android_arm64
# @author: inx limited

# This target siimply builds a standard linux version of EHS with android toolchain.
# It does not manage any events or resources such as graphics, so is unlikely to be useful
# other than to test compilation of standard code. It would run an exe using sysecec I expect.


#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# ehs is more generic
EHS_ARCH=arm
EHS_OS=linux-android
EHS_GNU_ARCH=arm64
#not arm64 because the arm toolchain is multilib
#This should be the same as $OS in Component/OS Support build scripts
EHS_GNU_OS=linux-android
TOOLCHAIN_NAME=arm-linux-android
#we need to override the component path back the default it would be if we didn't specify a specific toolchain toolchain  

#################################################################
#todo most of the followinf should be in the os-arch/ configs :
CC_SWITCHES+=-fpic

COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=arm64-linux-android


EHS_ANDROID=yes
DEFS += EHS_BSD EHS_ANDROID

CC_OVERRIDE=aarch64-linux-android30-clang
LINK_OVERRIDE=aarch64-linux-android30-clang

#WTF is this for and why here?
DEFS += EHS_BSD EHS_ANDROID
################################################################

################################################################################################################
# Configure debug/production levels
################################################################################################################

# Or use one of the more fine-grained debug congurations
# Or enable only stdout & serial console logging
#enable TCPIP debugger connections (Do not enable for secure production builds)
# Set ALL debug use this:
EHS_DEBUGALL=yes

# enable eRT1 support
ERT_SODL_VERSION=1

# There are some #ifdeferies in some general code specific to Android rather than duplicating code 
EHS_ANDROID=yes
DEFS += EHS_BSD EHS_ANDROID


################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

#tcpip debug is still enabled
EHS_NETWORKING_SUPPORT=all


#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_SUPPORT=http

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# set EHS_DEBUG_AV for verbose debugg from the media sub system
#DEFS += EHS_DEBUG_AV for more verbose debugging of AV media susbsystems
DEFS += EHS_DEBUG_AV

# To enable full TCPIP networking toolbox ("netx" DCC=3)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                   #
EHS_COMPONENT_NETWORKING_SUPPORT=all

# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#This include RCUs, text displays, etc.
EHS_PERIPHERAL_DEVICE_SUPPORT=all

# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
# Set this to match one of the graphics types in EHS/target/graphics/
EHS_GUI_SUPPORT=OpenGLE1_1

# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
EHS_VIDEO_SUPPORT=yes
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
EHS_MEDIA_SUPPORT=all

# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
EHS_AV_SUPPORT=android
################################### END OF TOOLBOX CONFIGURATION ###################################################

DEVMAN_SERVER_DOMAIN=devman.inx-systems.com

DEVMAN_SERVER_PROTOCOL=https

#Just install via adb install
#export EHS_ANDROID_SIMPLE_APK=yes
## todo make the base scripts export the above...
EHS_ANDROID_PACKAGE_SIGNING_PATH=show.online.google.play
