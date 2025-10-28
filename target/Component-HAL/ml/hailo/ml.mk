# configure machine learning (ml) component hal

OBJECTS+=hailo_ml.$(OBJ) 
LIB+=hailort

VPATH+=$(EHS_TARGET_ML_PATH)/../tensorflow-lite/utils
VPATH+=$(EHS_TARGET_ML_PATH)/../tensorflow-lite
INC_DIRS+=$(EHS_TARGET_ML_PATH)/../tensorflow-lite/utils
INC_DIRS+=$(EHS_TARGET_ML_PATH)/../tensorflow-lite
OBJECTS+=tflite_utils.$(OBJ)
OBJECTS+=tflite_nms.$(OBJ)
OBJECTS+=tflite_yolo.$(OBJ) 
OBJECTS+=tflite_model.$(OBJ) 
LIB+=tensorflowlite_c