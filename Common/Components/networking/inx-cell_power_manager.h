/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

#ifndef INXWARE_cell_power_manager
#define INXWARE_cell_power_manager
#include "inx-component.h"

/*****************************************************
 * Cellular power saving - PSM / eDRX / RAI request and grant
 *****************************************************/
#define INXWARE_FB_ID_cell_power_manager 0x0322
#define INXWARE_FB_NAME_cell_power_manager "cell_power_manager"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(cell_power_manager); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(cell_power_manager); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(cell_power_manager); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(cell_power_manager_do_apply); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(cell_power_manager_do_read); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(cell_power_manager)

#endif /* INXWARE_cell_power_manager */
