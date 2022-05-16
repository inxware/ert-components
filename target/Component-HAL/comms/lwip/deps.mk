#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Fri Nov  9 15:13:35 GMT 2018


targetcomms_init.h : target.h

target_tcp.h : target.h 

targetcomms_init.$(OBJ) : targetcomms_init.h hal_console.h 

target_tcp.$(OBJ) : target_tcp.h hal_string.h