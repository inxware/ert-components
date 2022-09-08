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

#todo2022 the unity verion should just be the mingwi686 toolchain. and the following conditionals can then be removed
ifeq ($(SYSTEM_VARIANT),unity)
    override EXE = dll
    LNKFLAGS+=-mwindows# Removes console output for debug versions
    LNKFLAGS+=-Wl,-lws2_32
    export LD_SWITCHES+= -shared -no-canonical-prefixes  -Wl,--no-undefined  -Wl,--subsystem,windows
    # these wer needed for the android so, but might not be needed for the mingw version
    # LD_SWITCHES+=-lgcc -no-canonical-prefixes  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now
    # changing to the latest apt-get mingw compiler
    # CFLAGS  += -I/usr/i686-w64-mingw32/include
    export CC_OVERRIDE=i686-w64-mingw32-gcc
    export LINK_OVERRIDE=i686-w64-mingw32-gcc
else
    ifeq ($(EHS_DEBUG),yes)
        LNKFLAGS+=-mconsole#Adds the console support
    else 
       LNKFLAGS+=--mwindows
        # Removes console output for debug versions
    endif
    #export CC_OVERRIDE=i586
    export CC_OVERRIDE=i586-mingw32msvc-gcc
    export LINK_OVERRIDE=i586-mingw32msvc-gcc
    #CFLAGS += -I/usr/i686-w64-mingw32/include
endif 

include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/toolchain.mk
