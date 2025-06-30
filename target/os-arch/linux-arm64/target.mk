#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#
# @author: inx limited 

# Makefile fragment to build target specific component and support code.
# Called by ../../platform/<platform-type>

# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_OS_HW_PATH - path to the current directory (set by platform makefile)


# !!! The following are DEBIAN ONLY !!!!! #
# !!! We should have a ifdef DEBIAN !!!!! #
# include sourcecode from this dir in build

ifeq ($(TOOLCHAIN_NAME),HOST)
	#Assume this is a host build all the time for clang-arm64?
	#Debian contrib libraries are found here: 
	export INC_DIRS+=/usr/include/aarch64-linux-gnu/
	#It seems libexpat.h is only found in the host's /usr/include after installing the -dev 
	export INC_DIRS+=/usr/include/
	export LIB_DIRS += /usr/lib/aarch64-linux-gnu/
#We usually have glib...
	#ifdef ($(EHS_DEBIAN_VERSION)) 
		 export INC_DIRS +=  /usr/lib/aarch64-linux-gnu/glib-2.0/include/
		 export INC_DIRS += /usr/lib/aarch64-linux-gnu/gtk-2.0/include/
	#endif
else
	#No special target paths here unless we are specific target
endif
include $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/target.mk
include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/target.mk

#libs found for this build - sysroot extracted from Blaze board


########################################################################
#  Set some default toolbox/component librarues for this target type
########################################################################

# Default to stubbing this for targets that don't declare a choice
ifndef EHS_PERIPHERALS_GPIO_SUPPORT
EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
endif

ifndef EHS_PERIPHERAL_DEVICE_SUPPORT
export EHS_PERIPHERAL_DEVICE_SUPPORT=all
endif

#TODO this should be conditional for raspberry pi
#DEFS += EHS_TARGETOS_INIT_SPECIFIC_REQUIRED
#OBJECTS += targetos_init_specific.$(OBJ)
OBJECTS += spi.$(OBJ)
