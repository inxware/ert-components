#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# Makefile fragment to build the graphics/video-specific code for EHS.

# Called by ../../platform/<platform-type>



include $(EHS_TARGET_AV_PATH)/../ffmpeg_common/video.mk

# TODO - add ffmpeg/lvgl dependencies
# export INC_DIRS +=  
# LIB+=


OBJECTS += target_os_dtv.$(OBJ)
