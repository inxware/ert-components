#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Fri Nov  9 15:13:35 GMT 2018


libgst_dll.h : graphics.h

targetvideo_init.h : target.h

target_dtv.$(OBJ) : hal_dtv.h hal_string.h   hal_viewport.h widget.h widget.h hal-api.h dtv_pvr_play2.h target_os_dtv.h

targetvideo_init.$(OBJ) : ../gst10_common/targetvideo_init.h  hal_viewport.h graphics.h