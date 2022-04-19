

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