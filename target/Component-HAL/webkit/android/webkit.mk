#
# Makefile fragment to build the graphics/video-specific code for EHS.

# Called by ../../platform/<platform-type>
#

# @author: Pierre Drezet
# @version: $Revision: 43 $
# @date: $Date: 2014-10-30  $
# 
# Copyright (c) inx Ltd, 2007. All rights reserved.
#
#


# include sourcecode from this dir in build
include $(EHS_TARGET_WEBKIT_PATH)/deps.mk
VPATH+=: $(EHS_TARGET_WEBKIT_PATH)
INC_DIRS += $(EHS_TARGET_WEBKIT_PATH)

OBJECTS += target_webkit.$(OBJ) 
