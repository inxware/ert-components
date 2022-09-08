#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
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



