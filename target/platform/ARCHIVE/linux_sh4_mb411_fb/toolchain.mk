#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#



#
# Target-specific definitions for the EHS Microsoft Windows/Visual Studio target.
# This file is called from a makefile
#
#  Uses $(ENVIRONMENT_PATH),       $(INC_DIRS), $(TARGET_NAME), $(DEFS)
#  Defines        $(CC), $(LINK), $(CFLAGS), $(LNKFLAGS), $(INC), $(LIB), $(EXE), $(OBJ), $(FINAL)
#

#
# Toolset specific directories
SH4_GCC_PATH=$(ENVIRONMENT_PATH)/sh4-unknown-linux-gnu
#SH4_GCC_PATH=/cygdrive/z/temp/Sam/sh4-unknown-linux-gnu
STAPI_PATH=$(ENVIRONMENT_PATH)/STAPI
DEFS+=ST_OSLINUX
# All of the following flags are required when compiling taps.
# # Note that we *must* use -fno-delayed-branch or else the topfield goes gaga
#
MANDATORY_C_FLAGS=
MANDATORY_CXX_FLAGS=$(MANDATORY_C_FLAGS)
MANDATORY_LD_FLAGS=
# -cref

# Specify crosscompilation prefix
CROSS_PREFIX:=$(SH4_GCC_PATH)/bin/sh4-unknown-linux-gnu-
AR=$(CROSS_PREFIX)ar
AS=$(CROSS_PREFIX)as
#LD=$(CROSS_PREFIX)ld $(MANDATORY_LD_FLAGS)
LINK=$(CROSS_PREFIX)gcc
NM=$(CROSS_PREFIX)nm
OBJCOPY=$(CROSS_PREFIX)objcopy
OBJDUMP=$(CROSS_PREFIX)objdump
RANLIB=$(CROSS_PREFIX)ranlib
SIZE=$(CROSS_PREFIX)size
STRINGS=$(CROSS_PREFIX)strings
STRIP=$(CROSS_PREFIX)strip
CC=$(CROSS_PREFIX)gcc $(MANDATORY_C_FLAGS)
CXX=$(CROSS_PREFIX)g++ $(MANDATORY_CXX_FLAGS)
INC_DIRS += $(STAPI_PATH)/include

LIB=$(SH4_GCC_PATH)/sh4-unknown-linux-gnu/lib

LNKFLAGS= -o ehs_$(TARGET).$(EXE) -lm -Xlinker -Map -Xlinker sh4.map -pthread $(LIB)/libpthread.so.0
#LNKFLAGS= -o ehs_$(TARGET).$(EXE) $(EHS_TARGET_PATH)/libEhsStapi.so -lm -Xlinker -Map -Xlinker sh4.map
#LNKFLAGS= -o ehs_$(TARGET).$(EXE) $(LIB)/crt1.o $(LIB)/crti.o $(LIB)/crtn.o -lc -lm $(LIB)/libgcc.a 
#LDFLAGS= -ltap -lc -lm 
CFLAGS= -O3
#CFLAGS=-g
CXXFLAGS= -O2

CFLAGS+=$(foreach i,$(DEFS),-D"$i")
CXXFLAGS+=$(foreach i,$(DEFS),-D"$i")


ifdef DEBUG
CFLAGS+=-DDEBUG
CXXFLAGS+=-DDEBUG
endif

%.linux: %.elf
	$(OBJCOPY) -O binary $< $@

%.a: %.o
	$(AR) -cr $@

# Build shared libraries used by EHS
%.so : %.c
	$(CC) $(CFLAGS) -c -fPIC $^ -o $*.$(OBJ)
	$(RM) $*.so.1.0.0
	$(LINK) -shared -o $@ $*.$(OBJ)
	cp $@ $*.so.1.0.0

# Compiler flags
COPTS=$(foreach i,$(INC_DIRS),-I $i)
CFLAGS+=$(COPTS) -c
CFLAGS+=-DEHS_DEBUG

#
# File extensions
#
export EXE=exe#We use these elsewhere
OBJ=o
FINAL=${EXE}

CLEAN_FILES=*.$(FINAL)

