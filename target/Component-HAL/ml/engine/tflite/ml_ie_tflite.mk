# LiteRT (TFLite) inference engine backend.
#
# On Linux targets the prebuilt shared libtensorflowlite_c.so is used
# (present in the platform's contrib target_libs build/ directory).
#
# On Android targets a static fat archive (liblitert_c.a) is used instead —
# the library is not a system component on Android and must be bundled.
# The archive is built from source by ert-contrib-middleware/inx_build_scripts/
# build-android-ehs.sh and contains TFLite + all its dependencies (XNNPACK,
# flatbuffers, abseil, ruy, …) merged into a single file.
#
# XNNPACK (CPU SIMD — NEON on ARM32/ARM64, SSE/AVX on x86) is enabled for all
# Android ABIs.  NNAPI was deprecated in Android 13 and is not used.

EHS_TARGET_ML_IE_TFLITE_PATH=$(EHS_TARGET_ML_ENGINE_PATH)/tflite
VPATH    += $(EHS_TARGET_ML_IE_TFLITE_PATH)
INC_DIRS += $(EHS_TARGET_ML_IE_TFLITE_PATH)

OBJECTS += ert_hal_tflite.$(OBJ)
OBJECTS += ert_hal_tflite_meta.$(OBJ)

DEFS    += EHS_ML_HWACCEL_SUPPORT_TFLITE

ifeq ($(EHS_ANDROID),yes)
# Static fat archive — linked via -l: so it is processed after object files
# (LNKFLAGS order), allowing GNU ld to resolve symbols that the .o files reference.
# The EHS_COMPONENT_SUPPORT_LIBS directory is already in LIB_DIRS via platform.mk.
LIB += :liblitert_c.a
# libc++_shared provides the C++ runtime on Android (system library, API 5.0+).
# TFLite's static objects reference C++ stdlib symbols so this must be linked.
LIB += c++_shared
else
# Linux: dynamic linking against the prebuilt shared library.
LIB += tensorflowlite_c
endif
