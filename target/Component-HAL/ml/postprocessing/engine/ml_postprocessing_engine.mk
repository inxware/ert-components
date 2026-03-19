
ifeq ($(EHS_ML_HARDWARE_ACCELERATION),hailo)
include $(EHS_TARGET_ML_ENGINE_POSTPROCESSING_PATH)/hailo/ml_postprocessing_engine_hailo.mk
endif

ifeq ($(EHS_ML_HARDWARE_ACCELERATION),nvidia)
include $(EHS_TARGET_ML_ENGINE_POSTPROCESSING_PATH)/tensorrt/ml_postprocessing_engine_tensorrt.mk
endif