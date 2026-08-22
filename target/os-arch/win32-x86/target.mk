#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# Makefile fragment to build the OS/compiler-specific code for EHS.

# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_OS_HW_PATH - path to the current directory (set by platform makefile)

# include sourcecode from this dir in build
include $(EHS_TARGET_OS_HW_PATH)/deps.mk
INC_DIRS += $(EHS_TARGET_OS_HW_PATH)
VPATH+=: $(EHS_TARGET_OS_HW_PATH)

#include sourcecode from components dir in build
include $(EHS_TARGET_OS_HW_PATH)/Components/deps.mk
INC_DIRS += $(EHS_TARGET_OS_HW_PATH)/Components
VPATH+=: $(EHS_TARGET_OS_HW_PATH)/Components

ehs_windows.res : ehs_windows.rc

OBJECTS += ehs_windows.res
#OBJECTS += target_file.$(OBJ)
#OBJECTS += target_main.$(OBJ)
#OBJECTS += target_math.$(OBJ)
#OBJECTS += target_process.$(OBJ) 
#OBJECTS += target_time.$(OBJ)
#OBJECTS += targetos_init.$(OBJ)
#OBJECTS += keypress.$(OBJ)
OBJECTS += win_components.$(OBJ)
OBJECTS += target_display.$(OBJ)
OBJECTS += target_sys_stat.$(OBJ)


