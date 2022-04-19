#
# Makefile fragment to build the DTV component code for EHS.

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


include $(EHS_COMMON_COMPONENTS_PATH)/deprecated/deps.mk


ifdef EHS_GUI_SUPPORT
# OBJECTS += guiimage.$(OBJ) # compile error
OBJECTS += guitextbox_orig.$(OBJ) 
OBJECTS += guitextbox.$(OBJ) 
# OBJECTS += guiimage1.$(OBJ) # compile error
OBJECTS += guitextbox1.$(OBJ)
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