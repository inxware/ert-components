#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
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
	# Use SYS_LIB_DIRS (not LIB_DIRS) so this system path is appended to the
	# link path AFTER middleware static libs. This prevents the system's OpenSSL 3.x
	# libcrypto.a from shadowing the middleware's BoringSSL libcrypto.a.
	SYS_LIB_DIRS+=/lib/aarch64-linux-gnu/
	LNKFLAGS+= --target=aarch64-linux-gnu
	CFLAGS+= -v --target=aarch64-linux-gnu
	CPPFLAGS+= -v --target=aarch64-linux-gnu
	# Set PKG_CONFIG_PATH for cross-compilation to find ARM64 libraries
	export PKG_CONFIG_PATH=/usr/lib/aarch64-linux-gnu/pkgconfig
endif
# We need the maths lib
LIB+=m 

#gcc flags sometimes useful
#CFLAGS+=-Wl,-m,armelf_linux_eabi -v --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard 

#-B/home/xxx/op-tee-v8/toolchains/aarch64/aarch64-linux-gnu/bin/ 

export EHS_TOOLCHAIN_TYPE=clang
include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/toolchain.mk # toolchain is gnu
include $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/toolchain.mk # in case we have any linux extras..
