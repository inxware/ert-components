/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

#ifndef INXWARE_rtc
#define INXWARE_rtc
#include "inx-component.h"

/*****************************************************
 * rtc
 *****************************************************/

#define INXWARE_FB_NAME_rtc "rtc"
#define INXWARE_FB_ID_rtc 0x32C7

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(rtc); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(rtc); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(rtc); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(rtc_set); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(rtc_read); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(rtc)

#endif /* INXWARE_rtc */
