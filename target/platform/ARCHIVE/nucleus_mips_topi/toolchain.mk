#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#


# Target-specific definitions for the EHS Microsoft Windows/Visual Studio target.
# This file is called from a makefile
#
#  Uses $(ENVIRONMENT_PATH),       $(INC_DIRS), $(TARGET_NAME)
#  Defines        $(CC), $(LINK), $(CFLAGS), $(LNKFLAGS), $(INC), $(LIB), $(EXE), $(OBJ), $(FINAL)
#

#
# Toolset specific directories
TAP_PATH=$(ENVIRONMENT_PATH)/TAP/BerliOS

#
# Tools
# Allow the path and SDK to be overriden with command line arguments
# Example: make TAP_GCC_DIR=/opt/tap-gcc TAP_HOME=/tap/home 
#
TAP_GCC_DIR?=/usr/local/topfield-gcc
TAP_HOME?=$(TAP_GCC_DIR)/topfield-sdk

# All of the following flags are required when compiling taps.
# # Note that we *must* use -fno-delayed-branch or else the topfield goes gaga
#
MANDATORY_C_FLAGS= -mqnxpic -fno-delayed-branch -mlong-calls -msoft-float -mno-gpopt -isystem $(TAP_HOME)/include 
# ACC removed -membedded-pic 
MANDATORY_CXX_FLAGS=$(MANDATORY_C_FLAGS) -fno-rtti -fno-exceptions -Wno-pmf-conversions -fno-weak
MANDATORY_LD_FLAGS=--script $(TAP_GCC_DIR)/mips/lib/tap.ld 
# -cref

# Specify crosscompilation prefix
CROSS_PREFIX:=$(TAP_GCC_DIR)/bin/mips-
AR=$(CROSS_PREFIX)ar
AS=$(CROSS_PREFIX)as
#LD=$(CROSS_PREFIX)ld $(MANDATORY_LD_FLAGS)
LINK=$(CROSS_PREFIX)ld $(MANDATORY_LD_FLAGS)
NM=$(CROSS_PREFIX)nm
OBJCOPY=$(CROSS_PREFIX)objcopy
OBJDUMP=$(CROSS_PREFIX)objdump
RANLIB=$(CROSS_PREFIX)ranlib
SIZE=$(CROSS_PREFIX)size
STRINGS=$(CROSS_PREFIX)strings
STRIP=$(CROSS_PREFIX)strip
CC=$(CROSS_PREFIX)gcc $(MANDATORY_C_FLAGS)
CXX=$(CROSS_PREFIX)g++ $(MANDATORY_CXX_FLAGS)

LNKFLAGS= -L$(TAP_HOME)/lib -ltap -lc -lm -Map ehs_$(TARGET).map -o ehs_$(TARGET).$(EXE)
# removed --embedded-relocs 
#LDFLAGS= -L$(TAP_HOME)/lib -ltap -lc -lm 
CFLAGS=  -Wa,-L -D_TAP -O0
CXXFLAGS= -D_TAP -O2

ifdef DEBUG
CFLAGS+=-DDEBUG
CXXFLAGS+=-DDEBUG
endif

%.tap: %.elf
	$(OBJCOPY) -O binary $< $@

#define link-elf
#$(LD) $^ $(LDFLAGS) -o $@
#endef#
#
#%.elf: %.o
#	$(link-elf)

%.a: %.o
	$(AR) -cr $@

# Compiler flags
COPTS=$(foreach i,$(INC_DIRS),-I $i)
#include $(TAP_PATH)/tools/build/tap.mk
CFLAGS+=$(COPTS) -c
CFLAGS+=-DEHS_DEBUG

#
# File extensions
#
EXE=elf
OBJ=o
FINAL=tap

CLEAN_FILES=*.$(FINAL)

