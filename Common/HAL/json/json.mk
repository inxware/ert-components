#
# Makefile fragment to build constributed json files for EHS.

# Called by ../hal.mk
#
# @author: Pierre Drezet
# @version: $Revision: 43 $
# @date: $Date: 2012-02-9  $
# 
# Copyright (c) inx Ltd, 2007. All rights reserved.
#
#
# Predefined variables

#  OBJ - File extension for object files

#  NOREV - Defined if no revision number checking is to be performed
# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

##EHS_COMMON_KERNEL_GRAPHICS_PATH=$(EHS_COMMON_KERNEL_PATH)/graphics # this is common support code in the HAL.
##INC_DIRS+=$(EHS_COMMON_KERNEL_GRAPHICS_PATH)

include $(EHS_COMMON_HAL_PATH)/json/contrib/yajl.mk

include $(EHS_COMMON_HAL_PATH)/json/deps.mk

