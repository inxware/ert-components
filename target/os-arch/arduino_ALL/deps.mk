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


target_types.h : ../base_small/base_types.h

# target_file.$(OBJ) : dirent.h sys/types.h  hal-api.h 

target_main.$(OBJ) : target_arduino_main.hpp

target_math.$(OBJ) : target.h

# target_process.$(OBJ) : target.h hal-api.h target_process.h
