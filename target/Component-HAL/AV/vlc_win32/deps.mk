#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#
#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 


include $(EHS_TARGET_AV_PATH)/../vlc_common/deps.mk

libvlc_dll.$(OBJ) :  hal_string.h hal_time.h libvlc_dll.h

targetvideo_init.$(OBJ) : targetvideo_init.h  hal_viewport.h
