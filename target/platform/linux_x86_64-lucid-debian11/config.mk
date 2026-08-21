#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_x86_64-lucid-debian11
# @author: inx limited

# this target gets included in inxware lucid tools

#include ./target/platform/linux_x86_64_clang_lvgl_debian11/config.mk
#include ./target/platform/linux_x86_64_clang_lvgl_gg_debian12/config.mk
# Parent must match this target's OWN Docker image, which is
# inxware/inx-debian11-clang11-lvgl -- Debian 11, glibc 2.31.
#
# The include had been moved to linux_x86_64_clang_lvgl_gg_debian13, which does
# not exist (the _gg variant was never created for debian13). That broke the
# target outright: EHS_OS and EHS_ARCH came out empty and the build looked for
# target/os-arch/-/Components/deps.mk.
#
# Two obvious-looking repairs both fail to LINK, for the same reason -- a
# debian13 or debian12 parent brings a contrib built against a newer glibc than
# this target's debian11 image provides:
#   _gg_debian12  -> x86_64-linux-gnu-clang14_debian12_base
#   _tflite_debian13 -> x86_64-linux-gnu-clang19_debian13_base
#                       undefined __isoc23_sscanf / __isoc23_strtol (glibc 2.38+)
#
# So it goes back to the debian11 parent, which uses
# x86_64-linux-gnu-clang10_clang10_base -- and that tree does carry
# libtensorflowlite, which this target needs for EHS_ML_SUPPORT below.
#
# To move this to debian13, change Dockerimagename as well as this include, and
# create linux_x86_64_clang_lvgl_gg_debian13 if greengrass is wanted.
include ./target/platform/linux_x86_64_clang_lvgl_debian11/config.mk

#EHS_DEBUGALL=true

# Machine Leaning & Machine Vision Support
EHS_ML_SUPPORT=yes
EHS_ML_FRAMEWORK_IMAGE_SUPPORT=tensorflow-lite

EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET=yes
EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET=yes

EHS_MV_SUPPORT=opencv
# enables a floating 'debug_cam_X' window with camera preview
#todo remove this
#DEFS+=EHS_OPENCV_DEBUG_PREVIEW=yes
#DEFS+=EHS_OPENCV_CROP_DEBUG_PREVIEW=yes

#don't start this on desktops
EHS_AUTO_START=--no-autostart

# this forces eRT to always launch app in default directory when the program launches
EHS_DEFAULT_APP=systemapps/Home

# enable eRT1 support
ERT_SODL_VERSION=1

include ./target/devman-configs/devman.inxware.io.mk
DEVMAN_SERVER_PROTOCOL=http

# appland deployment config
EHS_APPLAND_INST_SUPPORT=yes
EHS_APPLAND_INST_DEPLOY_NAME=PC-Debian-12-64bit
EHS_APPLAND_INST_OS_NAME=linux
