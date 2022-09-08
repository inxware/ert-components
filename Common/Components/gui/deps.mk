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


gui_image_file.h : 

guileds.h : 

guiparams.h :  graphics.h 

guipatch.h : 

guitextbox2.h : 

guiviewport.h : 

gui_components.$(OBJ) :  gui_image_file.h guitextbox2.h guipatch.h gui_image_file.h guiviewport.h guileds.h

gui_image_file.$(OBJ) : target.h gui_image_file.h guiparams.h   widget.h hal-api.h

guileds.$(OBJ) : target.h guileds.h    hal_string.h 

guiparams.$(OBJ) : target.h guiparams.h   hal_string.h font.h

guipatch.$(OBJ) : target.h guipatch.h guiparams.h   widget.h   hal-api.h

guitextbox2.$(OBJ) : target.h guitextbox2.h guiparams.h  widget.h font.h   hal-api.h

guiviewport.$(OBJ) : target.h guiviewport.h guiparams.h   widget.h widget_viewport.h   hal-api.h
