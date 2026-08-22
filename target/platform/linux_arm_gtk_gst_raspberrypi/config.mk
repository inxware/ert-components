#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_arm_gtk_gst_raspberrypi
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
EHS_OS=linux

# NOTE: the trailing comment here used to read "this must map onto the component
# library". That is stale — the contrib key comes from
# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH below, not from EHS_GNU_ARCH, so
# this token now only selects the KERNEL archive key.
# It currently resolves to `arm-linux-gnu`, which has no directory. See the
# TODO under EHS_TARGET_LIB_VARIANT.
EHS_GNU_ARCH=arm
EHS_GNU_OS=linux-gnu
# Was  EHS_TARGET_LIB_VARIANT=""  — make assigns the two-character string `""`,
# not empty, so the kernel key became `arm-linux-gnu""`. A bare `=` means empty.
#
# TODO: this target still cannot link. With the quotes fixed the key is
# `arm-linux-gnu`, which has no archive. Two options, needs a decision and a
# link test:
#   (a) build a kernel under arm-linux-gnu in ../EHS-kernel; or
#   (b) set EHS_GNU_ARCH=armv7l to reuse armv7l-linux-gnu, which exists — but
#       that archive is built with clang10ubuntu18 while this target uses
#       gcc-linaro, so ABI compatibility must be confirmed first.
EHS_TARGET_LIB_VARIANT=
#Optional if different clib build is required - dangerous!
#KERNEL_VERSION=linux/2.6.35.9

#Toolchain information
# TOOLCHAIN_NAME is an optional alternative location to find the toolchain.
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_
TOOLCHAIN_NAME=gcc-linaro-arm-linux-gnueabihf-raspbian
CC_OVERRIDE=arm-linux-gnueabihf-gcc
LD_SWITCHES+=-fno-use-linker-plugin

# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).
# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features
SYSTEM_VARIANT=RASPBERRYPI

# Contributed library dependencies variant
COMPONENT_VARIANT=

# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH removed (the variable no longer
# exists). It pointed at contrib target_libs/arm-linux-gnu-arm-none-linux-gnueabi-4.6.1-gtk_gst_raspberrypi.
# This target cannot link today, so it is part of the legacy-target batch in
# docs/target-libs-naming-audit.md; if it is revived, rename that directory to
# the key the composition rule derives rather than reinstating an override.


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Debug/Production mode
#EHS_DEBUGALL=1


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- Networking Features -----
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all

#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http#@todo this should be the same as above?

#----- GUI Features -----
EHS_GUI_SUPPORT=gtk
EHS_AV_SUPPORT=gst
EHS_VIDEO_SUPPORT=yes
EHS_MEDIA_SUPPORT=all


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# (No application/packaging settings for this target)


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

#DEVMAN_SERVER_DOMAIN=devman.inx-systems.com
#DEVMAN_SERVER_PROTOCOL=https

include ./target/devman-configs/inx-systems.com.mk


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

#Anomolous stuff apparently just for this platform
#LIB += crypto
LIB += selinux
LIB += pcre


# LoRaWAN modem support (WIO-E5 via UART/serial)
EHS_LORAWAN_SUPPORT=wio_e5

################################### END OF CONFIGURATION ###################################################
