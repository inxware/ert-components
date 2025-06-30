#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# This config is basically a copy of the GNU_ALL based config.
# It does the following:
# 1. Setup the toolchain path (defaulting to HOST or EHS_DISTRO_VERSION [or EHS_DISTRO_VERSION] if TOOLCHAIN_NAME=HOST) 
#    This step identifies the build host's machine's architecture and selects the target from there 
# 2. Sets the basic compiler and linker flags to build and create an executable 
# 3. Sets the sysroot to eithr the toolchain's, ert-build-support or ert-contrib-middleware target_libs 
#    (Priority is given in this order depending on what is found at the expected paths)

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

ifndef CC_OVERRIDE
   ifeq ($(EHS_TOOLCHAIN_TYPE),clang)
      export CC:=clang
   else 
      export CC:=gcc
   endif
else 
   export CC:=$(CC_OVERRIDE)
endif

ifndef CXX_OVERRIDE
   ifeq ($(EHS_TOOLCHAIN_TYPE),clang)
      export CPP:=clang      
   else 
      export CPP:=g++
   endif
else 
   export CPP:=$(CXX_OVERRIDE)
endif

# ifndef CC_OVERRIDE
#    ifeq ($(EHS_TOOLCHAIN_TYPE),clang)
#       export CC:=clang
#       export CPP:=clang      
#    else 
#       export CC:=gcc
#       export CPP:=g++
#    endif
# else 
#    export CC:=$(CC_OVERRIDE)
#    export CPP:=$(CXX_OVERRIDE)
# endif

ifndef LINK_OVERRIDE 
   export LINK:=$(CC)
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
      CC:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(CC)
      AS:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(AS)
      CPP:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(CPP)
      LINK:=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/$(LINK)
      CFLAGS+=$(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/
   endif
  
endif

#setup the compiler includes
INC+=$(foreach i,$(INC_DIRS),-I$i)
#@todo pkg-config is set up here for host native builds 

#setip the compiler flags - inherit from any files that include
CFLAGS+=$(foreach i,$(DEFS),-D$i )
CPPFLAGS+=$(foreach i,$(DEFS),-D$i )
CFLAGS+= -c $(INC)

CPPFLAGS+= $(INC)

#setup linker paths
LIB_DIRS+=$(EHS_ROOT_PATH)
# The following path is added for very specific platform support (if there are any).

#setup linker flags / options
ifndef EHS_PLUGIN_LIBRARY_DEPENDENCY
# this cannot be used when building as a static library
LNKFLAGS+= -Wl,-o$(TARGET_NAME).$(EXE)
LNKFLAGS+= -Wl,-E 
# Concatentate linker options, source and paths with -Wl for linkers called via gcc and clang
LNKFLAGS+=$(foreach i,$(LIB_DIRS),-Wl,-L$i)
LNKFLAGS+= $(foreach i,$(LIB),-Wl,-l$i)
else
DEFS+=EHS_PLUGIN_LIBRARY
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

