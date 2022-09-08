#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 


dtv_diag.h :  

dtv_pvr_play2.h : 

dtv_components.$(OBJ) : dtv_diag.h dtv_pvr_play2.h

dtv_pvr_play2.$(OBJ) : target.h target_os_dtv.h dtv_pvr_play2.h     graphics.h guiparams.h hal-api.h hal_dtv.h
