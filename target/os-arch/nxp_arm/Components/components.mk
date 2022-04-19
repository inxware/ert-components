#
# Makefile fragment to build the target-specific code for EHS.

# Called by ../target.mk
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
#  VPATH - where to look for source code


include $(EHS_TARGET_OS_HW_PATH)/Components/deps.mk

VPATH+=: $(EHS_TARGET_OS_HW_PATH)/Components
