#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Fri Mar  4 03:43:20 GMT 2011


targetcomms_init.h : target.h

target_tcp.h : target.h 

targetcomms_init.$(OBJ) : targetcomms_init.h hal_console.h

target_console-NOT_USED-sameasBSD-but-has-some-gui-console-flagging-to-save.$(OBJ) : target.h    hal_mem.h hal_string.h

target_tcp.$(OBJ) : target_tcp.h console_server.h