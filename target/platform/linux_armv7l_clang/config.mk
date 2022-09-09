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

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
export EHS_GNU_ARCH=armv7l
export EHS_GNU_OS=linux-gnu
#todo2022 Shouldn't the following just be one? 
export EHS_GNU_OS_VERSION=-clang10ubuntu18
export EHS_DEBIAN_VERSION=10
# EHS Section 
# ehs is more generic
export EHS_ARCH=arm
export EHS_OS=linux_clang
#This is for the idn library version

#export TOOLCHAIN_NAME=HOST
#export CC_OVERRIDE=clang
#export LINK_OVERRIDE=llvm-ar

################################################################################################################
# Configure debug/production levels
################################################################################################################

#DEBUG OPTIONS
EHS_DEBUGALL=true
#endif

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

export EHS_NETWORKING_SUPPORT=all
# To enable full TCPIP networking toolbox ("netx" DCC=3)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                   #
export EHS_COMPONENT_NETWORKING_SUPPORT=all
#set EHS_DEVMAN_SUPPORT to mkae the target environment build include credentials for inx  supported Devman servers
export EHS_DEVMAN_SUPPORT=all
export EHS_DEVMAN_MON_SUPPORT=yes 
#todo there should be a better conversion of 'all' into each devman required - maybe scrap EHS_DEVMAN_SUPPORT?
export  EHS_MQTT_SUPPORT=greengrass

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################

#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
export COMPONENT_VARIANT=aws-iot

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################

# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
export  EHS_AV_SUPPORT=devmanonly
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
export  EHS_MEDIA_SUPPORT=all

################################################################################################################

ifdef NOT_DEFD

export INX_SYS_ROOT=../ert-contrib-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build
CFLAGS+=-I${INX_SYS_ROOT}/usr/include/atk-1.0 -I${INX_SYS_ROOT}/usr/include/gdk-pixbuf-2.0 -I${INX_SYS_ROOT}/usr/lib/arm-linux-gnueabihf/gtk-2.0/include -I${INX_SYS_ROOT}/usr/include/pango-1.0 -I${INX_SYS_ROOT}/usr/include/cairo -I${INX_SYS_ROOT}/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I${INX_SYS_ROOT}/usr/include/glib-2.0 -I${INX_SYS_ROOT}/usr/include/gtk-2.0 -Wl,-m,armelf_linux_eabi -v -B ${INX_SYS_ROOT}/usr/lib/gcc/arm-linux-gnueabihf/8 -B ${INX_SYS_ROOT}/usr/lib/ --sysroot=${INX_SYS_ROOT} --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard -B..ertS-build-support/toolchains/x86_64/x86_64-linux-gnu_clang10ubuntu18/bin
LNKFLAGS+=--target=armv7l-pc-linux-gnueabihf -v -L${INX_SYS_ROOT}/usr/lib/gcc/arm-linux-gnueabihf/8 --sysroot=${INX_SYS_ROOT} -L${INX_SYS_ROOT}/lib

CFLAGS+=-Wl,-m -v --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard 

LNKFLAGS+=--target=armv7l-pc-linux-gnueabihf -v 
#the following are to stop clang selecting the host /usr/bin/ld
LNKFLAGS+=-B../ert-build-support/toolchains/x86_64/armv7l-linux-gnu-clang10ubuntu18/bin
LNKFLAGS+=-fuse-ld=lld
endif

################################### END OF TOOLBOX CONFIGURATION ###################################################

