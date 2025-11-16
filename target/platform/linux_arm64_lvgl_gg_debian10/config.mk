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



include ./target/platform/linux_arm64_gtk_gst_gg_debian10/config.mk

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################


################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this: 
#too2023 - we need to add this to everyting until we decide if we will have a debug and non-debug kernel. (I guess we ultimately wan the latter to mimise sizes.)
EHS_DEBUGALL=true

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################


################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################


################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################


# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub, lvgl}, depending support for your target   #
# Set this to match one of the graphics types in EHS/target/graphics
EHS_GUI_SUPPORT=lvgl
DEFS += SDL_FULLSCREEN

# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc,ffmpeg}, depending support for your target                                                   #
EHS_AV_SUPPORT=ffmpeg

DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=none
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=no

include ./target/devman-configs/devman.inxware.io.mk

################################### END OF TOOLBOX CONFIGURATION ###################################################
################################################################################################################
# Define any OS install scripts that should be run on first install
################################################################################################################
