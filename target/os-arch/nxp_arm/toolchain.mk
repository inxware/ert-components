# copyright INX Ltd 2010
# Target-specific definitions .
# This file is called from Makefile
#
#  Uses $(INC_DIRS), $(TARGET_NAME), $(DEFS)
#  Defines $(CC), $(LINK), $(CFLAGS), $(LNKFLAGS), $(INC), $(LIB), $(EXE), $(OBJ), $(FINAL)
#

#
# Toolset specific
export EHS_NXP_BUILD=1

#Setup the toolchain path
#defaulting to HOST gcc
CC:=gcc
CPP:=gcc
LINK:=gcc
#todo this should move one level of genricity up
ifdef CC_OVERRIDE
CC:=$(CC_OVERRIDE)
CPP:=$(CC_OVERRIDE)
LINK:=$(CC_OVERRIDE)
endif

ifdef TOOLCHAIN_PATH
#ifneq "$(TOOLCHAIN)" ""
CC:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(CC)
CPP:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(CPP)
LINK:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(LINK)
#endif
endif

# Assembler files are built with the same compiler, but different flags
AS:=$(CC)

#setup the compiler flags
CFLAGS+=$(foreach i,$(DEFS),-D$i )

##
## Linker Options
##
OBJ:=o
EXE=axf
FINAL=$(EXE)