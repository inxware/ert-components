#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Tue Jul  1 11:30:53 GMTST 2008


stddefs.h : stlite.h

targetgfx_init.h : target.h

libEhsStapi.$(OBJ) : stlayer.h stblit.h stgxobj.h stvtg.h

target_dtv.$(OBJ) : hal_dtv.h hal_string.h  target.h

target_viewport.$(OBJ) : hal_string.h hal_viewport.h widget.h  hal_mem.h

targetgfx_init.$(OBJ) : targetgfx_init.h  hal_viewport.h 