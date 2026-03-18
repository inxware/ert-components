# TensorRT ML framework — NVIDIA Jetson (aarch64).
#
# Requires:
#   - TensorRT 8.x installed under /usr/include/aarch64-linux-gnu and
#     /usr/lib/aarch64-linux-gnu (standard JetPack layout).
#   - CUDA toolkit headers under /usr/local/cuda/include.
#
# The framework .cpp file uses the NvInfer C++ API (header-only C API wrappers
# are not available for TRT 8.x).  Link order matters: nvinfer must come before
# cudart so the CUDA runtime symbols resolve correctly.

EHS_TARGET_ML_FRAMEWORK_TENSORRT_PATH=$(EHS_TARGET_ML_FRAMEWORK_PATH)/tensorrt
VPATH += $(EHS_TARGET_ML_FRAMEWORK_TENSORRT_PATH)
INC_DIRS += $(EHS_TARGET_ML_FRAMEWORK_TENSORRT_PATH)

OBJECTS += ert_hal_tensorrt.$(OBJ)

CPPFLAGS += -std=c++17

# TensorRT and CUDA headers installed from the NVIDIA Jetson OTA apt repo.
# libnvinfer-dev:arm64 installs to /usr/include/aarch64-linux-gnu/ — the
# standard multiarch cross-compilation path, safe for arm64 cross-builds.
# TensorRT headers (Jetson OTA repo → /usr/include/aarch64-linux-gnu/)
CXX_INC_DIRS += /usr/include/aarch64-linux-gnu
# CUDA arm64 runtime headers — extracted from cuda-cudart-dev-12-6:arm64 deb
# during Docker image build (see Dockerfile).  Provides cuda_runtime_api.h and
# crt/host_defines.h for the aarch64 target.
CXX_INC_DIRS += /usr/local/cuda/targets/aarch64-linux/include

LIB += nvinfer
LIB += nvinfer_plugin
LIB += cudart

DEFS += EHS_ML_HWACCEL_SUPPORT_NVIDIA
