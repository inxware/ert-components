#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_arm_gtk_gst_blaze
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################
# These will match the path in target_lib as follows:
EHS_GNU_ARCH=arm#this must map onto the component library
EHS_GNU_OS=linux-gnu
EHS_TARGET_LIB_VARIANT=glibc-2.12.1
#Optional if different clib build is required - dangerous!
#Seems the core stuff has the same name 
EHS_CLIB_OVERRIDE_PATH=arm-linux-gnu-glibc-2.12.1-ti-blaze-ubuntu-10_10
KERNEL_VERSION=linux/2.6.35-980-omap4

# ehs is more generic
EHS_ARCH=arm
EHS_OS=linux

#Toolchain details
CC_OVERRIDE=arm-none-linux-gnueabi-gcc
TOOLCHAIN_NAME=arm-none-linux-gnueabi-4.4.6

# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).  
# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
SYSTEM_VARIANT=OMAP4_TI

################################################################################################################
# Configure debug/production levels
################################################################################################################
#DEBUG OPTIONS
EHS_DEBUGALL=yes

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################

# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH removed (the variable no longer
# exists). It pointed at contrib target_libs/arm-linux-gnu-glibc-2.12.1-ti-blaze-ubuntu-10_10-gtk_gst.
# This target cannot link today, so it is part of the legacy-target batch in
# docs/target-libs-naming-audit.md; if it is revived, rename that directory to
# the key the composition rule derives rather than reinstating an override.

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################
# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#This include RCUs, text displays, etc.
EHS_PERIPHERAL_DEVICE_SUPPORT=all

# Set this to match one of the graphics types in EHS/target/graphics
# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
EHS_GUI_SUPPORT=gtk
# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
EHS_AV_SUPPORT=gst

# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 

EHS_VIDEO_SUPPORT=yes
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
EHS_MEDIA_SUPPORT=all

# To enable full TCPIP networking toolbox ("netx" DCC=3)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                   #
EHS_COMPONENT_NETWORKING_SUPPORT=all

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################
EHS_NETWORKING_SUPPORT=all

#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_SUPPORT=http#@todo this should be the same as above?


################################### END OF TOOLBOX CONFIGURATION ###################################################

#Anomolous stuuf just for this platform:

LIB+= uuid
LIB+= pcre
LIB+= audio
LIB+= crypto

#DEVMAN_SERVER_DOMAIN=devman.inx-systems.com
#DEVMAN_SERVER_PROTOCOL=https
include ./target/devman-configs/inx-systems.com.mk
