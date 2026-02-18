#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
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
# This is for Raspberry Pis - it is aimed at being used with inxware demo


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
EHS_ARCH=arm64
EHS_OS=linux

EHS_GNU_ARCH=arm64
EHS_GNU_OS=linux-gnu

#This determines the /usr/<arch> when using the host's toolchain
EHS_HOST_DEBIAN_BUILD=arm64

#This will indicate what ert-contrib-middleware is used and toolchains if not using a host toolchain
EHS_GNU_OS_VERSION=-clang19_debian13

#This determines some library version choices and for the deb packager
EHS_DEBIAN_VERSION=13

#select the os-arch directory with these
EHS_TOOLCHAIN_TYPE=clang
#if we have a specific version of the toolchain to suffix on the os-arch defined toolchain path
#COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=arm64-linux-VX_VX_base
#todo2023 - lets finally make this bit automatic tuple of $EHS_OS-$EHS_ARCH-$EHS_TOOLCHAIN_TYPE-$EHS_DEBIAN_VERSION (OR Make this a new variable EHS_OS_TYPE_VERSION) to allow for non debian

# Toolchain Selection
TOOLCHAIN_NAME=HOST

# Done os-arch for arm64 dockerised host environment:
#EHS_SYSROOT_ABS_PATH_OVERRIDE=/usr/aarch64-linux-gnu/

#OR target a specific toolchain: This target is using the same compiler as for the 32bit vlang build:
#TOOLCHAIN_PATH=
LINK_OVERRIDE=clang

# Contributed library dependencies variant
COMPONENT_VARIANT=base


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Debug/Production mode
EHS_DEBUGALL=true


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# eRT1 SODL support
ERT_SODL_VERSION=1

#----- Networking Features -----
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all
EHS_MQTT_SUPPORT=none
#EHS_MQTT_SUPPORT=aws_green_grass

#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http

#----- GUI Features -----
EHS_GUI_SUPPORT=lvgl
EHS_AV_SUPPORT=devmanonly
EHS_MEDIA_SUPPORT=all
#EHS_TOOLKIT_DEPRECATED=yes

# Enable machine vision support for testing C++ integration
EHS_MV_SUPPORT=stubbed

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all
#Currently these are exceptions rather than the norm for peripherals toolbox.
#Can we assume this is the same as the android kernels SYSFS format for GPIO?
EHS_PERIPHERALS_GPIO_SUPPORT=sysfs_linux_arm
#EHS_PERIPHERALS_ADC_DAC_SUPPORT=SPI_A6_LTC241X
EHS_PERIPHERALS_ADC_DAC_SUPPORT=none


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

EHS_DEFAULT_APP=systemapps/Home

EHS_PACKAGER_TYPE=deb


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

# (No devman config for this target - no-certs variant)


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# we should try wifi by default in rpi devices
DEFS+=EHS_USE_WIFI_INTERFACE=1


################################### END OF CONFIGURATION ###################################################
