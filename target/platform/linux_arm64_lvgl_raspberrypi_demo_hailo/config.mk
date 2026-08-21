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

include ./target/platform/linux_arm64_lvgl_raspberrypi_demo/config.mk

# Machine Leaning & Machine Vision Support
EHS_ML_SUPPORT=hailo
SDL_FULLSCREEN=no
EHS_DEFAULT_APP=hardware-demos/rpi-hailo

################################### END OF TOOLBOX CONFIGURATION ###################################################
################################################################################################################
# Define any OS install scripts that should be run on first install
################################################################################################################
