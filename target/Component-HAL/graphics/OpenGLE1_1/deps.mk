#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Thu Feb  9 15:32:01 GMT 2012


stddefs.h : stlite.h

targetgfx_init.h : target.h

target_viewport.h : graphics.h

targetgfx_init.$(OBJ) : targetgfx_init.h  hal_viewport.h 

target_viewport.$(OBJ) :  hal_string.h hal_viewport.h hal_process.h widget.h graphics.h hal.h hal_mem.h hal_time.h  