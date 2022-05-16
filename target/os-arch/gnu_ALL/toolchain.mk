# This file can be included bny any linux hardware toolchain.mk file


#Setup the toolchain path
#defaulting to HOST gcc
# Set up the toolchain paths, depending on platform target paramters. 
# TOOLCHAIN_NAME is an override, should only be set by config.mk and not constructed
#set the build host's machine's architecture (it is always linux so far...)
export EHS_BUILD_MAC_ARCH=$(shell uname -m)

ifdef TOOLCHAIN_NAME
    ifeq ($(TOOLCHAIN_NAME),HOST)
        export EHS_HOST_DEBIAN_BUILD=yes
        export TOOLCHAIN_PATH=HOST
else
        export TOOLCHAIN_PATH=$(EHS_BUILD_MAC_ARCH)/$(TOOLCHAIN_NAME)
    endif
else
	export TOOLCHAIN_PATH=$(EHS_BUILD_MAC_ARCH)/$(EHS_GNU_OS_ARCH)$(EHS_GNU_OS_VERSION)
endif

CC:=gcc
CPP:=gcc
LINK:=gcc
#todo this should move one level of genricity up
ifdef CC_OVERRIDE
CC:=$(CC_OVERRIDE)
CPP:=$(CC_OVERRIDE)
LINK:=$(CC_OVERRIDE)
endif

ifdef LINK_OVERRIDE
    LINK:=$(LINK_OVERRIDE)
endif

ifdef TOOLCHAIN_PATH
ifneq ($(TOOLCHAIN_PATH),HOST)
CC:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(CC)
CPP:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(CPP)
LINK:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(LINK)
#this is needed for toolchains with dynamic libcloog
export LD_LIBRARY_PATH=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/lib/
#@todo we should make the above a non os specific override flag - we shouldn't know about os names here
ifneq ($(EHS_GNU_OS),mingw32msvc) 
#If there is a sysroot directory in the toolchain we'll try and use it otherwise we will revert to the core support library root
    ifneq ($(wildcard $(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/sysroot),"")
	export CC_SWITCHES+=--sysroot=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/sysroot
	export LD_SWITCHES+=--sysroot=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/sysroot
    else
	ifdef EHS_CLIB_OVERRIDE_PATH
	    export CC_SWITCHES+=--sysroot=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build
	    export LD_SWITCHES+=--sysroot=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build
	else
##we'llidentify it from gcc
#$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)
#@todo - we should probably put this back??
            #export CC_SWITCHES+=--sysroot=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)/build
            #export LD_SWITCHES+=--sysroot=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)/build
	endif
    endif
endif 
endif
endif 


#setup the compiler includes
INC+=$(foreach i,$(INC_DIRS),-I$i)
#@todo pkg-config is set up here for host native builds 

#setip the compiler flags - inherit from any files that include
CFLAGS+=$(foreach i,$(DEFS),-D$i )
CPPFLAGS+=$(foreach i,$(DEFS),-D$i )
CFLAGS+= -c $(INC)
CFLAGS+= -g -D_POSIX_C_SOURCE=199309 
# provide support for long long constants
//CFLAGS+=-std=c99
CFLAGS+=-std=gnu99


#Reorganise graphics structure to work with GDK better
CPPFLAGS+= $(INC)

#setup linker paths
LIB_DIRS+=$(EHS_ROOT_PATH)
# The following path is added for very specific platform support (if there are any).

#add specific libs
#LIB+=dl #@todo remove this for all but ST versions

#setup linker flags / options
LNKFLAGS+= -Wl,-o$(TARGET_NAME).$(EXE)
LNKFLAGS+= -Wl,-E 
# -rpath - We may want to include rpath =${ORIGIN}/cslib instead of explicitly calling this at run time with ./cslib/ld-linux.so.2 --library-path ./cslib/	./ehs.exe
#, but probably not as all libs wouls need to be built with this option also so we'll stick with the launcher method instead
#-E is needed for the dlopen stuff VLC and LUA does

##-rpath - don't want hard-coded lib paths!   ##export dynamics is required for LUA @todo move this to the most basic linux make file level or the LUA make file.


# Concatentate linker options, source and paths.
LNKFLAGS+=$(foreach i,$(LIB_DIRS),-Wl,-L$i)
LNKFLAGS+= $(foreach i,$(LIB),-Wl,-l$i)

#don't use host directories @todo find switch
#Core libraries

ifeq ($(EHS_GNU_OS),mingw32msvc)
#LIB+=pthreadGC2
LIB+=pthreadGCE2# apprently we need this one?
endif
# This is needed for some clibs with stupid ld scripts pointing at absolute paths 


#clib also @todo

#
# File extensions
# @todo these aren't actually used - just for a PHONY target. - would be good to use these...
EXE = exe
OBJ = o
FINAL=$(EXE)

CLEAN_FILES=
