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


include $(EHS_COMMON_COMPONENTS_PATH)/AV/deps.mk

INC_DIRS+=$(EHS_COMMON_COMPONENTS_PATH)/AV
VPATH+= $(EHS_COMMON_COMPONENTS_PATH)/AV
# @todo dtv_pcr_play2 is used for audio and video so we don't ifndef this 

ifdef EHS_VIDEO_SUPPORT
OBJECTS += dtv_pvr_play2.$(OBJ)
OBJECTS += dtv_components.$(OBJ)
endif
