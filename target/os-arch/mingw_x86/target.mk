#
# Makefile fragment to build the OS/compiler-specific code for EHS.

# Called by ../../platform/<platform-type>
#

# @author: inx limited
# @version: $Revision: 43 $
# @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
# 
# Copyright (c) inx limited, 2007. All rights reserved.
#
#
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_OS_HW_PATH - path to the current directory (set by platform makefile)

# include sourcecode from this dir in build




include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/target.mk

EHS_MINGW=yes

DEFS += EHS_GST_010
DEFS += EHS_MINGW
DEFS += EHS_WIN32_RUNENV

LIB += iphlpapi
LIB += archive-2

OBJECTS += target_gapfiller.$(OBJ)
OBJECTS += target_time.$(OBJ)
OBJECTS += targetos_init.$(OBJ)
