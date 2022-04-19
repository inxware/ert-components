#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Sun 17 Apr 23:35:18 BST 2022


libgst_dll.h : graphics.h

targetvideo_init.h : target.h

target_dtv.$(OBJ) : hal_dtv.h hal_string.h messages.h ehs_types.h hal_viewport.h widget.h widget.h hal-api.h dtv_pvr_play2.h target_os_dtv.h

targetvideo_init.$(OBJ) : targetvideo_init.h hal_viewport.h graphics.h