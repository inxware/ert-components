# Inference engine layer — selector + dispatch.
#
# This file does two things:
#
# 1. Includes the dispatch layer (ml_inf_engine.c) which routes Create /
#    Destroy / SetInputData / RunInference calls to the correct backend based
#    on ctx->hw_accel.
#
# 2. Selects which backend(s) to compile in based on the platform config
#    variables EHS_ML_IE_IMAGE_SUPPORT and EHS_ML_HARDWARE_ACCELERATION.
#
# Included by ml_common.mk when EHS_ML_SUPPORT=yes.

EHS_TARGET_ML_ENGINE_PATH=$(EHS_TARGET_ML_ROOT_PATH)/engine

# ── Dispatch layer ────────────────────────────────────────────────────────────
VPATH    += $(EHS_TARGET_ML_ENGINE_PATH)
INC_DIRS += $(EHS_TARGET_ML_ENGINE_PATH)
OBJECTS  += $(EHS_TARGET_ML_ENGINE_PATH)/ml_inf_engine.$(OBJ)

# ── CPU / software inference engine ──────────────────────────────────────────
# EHS_ML_IE_IMAGE_SUPPORT selects the primary CPU-based runtime.
# This also serves as the TFLite fallback path when a hardware accelerator
# backend is selected but not compiled in (see ml_inf_engine.c).

ifeq ($(EHS_ML_IE_IMAGE_SUPPORT),tensorflow-lite)
include $(EHS_TARGET_ML_ENGINE_PATH)/tflite/ml_ie_tflite.mk

else ifeq ($(EHS_ML_IE_IMAGE_SUPPORT),tensorflow-lite-micro)
include $(EHS_TARGET_ML_ENGINE_PATH)/tflite-micro/ml_ie_tflite_micro.mk

else ifeq ($(EHS_ML_IE_IMAGE_SUPPORT),none)
# No CPU-based runtime — hardware engine only (e.g. TensorRT on Jetson).
# EHS_ML_HWACCEL_SUPPORT_TFLITE is intentionally NOT defined so that
# ml_inf_engine.c falls through to EHS_ML_NOT_SUPPORTED if the HW engine
# is also absent.

endif

# ── Hardware accelerator engine ───────────────────────────────────────────────
# EHS_ML_HARDWARE_ACCELERATION selects the hardware-specific runtime.
# Multiple accelerators could in principle be compiled in simultaneously;
# runtime selection is handled by EhsML_HWAccel_supported() in ml_common.c.

ifeq ($(EHS_ML_HARDWARE_ACCELERATION),nvidia)
include $(EHS_TARGET_ML_ENGINE_PATH)/tensorrt/ml_ie_tensorrt.mk
DEFS += EHS_ML_HWACCEL_SUPPORT_NVIDIA
endif

ifeq ($(EHS_ML_HARDWARE_ACCELERATION),hailo)
include $(EHS_TARGET_ML_ENGINE_PATH)/hailo/ml_ie_hailo.mk
DEFS += EHS_ML_HWACCEL_SUPPORT_HAILO
endif

# ── Audio / text engines (future) ────────────────────────────────────────────
# EHS_ML_IE_TEXT_SUPPORT and EHS_ML_IE_AUDIO_SUPPORT are
# reserved for future engine integrations (LLMs, audio classifiers, etc.).
