/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/
/**
 * state_condition.h
 *
 * function prototypes for state_debug.c
 *
 * @author: inx limited
 *
 */

#ifndef EHS_STATE_DEBUG_H
#define EHS_STATE_DEBUG_H

/******************************************************************************/
/* Declare State */
#define EHS_FB_NAME_state_debug "state_debug"
#define EHS_FB_ID_state_debug 0x37D5

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(state_debug);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(state_debug);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(state_debug_Run);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(state_debug)


#endif /* EHS_STATE_DEBUG_H */


