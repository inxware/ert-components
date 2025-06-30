# configure machine learning (ml) component hal

EHS_TARGET_ML_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/ml/$(EHS_ML_SUPPORT)
DEFS += EHS_ML_SUPPORT

# include sourcecode path of this dir in build
VPATH+=$(EHS_TARGET_ML_PATH)
INC_DIRS+=$(EHS_TARGET_ML_PATH)

include $(EHS_TARGET_ML_PATH)/ml.mk