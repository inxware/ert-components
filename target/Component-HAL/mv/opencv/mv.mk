
OBJECTS+=mv.$(OBJ)
OBJECTS+=camera.$(OBJ)

ifeq ($(EHS_DEBIAN_VERSION),13)

CPPFLAGS += -std=c++17

OBJECTS+=opencv_wrapper.$(OBJ)

CXX_INC_DIRS += /usr/include/opencv4

LIB+=opencv_stitching
LIB+=opencv_alphamat
LIB+=opencv_aruco
LIB+=opencv_bgsegm
LIB+=opencv_bioinspired
LIB+=opencv_ccalib
LIB+=opencv_cvv
LIB+=opencv_dnn_objdetect
LIB+=opencv_dnn_superres
LIB+=opencv_dpm
LIB+=opencv_face
LIB+=opencv_freetype
LIB+=opencv_fuzzy
LIB+=opencv_hdf
LIB+=opencv_hfs
LIB+=opencv_img_hash
LIB+=opencv_intensity_transform
LIB+=opencv_line_descriptor
LIB+=opencv_mcc
LIB+=opencv_quality
LIB+=opencv_rapid
LIB+=opencv_reg
LIB+=opencv_rgbd
LIB+=opencv_saliency
LIB+=opencv_shape
LIB+=opencv_signal
LIB+=opencv_stereo
LIB+=opencv_structured_light
LIB+=opencv_phase_unwrapping
LIB+=opencv_superres
LIB+=opencv_optflow
LIB+=opencv_surface_matching
LIB+=opencv_tracking
LIB+=opencv_highgui
LIB+=opencv_datasets
LIB+=opencv_text
LIB+=opencv_plot
LIB+=opencv_ml
LIB+=opencv_videostab
LIB+=opencv_videoio
LIB+=opencv_viz
LIB+=opencv_wechat_qrcode
LIB+=opencv_ximgproc
LIB+=opencv_video
LIB+=opencv_xobjdetect
LIB+=opencv_objdetect
LIB+=opencv_calib3d
LIB+=opencv_imgcodecs
LIB+=opencv_features2d
LIB+=opencv_dnn
LIB+=opencv_flann
LIB+=opencv_xphoto
LIB+=opencv_photo
LIB+=opencv_imgproc
LIB+=opencv_core
else
LIB+=opencv_wrapper
endif

ifdef EHS_USE_LIBCAMERA
export EHS_USE_LIBCAMERA
DEFS+=EHS_USE_LIBCAMERA=1
ifeq ($(EHS_DEBIAN_VERSION),13)
# CPPFLAGS += -DCV_LIBCAMERA_SUPPORT
DEFS+=CV_LIBCAMERA_SUPPORT
LIB+=camera-base
LIB+=camera
CXX_INC_DIRS += /usr/include/libcamera
CXX_INC_DIRS += /usr/local/include

OBJECTS += lccv.$(OBJ)
OBJECTS += libcamera_app_options.$(OBJ)
OBJECTS += libcamera_app.$(OBJ)
else
LIB+=lccv
endif
#Note we might want to do this more gernerally if there are any CPP files compiled, but this detection is working properly yet.
LIB+=stdc++
endif