#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

#
# Makefile fragment to build the HAL file support for EHS.

# Called by ../kernel.mk
#

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

#  NOREV - Defined if no revision number checking is to be performed
# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

include $(EHS_COMMON_HAL_PATH)/file/deps.mk

ifeq ($(EHS_FILESYSTEM_SUPPORT),none)
	DEFS += EHS_FILESYSTEM_SUPPORT__NONE
endif
ifeq ($(EHS_FILESYSTEM_SUPPORT),stubbed)
	OBJECTS += hal_file_stubbed.$(OBJ)
	DEFS+=EHS_FILESYSTEM_SUPPORT__STUBBED
else 
	OBJECTS+= hal_file.$(OBJ)
endif

# TODO2025 the stubbed and micro file system should be alternative VPATHs for code. 
# target-specific filesystem support for libc still the same but should only apply in the posix case? 
VPATH+=: $(EHS_COMMON_HAL_PATH)/file

