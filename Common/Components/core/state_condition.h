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
 * function prototypes for state_condition.c
 *
 * @author: inx limited
 *
 */

#ifndef EHS_STATE_CONDITION_H
#define EHS_STATE_CONDITION_H

/******************************************************************************/
/* Declare State */
#define EHS_FB_NAME_state_condition "state_condition"
#define EHS_FB_ID_state_condition 0xFDEE

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(state_condition);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(state_condition);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(state_condition_doCondition);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(state_condition)


#endif /* EHS_STATE_CONDITION_H */


