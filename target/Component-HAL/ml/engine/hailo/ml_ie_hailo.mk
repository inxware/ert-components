# Hailo inference engine backend.
# Wraps HailoRT SDK for Hailo-8 / Hailo-8L NPU accelerators.

EHS_TARGET_ML_IE_HAILO_PATH=$(EHS_TARGET_ML_ENGINE_PATH)/hailo
VPATH    += $(EHS_TARGET_ML_IE_HAILO_PATH)
INC_DIRS += $(EHS_TARGET_ML_IE_HAILO_PATH)
LIB      += hailort

OBJECTS  += ert_hal_hailo.$(OBJ)
