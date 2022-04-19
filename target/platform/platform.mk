#
# Makefile fragment using the target's config.mk data to find the correct component and core support code.
# Called by ../../Makefile
#
# This file builds paths to target specific support libraries and toolchains:
# External Support library Path are defined:
# $(EHS_GNU_ARCH)-$(EHS_GNU_OS)-$(EHS_GNU_OS_VERSION)-$(COMPONENT_BASE_TECHNOLOGIES)
#
#
#
# @author: inx limited, Pierre Drezet
# (C) 2008
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_PLATFORM_PATH - path to the current directory (set by platform makefile)

################## Get the platform parameters from the platform config.mk file #######
include $(EHS_PLATFORM_PATH)/config.mk

# pickup the debug flag if given

ifdef EHS_DEBUG_TRACE
DEFS += EHS_BUILDOPT_STDIO_MESSAGE_TRACE #this is for specific messages
DEFS += EHS_BUILDOPT_STDIO_ENABLE_FUNCTION_TRACING # this is the legacy tracing @todo remove the argument number specificity
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_DEBUG# (Switch on all Debug?)
endif

ifdef EHS_DEBUG
DEFS += EHS_DEBUG# (Switch on all Debug?)
endif
ifdef EHS_DEBUG_AV
DEFS += EHS_DEBUG_AV# (Switch on all Debug?)
endif

DEFS += $(SYSTEM_VARIANT)#todo as above!

export EHS_BUILD_MAC_ARCH=$(shell uname -m)
#Toolchains are arrangd in th rt-build support fist y th host architctur thy iill run and thn by th targt archictur they produc code for.

#GNU OS Version is the target's core sysroot path. This can b set to an alternative path toth toolchains' sysroot. 
# todo2022 - this looks like nonsense - we pnly use EHS_GNU_OS_VERSION as an overide
#ifndef EHS_GNU_OS_VERSION
#EHS_GNU_OS_VERSION=$(TOOLCHAIN_PATH)
#endif


#Define the OS_HW PATH for toolchain

export EHS_TARGET_OS_HW_PATH=$(EHS_TARGETS_ROOT_PATH)/os-arch/$(EHS_OS)_$(EHS_ARCH)
export EHS_TARGET_COMPONENT_HAL_PATH=$(EHS_TARGETS_ROOT_PATH)/Component-HAL


################## setup the target library linker paths for component support ################## 
## This is done globally as all components add their lot into the global component support directory.
# always need this for libc 
ifdef CLIB_USEHOST #- switch if CLIB is defined as "use host"
export INC_DIRS+=/usr/inlcude
export LIB_DIRS+=/usr/lib
export LIB_DIRS+=/usr/lib/x86_64-linux-gnu/
else 
ifdef EHS_CLIB_OVERRIDE_PATH
# The clib overrde path should be used if a  different clib to the one used to identify the ert-middleware is desired.
export INC_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/include/
export LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/lib/
export LD_LIBRARY_PATH+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/lib/
export LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/kernel/
else

#*************************** DEFAULT ert-build support  *******************************************************
#Note the following might not exist or be needed if all clib stuff is provided in the toolchain's sysroot, which is most likely
# The folllowing defines the path in rt-build-support for ctandard clibraries
# ./ert-build-support/support_libs/target_libs/<arch-OS-libc version>/build/[include/lib]
export INC_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_ARCH)-$(EHS_GNU_OS)$(EHS_GNU_OS_VERSION)$(EHS_SPECIAL_CLIB_EXT)/build/include/
export LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_ARCH)-$(EHS_GNU_OS)$(EHS_GNU_OS_VERSION)$(EHS_SPECIAL_CLIB_EXT)/build/lib/
export LD_LIBRARY_PATH+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_ARCH)-$(EHS_GNU_OS)$(EHS_GNU_OS_VERSION)$(EHS_SPECIAL_CLIB_EXT)/build/lib/
#************************************************************************************************************
#the following provides tehe path to the ert kernel library in the following ert-build-upport directory:
# ./ert-build-support/support_libs/target_libs/<arch-OS-libc version>/kernel/
#The minimum requirement is for a file called libhs.a
export LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_ARCH)-$(EHS_GNU_OS)$(EHS_GNU_OS_VERSION)/kernel/
endif
endif

export KERNEL_HEADERS_BASE_DIR=$(EHS_CORE_SUPPORT_BASE)/kernel-dependencies/
export KERNEL_HEADERS_RELPATH 
export INC_DIRS += ${KERNEL_HEADERS_BASE_DIR}/$(KERNEL_HEADERS_RELPATH)


