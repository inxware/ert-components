#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_android_arm
# @author: inx limited

#Target Platform Uses
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

# Optionally set the following if contrib build uses GNU-specific OS and ARCH naming conventions
# - Select the os-arch directory with these
EHS_GNU_ARCH=armv7a

#This should be the same as $OS in Component/OS Support build scripts
EHS_GNU_OS=linux-android
#If a specific clibrary (not the toolchains) is used the give the version string here: (should be the same as INX_GLIBC_VERSION)

CC_OVERRIDE=armv7a-linux-androideabi30-clang
LINK_OVERRIDE=armv7a-linux-androideabi30-clang
#And any additional compiler switches not normally used
CC_SWITCHES+=-fpic

# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).
# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features
#SYSTEM_VARIANT=

# There are some #ifdeferies in some general code specific to Android rather than duplicating code
#todo consider using EHS_OS=linux-android instead. See of we can get rid of this:
EHS_ANDROID=yes


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

#todo2023 we need to di this to keep the debug enabled kernels happy.
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
#todo - do we need this as well as EHS_AV_SUPPORT=android below.
EHS_VIDEO_SUPPORT=yes
EHS_MEDIA_SUPPORT=all
EHS_AV_SUPPORT=android

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

#Using the following packing defaults
#default for all android targets is to use Android 9
EHS_ANDROID_INSTALL_VERSION=9.0

EHS_PACKAGER_TYPE=apk

# Path to a direcory in DevmanSecurity repo with items used for signing android apk and aab files
#export EHS_ANDROID_SIMPLE_APK=yes
## todo make the base scripts export the above...
EHS_ANDROID_PACKAGE_SIGNING_PATH=show.online.google.play
EHS_ANDROID_PACKAGE_SIGNING_PATH=ehs.google.play


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

include ./target/devman-configs/inx-systems.com.mk

#We'll make this the default - if you done want it in a specific platform then set to none
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

DEFS += EHS_BSD EHS_ANDROID


################################### END OF CONFIGURATION ###################################################
