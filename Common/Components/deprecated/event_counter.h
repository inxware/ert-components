/**
 * EventCounter.h
 *
 * function prototypes for EventCounter.c
 *
 * @author: ADS
 * @version: $Revision: 794 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
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
#define EHS_FB_EVENT_COUNTER_VERSATILE_NAME "EventCounterVersatile"
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
