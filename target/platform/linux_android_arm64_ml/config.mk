#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_android_arm64_ml
# @author: inx limited

# Standalone arm64-v8a Android target with ML and MV support.
# Builds a native APK (no Unity dependency).
# TFLite with full XNNPACK (NEON) acceleration on arm64.
# MV is stubbed — frames arrive via Camera2 NDK JNI bridge or test files.
# When an Android Camera2 NDK HAL is implemented, switch EHS_MV_SUPPORT=android.
#
# Prerequisites:
#   1. Build contrib libs for arm64:
#        cd ../ert-contrib-middleware/inx_build_scripts
#        bash build-android-ehs.sh --abi arm64-v8a --api 30
#      This produces target_libs/arm64-linux-android/build/lib/liblitert_c.a
#      and libc++_shared.so alongside it.
#   2. make prepdeps  (first time / after switching target)
#   3. make all_docker

include ./target/platform/linux_android_arm64/config.mk

#################################################################################################################
# Architecture, OS, Toolchain
#################################################################################################################

# Android 11 = API 30. Camera2 NDK available from API 24; XNNPACK arm64 full support.
EHS_ANDROID_API=30
EHS_ANDROID_INSTALL_VERSION=11.0

# arm64-v8a ABI — drives jniLibs path in APK build scripts and Unity export.
EHS_ANDROID_NATIVE_ABI=arm64

#################################################################################################################
# Packaging
#################################################################################################################

EHS_PACKAGER_TYPE=apk
EHS_ANDROID_PACKAGE_SIGNING_PATH=ehs.google.play

include ./target/devman-configs/inx-systems.com.mk
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=yes
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=yes

#################################################################################################################
# Machine Learning — LiteRT (TFLite C API), static library.
#
# liblitert_c.a for arm64-v8a is in:
#   ../ert-contrib-middleware/target_libs/arm64-linux-android/build/lib/
# which maps via COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=arm64-linux-android
# (set in the base linux_android_arm64/config.mk).
#
# XNNPACK is fully enabled on arm64 — all NEON operations run at full speed.
# NNAPI is deprecated since Android 13 and is not used.
#################################################################################################################
EHS_ML_SUPPORT=yes
EHS_ML_IE_IMAGE_SUPPORT=tensorflow-lite
EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET=yes
EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET=yes

#################################################################################################################
# Machine Vision — Android Camera2 NDK HAL.
#
# EHS_MV_SUPPORT=android builds target/Component-HAL/mv/android/camera.c +
# mv.c.  Camera2 NDK (NdkCamera.h, API 24+) is used for capture.
# YUV_420_888 frames are converted to packed RGB on each grab.
#
# Camera ID passed to EhsCameraStart:
#   ""/"back"  → back-facing camera (default)
#   "front"    → front-facing camera
#   "0","1",…  → camera by enumerate index
#
# Permission: android.permission.CAMERA must be granted.
# Grant once for testing: adb shell pm grant com.inx.ehs android.permission.CAMERA
#################################################################################################################
EHS_MV_SUPPORT=android

################################### END OF CONFIGURATION ###################################################
