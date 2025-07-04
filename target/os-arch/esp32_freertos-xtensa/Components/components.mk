#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#
# Makefile fragment to build the target-specific code for EHS.
# Called by ../target.mk
# @author: inx limited

# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code



#include $(EHS_TARGET_OS_HW_PATH)/Components/dtv/components.mk

include $(EHS_TARGET_OS_HW_PATH)/Components/deps.mk

# @todo moved to gtk directory: OBJECTS += keypress.$(OBJ) gtk_components.$(OBJ)

VPATH+=: $(EHS_TARGET_OS_HW_PATH)/Components
