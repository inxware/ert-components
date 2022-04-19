#
# Makefile fragment to build the playerManager component code for EHS.

# Called by ../components.mk
#

# @author: Patrick Beaumont
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


include $(EHS_COMMON_COMPONENTS_PATH)/media/deps.mk

OBJECTS += playManager.$(OBJ)
OBJECTS += media_components.$(OBJ)
ifeq ($(EHS_NATIVE_DEBIAN_BUILD),yes)
INC_DIRS += /usr/include/libxml2
endif


VPATH+= $(EHS_COMMON_COMPONENTS_PATH)/media

#This is for playManager
ifdef EHS_ANDROID
LIB+=:libxml2.a 
else
LIB+=xml2 
endif
