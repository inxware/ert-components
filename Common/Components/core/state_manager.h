/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/
/**
 * state_manager.h
 *
 * function prototypes for state_manager.c
 *
 * @author: inx limited
 *
 */

#ifndef EHS_STATE_MANAGER_H
#define EHS_STATE_MANAGER_H

/**
 * Context for a state manager block.
 */
typedef struct
{
    void (*onStateChanged)(void* self);
    
    EhsFunctionInstanceDataType* pFIdata;
    ehs_sint32 nInitalState;
    ehs_sint32 nState;
    ehs_bool bEnabled;

} EhsStateManagerType;

/******************************************************************************/
/* Declare State */
#define EHS_FB_NAME_state_manager "state_manager"
#define EHS_FB_ID_state_manager 0x4639

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(state_manager);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(state_manager);

EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(state_manager_enabled);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(state_manager)


#endif /* EHS_STATE_MANAGER_H */


