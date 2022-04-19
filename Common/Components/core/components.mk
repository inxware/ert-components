#
# Makefile fragment to build the generic component code for EHS.

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

include $(EHS_COMMON_COMPONENTS_PATH)/core/deps.mk
OBJECTS += fs_dir_list1.$(OBJ) fs_dir_create_remove.$(OBJ)
OBJECTS += event_counter1.$(OBJ) SFCBarGroup1.$(OBJ) SFCBarGroup1Event.$(OBJ) arraystring1.$(OBJ) buffer.$(OBJ) calc_i.$(OBJ) const1.$(OBJ) convertor.$(OBJ) core_components.$(OBJ) 
OBJECTS += demux.$(OBJ) file.$(OBJ) fileFunctionsUtils.$(OBJ) ehs_if.$(OBJ) logic.$(OBJ)  mux.$(OBJ) operator1.$(OBJ) sfc.$(OBJ) state.$(OBJ) stringfn.$(OBJ) 
OBJECTS += time_clock.$(OBJ) trigger.$(OBJ) system_exec.$(OBJ) rtinfo.$(OBJ) wall_clock.$(OBJ) appinfo.$(OBJ)
ifdef EHS_DEVMAN_SUPPORT
OBJECTS +=  appget.$(OBJ) 
endif


VPATH+=: $(EHS_COMMON_COMPONENTS_PATH)/core


