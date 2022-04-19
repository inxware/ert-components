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

include $(EHS_COMMON_HAL_PATH)/lua/deps.mk

INC_DIRS+=$(EHS_COMPONENT_SUPPORT_INCLUDE)/lua

OBJECTS+= hal_lua.$(OBJ)
VPATH+= $(EHS_COMMON_HAL_PATH)/lua

#no pkgconfig ofr lua:
CFLAGS+=-DLUA_USE_LINUX
LIB+=lua
##LNKFLAGS+=-mwindows 
##This is needed for LUA  - I think..