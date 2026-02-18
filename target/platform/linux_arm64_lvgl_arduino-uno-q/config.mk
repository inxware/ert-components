#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# @author: inx limited

# Usage Description
# This is for Raspberry Pis - it is aimed at being used with inxware demo

# Add Demo features like OpenCV and EdgeML support to arm64 debian 11 build

include ./target/platform/linux_arm64_debian13_base/config.mk

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

SYSTEM_VARIANT=RASPBERRYPI

# Machine Leaning & Machine Vision Support
EHS_ML_SUPPORT=yes
EHS_ML_FRAMEWORK_IMAGE_SUPPORT=tensorflow-lite
EHS_ML_HARDWARE_ACCELERATION=none

EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET=yes
EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET=yes
EHS_MV_SUPPORT=opencv
#EHS_MV_SUPPORT=stubbed

#Some C++ mixed buid test flags to remove when confirmed
#Need to set  EHS_MV_SUPPORT=stubbed to test this too
#EHS_CPPMV_SUPPORT_TEST=yes
#DEFS+=EHS_CPPMV_SUPPORT_TEST

#TODO We need to fix the build when this isn't set.
EHS_DEVMAN_SUPPORT=http

# use libcamera on top of opencv if supported
EHS_USE_LIBCAMERA=yes
# enables a floating 'debug_cam_X' window with camera preview
#DEFS+=EHS_OPENCV_FRAMEGRAB_DEBUG_PREVIEW=1

#SDL_FULLSCREEN=yes

################################### END OF TOOLBOX CONFIGURATION ###################################################
################################################################################################################
# Define any OS install scripts that should be run on first install
################################################################################################################
