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
#

#
# Toolset specific 

#We have floating Point
#Experiments for Raspberyy PI:
#CFLAGS+= -mfloat-abi=hard
#CFLAGS+= -mfpu=vfp
#CFLAGS+= -mfloat-abi=softfp
##
## Linker Options
##
LNKFLAGS+= -Wl,-lm #@todo this isn'r for all arms - should move to platform
#CFLAGS+=-Wl,-m,armelf_linux_eabi -v --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard 
CFLAGS+=-Wl,-m -v --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard 

ifndef CC_OVERRIDE
export CC_OVERRIDE=clang
endif
ifndef LINK_OVERRIDE
export LINK_OVERRIDE=clang
endif
#error
#LNKFLAGS+=--target=armv7l-pc-linux-gnueabihf -v -L../comp-lib-support/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/usr/lib/gcc/arm-linux-gnueabihf/8 --sysroot=../ert-contrib-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/ -L../ert-contrib-middleware/target_libs/armv7l-pc-linux-gnueabihf_x86_64-linux-gnu_clang10ubuntu18_x86_64-linux-gnu_clang10ubuntu18_base/build/lib
LNKFLAGS+=--target=armv7l-pc-linux-gnueabihf -v 
#the following are to stop clang selecting the host /usr/bin/ld
LNKFLAGS+=-B../ert-build-support/toolchains/x86_64/armv7l-linux-gnu-clang10ubuntu18/bin
LNKFLAGS+=-fuse-ld=lld


export EHS_TOOLCHAIN_TYPE=clang
include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/toolchain.mk # toolchain is gnu
include $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/toolchain.mk # in case we have any linux extras..
