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

#ifdef EHS_FILESYSTEM_SUPPORT

ifneq ($(EHS_FILESYSTEM_SUPPORT),none)
#ifneq ($(EHS_FILESYSTEM_SUPPORT),stubbed)
	OBJECTS += fs_dir_list1.$(OBJ) fs_dir_create_remove.$(OBJ) file.$(OBJ) fileFunctionsUtils.$(OBJ)
#endif
endif

#TODO Neither of below should be in the core toolbox
ifdef EHS_DEVMAN_SUPPORT
ifneq ($(EHS_DEVMAN_SUPPORT),none)
	OBJECTS +=  appget.$(OBJ) 
endif
endif

ifeq ($(EHS_SYSTEMEXEC_SUPPORT),none)
	DEFS += EHS_COMPONENTS_SYSTEMEXEC_SUPPORT__NONE
else
   OBJECTS += system_exec.$(OBJ)
endif

#These are depricated, but should be removed completely now (hopeflly sfc is not the new state machine?)
OBJECTS += SFCBarGroup1.$(OBJ) SFCBarGroup1Event.$(OBJ) sfc.$(OBJ)

OBJECTS += event_counter1.$(OBJ)  arraystring1.$(OBJ) buffer.$(OBJ) calc_i.$(OBJ) const1.$(OBJ) convertor.$(OBJ) core_components.$(OBJ) 
OBJECTS += demux.$(OBJ) ehs_if.$(OBJ) logic.$(OBJ)  mux.$(OBJ) operator1.$(OBJ)  state_condition.$(OBJ) state_debug.$(OBJ) state_manager.$(OBJ) stringfn.$(OBJ) 
OBJECTS += time_clock.$(OBJ) trigger.$(OBJ) rtinfo.$(OBJ) wall_clock.$(OBJ) appinfo.$(OBJ)
OBJECTS += inx-rng.$(OBJ)


VPATH+=: $(EHS_COMMON_COMPONENTS_PATH)/core


