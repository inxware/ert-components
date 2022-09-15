#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

#
# Makefile fragment to build the target-specific code for EHS.

# Called by ../../Makefile
#

# @author: inx limited
# 
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
