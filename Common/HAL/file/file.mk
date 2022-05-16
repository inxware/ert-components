#
# Makefile fragment to build the HAL file support for EHS.

# Called by ../kernel.mk
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

#  NOREV - Defined if no revision number checking is to be performed
# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

include $(EHS_COMMON_HAL_PATH)/file/deps.mk

OBJECTS+= hal_file.$(OBJ)

VPATH+=: $(EHS_COMMON_HAL_PATH)/file

# This doesn't work for some reason
#ifdef EHS_MINGW2
#LIB += archive-2
#else
#LIB += archive
#endif

