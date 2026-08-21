/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

#ifndef INXWARE_softsim_prov
#define INXWARE_softsim_prov
#include "inx-component.h"

/*****************************************************
 * Provision a SoftSIM subscription profile
 *****************************************************/
#define INXWARE_FB_ID_softsim_prov 0x870B
#define INXWARE_FB_NAME_softsim_prov "softsim_prov"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(softsim_prov); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(softsim_prov); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(softsim_prov); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(softsim_prov_do_provision); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(softsim_prov_do_check); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(softsim_prov)

#endif /* INXWARE_softsim_prov */
