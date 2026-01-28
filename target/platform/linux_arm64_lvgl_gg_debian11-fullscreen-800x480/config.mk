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



include ./target/platform/linux_arm64_lvgl_gg_debian11/config.mk

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# this means the mouse points are incorrect?
DEFS += SDL_FULLSCREEN 

# appland deployment config
EHS_APPLAND_INST_SUPPORT=yes
EHS_APPLAND_INST_DEPLOY_NAME=RaspberryPi-Debian-11-64bit-Fullscreen-800x400
EHS_APPLAND_INST_OS_NAME=raspberrypi

################################### END OF TOOLBOX CONFIGURATION ###################################################
################################################################################################################
# Define any OS install scripts that should be run on first install
################################################################################################################