##################################################################################
# Set up the Component Library support Path

ifdef COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH
export COMPONENT_BASE_TECHNOLOGIES:=$(COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH)
else
ifdef TOOLCHAIN_NAME 
# Some platforms may hav created the middleware binaries using some other toolchain name and thi s should be  prefixed on to the ert-middeware path.
# todo2022 - Do we  really need tihis? What uses oomponent variant?
export COMPONENT_BASE_TECHNOLOGIES:=$(TOOLCHAIN_NAME)_$(EHS_GNU_ARCH)-$(EHS_GNU_OS)$(EHS_GNU_OS_VERSION)_$(COMPONENT_VARIANT)
else 
#if we don't use a tool chain from ert-build-support, then name the component libraries using the more generic arch-os identifiers
ifndef EHS_GNU_OS_VERSION
#EHS_GNU_OS_VERSION=$(TOOLCHAIN_PATH)
export COMPONENT_BASE_TECHNOLOGIES:=$(EHS_GNU_ARCH)-$(EHS_GNU_OS)_$(EHS_GNU_ARCH)-$(EHS_GNU_OS)_$(COMPONENT_VARIANT)
else
#*************************** DEFAULT  ert-middleware**********************************************************  
# The ert-middlewre ipath is prefixed by the tolchain identifier and the clib identiifier 
export COMPONENT_BASE_TECHNOLOGIES:=$(EHS_GNU_ARCH)-$(EHS_GNU_OS)$(EHS_GNU_OS_VERSION)_$(EHS_GNU_ARCH)-$(EHS_GNU_OS)$(EHS_GNU_OS_VERSION)_$(COMPONENT_VARIANT)
#************************************************************************************************************   
endif
endif
endif


export EHS_COMPONENT_SUPPORT_INCLUDE:=$(EHS_COMPONENT_SUPPORT_BASE)/target_libs/$(COMPONENT_BASE_TECHNOLOGIES)/build/include/
export EHS_COMPONENT_SUPPORT_LIBS:=$(EHS_COMPONENT_SUPPORT_BASE)/target_libs/$(COMPONENT_BASE_TECHNOLOGIES)/build/lib/

ifeq ($(EHS_NATIVE_DEBIAN_BUILD),yes)
export INC_DIRS+=/usr/include
export LIB_DIRS+=/usr/lib/x86_64-linux-gnu/
else
export INC_DIRS+=$(EHS_COMPONENT_SUPPORT_INCLUDE)
export LIB_DIRS+=$(EHS_COMPONENT_SUPPORT_LIBS)
endif
export INC_DIRS+=../EHS-kernel/Common/Kernel

##export LD_LIBRARY_PATH+=:$(EHS_COMPONENT_SUPPORT_LIBS)
####################   Configure the toolchain parameters ############################## 
include $(EHS_TARGET_OS_HW_PATH)/toolchain.mk


################# Setup the target include paths for core EHS ##########################
INC_DIRS +=$(EHS_PLATFORM_PATH)
VPATH += $(EHS_PLATFORM_PATH)
#VPATH+= $(EHS_TARGET_OS_HW_PATH)
#OBJECTS+=: none - there are no c-files for each platform just config parameters.
# Do the rest in the os make file

include $(EHS_TARGET_OS_HW_PATH)/deps.mk
INC_DIRS += $(EHS_TARGET_OS_HW_PATH)
VPATH+= $(EHS_TARGET_OS_HW_PATH)

#include sourcecode from components dir in build
include $(EHS_TARGET_OS_HW_PATH)/Components/deps.mk
INC_DIRS += $(EHS_TARGET_OS_HW_PATH)/Components
VPATH+= $(EHS_TARGET_OS_HW_PATH)/Components

################## Build the core target specific support code #########################
# include files to build the core EHS target specific for this platform + any special OS Compoents.

 # Required for EHS core stuff (What is the difference between the following?)
include $(EHS_TARGET_OS_HW_PATH)/target.mk
##include $(EHS_TARGET_OS_HW_PATH)/ehs-core-hal.mk @todo to delete
include $(EHS_TARGET_COMPONENT_HAL_PATH)/component-hal.mk

############## Agregate OS configuration scripts from config.mk recipe and command line ###########
export HOST_OS_CONFIG_SCRIPTS+=$(HOST_OS_CONFIG_SCRIPTS_EXTRA)


 