#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Tue Feb  7 20:01:15 GMT 2012


libvlc_dll.h : graphics.h

targetvideo_init.h : target.h

target_dtv.$(OBJ) : hal_dtv.h hal_string.h   libvlc_dll.h hal_viewport.h widget.h widget.h hal.h hal_time.h hal_mem.h dtv_pvr_play2.h

targetvideo_init.$(OBJ) : targetvideo_init.h  hal_viewport.h graphics.h libvlc_dll.h