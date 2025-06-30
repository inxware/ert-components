#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_x86_64_clang_ehrt1
# @author: inx limited

ERT_SODL_VERSION=1

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

#use toolchain clib environment (We prolly don't need this as we don't pick up any dependencies)
#EHS_GNU_OS_VERSION=-clang10_clang10_base
EHS_GNU_OS_VERSION=-clang10_clang10


#CC_OVERRIDE=clang
#LINK_OVERRIDE=clang
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
#DEBUG OPTIONS
EHS_DEBUGALL=yes

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

EHS_NETWORKING_SUPPORT=all
# To enable full TCPIP networking toolbox ("netx" DCC=3)
EHS_COMPONENT_NETWORKING_SUPPORT=all
EHS_MQTT_SUPPORT=greengrass
#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers
EHS_DEVMAN_SUPPORT=all
#unset EHS_DEVMAN_MON_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_MON_SUPPORT=yes

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

# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
EHS_MEDIA_SUPPORT=all
EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
EHS_OTA_SUPPORT=stubbed

################################### END OF TOOLBOX CONFIGURATION ###################################################


