#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Sun 17 Apr 23:35:17 BST 2022


stddefs.h : stlite.h

targetgfx_init.h : target.h

target_viewport.h : graphics.h

targetgfx_init.$(OBJ) : targetgfx_init.h  hal_viewport.h 

target_viewport.$(OBJ) :  hal_string.h hal_viewport.h hal_process.h widget.h graphics.h hal-api.h  keypress.h