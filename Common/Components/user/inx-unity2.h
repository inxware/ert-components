/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

#ifndef INXWARE_unity2
#define INXWARE_unity2
#include "inx-component.h"

/*****************************************************
 * unity
 *****************************************************/

#define INXWARE_FB_NAME_unity2 "unity2"
#define INXWARE_FB_ID_unity2 0x6F1D

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(unity2); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(unity2); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(unity2); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(unity2_create); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(unity2_destroy);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(unity2_setmedia);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(unity2_stop);
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(unity2)

#endif /* INXWARE_unity2 */
