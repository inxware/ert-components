#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# Makefile fragment to build the HAL logger support for EHS.

# Called by ../kernel.mk
#

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

#  NOREV - Defined if no revision number checking is to be performed
# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

include $(EHS_COMMON_HAL_PATH)/memory/deps.mk

OBJECTS+= hal_mem.$(OBJ)

VPATH+= $(EHS_COMMON_HAL_PATH)/memory

