#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Sun 17 Apr 23:35:17 BST 2022


target_leds.h : target_types.h

target_textdisplay.h : target_types.h

target_leds.$(OBJ) : target_textdisplay.h usblcd.h widgets.h usblcd_util.h target_config.h

target_textdisplay.$(OBJ) : usblcd.h widgets.h usblcd_util.h target_textdisplay.h hal_process.h target_config.h