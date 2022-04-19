#
# Makefile fragment to build the target-specific code for EHS.

# Called by ../../Makefile
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



ehs_windows.res : ehs_windows.rc

# This target supports TCP/IP connectivity
include $(EHS_COMMON_HAL_PATH)/tcp/tcp.mk

OBJECTS += target.$(OBJ)
OBJECTS += keypress.$(OBJ)
OBJECTS += target_console.$(OBJ)
OBJECTS += target_main.$(OBJ) 
OBJECTS += target_process.$(OBJ)
OBJECTS += target_tcp.$(OBJ)
OBJECTS += target_timer.$(OBJ)
OBJECTS += target_viewport.$(OBJ)
OBJECTS += ehs_windows.res

INC_DIRS += $(EHS_TARGET_PATH)/Components
INC_DIRS += $(EHS_TARGET_PATH)/Components/dtv
VPATH+= $(EHS_TARGET_PATH)
VPATH+= $(EHS_TARGET_PATH)/Components
