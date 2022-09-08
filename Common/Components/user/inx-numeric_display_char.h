/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

#ifndef INXWARE_ndc
#define INXWARE_ndc
#include "inx-component.h"

/*****************************************************
 * numeric_display_char
 *****************************************************/

#define INXWARE_FB_NAME_ndc "ndc"
#define INXWARE_FB_ID_ndc 0x340B

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ndc); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ndc); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(ndc); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ndc_set); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ndc)

#endif /* INXWARE_ndc */
