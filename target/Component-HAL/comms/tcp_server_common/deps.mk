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


console_server.h : target_types.h target_tcp.h

console_server.$(OBJ) : target_types.h target_tcp.h console_server.h  globals.h hal-api.h

target_console.$(OBJ) : target.h globals.h messages.h hal_string.h 
