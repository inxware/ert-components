#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Sun 17 Apr 23:35:17 BST 2022


target_types.h : ../base_full/base_types.h

target_file.$(OBJ) : dirent.h sys/types.h  hal-api.h 

target_main.$(OBJ) : target.h console_server.h  ehs_main.h hal-api.h

target_math.$(OBJ) : target.h

target_process.$(OBJ) : target.h hal-api.h target_process.h