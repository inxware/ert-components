#
#  eRT components platform build configuration
#
# Makefile fragment using the target's config.mk data to find the correct component and core support code.
# Called by ../../Makefile
#
# This file builds paths to target specific support libraries and toolchains:#
#
# @author: inx limited
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

############# Set up paths to toolchains, libc and middleware dependenciws    ##########
# Set up bthe toolchain paths, depending on platform target paramters. 
# We still set the tool chain OS & ARCH even for host builds as the kernel is placed there
ifdef EHS_GNU_OS
	TOOLCHAIN_OS=$(EHS_GNU_OS)
else
	TOOLCHAIN_OS=$(EHS_OS)
endif
ifdef EHS_GNU_ARCH
	TOOLCHAIN_ARCH=$(EHS_GNU_ARCH)
else 
	TOOLCHAIN_ARCH=$(EHS_ARCH)
endif

#Note we have some toolchains in the oposite order e.g. linux-android-armv7a 
#- in which case we can either fix it in the support repo or use the TOOLCHAIN_NAME override

############# Set up paths to  libc and middleware dependencies    ##########

#if the platform doesn't specify a specific libc/middleware version with EHS_GNU_OS_VERSION then set it to the same as the toolchain
ifdef EHS_GNU_OS_VERSION
	EHS_GNU_OS_ARCH=$(TOOLCHAIN_ARCH)-$(TOOLCHAIN_OS)$(EHS_GNU_OS_VERSION)
else 
    EHS_GNU_OS_ARCH=$(TOOLCHAIN_ARCH)-$(TOOLCHAIN_OS)
endif
export EHS_GNU_OS_ARCH

## Set upvariables first 
################# Some components might need kernel headers e.g. sizes.h       ###################
export KERNEL_HEADERS_BASE_DIR=$(EHS_CORE_SUPPORT_BASE)/kernel-dependencies/
export KERNEL_HEADERS_RELPATH 
export INC_DIRS += ${KERNEL_HEADERS_BASE_DIR}/$(KERNEL_HEADERS_RELPATH)

############## Define the OS_HW PATH for dependencies within the eRT-componnents source tree ######
export EHS_TARGET_OS_HW_PATH=$(EHS_TARGETS_ROOT_PATH)/os-arch/$(EHS_OS)-$(EHS_ARCH)

####################   Configure the toolchain parameters ############################## 
include $(EHS_TARGET_OS_HW_PATH)/toolchain.mk

################# Set up the Component Library Support Paths ####################################

ifdef COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH
#if you want component support from a path defined by the toolchain name or something else specific then use this override method 
export COMPONENT_BASE_TECHNOLOGIES:=$(COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH)
else
	ifdef COMPONENT_VARIANT
        export COMPONENT_BASE_TECHNOLOGIES:=$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)_$(COMPONENT_VARIANT)
else 
        export COMPONENT_BASE_TECHNOLOGIES:=$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)
endif
endif

export EHS_TARGET_COMPONENT_HAL_PATH=$(EHS_TARGETS_ROOT_PATH)/Component-HAL

ifndef COMPONENT_BASE_TECHNOLOGIES
$(error COMPONENT_BASE_TECHNOLOGIES  is not defined)
endif


################## Build the core target specific support code #########################
# include files to build the core EHS target specific for this platform + any special OS Compoents.

 # Required for EHS core stuff (What is the difference between the following?)
include $(EHS_TARGET_OS_HW_PATH)/target.mk

################## Set up any debugging settings.                               ####### 
# Check if call trace logging has been enabled
ifdef EHS_DEBUG_TRACE
DEFS += EHS_BUILDOPT_STDIO_MESSAGE_TRACE #this is for specific messages
DEFS += EHS_BUILDOPT_STDIO_ENABLE_FUNCTION_TRACING # this is the legacy tracing @todo remove the argument number specificity
DEFS += EHS_RUNTIME_LOGGER_ENABLED
DEFS += EHS_DEBUG# (Switch on all Debug?)
endif
# set the standard error/warning/info debug macro if set
ifdef EHS_DEBUG
DEFS += EHS_DEBUG# (Switch on all Debug?)
endif
# set the AV middleware debugger on
ifdef EHS_DEBUG_AV
DEFS += EHS_DEBUG_AV# (Switch on all Debug?)
endif

