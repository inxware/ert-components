#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
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

DEFS += JSMN_STRICT
DEFS += JSMN_PARENT_LINKS

VPATH+= $(EHS_COMMON_HAL_PATH)/json/contrib 
