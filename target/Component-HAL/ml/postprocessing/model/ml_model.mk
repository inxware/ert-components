#/BEGIN Machine Learning Model Path Setup #
VPATH+=$(EHS_TARGET_ML_MODEL_PATH)
INC_DIRS+=$(EHS_TARGET_ML_MODEL_PATH)
# ENDED Machine Learning Model Path Setup/#

#/BEGIN Machine Learning Model Utilities #
EHS_TARGET_ML_MODEL_UTILS_PATH=$(EHS_TARGET_ML_MODEL_PATH)/ml_utils
include $(EHS_TARGET_ML_MODEL_UTILS_PATH)/ml_model_utils.mk
# ENDED Machine Learning Model Utilities/#

#/BEGIN Machine Learning Model Common #
OBJECTS += $(EHS_TARGET_ML_MODEL_PATH)/ml_model_common.$(OBJ)
# ENDED Machine Learning Model Common/#

#/BEGIN Machine Learning Model Support #
ifeq ($(EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET),yes)
DEFS += EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET
OBJECTS += $(EHS_TARGET_ML_MODEL_PATH)/yolov5_objdet.$(OBJ)
EHS_ML_MODEL_SUPPORT_INCLUDE_UTILS=yes
endif

ifeq ($(EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET),yes)
DEFS += EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET
OBJECTS += $(EHS_TARGET_ML_MODEL_PATH)/yolov8_objdet.$(OBJ)
EHS_ML_MODEL_SUPPORT_INCLUDE_UTILS=yes
endif

ifeq ($(EHS_ML_MODEL_SUPPORT_YOLOV8_POSE),yes)
DEFS += EHS_ML_MODEL_SUPPORT_YOLOV8_POSE
OBJECTS += $(EHS_TARGET_ML_MODEL_PATH)/yolov8_pose.$(OBJ)
EHS_ML_MODEL_SUPPORT_INCLUDE_UTILS=yes
endif


# ENDED Machine Learning Model Support/#