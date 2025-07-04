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


ehs_windows.h : resource.h

target_file.h : 

target_time.h : target_types.h

targetos_init.h : target.h

target_file.$(OBJ) :  hal_file.h

target_math.$(OBJ) : target.h

target_process.$(OBJ) : hal_process.h

target_time.$(OBJ) : hal_time.h 

targetos_init.$(OBJ) : targetos_init.h   hal_process.h
