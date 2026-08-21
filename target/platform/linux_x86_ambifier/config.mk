#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_x86_ambifier
# @author: inx limited

#Target Platform Uses
# For 32 bit linux running on Debian 7-9. This will not run ambifier2 capable OSs
# NOTE: THIS TARGET IS NOW CONSIDERED DEPRECTATED AND THE CONFIG FILE DOES NOT CONFORM TO STANDARD FORMAT
# BEWARE THIS MAY NOT BUILD AS IT DID BEFORE!!!!!

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
EHS_GNU_ARCH=i686
EHS_GNU_OS=linux-gnu
EHS_TARGET_LIB_VARIANT=i686-pc-linux-gnu-4.4.6#use toolchain clib environment
KERNEL_VERSION=linux/2.6.35.9
# EHS Section 
# ehs is more generic
EHS_ARCH=x86
EHS_OS=linux
# TOOLCHAIN_NAME is an optional alternative location to find the toolchain. 
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_ 
TOOLCHAIN_NAME=i686-pc-linux-gnu-4.4.6
CC_OVERRIDE=i686-pc-linux-gnu-gcc
# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts). 
# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
SYSTEM_VARIANT=ambifier
EHS_DEBIAN_VERSION=8
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
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
#Note - windows targets in componentlibrary use hyphens between components (randomly)
# Ambifiers can optionally install gstreamer from debian package server.
COMPONENT_VARIANT=base

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################
# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target  
 EHS_AV_SUPPORT=devmanonly
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
 EHS_MEDIA_SUPPORT=all

################################### END OF TOOLBOX CONFIGURATION ###################################################

################################################################################################################
# Define any OS install scripts that should be run on first install
################################################################################################################
HOST_OS_CONFIG_SCRIPTS= \
0100-configure-ambifier-debian

include ./target/devman-configs/ambifier.com.mk
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes
EHS_DEFAULT_APP=customer-apps/TSA/ambifier-v0.6.0-integrated
