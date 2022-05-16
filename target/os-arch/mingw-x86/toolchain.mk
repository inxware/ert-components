# copyright INX Ltd 2010
# Target-specific definitions .
# This file is called from Makefile
#
#  Uses $(INC_DIRS), $(TARGET_NAME), $(DEFS)
#  Defines $(CC), $(LINK), $(CFLAGS), $(LNKFLAGS), $(INC), $(LIB), $(EXE), $(OBJ), $(FINAL)
#

#
# Toolset specific 


#ifeq ( 1,1 )


#i686-w64-mingw32


#We have floating Point

CFLAGS += -mms-bitfields
LNKFLAGS+= -Wl,-lm
LNKFLAGS+= -Wl,-lmingw32
ifeq ($(SYSTEM_VARIANT),unity)
CFLAGS += -D ADD_EXPORTS
endif

##
## Linker Options
##

ifeq ($(SYSTEM_VARIANT),unity)
override EXE = dll
LNKFLAGS+=-mwindows# Removes console output for debug versions
LNKFLAGS+=-Wl,-lws2_32
export LD_SWITCHES+= -shared -no-canonical-prefixes  -Wl,--no-undefined  -Wl,--subsystem,windows
# these wer needed for the android so, but might not be needed for the mingw version
# LD_SWITCHES+=-lgcc -no-canonical-prefixes  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now
# changing to the latest apt-get mingw compiler
export CC_OVERRIDE=i686-w64-mingw32-gcc
export LINKER_OVERRIDE=i686-w64-mingw32-gcc
else
ifeq ($(EHS_DEBUG),yes)
LNKFLAGS+=-mconsole#Adds the console support
else 
LNKFLAGS+=-mwindows# Removes console output for debug versions
endif
export CC_OVERRIDE=i586-mingw32msvc-gcc
##export LINKER_OVERRIDE=i586-mingw32msvc-gcc
endif 

include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/toolchain.mk
