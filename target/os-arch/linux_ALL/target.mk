#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# HW independent linux build environment parameters 


#target types are always the same for all linux so just use one file
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/platform
KERNEL_HEADERS_RELPATH =$(KERNEL_VERSION)/headers/include/

VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/

# todo we might want a common linux bin to be created
# moves to specific linux: OBJECTS += targetos_init.$(OBJ) 

OBJECTS += target_time.$(OBJ)
OBJECTS += targetos_init.$(OBJ)


