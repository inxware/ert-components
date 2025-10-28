#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# Makefile fragment to build target specific component and support code.

# Called by ../../platform/<platform-type>
# @author: inx limited

# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_OS_HW_PATH - path to the current directory (set by platform makefile)

ifeq ($(TOOLCHAIN_NAME),HOST)
	ifeq ($(EHS_GNU_ARCH),x86_64)
		#Debian contrib libraries are found here: 
		#export INC_DIRS+=/usr/include/
		export LIB_DIRS += /usr/lib/x86_64-linux-gnu/
	else
		#Debian contrib libraries are found here: 
		#export INC_DIRS+=/usr/include/
		#export LIB_DIRS += /usr/lib//usr/lib/x86-linux-gnu/
	endif
else
#No special target paths here unless we are specific target as below	
endif

# include sourcecode from this dir in build

include $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/target.mk
include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/target.mk




