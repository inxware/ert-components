#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Tue Jun  3 14:26:18 GMTDT 2008


ehs_windows.h : resource.h

target_file.h : 

target_time.h : target_types.h

targetos_init.h : target.h

target_file.$(OBJ) :  hal_file.h

target_math.$(OBJ) : target.h

target_process.$(OBJ) : hal_process.h

target_time.$(OBJ) : hal_time.h 

targetos_init.$(OBJ) : targetos_init.h   hal_process.h