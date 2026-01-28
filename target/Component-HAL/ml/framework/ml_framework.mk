# #/BEGIN Machine Learning Framework Path Setup #
# VPATH+=$(EHS_TARGET_ML_FRAMEWORK_PATH)
# INC_DIRS+=$(EHS_TARGET_ML_FRAMEWORK_PATH)
# # ENDED Machine Learning Framework Path Setup/#

#TODO define what we mean by a framework  we have a mixture of applications and runtime/engines here  these are two different things

ifeq ($(EHS_ML_FRAMEWORK_IMAGE_SUPPORT),tensorflow-lite)
# Image related machine learning framework support
# Tensorflow Lite is always included as fallback and CPU-only computation for OS target
include $(EHS_TARGET_ML_FRAMEWORK_PATH)/tensorflow-lite/ml_fw_tflite.mk
else ifeq ($(EHS_ML_FRAMEWORK_IMAGE_SUPPORT),tensorflow-lite-micro)
# Tensorflow Lite Micro is always included as fallback and CPU-only computation for resource-constrained embedded target
#TODO
$(error Tensorflow Lite Micro is not supported yet)
endif

#TODO THESE ARE NOT FRAMEWORK RELATED PARAMTERS AND SHOLD BE ELSEWHERE. NOT ALL THESE WILLBE COMPATIBLE WITH ALL FRAMEWORKS BUT WILL BE SHARED AND COME FROM DIFFERENT PLACES.
ifeq ($(EHS_ML_FRAMEWORK_TEXT_SUPPORT),none)
# Text related machine learning framework support
#TODO
endif

ifeq ($(EHS_ML_FRAMEWORK_AUDIO_SUPPORT),none)
# Audio related machine learning framework support
#TODO
endif

# Check EHS_ML_HARDWARE_ACCELERATION to include additional framework
ifeq ($(EHS_ML_HARDWARE_ACCELERATION),hailo)
include $(EHS_TARGET_ML_FRAMEWORK_PATH)/hailo/ml_fw_hailo.mk
DEFS += EHS_ML_HARDWARE_ACCELERATION=EHS_ML_HWACCEL_HAILO
endif #EHS_ML_HARDWARE_ACCELERATION
