#
# config.mk - Configuration properties of the current platform
# 
# Called by ../../../Makefile
#

# @author: inx limited, Pierre Drezet
# @version: $Revision: 43 $
# @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
# 
# Copyright (c) inx limited, 2007. All rights reserved.
#
#

#DEBUG OPTIONS
#EHS_DEBUGALL=true
ifdef EHS_DEBUGALL
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_DEBUG_AV
export EHS_DEBUG=yes
endif
DEFS += EHS_DEBUG_TCPIP_CONSOLE
DEFS += EHS_MQTT_SUPPORT
export EHS_MQTT_SUPPORT=yes

################################################################################################################
# Define the specific variant of the architecture and OS - this selects different component support library sets
################################################################################################################

# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
#export SYSTEM_VARIANT=

#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
export COMPONENT_VARIANT=base

# COMPONENT_BASE_TECHNOLOGIES_OVERRIDE allows non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms).
#export COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
export EHS_GNU_ARCH=arm
export EHS_GNU_OS=linux-gnu
export EHS_GNU_OS_VERSION=x86_64-linux-gnu_clang10ubuntu18#use toolchain clib environment
#Optional if different clib build is required - dangerous!
#export EHS_GNU_CLIB_ARCH_OVERRIDE=i686
export KERNEL_VERSION=linux/2.6.35.9
export EHS_PNG_LIB=png16
# EHS Section 
# ehs is more generic
export EHS_ARCH=arm
export EHS_OS=linux
export COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base
export TOOLCHAIN_NAME=x86_64-linux-gnu_clang10ubuntu18
export CC_OVERRIDE=clang
export INX_SYS_ROOT=../ert-contrib-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build
CFLAGS+=-I${INX_SYS_ROOT}/usr/include/atk-1.0 -I${INX_SYS_ROOT}/usr/include/gdk-pixbuf-2.0 -I${INX_SYS_ROOT}/usr/lib/arm-linux-gnueabihf/gtk-2.0/include -I${INX_SYS_ROOT}/usr/include/pango-1.0 -I${INX_SYS_ROOT}/usr/include/cairo -I${INX_SYS_ROOT}/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I${INX_SYS_ROOT}/usr/include/glib-2.0 -I${INX_SYS_ROOT}/usr/include/gtk-2.0 -Wl,-m,armelf_linux_eabi -v -B ${INX_SYS_ROOT}/usr/lib/gcc/arm-linux-gnueabihf/8 -B ${INX_SYS_ROOT}/usr/lib/ --sysroot=${INX_SYS_ROOT} --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard -B..ertS-build-support/toolchains/x86_64/x86_64-linux-gnu_clang10ubuntu18/bin
LNKFLAGS+=--target=armv7l-pc-linux-gnueabihf -v -L${INX_SYS_ROOT}/usr/lib/gcc/arm-linux-gnueabihf/8 --sysroot=${INX_SYS_ROOT} -L${INX_SYS_ROOT}/lib

# uncomment this variable if the platform requires graphics/video support
#EHS_GUI_SUPPORT=yes
#
# Set this to match one of the graphics types in EHS/target/graphics
#EHS_GUI=none
export EHS_GUI_SUPPORT=gtk
#IS_RGBA=yes - delete this it is not used ..

#
# uncomment this variable if the platform requires audio / video support
export  EHS_AV_SUPPORT=devmanonly
# @todo this should be the same as media support?

#
# uncomment this variable if the platform requires media manager support (e.g. SMIL, DLNA).
#EHS_VIDEO_SUPPORT=yes
#EHS_VIDEO=none
export  EHS_MEDIA_SUPPORT=all

#
# uncomment this variable if the platform requires NETWORKING e.g. devman plugins 

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
export EHS_PERIPHERAL_DEVICE_SUPPORT=all