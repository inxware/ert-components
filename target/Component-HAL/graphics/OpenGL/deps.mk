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


stddefs.h : stlite.h

targetgfx_init.h : target.h

target_viewport.h : graphics.h

targetgfx_init.$(OBJ) : targetgfx_init.h  hal_viewport.h 

target_viewport.$(OBJ) :  hal_string.h hal_viewport.h hal_process.h widget.h graphics.h hal.h hal_mem.h hal_time.h  
