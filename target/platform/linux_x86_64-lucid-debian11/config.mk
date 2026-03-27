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

include ./target/platform/linux_x86_64_clang_lvgl_debian11/config.mk

###!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
### NOTE: This config gets a lot of junk that is probably not needed (on a normal system)
### from ../ert-contrib-middleware/target_libs/x86_64-linux-gnu-clang10_clang10_base/target_packages/cslib/
###  Ultimately this build should not be using the clang10_clang10_base contrib middleware anyway.
###!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

# EHS_DEBUGALL=true

# Machine Leaning & Machine Vision Support
EHS_ML_SUPPORT=yes
EHS_ML_FRAMEWORK_IMAGE_SUPPORT=tensorflow-lite

EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET=yes
EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET=yes

#This needs a different alphamat library for debian 11 EHS_MV_SUPPORT=opencv

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
EHS_APPLAND_INST_DEPLOY_NAME=PC-Debian-11-64bit
EHS_APPLAND_INST_OS_NAME=linux
