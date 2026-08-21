/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

#ifndef INXWARE_sim_manager
#define INXWARE_sim_manager
#include "inx-component.h"

/*****************************************************
 * SIM state, identity and runtime SIM-type switching
 *****************************************************/
#define INXWARE_FB_ID_sim_manager 0xD29B
#define INXWARE_FB_NAME_sim_manager "sim_manager"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(sim_manager); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(sim_manager); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(sim_manager); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(sim_manager_do_read); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(sim_manager_do_select_type); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(sim_manager_do_enter_pin); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(sim_manager)

#endif /* INXWARE_sim_manager */
