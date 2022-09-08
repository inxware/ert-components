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


arraystring.h : 

const.h : 

dtv_pvr_list.h : 

dtv_pvr_play.h : 

dtv_remote1.h : 

dtv_remote.h : 

event_counter.h : 

guiimage1.h : 

guiimage.h : 

guitextbox1.h : 

guitextbox.h : 

guitextbox_orig.h : 

arraystring.$(OBJ) : arraystring.h  target.h  hal_mem.h hal_string.h

const.$(OBJ) : target.h  const.h  hal-api.h

deprecated_components.$(OBJ) :   guitextbox.h guitextbox1.h guitextbox_orig.h guiimage.h guiimage1.h dtv_remote.h dtv_remote1.h event_counter.h operator.h arraystring.h const.h dtv_pvr_play.h dtv_pvr_list.h calc_i.h

dtv_pvr_list.$(OBJ) : target.h dtv_pvr_list.h hal_dtv.h hal_string.h   hal-api.h

dtv_pvr_play.$(OBJ) : target.h dtv_pvr_play.h    graphics.h hal-api.h hal_dtv.h

event_counter.$(OBJ) :  event_counter.h  target.h hal-api.h

guiimage1.$(OBJ) : target.h guiimage1.h guiparams.h   widget.h hal-api.h

guiimage.$(OBJ) : target.h guiimage.h guiparams.h   widget.h hal-api.h

guitextbox1.$(OBJ) : target.h guitextbox1.h guiparams.h  widget.h font.h hal-api.h

guitextbox.$(OBJ) : target.h guitextbox.h guiparams.h  widget.h   hal_string.h hal-api.h

guitextbox_orig.$(OBJ) : target.h guitextbox_orig.h guiparams.h  widget.h  hal_string.h font.h

operator.$(OBJ) :  target.h operator.h  hal-api.h
