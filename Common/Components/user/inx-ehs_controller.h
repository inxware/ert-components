/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#ifndef INXWARE_ehs_controller
#define INXWARE_ehs_controller
#include "inx-component.h"

/*****************************************************
 * Controls the current EHS window
 *****************************************************/
#define INXWARE_FB_NAME_ehs_controller "ehs_controller"
#define INXWARE_FB_ID_ehs_controller 0xE08E

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ehs_controller); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ehs_controller); // this relates to C-file init function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ehs_controller_exit_ehs); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ehs_controller_move_to_foreground); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ehs_controller_move_to_background); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ehs_controller)

#endif /* INXWARE_ehs_controller */
