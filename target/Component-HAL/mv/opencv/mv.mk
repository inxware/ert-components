
OBJECTS+=mv.$(OBJ)
OBJECTS+=camera.$(OBJ)

LIB+=opencv_wrapper

ifdef EHS_USE_LIBCAMERA
export EHS_USE_LIBCAMERA
DEFS+=EHS_USE_LIBCAMERA=1
LIB+=lccv
endif