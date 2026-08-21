#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_android_arm_jni
# @author: inx limited
# This Target is a proper Android app honouring the activiy model of android.

#DEBUG OPTIONS
EHS_DEBUGALL=yes

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################
EHS_ANDROID=yes
EHS_ANDROID_JNI=yes
# @todo remove the EHS_ANDROID_JNI as an additional flag
DEFS += EHS_BSD EHS_ANDROID

# MUST SET the following for any component config.
# These determin the location withint the component-lib-support & optionally the core clib entry in ert-build-support/support_libs
EHS_GNU_ARCH=arm
EHS_GNU_OS=linux-android
EHS_TARGET_LIB_VARIANT=androideabi-9

CC_OVERRIDE=arm-linux-androideabi-gcc
# TOOLCHAIN_NAME is an optional alternative location to find the toolchain. 
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_ 
TOOLCHAIN_NAME=arm-none-linux-androideabi-9

# ehs is more generic for libraries more specific for structure (e.g. different main etc.)
EHS_ARCH=arm
EHS_OS=linux-android

# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts). 
# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
SYSTEM_VARIANT=android_jni

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

# uncomment this variable if the platform requires URL NETWORKING e.g. devman plugins 
#tcpip debug is still enabled
EHS_NETWORKING_SUPPORT=all



#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_SUPPORT=http


################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################

#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
#Note - windows targets in component library use hyphens between components (randomly)
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
COMPONENT_VARIANT=android_native

# For non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms  rather than built in ert-ccontriib-middleware).:
# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH removed (the variable no longer
# exists). It pointed at contrib target_libs/arm-linux_arm-none-linux-androideabi-9_arm-none-linux-androideabi-9_android-native.
# This target cannot link today, so it is part of the legacy-target batch in
# docs/target-libs-naming-audit.md; if it is revived, rename that directory to
# the key the composition rule derives rather than reinstating an override.

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#This include RCUs, text displays, etc.
##EHS_PERIPHERAL_DEVICE_SUPPORT=all

# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
EHS_GUI_SUPPORT=OpenGLE1_1

# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
EHS_MEDIA_SUPPORT=all

# To enable full TCPIP networking toolbox ("netx" DCC=3)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                   #
EHS_COMPONENT_NETWORKING_SUPPORT=all

################################### END OF TOOLBOX CONFIGURATION ###################################################

#DEVMAN_SERVER_DOMAIN=devman.inx-systems.com
include ./target/devman-configs/inx-systems.com.mk
#DEVMAN_SERVER_PROTOCOL=https

EHS_ANDROID_INSTALL_VERSION=9.0

