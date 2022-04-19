#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Thu Feb  9 15:32:01 GMT 2012


targetvideo_init.h : target.h

target_dtv.$(OBJ) : hal_dtv.h hal_string.h   hal_viewport.h widget.h widget.h hal.h hal_time.h hal_mem.h dtv_pvr_play2.h target_os_dtv.h

targetvideo_init.$(OBJ) : targetvideo_init.h  hal_viewport.h graphics.h