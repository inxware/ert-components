#---------------------------------------------------------------
# Copyright (C) 2025 inx limited, UK - All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit:
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------

# @file config.mk
# inxware ERT configuration file for linux_x86_64_clang (no graphics)
# @author: inx limited

#Target Platform Uses
# For 64 bit linux running on Debian 11. Generic Vanilla version.
# All contributed middlware is provided by the container container inxware/inx-debian11


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
EHS_ARCH=x86
#todo2022 change this to just linux (no clang)
EHS_OS=linux

EHS_GNU_ARCH=x86_64
EHS_GNU_OS=linux-gnu
#use toolchain clib environment
#EHS_GNU_OS_VERSION=-clang10_clang10
#todo2025 - We whould be supporting debian11 and debian 12 - still using debian 10 built libraries?
#EHS_GNU_OS_VERSION=-clang11_debian11
EHS_GNU_OS_VERSION=-clang10_clang10

EHS_TOOLCHAIN_TYPE=clang
#CC_OVERRIDE=clang
TOOLCHAIN_NAME=HOST
#IMPORTANT - ELF files built with llvm make object files in a Xbit format that the gcc ld linker doesn't recognize.
# So we must use the llvm linker instead and remove all the gcc -Wl directoves to linker options..
#LINK_OVERRIDE=ld.lld
#clang linker is the ld.ldd called via clang so it gets all the magic incantations.
LINK_OVERRIDE=clang
EHS_DEBIAN_VERSION=11

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

#----- Networking Features -----
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all
EHS_MQTT_SUPPORT=aws_green_grass

#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# (No application/packaging settings for this target)


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

include ./target/devman-configs/inx-systems.com.mk


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# (No legacy DEFS for this target)


################################### END OF CONFIGURATION ###################################################
