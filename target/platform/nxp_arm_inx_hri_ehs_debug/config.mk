#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for nxp_arm_inx_hri_ehs_debug
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

# (No parent config - this is a base configuration)


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# CPU and OS Type Selection
EHS_ARCH=arm
EHS_OS=nxp-redlib-freertos

CC_OVERRIDE=arm-none-eabi-gcc
TOOLCHAIN_NAME=arm-nxp

#don't build the normal HR controller
HEATROD_CONTROLLER_PROJECT=0


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

#todo2022 the following should be in the os_arch?
#EHS_DEBUG_TCPIP_CONSOLE=yes
EHS_DEBUGALL=yes


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# eRT1 SODL support
ERT_SODL_VERSION=1

#----- Networking Features -----
EHS_MQTT_SUPPORT=lwip_nxp

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_PERIPHERALS_GPIO_SUPPORT=NXP_K64

#todo2022 we need to fix the common info getter code to note use sys/types.h and other libc headers that aren't normal.
EHS_SKIP_APPLICATION_INFO_GETTER=y

EHS_NXP_SUPPORT=yes


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# (No application/packaging settings for this target)


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

# (No devman config for this target)


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

DEFS += EHS_NXP_SUPPORT=1
DEFS += EHS_LWIP=1

DEFS += INX_NXP_KENETIS_BOARD_VARIANT=INX_HR_HRc
DEFS += CONFIG_MQTT_LOCATION=CONFIG_MQTT_LOCATION_INX
DEFS += CONFIG_LIFESTYLE=INX_LIFESTYLE_EHS


################################### END OF CONFIGURATION ###################################################
