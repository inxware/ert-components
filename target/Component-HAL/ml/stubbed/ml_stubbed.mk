# configure machine learning (ml) component hal

# Add stubbed source to VPATH
VPATH+=$(EHS_TARGET_ML_ROOT_PATH)/stubbed
INC_DIRS+=$(EHS_TARGET_ML_ROOT_PATH)/stubbed

OBJECTS+=stubbed_ml.$(OBJ) 