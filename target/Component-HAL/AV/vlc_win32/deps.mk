#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Tue Jun  3 14:26:20 GMTDT 2008


include $(EHS_TARGET_AV_PATH)/../vlc_common/deps.mk

libvlc_dll.$(OBJ) :  hal_string.h hal_time.h libvlc_dll.h

targetvideo_init.$(OBJ) : targetvideo_init.h  hal_viewport.h
