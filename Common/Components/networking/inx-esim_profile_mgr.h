/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

#ifndef INXWARE_esim_profile_mgr
#define INXWARE_esim_profile_mgr
#include "inx-component.h"

/*****************************************************
 * Local eSIM profile lifecycle (GSMA ES10c)
 *****************************************************/
#define INXWARE_FB_ID_esim_profile_mgr 0x6004
#define INXWARE_FB_NAME_esim_profile_mgr "esim_profile_mgr"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(esim_profile_mgr); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(esim_profile_mgr); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(esim_profile_mgr); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(esim_profile_mgr_do_list_profiles); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(esim_profile_mgr_do_get_profile); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(esim_profile_mgr_do_enable_profile); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(esim_profile_mgr_do_disable_profile); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(esim_profile_mgr_do_delete_profile); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(esim_profile_mgr)

#endif /* INXWARE_esim_profile_mgr */
