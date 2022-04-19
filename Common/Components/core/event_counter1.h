/**
 * EventCounter1.h
 *
 * function prototypes for EventCounter1.c
 *
 * @author: ADS
 * @version: $Revision: 4649 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef EHS_EVENT_COUNTER1_H
#define EHS_EVENT_COUNTER1_H
#include "ehs_fb_types.h"


/******************************************************************************/
/* Declare versatile event counter */
#define EHS_FB_EVENT_COUNTER_VERSATILE1_NAME "EventCounterVersatile1"
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
