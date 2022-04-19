#
# Makefile fragment to build the GUI component code for EHS.

# Called by ../components.mk
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


DEFS+=EHS_GUI_SUPPORT
INC_DIRS+=: $(EHS_COMMON_TOOLKIT_GUI_PATH)
#INC_DIRS+=$(EHS_COMMON_TOOLKIT_PNG_PATH)
VPATH+=: $(EHS_COMMON_TOOLKIT_GUI_PATH)

include $(EHS_COMMON_TOOLKIT_GUI_PATH)/deps.mk


OBJECTS += gui_components.$(OBJ) guiparams.$(OBJ) guitextbox2.$(OBJ) guipatch.$(OBJ)
OBJECTS += gui_image_file.$(OBJ)
OBJECTS += guiviewport.$(OBJ)
