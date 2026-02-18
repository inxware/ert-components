#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# This file can be included bny any linux hardware toolchain.mk file
# It does the following:
# 1. Setup the toolchain path (defaulting to HOST or EHS_DISTRO_VERSION [or EHS_DISTRO_VERSION] if TOOLCHAIN_NAME=HOST)
#    This step identifies the build host's machine's architecture and selects the target from there
# 2. Sets the basic compiler and linker flags to build and create an executable
# 3. Sets the sysroot to eithr the toolchain's, ert-build-support or ert-contrib-middleware target_libs
#    (Priority is given in this order depending on what is found at the expected paths)

ifndef CC_OVERRIDE
   ifeq ($(EHS_TOOLCHAIN_TYPE),clang)
      export CC:=clang
      export CPP:=clang++
   else
      export CC:=gcc
      export CPP:=gcc
   endif
else
   export CC:=$(CC_OVERRIDE)
   export CPP:=$(CC_OVERRIDE)
#   GCC_EXEC_PREFIX:=
endif

ifndef LINK_OVERRIDE
   # Use C++ compiler for linking when there are C++ source files in the build
   # This is necessary for proper C++ runtime initialization and library linking
   CPP_SOURCES := $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard target/Component-HAL/*/*.cpp) $(wildcard target/Component-HAL/*/*/*.cpp)
   ifneq ($(CPP_SOURCES),)
      export LINK:=$(CPP)
      LIB+=stdc++
   else
      export LINK:=$(CC)
   endif
else
   export LINK:=$(LINK_OVERRIDE)
endif

ifndef AS_OVERRIDE
   AS:=as
else
   AS:=$(AS_OVERRIDE)
endif

# Set the sysroot of the compiler and an absolute path to the binary if it is not an installed one.
ifdef TOOLCHAIN_PATH
ifneq ($(TOOLCHAIN_PATH),HOST)
   #set gcc's override path for the compiler (otherwise gcc just wants to make code and doesn't really care what it is.
   #export COMPILER_PATH=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/
   #CFLAGS+= -B$(COMPILER_PATH)

	CC:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(CC)
	AS:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(AS)
	CPP:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(CPP)
	LINK:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(LINK)
endif
ifdef EHS_SYSROOT_ABS_PATH_OVERRIDE
      export EHS_BUILD_SYSROOT = $(EHS_SYSROOT_ABS_PATH_OVERRIDE)
      export CC_SWITCHES+=--sysroot=$(EHS_BUILD_SYSROOT)
      export LD_SWITCHES+=--sysroot=$(EHS_BUILD_SYSROOT)
  else
   ifneq ($(TOOLCHAIN_PATH),HOST)
      #this is needed for toolchains with dynamic libcloog
      export LD_LIBRARY_PATH=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/lib/
      #@todo we should make the above a non os specific override flag - we shouldn't know about os names here
      ifneq ($(EHS_GNU_OS),mingw32posix)
         #If there is a sysroot directory in the toolchain we'll try and use it otherwise we will revert to the core support library root
         ifdef EHS_CLIB_OVERRIDE_PATH
                  export EHS_BUILD_SYSROOT = $(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build
                  export CC_SWITCHES+=--sysroot=$(EHS_BUILD_SYSROOT)
                  export LD_SWITCHES+=--sysroot=$(EHS_BUILD_SYSROOT)
         else
            ifneq ($(wildcard $(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/sysroot),)
               export EHS_BUILD_SYSROOT = $(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/sysroot
               export CC_SWITCHES+=--sysroot=$(EHS_BUILD_SYSROOT)
               ifndef LINK_OVERRIDE
                 ## assume if we are not using gcc then it wont understand sysroot
                 export LD_SWITCHES+=--sysroot=$(EHS_BUILD_SYSROOT)
               endif
            else
            ## We do have a sysroot directory at the toolchain's binary path, but first lets see if there is one in the build support
               ifneq ($(wilcard $(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)/build),)
                     export CC_SWITCHES+=--sysroot=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)/build
                     export LD_SWITCHES+=--sysroot=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)/build
               else
                  ## And lets also check of there is a /usr/ in the contrib middleware build dir.
                  ifneq ($(wildcard $(EHS_COMPONENT_SUPPORT_BUILD)/usr),)
                     # if nothing in the build support then we might have a full sysroot in the component support repo (This seems to be needed for nxp builds currently
                     # but if not if we are clang toolchain that has headers and libs in a weird place (i.e. not is it's own sysroot
                        ifneq ($(EHS_TOOLCHAIN_TYPE),clang)
                           export EHS_BUILD_SYSROOT = $(EHS_COMPONENT_SUPPORT_BUILD)
                           export CC_SWITCHES+=--sysroot=$(EHS_BUILD_SYSROOT)
                           export LD_SWITCHES+=--sysroot=$(EHS_BUILD_SYSROOT)
                        endif
                  endif
               endif
            endif
         endif
      endif
   endif
  endif
endif

# Suuport the llvm (clang) linker/archiver switches to make an executable if specified
ifeq ($(LINK_OVERRIDE),llvm-ar)
    #This is for gcc's ar to create a static library:
    ifndef EHS_MINGW
    #mingw doesn't support this..
      export LD_SWITCHES += rcs ${TARGET_NAME}.${FINAL}
   endif
else
    #export LD_SWITCHES += -o ${TARGET_NAME}.${FINAL}
endif

#setup the compiler includes
INC+=$(foreach i,$(INC_DIRS),-I$i)
#@todo pkg-config is set up here for host native builds

#setip the compiler flags - inherit from any files that include
CFLAGS+=$(foreach i,$(DEFS),-D$i )
CPPFLAGS+=$(foreach i,$(DEFS),-D$i )
CFLAGS+= -c $(INC)

#todo - This should be a specific macro to not use c99 that is set in the esp32's toolchain.mk file -
ifndef EHS_ESP32
   CFLAGS+= -g -D_POSIX_C_SOURCE=199309 
   # provide support for long long constants
   #CFLAGS+=-std=c99
   CFLAGS+=-std=gnu99
endif

# CXX_INC_DIRS holds C++-only include paths (e.g. Qt headers that require C++17)
CXX_INC=$(foreach i,$(CXX_INC_DIRS),-I$i)
# CPPFLAGS+= -c $(INC) $(CXX_INC)
CPPFLAGS+= -c $(CXX_INC)

#setup linker paths
LIB_DIRS+=$(EHS_ROOT_PATH)
# The following path is added for very specific platform support (if there are any).

#setup linker flags / options

#todo we should move this into a more generic .mk file for not just gnu compilers as llvm etc. uses this too
#We'll support the llvm linker called directly (though it's not a good idea - best to use lld via clan as below)
ifeq ($(LINK),ld.lld)
   LNKFLAGS+= -o$(TARGET_NAME).$(EXE)
   LNKFLAGS+= -E
   #for clang we need to explicitly add the stdc lib and math etc.
   LIB+=c m
   # note LLVM-7 is needed for __umodti3 (used by greengrass)

   # Concatentate linker options, source and paths.
   LNKFLAGS+=$(foreach i,$(LIB_DIRS),-L$i)
   LNKFLAGS+= $(foreach i,$(LIB),-l$i)

else
   ifeq ($(LINK),clang)
      ifeq ($(EHS_GNU_ARCH),x86_64)
      #todo2023 - the following is hack because the arm clang compiler doesn't like this
         LD_SWITCHES += -fuse-ld=lld
      endif
      LIB += m
   endif
   #gnu ld:
   LNKFLAGS+= -Wl,-o$(TARGET_NAME).$(EXE)
   LNKFLAGS+= -Wl,-E
   # Concatentate linker options, source and paths with -Wl for linkers called via gcc and clang
   LNKFLAGS+=$(foreach i,$(LIB_DIRS),-Wl,-L$i)
   LNKFLAGS+= $(foreach i,$(LIB),-Wl,-l$i)
endif

#Core libraries we always use for gnu platforms
ifeq ($(EHS_GNU_OS),mingw32posix)
   #LIB += pthreadGC2
   #LIB += pthreadGCE2
   LIB += pthread
else

endif

#
# Build output extensions
#
# Define what the linked executable extenion should be
ifndef EXE
   EXE = exe
endif
# Define what the intermdiate source object extenions should be
ifndef OBJ
   OBJ = o
endif

FINAL=$(EXE)

CLEAN_FILES=

#
# Add and debug & profiling instrumentation/symbols
#

ifdef EHS_INSTRUMENT_GPERF_PROFILING
	CFLAGS += -pg
	LNKFLAGS += -pg
endif

ifeq ($(EHS_TOOLCHAIN_TYPE),clang)
   CFLAGS += -fdiagnostics-color=always
   CPPFLAGS += -fdiagnostics-color=always
endif
