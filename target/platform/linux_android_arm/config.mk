#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_android_arm
# @author: inx limited

#Target Platform Uses
# This target siimply builds a standard linux version of EHS with android toolchain.
# It does not manage any events or resources such as graphics, so is unlikely to be useful
# other than to test compilation of standard code. It would run an exe using sysecec I expect.

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################
# MUST SET the following for any component config: 
# EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
# EHS Section 
# ehs is more generic, selects the ./target/os-arch/<type>
EHS_ARCH=arm
EHS_OS=linux-android

# Optionally set the following if contrib build uses GNU-specific OS and ARCH naming conventions 
# - Select the os-arch directory with these
EHS_GNU_ARCH=armv7a

#This should be the same as $OS in Component/OS Support build scripts
EHS_GNU_OS=linux-android
#If a specific clibrary (not the toolchains) is used the give the version string here: (should be the same as INX_GLIBC_VERSION)

CC_OVERRIDE=armv7a-linux-androideabi30-clang
LINK_OVERRIDE=armv7a-linux-androideabi30-clang
#And any additional compiler switches not normally used
CC_SWITCHES+=-fpic

# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).  
# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
#SYSTEM_VARIANT=

################################################################################################################
# Configure debug/production levels
################################################################################################################

#todo2023 we need to di this to keep the debug enabled kernels happy.
EHS_DEBUGALL=yes

# enable eRT1 support
ERT_SODL_VERSION=1

# Or use one of the more fine-grained debug congurations
# Or enable only stdout & serial console logging

# There are some #ifdeferies in some general code specific to Android rather than duplicating code 
#todo consider using EHS_OS=linux-android instead. See of we can get rid of this:
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

# Set this to match one of the graphics types in EHS/target/graphics/
# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
EHS_GUI_SUPPORT=OpenGLE1_1

# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
#todo - do we need this as well as EHS_AV_SUPPORT=android below.
EHS_VIDEO_SUPPORT=yes

# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
EHS_MEDIA_SUPPORT=all

# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
EHS_AV_SUPPORT=android

# To enable full TCPIP networking toolbox ("netx" DCC=3)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                   #
EHS_COMPONENT_NETWORKING_SUPPORT=all

# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#This include RCUs, text displays, etc.
EHS_PERIPHERAL_DEVICE_SUPPORT=all

################################### END OF TOOLBOX CONFIGURATION ###################################################

include ./target/devman-configs/inx-systems.com.mk

#We'll make this the default - if you done want it in a specific platform then set to none
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes

#Using the following packing defaults
#default for all android targets is to use Android 9
EHS_ANDROID_INSTALL_VERSION=9.0

#Packager
#todo the packer could inculdue the apk and supervisr option to disintangle the horrors of the installer for normal cases
EHS_PACKAGER_TYPE=apk

# Path to a direcory in DevmanSecurity repo with items used for signing android apk and aab files

#export EHS_ANDROID_SIMPLE_APK=yes
## todo make the base scripts export the above...
EHS_ANDROID_PACKAGE_SIGNING_PATH=show.online.google.play
EHS_ANDROID_PACKAGE_SIGNING_PATH=ehs.google.play

