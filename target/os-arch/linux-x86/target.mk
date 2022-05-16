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

INC_DIRS += $(EHS_TARGET_OS_HW_PATH)/textdisplay #@todo this should be moved when win support is available

ifdef EHS_PERIPHERALS_LEDS
OBJECTS += target_leds.$(OBJ)
endif

ifdef EHS_PERIPHERALS_USBLCD
OBJECTS += target_textdisplay.$(OBJ)
endif



