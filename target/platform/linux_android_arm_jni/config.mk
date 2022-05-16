#
# This Target is a proper Android app honouring the activiy model of android.
# 
#
# config.mk - Configuration properties of the current platform
# 
# Called by ../../../Makefile
#

# @author: Dr Pierre Drezet
# @version: $Revision: 43 $
# @date: $Date: 2011-10-30 05:05:44 +0000 (Mon, 30 Oct 2011) $
# 
# Copyright (c) inx limited, 2007. All rights reserved.
#
#



#DEBUG OPTIONS
EHS_DEBUGALL=true
ifdef EHS_DEBUGALL
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_DEBUG_AV
export EHS_DEBUG=yes
endif

export EHS_ANDROID=yes
export EHS_ANDROID_JNI=yes
# @todo remove the EHS_ANDROID_JNI as an additional flag
DEFS += EHS_BSD EHS_ANDROID


# MUST SET the following for any component config.
# These determin the location withint the component-lib-support & optionally the core clib entry in ert-build-support/support_libs
export EHS_GNU_ARCH=arm
export EHS_GNU_OS=linux
export EHS_GNU_OS_VERSION=androideabi-9

export COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=arm-linux_arm-none-linux-androideabi-9_arm-none-linux-androideabi-9_android-native

export CC_OVERRIDE=arm-linux-androideabi-gcc
export TOOLCHAIN_NAME=arm-none-linux-androideabi-9

# ehs is more generic for libraries more specific for structure (e.g. different main etc.)
export EHS_ARCH=arm
export EHS_OS=linux-android

################################################################################################################
# Define the specific variant of the architecture and OS - this selects different component support library sets
################################################################################################################

# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
export SYSTEM_VARIANT=android_jni

#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
#Note - windows targets in component library use hyphens between components (randomly)
export COMPONENT_VARIANT=android_native

# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
#export COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=


#
# uncomment this variable if the platform requires graphics/video support
#export EHS_GUI_SUPPORT=OpenGLE2
export EHS_GUI_SUPPORT=OpenGLE1_1


#
# uncomment this variable if the platform requires media manager support (e.g. SMIL, DLNA).
#export EHS_VIDEO_SUPPORT=yes
#export EHS_VIDEO=none
export EHS_MEDIA_SUPPORT=all
#
# uncomment this variable if the platform requires audio / video support
#export EHS_AV_SUPPORT=

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


#This include RCUs, text displays, etc.
##export EHS_PERIPHERAL_DEVICE_SUPPORT=all
