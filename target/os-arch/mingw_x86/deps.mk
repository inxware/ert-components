#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Fri Mar  4 03:43:20 GMT 2011


target_file.h : target_types.h

target_leds.h : target_types.h

targetos_init.h : target.h

target_textdisplay.h : target_types.h

target_time.h : target_types.h

target_file.$(OBJ) :  hal_file.h

target_leds.$(OBJ) : target_textdisplay.h

target_main.$(OBJ) : target.h console_server.h  ehs_main.h hal_process.h

target_math.$(OBJ) : target.h

targetos_init.$(OBJ) : targetos_init.h   hal_process.h console_server.h hal_mem.h hal.h 

target_process.$(OBJ) : target.h hal_process.h target_process.h

target_textdisplay.$(OBJ) : target_textdisplay.h target_textdisplay.h hal_process.h

target_time.$(OBJ) : hal_time.h 