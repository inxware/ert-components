/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/
/**
 * state.h
 *
 * function prototypes for state.c
 *
 * @author: inx limited
 *
 */

#ifndef EHS_STATE_H
#define EHS_STATE_H

/******************************************************************************/
/* Declare State */
#define EHS_FB_NAME_state "state"
#define EHS_FB_ID_state 0xCEA8

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(state);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(state);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(state);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(state)


#endif /* EHS_STATE_H */


