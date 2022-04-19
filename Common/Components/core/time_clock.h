/**
 * time_clock.h
 *
 * function prototypes for time_clock.c
 *
 * @author: inx limited
 * @version: $Revision: 584 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef EHS_TIME_CLOCK_H
#define EHS_TIME_CLOCK_H

/******************************************************************************/
/* Declare State */
#define EHS_TIME_CLOCK_NAME "time_clock"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(time_clock);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(time_clock);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(time_clock_start);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(time_clock_stop);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(time_clock_tick);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(time_clock)


#endif /* EHS_TIME_CLOCK_H */


