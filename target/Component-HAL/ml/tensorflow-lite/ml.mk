# configure machine learning (ml) component hal

VPATH+=$(EHS_TARGET_ML_PATH)/utils
INC_DIRS+=$(EHS_TARGET_ML_PATH)/utils

#$(error $(EHS_COMPONENT_SUPPORT_INCLUDE)/tesnorflow )

OBJECTS+=tflite_utils.$(OBJ)
OBJECTS+=tflite_nms.$(OBJ)
OBJECTS+=tflite_yolo.$(OBJ) 
OBJECTS+=tflite_model.$(OBJ) 
OBJECTS+=tensorflow-lite_ml.$(OBJ)

LIB+=tensorflowlite_c