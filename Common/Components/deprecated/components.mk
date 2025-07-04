#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#
# Makefile fragment to build the DTV component code for EHS.

# Called by ../components.mk
#

# @author: inx limited
#
# Predefined variables
#  OBJ - File extension for object files
# Expected variables
# OBJECTS - list of object files added by this makefile
# VPATH - where to look for source code


include $(EHS_COMMON_COMPONENTS_PATH)/deprecated/deps.mk


ifdef EHS_GUI_SUPPORT
ifneq ($(EHS_GUI_SUPPORT),none)
# disable depricated gui widgets for mode B
ifneq ($(EHS_RENDER_MODE),B)
# OBJECTS += guiimage.$(OBJ) # compile error
OBJECTS += guitextbox_orig.$(OBJ) 
OBJECTS += guitextbox.$(OBJ) 
# OBJECTS += guiimage1.$(OBJ) # compile error
OBJECTS += guitextbox1.$(OBJ)
endif
endif
endif
OBJECTS += event_counter.$(OBJ)
OBJECTS += deprecated_components.$(OBJ) 
OBJECTS += operator.$(OBJ)
OBJECTS += arraystring.$(OBJ)
OBJECTS += const.$(OBJ)

ifdef EHS_VIDEO_SUPPORT
# OBJECTS += dtv_pvr_play.$(OBJ) # compile error
# OBJECTS += dtv_pvr_list.$(OBJ) # compile error
# OBJECTS += calc_i.$(OBJ) # already included in core.mk
# OBJECTS += dtv_remote.$(OBJ) # already included in dtv.mk
endif
VPATH+=: $(EHS_COMMON_TOOLKIT_DEPRECATE_PATH)
