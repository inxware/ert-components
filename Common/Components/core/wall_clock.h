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
 * function prototypes for wall_clock.c
 *
 * @author: inx limited
 *
 */

#ifndef EHS_WALL_CLOCK_H
#define EHS_WALL_CLOCK_H

/******************************************************************************/
/* Declare State */

#define EHS_FB_NAME_wall_clock "wall_clock"
#define EHS_FB_ID_wall_clock 0xED30


EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(wall_clock);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(wall_clock);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(wall_clock_get);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(wall_clock_set);
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(wall_clock)
#endif /* EHS_TIME_CLOCK_H */


