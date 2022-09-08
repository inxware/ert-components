#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 


target_types.h : ../base_full/base_types.h

target_gpio.h : target_types.h

target_gpio.$(OBJ) : target.h target_gpio.h target_config.h

target_file.$(OBJ) :  hal_file.h hal_string.h hal.h  dirent.h sys/types.h

target_main.$(OBJ) : target.h console_server.h  ehs_main.h hal_process.h hal.h

target_math.$(OBJ) : target.h

target_process.$(OBJ) : target.h hal.h hal_process.h target_process.h
