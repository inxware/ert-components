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
#

#
# Toolset specific 


#i686-w64-mingw32


#We have floating Point

CFLAGS += -mms-bitfields
LIB += m mingw32
CFLAGS += -static-libgcc 
LNKFLAGS += -static-libgcc 
#-static

ifeq ($(SYSTEM_VARIANT),unity)
    CFLAGS += -D ADD_EXPORTS
endif

##
## Linker Options
##

#todo2022 the unity verion should just be the mingwi686 toolchain. and the following conditionals can then be removed
ifeq ($(SYSTEM_VARIANT),unity)
    override EXE = dll
    LNKFLAGS+=-mwindows# Removes console output for debug versions
    LNKFLAGS+=-Wl,-lws2_32
    export LD_SWITCHES+= -shared -no-canonical-prefixes  -Wl,--no-undefined  -Wl,--subsystem,windows
    # these wer needed for the android so, but might not be needed for the mingw version
    # LD_SWITCHES+=-lgcc -no-canonical-prefixes  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now
    # changing to the latest apt-get mingw compiler
else
    ifeq ($(EHS_RUNTIME_LOGGER_ENABLED),yes)
        LNKFLAGS+=-mconsole # Adds console output for debug 
    else
        LNKFLAGS+=-mwindows # Removes console output for debug versions
    endif
endif

#Use i686-migw toolchain as a default
ifndef CC_OVERRIDE
    export CC_OVERRIDE=i686-w64-mingw32-gcc-10-posix
endif
ifndef LINK_OVERRIDE
     export LINK_OVERRIDE=i686-w64-mingw32-gcc
endif

include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/toolchain.mk
