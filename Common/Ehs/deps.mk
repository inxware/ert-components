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


callback_queue.h : ehs_fb_types.h

CBUtils.h : functable.h types.h

messages.h : hal_console.h hal_logger.h

callback_queue.$(OBJ) : callback_queue.h globals.h

CBUtils.$(OBJ) : CBUtils.h target.h
