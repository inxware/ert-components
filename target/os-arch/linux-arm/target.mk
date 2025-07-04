#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#
# @author: inx limited 

# Makefile fragment to build target specific component and support code.

# Called by ../../platform/<platform-type>
#

# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_OS_HW_PATH - path to the current directory (set by platform makefile)

# include sourcecode from this dir in build

ifeq ($(TOOLCHAIN_NAME),HOST)
	ifeq ($(EHS_GNU_ARCH),arm64)
		#Debian contrib libraries are found here: 
		export INC_DIRS+=/usr/include/arm-linux-gnu/
		#It seems libexpat.h is only found in the host's /usr/include after installing the -dev 
		#todo2023 - this looks dubious: !!!!
		export INC_DIRS+=/usr/include/
		export LIB_DIRS += /usr/lib/arm-linux-gnu/
	else
		#Debian contrib libraries are found here: 
		export INC_DIRS+=/usr/include/arm-linux-gnueabihf/
		#It seems libexpat.h is only found in the host's /usr/include after installing the -dev 
		#export INC_DIRS+=/usr/include/
		#export LIB_DIRS += /usr/lib/arm-linux-gnu/
		CFLAGS += -mfloat-abi=hard
		export LIB_DIRS += /usr/lib/arm-linux-gnueabihf/
	endif
else
	#todo2022 this is a hack approach from a RPI gouged OS?
	#export INX_SYS_ROOT=../ert-contrib-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build
	#CFLAGS+=-I${INX_SYS_ROOT}/usr/include/atk-1.0 -I${INX_SYS_ROOT}/usr/include/gdk-pixbuf-2.0 -I${INX_SYS_ROOT}/usr/lib/arm-linux-gnueabihf/gtk-2.0/include -I${INX_SYS_ROOT}/usr/include/pango-1.0 -I${INX_SYS_ROOT}/usr/include/cairo -I${INX_SYS_ROOT}/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I${INX_SYS_ROOT}/usr/include/glib-2.0 -I${INX_SYS_ROOT}/usr/include/gtk-2.0 -Wl,-m,armelf_linux_eabi -v -B ${INX_SYS_ROOT}/usr/lib/gcc/arm-linux-gnueabihf/8 -B ${INX_SYS_ROOT}/usr/lib/ --sysroot=${INX_SYS_ROOT} --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard -B..ertS-build-support/toolchains/x86_64/x86_64-linux-gnu_clang10ubuntu18/bin
	#LNKFLAGS+=--target=armv7l-pc-linux-gnueabihf -v -L${INX_SYS_ROOT}/usr/lib/gcc/arm-linux-gnueabihf/8 --sysroot=${INX_SYS_ROOT} -L${INX_SYS_ROOT}/lib
	#No special target paths here unless we are specific target as below	
endif

#libs found for this build - sysroot extracted from Blaze board
#TODO2022 This should go in the platform/.../config.mk 
ifeq ($(SYSTEM_VARIANT),OMAP4_TI)
	LIB+= uuid
	LIB+= pcre
	LIB+= audio
	LIB+= crypto
else
   ifeq ($(SYSTEM_VARIANT),RASPBERRYPI)
	LIB += crypto
	LIB += selinux
	LIB += pcre
   endif
endif

OBJECTS += spi.$(OBJ)

include $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/target.mk
include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/target.mk



