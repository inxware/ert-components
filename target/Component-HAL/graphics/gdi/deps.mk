#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Tue Jun  3 14:26:20 GMTDT 2008


#libvlc_dll.h : vlc/libvlc.h vlc_threads.h

targetgfx_init.h : target.h

#libvlc_dll.$(OBJ) :  hal_string.h hal_time.h libvlc_dll.h

#target_dtv.$(OBJ) : hal_dtv.h hal_string.h  libvlc_dll.h hal_viewport.h

target_viewport.$(OBJ) : hal_viewport.h hal_process.h widget.h hal_mem.h  hal_string.h

targetgfx_init.$(OBJ) : targetgfx_init.h  hal_viewport.h