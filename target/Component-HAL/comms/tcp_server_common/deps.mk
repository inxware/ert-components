#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Sun 17 Apr 23:35:18 BST 2022


console_server.h : target_types.h target_tcp.h

console_server.$(OBJ) : target_types.h target_tcp.h console_server.h  globals.h hal-api.h

target_console.$(OBJ) : target.h globals.h messages.h hal_string.h 