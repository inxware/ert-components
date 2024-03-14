#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# Makefile fragment to build the OS/compiler-specific code for EHS.
# For mingw compiler builds

# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_OS_HW_PATH - path to the current directory (set by platform makefile)

# include sourcecode from this dir in build

ifndef  EHS_COMMS_API_SUPPORT
export EHS_COMMS_API_SUPPORT=winsock
endif

EHS_MINGW=yes
DEFS += EHS_MINGW
DEFS += EHS_WIN32_RUNENV

LIB += iphlpapi
OBJECTS += target_gapfiller.$(OBJ)
OBJECTS += target_time.$(OBJ)
OBJECTS += targetos_init.$(OBJ)

include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/target.mk