############## Set up some eRT Source level conditional build macros          ##########
# If the platform as system variant, let the code use the macro for build configuration.
DEFS += $(SYSTEM_VARIANT)#todo as above!


# IF WE HAVE A NATIVE BUILD (e.g. docker) THEN MUCH OF THE ABOVE SHOULD PROBABLY BE REMOVED? 
# Though it probably doesn't do any harm having linkes to resources in ert-* support repos if there's nothing in them.
# TODO2022 we want this to be set more generically for platform.mk that wnt to build under a specific docker or vagrant environment.

ifeq ($(EHS_HOST_DEBIAN_BUILD),yes)
#todo2022 not sure if either of the following are actually required, as they sohuld be set up by docker or vagrant for the speicific target
export INC_DIRS+=/usr/include
    #todo2022 the following line needs to be done better (i.e. to pick  up the required target) if it is needed.
export LIB_DIRS+=/usr/lib/x86_64-linux-gnu/
    export LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)/kernel/
else
# Add paths the ert-build-support's LIBC
	ifdef EHS_CLIB_OVERRIDE_PATH
		export INC_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/include/
		export LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/lib/
        export LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/kernel/
		export LD_LIBRARY_PATH+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/lib/
	else
#Note the following is usally handled with the gcc --sysroot, but we'll add INC and LIB paths explicitly too.
		export INC_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)/build/include/
		export LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)/build/lib/
        export LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)/kernel/
		export LD_LIBRARY_PATH+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_GNU_OS_ARCH)$(EHS_SPECIAL_CLIB_EXT)/build/lib/
	endif
#Add the component lib paths as distilled above
export INC_DIRS+=$(EHS_COMPONENT_SUPPORT_INCLUDE)
export LIB_DIRS+=$(EHS_COMPONENT_SUPPORT_LIBS)
endif



################# Setup the target include paths for core EHS ##########################
INC_DIRS +=$(EHS_PLATFORM_PATH)
VPATH += $(EHS_PLATFORM_PATH)
#VPATH+= $(EHS_TARGET_OS_HW_PATH)
#OBJECTS+=: none - there are no c-files for each platform just config parameters.
# Do the rest in the os make file

include $(EHS_TARGET_OS_HW_PATH)/deps.mk
INC_DIRS += $(EHS_TARGET_OS_HW_PATH)
VPATH+= $(EHS_TARGET_OS_HW_PATH)

# and apply to the compiler paths 
export EHS_COMPONENT_SUPPORT_INCLUDE:=$(EHS_COMPONENT_SUPPORT_BASE)/target_libs/$(COMPONENT_BASE_TECHNOLOGIES)/build/include/
export EHS_COMPONENT_SUPPORT_LIBS:=$(EHS_COMPONENT_SUPPORT_BASE)/target_libs/$(COMPONENT_BASE_TECHNOLOGIES)/build/lib/

#include sourcecode from components dir in build
include $(EHS_TARGET_OS_HW_PATH)/Components/deps.mk
INC_DIRS += $(EHS_TARGET_OS_HW_PATH)/Components
VPATH+= $(EHS_TARGET_OS_HW_PATH)/Components

##include $(EHS_TARGET_OS_HW_PATH)/ehs-core-hal.mk @todo to delete
include $(EHS_TARGET_COMPONENT_HAL_PATH)/component-hal.mk

#
# add required object files to OBJECTS
#
#Core EHS
ifndef EXCLUDE_EHS_COMMON
ifdef EHS_BUILD_MONOLITHIC_KERNEL
include $(EHS_COMMON_KERNEL_PATH)/kernel.mk
endif
include $(EHS_COMMON_HAL_PATH)/HAL.mk

#Configure the Components Code used
#build the common Layer (The common components.mk file will conditionally compile depending on Component Options
include $(EHS_COMMON_COMPONENTS_PATH)/components.mk
#All target stuff is done from the platform.mk file (indirectly) relative target specific layer
include $(EHS_COMMON_EHS_PATH)/ehs.mk
endif

############## Agregate OS configuration scripts from config.mk recipe and command line ###########
export HOST_OS_CONFIG_SCRIPTS+=$(HOST_OS_CONFIG_SCRIPTS_EXTRA)
