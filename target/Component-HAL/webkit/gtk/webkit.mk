#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#
# Makefile fragment to build the graphics/video-specific code for EHS.

# Called by ../../platform/<platform-type>
#

# @author: inx limited


# include sourcecode from this dir in build
include $(EHS_TARGET_WEBKIT_PATH)/deps.mk
VPATH+=: $(EHS_TARGET_WEBKIT_PATH)
INC_DIRS += $(EHS_TARGET_WEBKIT_PATH)

OBJECTS += target_webkit.$(OBJ) 
