#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# HW independent linux build environment parameters 

ifndef EHS_COMMS_API_SUPPORT
	export  EHS_COMMS_API_SUPPORT=bsdsockets
	DEFS += EHS_COMMS_API_SUPPORT_BSDSOCKETS
    DEFS += EHS_COMMS_API_SUPPORT
endif

#target types are always the same for all linux so just use one file
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/
VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/

OBJECTS += target_file.$(OBJ)
OBJECTS += target_process.$(OBJ) 
OBJECTS += target_main.$(OBJ)
OBJECTS += target_math.$(OBJ) 

# We nearly always need this for GNU targets
LIB+=z



