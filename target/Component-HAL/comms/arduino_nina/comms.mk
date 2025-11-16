#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#
#
# Makefile fragment to build the comms-specific code for EHS.

# Called by ../../platform/<platform-type>
#

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_COMMS_API_PATH - path to the current directory (set by platform makefile)


include $(EHS_TARGET_COMMS_API_PATH)/deps.mk
include $(EHS_TARGET_COMMS_API_PATH)/Components/components.mk

VPATH+=: $(EHS_TARGET_COMMS_API_PATH)
INC_DIRS += $(EHS_TARGET_COMMS_API_PATH)


OBJECTS += target_tcp.$(OBJ)
OBJECTS += targetcomms_init.$(OBJ)