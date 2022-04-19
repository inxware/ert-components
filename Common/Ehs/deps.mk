#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Sun 17 Apr 23:35:17 BST 2022


callback_queue.h : ehs_fb_types.h

CBUtils.h : functable.h types.h

messages.h : hal_console.h hal_logger.h

callback_queue.$(OBJ) : callback_queue.h globals.h

CBUtils.$(OBJ) : CBUtils.h target.h