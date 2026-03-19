# TFLite inference engine backend.
# CPU-based runtime for Linux/RTOS targets.  Also serves as the fallback
# engine when a hardware accelerator backend is not compiled in.

EHS_TARGET_ML_IE_TFLITE_PATH=$(EHS_TARGET_ML_ENGINE_PATH)/tflite
VPATH    += $(EHS_TARGET_ML_IE_TFLITE_PATH)
INC_DIRS += $(EHS_TARGET_ML_IE_TFLITE_PATH)

OBJECTS += ert_hal_tflite.$(OBJ)
OBJECTS += ert_hal_tflite_meta.$(OBJ)
LIB     += tensorflowlite_c
DEFS    += EHS_ML_HWACCEL_SUPPORT_TFLITE
