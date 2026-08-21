/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license - see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

#ifndef INXWARE_cell_modem_manager
#define INXWARE_cell_modem_manager
#include "inx-component.h"

/*****************************************************
 * Connect to a cellular network (LTE-M / NB-IoT) and report modem identity
 *****************************************************/
#define INXWARE_FB_ID_cell_modem_manager 0x6AD8
#define INXWARE_FB_NAME_cell_modem_manager "cell_modem_manager"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(cell_modem_manager); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(cell_modem_manager); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(cell_modem_manager); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(cell_modem_manager_do_connect); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(cell_modem_manager_do_disconnect); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(cell_modem_manager_internal_monitor); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(cell_modem_manager_do_set); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(cell_modem_manager_do_read_info); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(cell_modem_manager)

#endif /* INXWARE_cell_modem_manager */
