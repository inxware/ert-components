#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 


target_leds.h : target_types.h

target_textdisplay.h : target_types.h

target_leds.$(OBJ) : target_textdisplay.h usblcd.h widgets.h usblcd_util.h target_config.h

target_textdisplay.$(OBJ) : usblcd.h widgets.h usblcd_util.h target_textdisplay.h hal_process.h target_config.h
