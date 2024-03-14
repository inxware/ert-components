#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

#
# Makefile fragment to build the GUI component code for EHS.

# Called by ../components.mk
#

# @author: inx limited
#
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

# Set this for any intra-code conditional builds
DEFS+=EHS_GUI_SUPPORT

INC_DIRS += $(EHS_COMMON_TOOLKIT_GUI_PATH)
VPATH += $(EHS_COMMON_TOOLKIT_GUI_PATH)

include $(EHS_COMMON_TOOLKIT_GUI_PATH)/deps.mk

# add extended ui widgets deps
INC_DIRS += $(EHS_COMMON_TOOLKIT_GUI_PATH)/ui_widgets
VPATH += $(EHS_COMMON_TOOLKIT_GUI_PATH)/ui_widgets
include $(EHS_COMMON_TOOLKIT_GUI_PATH)/ui_widgets/deps.mk

OBJECTS += inx-gui_patch.$(OBJ)
OBJECTS += inx-gui_image_file.$(OBJ)
OBJECTS += inx-gui_widget.$(OBJ)

OBJECTS += gui_components.$(OBJ) guiparams.$(OBJ) 
OBJECTS += guiviewport.$(OBJ)

OBJECTS += inx-ui_spinner.$(OBJ)
OBJECTS += inx-ui_chart.$(OBJ)
OBJECTS += inx-ui_list.$(OBJ)
