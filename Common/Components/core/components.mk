#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

#
# Makefile fragment to build the generic component code for EHS.

# Called by ../components.mk
#

# @author: inx limited
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


