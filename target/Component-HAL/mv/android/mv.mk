# Android Camera2 NDK MV HAL
# camera.c: Camera2 NDK capture (YUV_420_888 → RGB)
# mv.c:     frame lifecycle and CPU image operations

OBJECTS += mv.$(OBJ)
OBJECTS += camera.$(OBJ)

# Camera2 NDK — NdkCamera*.h, available from Android API 24+
LIB += camera2ndk
# Media NDK — AImageReader, AImage
LIB += mediandk
