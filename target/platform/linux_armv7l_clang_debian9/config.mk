#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_armv7l_clang
# @author: inx limited

#Target Platform Uses
# This is the horrific arm / raspberry pi config with scraped components and uses a clang compiler 
# so that is can potentially integrate green grass components built with the same compiler
# It should be refactored to used docker and Debians arm  cross compile environment for Rasberrian
# All contributed middlware is provided by a munge of things we want rid of ???

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# EHS Section 
# ehs is more generic
EHS_ARCH=arm
EHS_OS=linux

EHS_TOOLCHAIN_TYPE=clang
TOOLCHAIN_NAME=HOST

EHS_GNU_ARCH=armv7l
EHS_GNU_OS=linux-gnu

# SET THIS ONLY IF YOU ALSO WANT TO USE THE HOST'S /usr/include and library paths for depedencies 
EHS_HOST_DEBIAN_BUILD=armhf

#EHS_GNU_OS_VERSION=-clang10ubuntu18
EHS_DEBIAN_VERSION=9

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
EHS_DEBUGALL=true


################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

EHS_NETWORKING_SUPPORT=all
# To enable full TCPIP networking toolbox ("netx" DCC=3)
EHS_COMPONENT_NETWORKING_SUPPORT=all

#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers
EHS_DEVMAN_SUPPORT=all
#unset EHS_DEVMAN_MON_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_MON_SUPPORT=yes
#todo there should be a better conversion of 'all' into each devman required - maybe scrap EHS_DEVMAN_SUPPORT?

# Select the format of MQTT support for this target
#This isn't working yet!
#EHS_MQTT_SUPPORT=aws_green_grass

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################

#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
#Note - windows targets in component library use hyphens between components (randomly)
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
COMPONENT_VARIANT=base

# Note: This is a host build so we don't ned it but will add it in case we fdo have any bits we may build for the target.
# For non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms  rather than built in ert-ccontriib-middleware).:
# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
#COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
 EHS_AV_SUPPORT=devmanonly
# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
#EHS_VIDEO_SUPPORT=no
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
 EHS_MEDIA_SUPPORT=all

# The following toolbox contains legacy components that are no longer supported in the main toolsboxes and can b relegacted here in case               #
# backward compatability with previous apps  is required. Note this requires the toolbox hash checks to be  disabled                                                        #
EHS_TOOLKIT_DEPRECATED=yes

ifdef NOT_DEFD
#todo delete all this!!

#INX_SYS_ROOT=../ert-contrib-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build
#CFLAGS+=-I${INX_SYS_ROOT}/usr/include/atk-1.0 -I${INX_SYS_ROOT}/usr/include/gdk-pixbuf-2.0 -I${INX_SYS_ROOT}/usr/lib/arm-linux-gnueabihf/gtk-2.0/include -I${INX_SYS_ROOT}/usr/include/pango-1.0 -I${INX_SYS_ROOT}/usr/include/cairo -I${INX_SYS_ROOT}/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I${INX_SYS_ROOT}/usr/include/glib-2.0 -I${INX_SYS_ROOT}/usr/include/gtk-2.0 -Wl,-m,armelf_linux_eabi -v -B ${INX_SYS_ROOT}/usr/lib/gcc/arm-linux-gnueabihf/8 -B ${INX_SYS_ROOT}/usr/lib/ --sysroot=${INX_SYS_ROOT} --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard -B..ertS-build-support/toolchains/x86_64/x86_64-linux-gnu_clang10ubuntu18/bin
#LNKFLAGS+=--target=armv7l-pc-linux-gnueabihf -v -L${INX_SYS_ROOT}/usr/lib/gcc/arm-linux-gnueabihf/8 --sysroot=${INX_SYS_ROOT} -L${INX_SYS_ROOT}/lib

#FLAGS+=-Wl,-m -v --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard 

#LNKFLAGS+=--target=armv7l-pc-linux-gnueabihf -v 
#the following are to stop clang selecting the host /usr/bin/ld
#LNKFLAGS+=-B../ert-build-support/toolchains/x86_64/armv7l-linux-gnu-clang10ubuntu18/bin
#LNKFLAGS+=-fuse-ld=lld
endif

################################### END OF TOOLBOX CONFIGURATION ###################################################
################################################################################################################
# Define any OS install scripts that should be run on first install
################################################################################################################
#HOST_OS_CONFIG_SCRIPTS+= \

include ./target/devman-configs/inx-systems.com.mk
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes