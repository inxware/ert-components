/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

#ifndef INXWARE_cell_status
#define INXWARE_cell_status
#include "inx-component.h"

/*****************************************************
 * Cellular signal quality, link evaluation and data usage
 *****************************************************/
#define INXWARE_FB_ID_cell_status 0x0A36
#define INXWARE_FB_NAME_cell_status "cell_status"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(cell_status); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(cell_status); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(cell_status); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(cell_status_do_read); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(cell_status_do_reset_counters); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(cell_status)

#endif /* INXWARE_cell_status */
