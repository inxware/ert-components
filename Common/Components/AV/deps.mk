#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Sun 17 Apr 23:35:18 BST 2022


dtv_diag.h :  

dtv_pvr_play2.h : 

dtv_components.$(OBJ) :  dtv_remote2.h dtv_diag.h dtv_pvr_play2.h

dtv_pvr_play2.$(OBJ) : target.h target_os_dtv.h dtv_pvr_play2.h     graphics.h guiparams.h hal-api.h hal_dtv.h