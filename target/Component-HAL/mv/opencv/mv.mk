
OBJECTS+=mv.$(OBJ)
OBJECTS+=camera.$(OBJ)

LIB+=opencv_wrapper

ifdef EHS_USE_LIBCAMERA
export EHS_USE_LIBCAMERA
DEFS+=EHS_USE_LIBCAMERA=1
LIB+=lccv
#Note we might want to do this more gernerally if there are any CPP files compiled, but this detection is working properly yet.
LIB+=stdc++
endif