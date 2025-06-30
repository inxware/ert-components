#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_x86_gtk_gst_debian11
# @author: inx limited

#Target Platform Uses
# For 64 bit linux running on Debian 11. Generic Vanilla version.
# All contributed middlware is provided by the container container inxware/inx-debian11

include ./target/platform/linux_x86_64_clang_gtk_gst_gg_debian11/config.mk

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################
# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.


################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this: 
#too2023 - we need to add this to everyting until we decide if we will have a debug and non-debug kernel. (I guess we ultimately wan the latter to mimise sizes.)
EHS_DEBUGALL=true

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################


################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################


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
EHS_PID_SUPPORT=gnu
EHS_SCHEDULER_SUPPORT=1
EHS_MODBUS_SUPPORT=stubbed

################################### END OF TOOLBOX CONFIGURATION ###################################################
################################################################################################################
# Define any OS install scripts that should be run on first install
################################################################################################################
#HOST_OS_CONFIG_SCRIPTS+= \

include ./target/devman-configs/inx-systems.com.mk
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes
#Packager
EHS_PACKAGER_TYPE=deb
