#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#
# Makefile fragment to build constributed json files for EHS.

# Called by ../hal.mk
#
# @author: inx limited

# Predefined variables

#  OBJ - File extension for object files

#  NOREV - Defined if no revision number checking is to be performed
# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

##EHS_COMMON_KERNEL_GRAPHICS_PATH=$(EHS_COMMON_KERNEL_PATH)/graphics # this is common support code in the HAL.
##INC_DIRS+=$(EHS_COMMON_KERNEL_GRAPHICS_PATH)

ifndef EHS_SKIP_GNULIBRARIES
#TODO2024 - the YAJL SUPPORT SHOULDBE SPECIFIC NOT GNU RELATED.
include $(EHS_COMMON_HAL_PATH)/json/contrib/yajl.mk

#Note currently only the YAJL header are in the api directory
INC_DIRS+= $(EHS_COMMON_HAL_PATH)/json/contrib/api

endif

#We have the mini JSON parser also and we mix YAJL and mini  headers in this directory.
INC_DIRS+= $(EHS_COMMON_HAL_PATH)/json/contrib
VPATH+= $(EHS_COMMON_HAL_PATH)/json/contrib


include $(EHS_COMMON_HAL_PATH)/json/deps.mk

