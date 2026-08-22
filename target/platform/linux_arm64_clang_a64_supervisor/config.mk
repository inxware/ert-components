#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_arm64_clang_A6_supervisor
# @author: inx limited

# Usage Description
# This is for MSG200 supervisro channels

#todo share a common clang arm64 base config for greengradd builds

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
# EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
# EHS Section 
# ehs is more generic, selects the ./target/os-arch/<type>
EHS_ARCH=arm64
EHS_OS=linux

# MUST SET the following for any component config: 
# Note 
#select the os-arch directory with these
EHS_TOOLCHAIN_TYPE=clang
TOOLCHAIN_NAME=HOST

EHS_GNU_ARCH=arm64
EHS_GNU_OS=linux-gnu

# Done os-arch for arm64 dockerised host environment:
#EHS_SYSROOT_ABS_PATH_OVERRIDE=/usr/aarch64-linux-gnu/

#OR target a specific toolchain: This target is using the same compiler as for the 32bit vlang build:
#TOOLCHAIN_PATH=./x86_64/armv7l-linux-gnu-clang10ubuntu18
#LINK_OVERRIDE=ld
# SET THIS ONLY IF YOU ALSO WANT TO USE THE HOST'S /usr/include and library paths for depedencies 
EHS_HOST_DEBIAN_BUILD=arm64
#This is for the idn library version
EHS_DEBIAN_VERSION=11


# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
SYSTEM_VARIANT=msg200_supervisor

################################################################################################################
# Configure debug/production levels
################################################################################################################
#DEBUG OPTIONS
EHS_DEBUGALL=true

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all


#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_SUPPORT=http
#EHS_MQTT_SUPPORT=aws_green_grass

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
#COMPONENT_VARIANT=base

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#This include RCUs, text displays, etc.
EHS_PERIPHERAL_DEVICE_SUPPORT=all
#Currently these are exceptions rather than the norm for peripherals toolbox.
#Can we assume this is the same as the android kernels SYSFS format for GPIO?
EHS_PERIPHERALS_GPIO_SUPPORT=sysfs_linux_arm
# disable continous, as it's not supported on A6_SPI atm
EHS_PERIPHERALS_ADC_CONTINUOUS_SUPPORT=none
EHS_PERIPHERALS_ADC_DAC_SUPPORT=SPI_A6_LTC241X

# uncomment this variable if the platform requires graphics/video support
# Set this to match one of the graphics types in EHS/target/graphics
# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
#EHS_GUI_SUPPORT=gtk
# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
#EHS_AV_SUPPORT=devmanonly
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
#EHS_MEDIA_SUPPORT=all

# The following toolbox contains legacy components that are no longer supported in the main toolsboxes and can b relegacted here in case               #
# backward compatability with previous apps  is required. Note this requires the toolbox hash checks to be  disabled                                                        #
#EHS_TOOLKIT_DEPRECATED=yes

################################### END OF TOOLBOX CONFIGURATION ###################################################

DEFS += EHS_ADC_ARMBIAN_MSG200

#DEVMAN_SERVER_DOMAIN=devman.ambifier.com
#DEVMAN_SERVER_PROTOCOL=https

include ./target/devman-configs/ambifier.com.mk
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes
#Select Default App
EHS_DEFAULT_APP=customer-apps/Moodsonic/MSG200-supervisor-v2
#export EHS_SKIP_REPO_PULL=true
#EHS_DEFAULT_APP=customer-apps/Moodsonic/temp-supervisor

#EHS_PNG_LIB := png16

