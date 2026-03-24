#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
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

ifdef EHS_COMPONENTS_SYSTEMEXEC_SUPPORT
ifneq ($(EHS_COMPONENTS_SYSTEMEXEC_SUPPORT),none)
	DEFS += EHS_COMPONENTS_SYSTEMEXEC_SUPPORT
	OBJECTS += system_exec.$(OBJ)
endif
endif

#These are depricated, but should be removed completely now (hopeflly sfc is not the new state machine?)
OBJECTS += SFCBarGroup1.$(OBJ) SFCBarGroup1Event.$(OBJ) sfc.$(OBJ)

OBJECTS += event_counter1.$(OBJ)  arraystring1.$(OBJ) buffer.$(OBJ) calc_i.$(OBJ) const1.$(OBJ) convertor.$(OBJ) core_components.$(OBJ) 
OBJECTS += demux.$(OBJ) ehs_if.$(OBJ) logic.$(OBJ)  mux.$(OBJ) operator1.$(OBJ)  state_condition.$(OBJ) state_debug.$(OBJ) state_manager.$(OBJ) stringfn.$(OBJ) 
OBJECTS += time_clock.$(OBJ) trigger.$(OBJ) rtinfo.$(OBJ) wall_clock.$(OBJ) appinfo.$(OBJ) inx-elapsed_timer.$(OBJ)
OBJECTS += inx-rng.$(OBJ)

# Fine grained GNU dependency filtering
# This is to avoid havint to ser DEFs in target config.mk files and should stay here
# perhaps with the double negative removed. It does change the function block existance.
ifdef EHS_EXCLUDE_XML_PARSER=
DEFS += EHS_EXCLUDE_XML_PARSER==1
endif

# Broad GNU depency management.
# Components moved from user/ directory
ifndef EHS_SKIP_GNULIBRARIES
ifneq ($(EHS_SKIP_GNULIBRARIES),none)
	OBJECTS += inx-json_stream.$(OBJ)
	ifndef EHS_EXCLUDE_XML_PARSER
	ifneq ($(EHS_EXCLUDE_XML_PARSER),none)
		OBJECTS += inx-xml_stream.$(OBJ)
	endif
	endif
endif
endif

# This should probably be stubbed as it is part of core, but we will trip apps that use it for now.
ifdef EHS_DEVMAN_SUPPORT
ifeq ($(EHS_DEVMAN_SUPPORT),http)
  	OBJECTS +=  appget.$(OBJ) 
endif
endif



# JSON parser components
OBJECTS += inx-json_parser_int.$(OBJ)
OBJECTS += inx-json_parser_bool.$(OBJ)
OBJECTS += inx-json_parser_real.$(OBJ)
OBJECTS += inx-json_parser_str.$(OBJ)

# CGI to JSON converter
OBJECTS += inx-cgi2json.$(OBJ)

# Indexed multiplexer/demultiplexer components
OBJECTS += inx-indexed_mux_int.$(OBJ)
OBJECTS += inx-indexed_mux_str.$(OBJ)
OBJECTS += inx-indexed_mux_bool.$(OBJ)
OBJECTS += inx-indexed_mux_real.$(OBJ)
OBJECTS += inx-indexed_demux_int.$(OBJ)
OBJECTS += inx-indexed_demux_bool.$(OBJ)
OBJECTS += inx-indexed_demux_real.$(OBJ)
OBJECTS += inx-indexed_demux_str.$(OBJ)

VPATH+=: $(EHS_COMMON_COMPONENTS_PATH)/core


