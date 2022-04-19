#
# This target siimply builds a standard linux version of EHS with android toolchain.
# It does not manage any events or resources such as graphics, so is unlikely to be useful
# other than to test compilation of standard code. It would run an exe using sysecec I expect.
#
# config.mk - Configuration properties of the current platform
# 
# Called by ../../../Makefile
#

# @author: Dr Pierre Drezet
# @version: $Revision: 43 $
# @date: $Date: 20011-10-30 05:05:44 +0000 (Mon, 30 Oct 2011) $
# 
# Copyright (c) inx limited, 2007. All rights reserved.
#
#

ifdef EHS_DEBUGALL
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_DEBUG_AV
DEFS += EHS_DEBUG_TCPIP_CONSOLE
export EHS_DEBUG=yes
endif

# There are some #ifdeferies in some general code specific to Android rather than duplicating code 
export EHS_ANDROID=yes
DEFS += EHS_BSD EHS_ANDROID

################################################################################################################
# Define the specific variant of the architecture and OS - this selects different component support library sets
# Component Path is given by 
# $(EHS_GNU_ARCH)-$(EHS_GNU_OS)_$(TOOLCHAIN_PATH)_$(EHS_GNU_OS_VERSION)_$(COMPONENT_VARIANT)
################################################################################################################

# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
#export SYSTEM_VARIANT=

#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
#Note - windows targets in component library use hyphens between components (randomly)
export COMPONENT_VARIANT=android-native

# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
#untested nut includes the path to where our lbraries actually are for android
#target_libs/arm-linux-androideabi-9-arm-none-linux-android-9-headless/build/
export COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=arm-linux_x86_64/linux-android-armv7a_x86_64/linux-android-armv7a_android-native


# MUST SET the following for any component config to find external libraries.
#This should be the same as $ARCH in Component/OS Support build scripts
export EHS_GNU_ARCH=arm
#This should be the same as $OS in Component/OS Support build scripts
export EHS_GNU_OS=linux
#If a specific clibrary (not the toolchains) is used the give the version string here: (should be the same as INX_GLIBC_VERSION)

#Optional if different clib from toolchain is required. (e.g. bare toolchains - dangerous, but sometimes necessary!).
#export EHS_GNU_OS_VERSION=""


#If a different arbitrarilly named component library is used then enter the path here:
#export EHS_CLIB_OVERRIDE_PATH="????"

export CC_OVERRIDE=arm-linux-androideabi-gcc
export TOOLCHAIN_NAME=linux-android-armv7a
export CC_SWITCHES+=-fPIC
# ehs is more generic
export EHS_ARCH=arm
export EHS_OS=linux-android

#
# Set this to match one of the graphics types in EHS/target/graphics/
export EHS_GUI_SUPPORT=OpenGLE1_1
#IS_RGBA=yes - delete this it is not used ..

#
# Set this to match one of the graphics types in EHS/target/webkit/
#export EHS_WEBKIT_SUPPORT=android
#
# uncomment this variable if the platform requires media manager support (e.g. SMIL, DLNA).
export EHS_VIDEO_SUPPORT=yes
#export EHS_VIDEO=none
export EHS_MEDIA_SUPPORT=all
#
# uncomment this variable if the platform requires audio / video support
export EHS_AV_SUPPORT=android

#
# uncomment this variable if the platform requires URL NETWORKING e.g. devman plugins 
#tcpip debug is still enabled
export EHS_NETWORKING_SUPPORT=all
export EHS_COMPONENT_NETWORKING_SUPPORT=all 

#
# uncomment this variable if the platform requires devman monitor support
export EHS_DEVMAN_SUPPORT=all
export EHS_DEVMAN_MON_SUPPORT=yes
#todo there should be a better conversion of 'all' into each devman required - maybe scrap EHS_DEVMAN_SUPPORT?

#
# uncomment this variable if the platform needs to support deprecated toolkit
##export EHS_TOOLKIT_DEPRECATED=yes

#
# uncomment this variable if the platform requires networking support
#
# Set this to reflect the Comms API (e.g. BSD or Winsock)
export EHS_COMMS_API_SUPPORT=bsdsockets

#
# Set this to reflect the type of communication task (e.g. tcp_server_common, ...)
export EHS_COMMS_TASK=tcp_server_common
#@todo this should just go to the bdcsockets and winsockets .mk files
