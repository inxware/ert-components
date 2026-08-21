#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_x86_gtk_gst_ambifier2_vagrant_adnoc
# @author: inx limited

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
EHS_GNU_ARCH=amd64
EHS_GNU_OS=linux-gnu
KERNEL_VERSION=linux/4.9

EHS_ARCH=x86
EHS_OS=linux
# TOOLCHAIN_NAME is an optional alternative location to find the toolchain. 
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_ 
TOOLCHAIN_NAME=HOST
# SET THIS ONLY IF YOU ALSO WANT TO USE THE HOST'S /usr/include and library paths for depedencies 
EHS_HOST_DEBIAN_BUILD=x86
# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
SYSTEM_VARIANT=ambifier2-adnoc

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
EHS_DEBUGALL=yes

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################
EHS_NETWORKING_SUPPORT=all
# To enable  basic networking toolbox ("netx" DCC=2)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target  
EHS_COMPONENT_NETWORKING_SUPPORT=all


#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_SUPPORT=http

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
COMPONENT_VARIANT=gtk_gst

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################
# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
EHS_GUI_SUPPORT=gtk
# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target     
EHS_AV_SUPPORT=gst10
# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
EHS_VIDEO_SUPPORT=yes
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
EHS_MEDIA_SUPPORT=all

# The following toolbox contains legacy components that are no longer supported in the main toolsboxes and can b relegacted here in case               #
# backward compatability with previous apps  is required. Note this requires the toolbox hash checks to be  disabled    
EHS_TOOLKIT_DEPRECATED=yes

# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
EHS_PERIPHERAL_DEVICE_SUPPORT=all

EHS_PERIPHERALS_GPIO_SUPPORT=stubbed


################################### END OF TOOLBOX CONFIGURATION ###################################################


################################################################################################################
# Define any OS install scripts that should be run on first install
################################################################################################################
HOST_OS_CONFIG_SCRIPTS+= \
0090-update-debian8-npt-www-ambifier-com \
0100-configure-ambifier-debian9 \
0200-snd-add-csound-volume \
0210-snd-volume_max
#0090-configure-X-hotplug-inteli915 \
#0050-configure-jessie-repo-debian \

#0500-update-kernel-4.9-backport \
#0450-update-Realtek-net-firmware \
#0200-snd-add-csound-volume \
#0210-snd-tsa_std_volume \
#0501-patch-J1800-cstate-and-sata-speed

#DEVMAN_SERVER_DOMAIN=devman.ambifier.com
#DEVMAN_SERVER_PROTOCOL=https
include ./target/devman-configs/adnoc-green.local.mk

EHS_DEFAULT_APP=customer-apps/TSA/ambifier-v0.6.0-integrated
