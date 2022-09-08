#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#
#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 


libgst_dll.h : graphics.h

targetvideo_init.h : target.h

target_dtv.$(OBJ) : hal_dtv.h hal_string.h messages.h ehs_types.h hal_viewport.h widget.h widget.h hal-api.h dtv_pvr_play2.h target_os_dtv.h

targetvideo_init.$(OBJ) : targetvideo_init.h hal_viewport.h graphics.h
