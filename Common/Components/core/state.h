/**
 * state.h
 *
 * function prototypes for state.c
 *
 * @author: MDD
 * @version: $Revision: 278 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef EHS_STATE_H
#define EHS_STATE_H

/******************************************************************************/
/* Declare State */
#define EHS_STATE_NAME "state"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(state);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(state);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(state);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(state)


#endif /* EHS_STATE_H */


