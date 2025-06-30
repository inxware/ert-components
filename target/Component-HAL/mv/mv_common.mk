# configure machine vision (mv) component hal

EHS_TARGET_MV_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/mv/$(EHS_MV_SUPPORT)
DEFS += EHS_MV_SUPPORT

# include sourcecode path of this dir in build
VPATH+=$(EHS_TARGET_COMPONENT_HAL_PATH)/mv
VPATH+=$(EHS_TARGET_MV_PATH)
INC_DIRS+=$(EHS_TARGET_MV_PATH)

OBJECTS+=mv_common.$(OBJ)

include $(EHS_TARGET_MV_PATH)/mv.mk