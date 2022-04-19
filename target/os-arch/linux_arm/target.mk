#
# Makefile fragment to build target specific component and support code.

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

include $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/target.mk
include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/target.mk

#libs found for this build - sysroot extracted from Blaze board
ifeq ($(SYSTEM_VARIANT),OMAP4_TI)
LIB+= uuid
LIB+= pcre
LIB+= audio
LIB+= crypto
else
ifeq ($(SYSTEM_VARIANT),RASPBERRYPI)
LIB += crypto
LIB += selinux
LIB += pcre
endif
endif







