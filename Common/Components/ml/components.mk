#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#
# Makefile fragment to build the ml component code for EHS.

# Called by ../components.mk
#

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

include $(EHS_COMMON_COMPONENTS_PATH)/ml/deps.mk
INC_PATH+= $(EHS_COMPONENT_SUPPORT_INCLUDE)/

# function blocks that need frames from the camera
ifdef EHS_MV_SUPPORT
OBJECTS += inx-ml_image_inference.$(OBJ)
endif

OBJECTS += inx-ml_svm_inference.$(OBJ)

OBJECTS += ml_components.$(OBJ)

VPATH+= $(EHS_COMMON_COMPONENTS_PATH)/ml
