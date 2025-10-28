#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_x86_gtk_gst_debian11
# @author: inx limited

#Target Platform Uses
# For 64 bit linux running on Debian 11. Generic Vanilla version.
# All contributed middlware is provided by the container container inxware/inx-debian11

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
# EHS Section
EHS_ARCH=x86
EHS_OS=linux

EHS_GNU_ARCH=x86_64
EHS_GNU_OS=linux-gnu

#It seems the Debian 10 greengrass build works fine with debian 11 so we are using this for now.
EHS_GNU_OS_VERSION=-clang10_clang10

EHS_TOOLCHAIN_TYPE=clang
# SET THIS ONLY IF YOU ALSO WANT TO USE THE HOST'S /usr/include and library paths for depedencies 
TOOLCHAIN_NAME=HOST
EHS_HOST_DEBIAN_BUILD=x86_64
#IMPORTANT - ELF files built with llvm make object files in a bit format that the gcc ld linker doesn't recognize.
# So we must use the llvm linker instead and remove all the gcc -Wl directoves to linker options.. 
#LINK_OVERRIDE=ld.lld
#clang linker is the ld.ldd called via clang so it gets all the magic incantations.
LINK_OVERRIDE=clang
EHS_DEBIAN_VERSION=11

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this: 
#too2023 - we need to add this to everyting until we decide if we will have a debug and non-debug kernel. (I guess we ultimately wan the latter to mimise sizes.)
EHS_DEBUGALL=yes

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

EHS_NETWORKING_SUPPORT=all
# To enable full TCPIP networking toolbox ("netx" DCC=3)
EHS_COMPONENT_NETWORKING_SUPPORT=all
EHS_MQTT_SUPPORT=aws_green_grass


#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_SUPPORT=http

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
#Note - windows targets in component library use hyphens between components (randomly)
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components

#COMPONENT_VARIANT=gtk_gst - we don't need this because we use the host's dev libs
COMPONENT_VARIANT=base
# Note: This is a host build so we don't need it but will add it in case we fdo have any bits we may build for the target.
# For non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms  rather than built in ert-ccontriib-middleware).:
# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
#COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# To enable  IO features  (DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#This include RCUs, text displays, etc. We usually have this for arm linux so leaving this here
EHS_PERIPHERAL_DEVICE_SUPPORT=all

# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub, lvgl}, depending support for your target   #
# Set this to match one of the graphics types in EHS/target/graphics
EHS_GUI_SUPPORT=lvgl

# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
EHS_AV_SUPPORT=ffmpeg

# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
EHS_VIDEO_SUPPORT=yes
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
EHS_MEDIA_SUPPORT=all

# The following toolbox contains legacy components that are no longer supported in the main toolsboxes and can b relegacted here in case               #
# backward compatability with previous apps  is required. Note this requires the toolbox hash checks to be  disabled                                                        #
EHS_TOOLKIT_DEPRECATED=yes

# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT ) 
EHS_PERIPHERAL_DEVICE_SUPPORT=all

EHS_PERIPHERALS_ADC_DAC_SUPPORT=stubbed
EHS_PERIPHERALS_GPIO_SUPPORT=gui
EHS_PID_SUPPORT=stubbed
#EHS_PID_SUPPORT=gnu
EHS_SCHEDULER_SUPPORT=1
EHS_MODBUS_SUPPORT=stubbed

################################### END OF TOOLBOX CONFIGURATION ###################################################
################################################################################################################
# Define any OS install scripts that should be run on first install
################################################################################################################
#HOST_OS_CONFIG_SCRIPTS+= \

#Packager
EHS_PACKAGER_TYPE=deb
