#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Sun 17 Apr 23:35:18 BST 2022


targetcomms_init.h : target.h

target_tcp.h : target.h 

targetcomms_init.$(OBJ) : targetcomms_init.h hal_console.h 

target_tcp.$(OBJ) : target_tcp.h hal_string.h console_server.h