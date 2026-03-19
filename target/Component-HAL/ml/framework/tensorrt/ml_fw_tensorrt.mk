# TensorRT ML framework — NVIDIA Jetson (aarch64).
#
# Build environment: linux_arm64_lvgl_jetson_nano_ngc Docker image, based on
# nvcr.io/nvidia/jetpack-linux-aarch64-crosscompile-x86:6.1 which pre-installs
# TensorRT and CUDA 12.6 headers/stubs at the standard multiarch paths.
#
# The framework .cpp file uses the NvInfer C++ API.  Link order matters:
# nvinfer must come before cudart so the CUDA runtime symbols resolve correctly.

EHS_TARGET_ML_FRAMEWORK_TENSORRT_PATH=$(EHS_TARGET_ML_FRAMEWORK_PATH)/tensorrt
VPATH += $(EHS_TARGET_ML_FRAMEWORK_TENSORRT_PATH)
INC_DIRS += $(EHS_TARGET_ML_FRAMEWORK_TENSORRT_PATH)

OBJECTS += ert_hal_tensorrt.$(OBJ)

CPPFLAGS += -std=c++17

# TensorRT headers — pre-installed by NGC base image
CXX_INC_DIRS += /usr/include/aarch64-linux-gnu
# CUDA arm64 runtime headers — pre-installed by NGC base image
# /usr/local/cuda is a symlink to /usr/local/cuda-12.6 (created in Dockerfile)
CXX_INC_DIRS += /usr/local/cuda/targets/aarch64-linux/include

LIB += nvinfer
LIB += nvinfer_plugin
LIB += cudart

DEFS += EHS_ML_HWACCEL_SUPPORT_NVIDIA
