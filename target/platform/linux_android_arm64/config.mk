#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_android_arm64
# @author: inx limited

# This target siimply builds a standard linux version of EHS with android toolchain.
# It does not manage any events or resources such as graphics, so is unlikely to be useful
# other than to test compilation of standard code. It would run an exe using sysecec I expect.


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
EHS_OS=linux-android

EHS_GNU_ARCH=arm64
#not arm64 because the arm toolchain is multilib
#This should be the same as $OS in Component/OS Support build scripts
EHS_GNU_OS=linux-android
TOOLCHAIN_NAME=arm-linux-android
#we need to override the component path back the default it would be if we didn't specify a specific toolchain toolchain

#todo most of the followinf should be in the os-arch/ configs :
CC_SWITCHES+=-fpic

# No override needed: the contrib directory is now named
# arm64-linux-android-arm-linux-android, which is exactly what the composition
# rule derives (EHS_GNU_OS_ARCH + '-' + TOOLCHAIN_NAME).

# There are some #ifdeferies in some general code specific to Android rather than duplicating code
EHS_ANDROID=yes

CC_OVERRIDE=aarch64-linux-android30-clang
LINK_OVERRIDE=aarch64-linux-android30-clang


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Debug/Production mode
EHS_DEBUGALL=yes


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# eRT1 SODL support
ERT_SODL_VERSION=1

#----- Networking Features -----
#tcpip debug is still enabled
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all

#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http

#----- GUI Features -----
EHS_GUI_SUPPORT=OpenGLE1_1
EHS_VIDEO_SUPPORT=yes
EHS_MEDIA_SUPPORT=all
EHS_AV_SUPPORT=android

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

#Just install via adb install
#export EHS_ANDROID_SIMPLE_APK=yes
## todo make the base scripts export the above...
EHS_ANDROID_PACKAGE_SIGNING_PATH=show.online.google.play


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

DEVMAN_SERVER_DOMAIN=devman.inx-systems.com
DEVMAN_SERVER_PROTOCOL=https


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

#WTF is this for and why here?
DEFS += EHS_BSD EHS_ANDROID

# set EHS_DEBUG_AV for verbose debugg from the media sub system
#DEFS += EHS_DEBUG_AV for more verbose debugging of AV media susbsystems
DEFS += EHS_DEBUG_AV


################################### END OF CONFIGURATION ###################################################
