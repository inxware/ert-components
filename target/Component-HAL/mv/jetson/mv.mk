# Jetson MV HAL — GStreamer + NVMM camera capture, CPU/VIC image operations.

OBJECTS += mv.$(OBJ)
OBJECTS += camera.$(OBJ)

CPPFLAGS += -std=c++17

# Shared jetson utilities visible to both mv/jetson and ml/tensorrt
INC_DIRS += $(EHS_TARGET_COMPONENT_HAL_PATH)/jetson

# GStreamer core and app plugin (appsink/appsrc)
LIB += gstreamer-1.0
LIB += gstapp-1.0
LIB += gobject-2.0
LIB += glib-2.0

INC_DIRS += /usr/include/gstreamer-1.0
INC_DIRS += /usr/include/glib-2.0
INC_DIRS += /usr/lib/aarch64-linux-gnu/glib-2.0/include

LIB += stdc++
