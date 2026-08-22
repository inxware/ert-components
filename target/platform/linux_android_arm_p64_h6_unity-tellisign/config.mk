#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_android_arm_p64_h6_unity-tellisign
# @author: inx limited
# This target siimply builds a standard linux version of EHS with android toolchain.
# It does not manage any events or resources such as graphics, so is unlikely to be useful
# other than to test compilation of standard code. It would run an exe using sysecec I expect.

include ./target/platform/linux_android_arm_unity-tellisign/config.mk


#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

EHS_ANDROID_INSTALL_VERSION=9.0

# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).
# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features
SYSTEM_VARIANT=pine64_h6

BUILD_WITH_ANDROID_SUPERVISOR=yes

#DEFS+=EHS_DEBUG_URL

################################################################################################################
# Machine Learning — LiteRT (TFLite C API), static library.
#
# liblitert_c.a is built by ert-contrib-middleware/inx_build_scripts/build-android-ehs.sh
# and linked statically into the EHS .so (no runtime .so bundling needed in the APK).
#
# Hardware acceleration: XNNPACK (CPU SIMD/NEON) built into LiteRT.
# NNAPI is deprecated since Android 13 and is not used.
# GPU delegate support is a planned future addition.
################################################################################################################
EHS_ML_SUPPORT=yes
EHS_ML_IE_IMAGE_SUPPORT=tensorflow-lite
EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET=yes
EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET=yes

# EHS_MV_SUPPORT enables the ml_image_inference function block (0xF512).
# The block registration in ml_components.c and the object file in
# Common/Components/ml/components.mk are both gated on ifdef EHS_MV_SUPPORT.
# Android uses the 'stubbed' implementation — no OpenCV dependency needed;
# image frames arrive from the Unity side via the JNI bridge.
EHS_MV_SUPPORT=stubbed

################################### END OF TOOLBOX CONFIGURATION ###################################################
