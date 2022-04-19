/**
 * time_clock.h
 *
 * function prototypes for wall_clock.c
 *
 * @author: inx
 * @version: $Revision: 584 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx Ltd, 2011. All rights reserved.
 */

#ifndef EHS_WALL_CLOCK_H
#define EHS_WALL_CLOCK_H

/******************************************************************************/
/* Declare State */
//#define EHS_WALL_CLOCK_NAME "wall_clock"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(wall_clock);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(wall_clock);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(wall_clock_get);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(wall_clock_set);
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(wall_clock)
#endif /* EHS_TIME_CLOCK_H */


