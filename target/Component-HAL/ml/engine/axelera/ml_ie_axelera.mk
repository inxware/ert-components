# Axelera Voyager inference engine backend.
# Wraps the axruntime C API for Axelera Metis NPU accelerators.
#
# SDK install path: /opt/axelera/runtime-1.6.0-1/
# Header:  include/axruntime/axruntime.h
# Library: lib/libaxruntime.so  (-laxruntime)

EHS_TARGET_ML_IE_AXELERA_PATH=$(EHS_TARGET_ML_ENGINE_PATH)/axelera
VPATH    += $(EHS_TARGET_ML_IE_AXELERA_PATH)
INC_DIRS += $(EHS_TARGET_ML_IE_AXELERA_PATH)
CXX_INC_DIRS += /opt/axelera/runtime-1.6.0-1/include

# libaxruntime.so is loaded at runtime via dlopen (not linked at build time) so
# the binary can also run machines without the Axelera runtime instaled

LIB += dl

CPPFLAGS += -std=c++17

OBJECTS  += ert_hal_axelera.$(OBJ)

DEFS += EHS_ML_HWACCEL_SUPPORT_AXELERA
DEFS += EHS_ML_HARDWARE_ACCELERATION=EHS_ML_HWACCEL_AXELERA
