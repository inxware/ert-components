/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * EventCounter.h
 *
 * function prototypes for EventCounter.c
 *
 * @author: inx limited
 *
 */

#ifndef EHS_EVENT_COUNTER_H
#define EHS_EVENT_COUNTER_H
#include "ehs_fb_types.h"

/**
 * Event counter controller
 */
typedef struct
{
    ehs_bool bEventCounterRunning;
    ehs_sint32 nThreshold;		// wraparound value from object params. Nb no analysis performed for choice of 16 or 32 bit counter.
    ehs_sint32 nCount;			// actual count at any time.
} structEventCounterObj;



/******************************************************************************/
/* Declare versatile event counter */
#define EHS_FB_NAME_EventCounterVersatile "EventCounterVersatile"
#define EHS_FB_ID_EventCounterVersatile 0x7203

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(EventCounterVersatile);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(EventCounterVersatile);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventCounterVersatile_Reset);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventCounterVersatile_Start);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventCounterVersatile_Count);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventCounterVersatile_Decrement);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(EventCounterVersatile_Stop);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(EventCounterVersatile)
#endif /* EHS_EVENT_COUNTER_H */
