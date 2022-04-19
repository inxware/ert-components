#
# Makefile fragment to build the HAL file support for EHS.

# Called by ../kernel.mk
#

# @author: Pierre Drezet
# @version: $Revision: 43 $
# @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
# 
# Copyright (c) inx limited. All rights reserved.
#
#
# Predefined variables

#  OBJ - File extension for object files

#  NOREV - Defined if no revision number checking is to be performed
# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

# @Todo this is abit of an an outlier - should consider moving to the target directory as this is inked to target binaries.


include $(EHS_COMMON_HAL_PATH)/webkit/deps.mk


VPATH+= $(EHS_COMMON_HAL_PATH)/webkit

OBJECTS+= hal_webkit.$(OBJ)
