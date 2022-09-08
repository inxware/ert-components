/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/
/**
 * time_clock.h
 *
 * function prototypes for time_clock.c
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TIME_CLOCK_H
#define EHS_TIME_CLOCK_H

/******************************************************************************/
/* Declare State */

#define EHS_FB_NAME_time_clock "time_clock"
#define EHS_FB_ID_time_clock 0xEEFF

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(time_clock);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(time_clock);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(time_clock_start);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(time_clock_stop);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(time_clock_tick);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(time_clock)


#endif /* EHS_TIME_CLOCK_H */


