#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#


# Makefile fragment to build the target-specific code for EHS.

# Called by ../../Makefile
#

# @author: inx limited

# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code

# This target supports TCP/IP connectivity
include $(EHS_COMMON_HAL_PATH)/tcp/tcp.mk

OBJECTS += target.$(OBJ)
OBJECTS += target_console.$(OBJ)
OBJECTS += target_data.$(OBJ)
OBJECTS += target_main.$(OBJ) 
OBJECTS += target_process.$(OBJ)
OBJECTS += target_tcp.$(OBJ)
OBJECTS += target_timer.$(OBJ)
OBJECTS += target_viewport.$(OBJ)
OBJECTS += libEhsStapi.so


INC_DIRS += $(EHS_TARGET_PATH)/Components
INC_DIRS += $(EHS_TARGET_PATH)/Components/dtv
VPATH+=: $(EHS_TARGET_PATH)
