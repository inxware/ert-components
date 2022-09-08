#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# This file can be included bny any linux hardware toolchain.mk file
#Setup the toolchain path
#defaulting to HOST gcc
# Set up the toolchain paths, depending on platform target paramters. 
# TOOLCHAIN_NAME is an override, should only be set by config.mk and not constructed
#set the build host's machine's architecture (it is always linux so far...)
# done in platform.mk now: export EHS_BUILD_MAC_ARCH=$(shell uname -m)

## todo 2022 we should check for thte existance of the tools chain at this point - perhaps above to identify the root taken to not finding one.

ifndef CC_OVERRIDE
   CC:=gcc
   CPP:=gcc
else 
   CC:=$(CC_OVERRIDE)
   CPP:=$(CC_OVERRIDE)
endif

ifndef LINK_OVERRIDE 
   LINK:=gcc
else
   LINK:=$(LINK_OVERRIDE)
endif

ifndef AS_OVERRIDE 
   AS:=as
else
   AS:=$(AS_OVERRIDE)
endif

ifdef TOOLCHAIN_PATH
ifneq ($(TOOLCHAIN_PATH),HOST)
CC:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(CC)
AS:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(AS)
CPP:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(CPP)
LINK:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(LINK)
#this is needed for toolchains with dynamic libcloog
export LD_LIBRARY_PATH=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/lib/
#@todo we should make the above a non os specific override flag - we shouldn't know about os names here
ifneq ($(EHS_GNU_OS),mingw32msvc) 
    #If there is a sysroot directory in the toolchain we'll try and use it otherwise we will revert to the core support library root
    ifdef EHS_CLIB_OVERRIDE_PATH
            export CC_SWITCHES+=--sysroot=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build
            export LD_SWITCHES+=--sysroot=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build
    else
      ifneq ($(wildcard $(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/sysroot),)
	     export CC_SWITCHES+=--sysroot=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/sysroot
             ifndef LINK_OVERRIDE
		## assume if we are not using gcc then it wont understand sysroot
                export LD_SWITCHES+=--sysroot=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/sysroot
	     endif
      else
           ifneq ($(wilcard $(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)/build),)
                export CC_SWITCHES+=--sysroot=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)/build
                export LD_SWITCHES+=--sysroot=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)/build 
           else
		ifneq ($(wildcard $(EHS_COMPONENT_SUPPORT_BUILD)/usr),)
         	   # if nothing in the build support then we might have a full sysroot in the component support repo (This seems to be needed for nxp builds currently
                   # but if not if we are clang toolchain that has headers and libs in a weird place (i.e. not is it's own sysroot
                  ifneq ($(EHS_TOOLCHAIN_TYPE),clang)
			export CC_SWITCHES+=--sysroot=$(EHS_COMPONENT_SUPPORT_BUILD)
                  	export LD_SWITCHES+=--sysroot=$(EHS_COMPONENT_SUPPORT_BUILD)
		  endif
		endif
           endif
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
#CFLAGS+=-std=c99
CFLAGS+=-std=gnu99
# show include paths CFLAGS+= -v -x c -E
 
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
else

endif

#
# File extensions
# @todo these aren't actually used - just for a PHONY target. - would be good to use these...
ifndef EXE
EXE = exe
endif
ifndef OBJ
OBJ = o
endif
FINAL=$(EXE)

CLEAN_FILES=
