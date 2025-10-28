# configure machine vision (mv) component hal

EHS_TARGET_MV_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/mv/$(EHS_MV_SUPPORT)
DEFS += EHS_MV_SUPPORT

# include sourcecode path of this dir in build
VPATH+=$(EHS_TARGET_COMPONENT_HAL_PATH)/mv
VPATH+=$(EHS_TARGET_MV_PATH)
INC_DIRS+=$(EHS_TARGET_MV_PATH)

OBJECTS+=mv_common.$(OBJ)

ifdef EHS_CPPMV_SUPPORT_TEST
#This is just a test for building C++ Machine vision API access into ert-components (like we should be doing)
#We need to move the current OpenCV C binding code from ert-components to ert-commponents so we can extend it without having to build contrib middleware in weirdd ways.
OBJECTS+=mv_hello.$(OBJ)
endif

include $(EHS_TARGET_MV_PATH)/mv.mk