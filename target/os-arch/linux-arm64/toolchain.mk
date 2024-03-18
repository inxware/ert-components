#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

#  Uses $(INC_DIRS), $(TARGET_NAME), $(DEFS)
#  Defines $(CC), $(LINK), $(CFLAGS), $(LNKFLAGS), $(INC), $(LIB), $(EXE), $(OBJ), $(FINAL)

#This is the Debian cross compiler and libraries provided in docker image

# Toolset specific 

#We have floating Point
#Experiments for Raspberyy PI:
#CFLAGS+= -mfloat-abi=hard
#CFLAGS+= -mfpu=vfp
#CFLAGS+= -mfloat-abi=softfp
##
## Linker Options

#Don't use a sysroot for the installed clang it seems to have bits all over the place it is looking for.
# e.g. it expects libc to be at a specific path from sysroot, (which is /)
#EHS_SYSROOT_ABS_PATH_OVERRIDE=/usr/aarch64-linux-gnu/
ifeq ($(EHS_TOOLCHAIN_TYPE),clang)
	LIB_DIRS+=/lib/aarch64-linux-gnu/
	LNKFLAGS+= --target=aarch64-linux-gnu
	CFLAGS+= -v --target=aarch64-linux-gnu 
endif
# We need the maths lib - todo2-022, why not just have these as LIB+=m?
LIB+=m 

#gcc flags sometimes useful
#CFLAGS+=-Wl,-m,armelf_linux_eabi -v --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard 

#-B/home/xxx/op-tee-v8/toolchains/aarch64/aarch64-linux-gnu/bin/ 

#error
#LNKFLAGS+=--target=armv7l-pc-linux-gnueabihf -v -L../comp-lib-support/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/usr/lib/gcc/arm-linux-gnueabihf/8 --sysroot=../ert-contrib-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/ -L../ert-contrib-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/lib

export EHS_TOOLCHAIN_TYPE=clang
include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/toolchain.mk # toolchain is gnu
include $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/toolchain.mk # in case we have any linux extras..
