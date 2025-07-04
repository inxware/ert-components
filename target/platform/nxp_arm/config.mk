#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for nxp_arm
# @author: inx limited

# This config is mainly for the NXP Kenisis targets. On board flash (tiny FS not littelFS) and and typically no UI.

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
# EHS Section 
# ehs is more generic
EHS_ARCH=arm
EHS_OS=nxp-redlib-freertos

TOOLCHAIN_NAME=arm-nxp
CC_OVERRIDE=arm-none-eabi-gcc
LINK_OVERRIDE=arm-none-eabi-ar
LINK=arm-none-eabi-ar
#LINK_OVERRIDE=llvm-ar

#don't build the normal HR controller
##HEATROD_CONTROLLER_PROJECT=0

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#DEBUG OPTIONS
EHS_DEBUGALL=true
# Or use one of the more fine-grained debug congurations
# Or enable only stdout & serial console logging

#enable TCPIP debugger connections (Do not enable for secure production builds)
EHS_MQTT_SUPPORT=lwip_nxp

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
COMPONENT_VARIANT=base
DEFS+=INX_SOD_IN_FLASH

################################### END OF TOOLBOX CONFIGURATION ###################################################
