#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#
#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 


libgst_dll.h : graphics.h

targetvideo_init.h : target.h

target_dtv.$(OBJ) : hal_dtv.h hal_string.h   hal_viewport.h widget.h widget.h hal-api.h dtv_pvr_play2.h target_os_dtv.h

targetvideo_init.$(OBJ) : ../gst10_common/targetvideo_init.h  hal_viewport.h graphics.h
