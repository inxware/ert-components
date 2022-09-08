#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

include $(EHS_COMMON_HAL_PATH)/json/contrib/deps.mk

OBJECTS+= yajl_alloc.$(OBJ)
OBJECTS+= yajl_buf.$(OBJ)
OBJECTS+= yajl.$(OBJ)
OBJECTS+= yajl_encode.$(OBJ)
OBJECTS+= yajl_gen.$(OBJ)
OBJECTS+= yajl_lex.$(OBJ)
OBJECTS+= yajl_parser.$(OBJ)
OBJECTS+= yajl_tree.$(OBJ)
OBJECTS+= yajl_version.$(OBJ)

VPATH+= $(EHS_COMMON_HAL_PATH)/json/contrib 

INC_DIRS+= $(EHS_COMMON_HAL_PATH)/json/contrib
INC_DIRS+= $(EHS_COMMON_HAL_PATH)/json/contrib/api
