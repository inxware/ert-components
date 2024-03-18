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
#We re setting this in the target.mk conditional on
#CFLAGS+= -mfloat-abi=hard
#CFLAGS+= -mfpu=vfp
#CFLAGS+= -mfloat-abi=softfp
##
## Linker Options
##

ifeq ($(EHS_TOOLCHAIN_TYPE),clang)
	LNKFLAGS+= --target=arm-linux-gnueabihf
	CFLAGS+= -v --target=arm-linux-gnueabihf
endif
#CFLAGS+=-Wl,-m,armelf_linux_eabi -v --target=armv7l-pc-linux-gnueabihf -mfloat-abi=hard 

LIB+=m 

include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/toolchain.mk # toolchain is gnu
include $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/toolchain.mk # in case we have any linux extras..
