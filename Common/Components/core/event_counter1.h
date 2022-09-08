/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * EventCounter1.h
 *
 * function prototypes for EventCounter1.c
 *
 * @author: inx limited
 *
 */

#ifndef EHS_EVENT_COUNTER1_H
#define EHS_EVENT_COUNTER1_H
#include "ehs_fb_types.h"


/******************************************************************************/
/* Declare versatile event counter */
#define EHS_FB_NAME_EventCounterVersatile1 "EventCounterVersatile1"
#define EHS_FB_ID_EventCounterVersatile1 0xD5F3

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventCounterVersatile1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventCounterVersatile1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventCounterVersatile1_Reset);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventCounterVersatile1_Count);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventCounterVersatile1_Decrement);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventCounterVersatile1_Enable);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventCounterVersatile1_Disable);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventCounterVersatile1)
#endif /* EHS_EVENT_COUNTER_H */
