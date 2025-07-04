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
 * function definitions for state_condition.c
 *
 * @author: inx limited
 *
 */
#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

#include "ehs_fb_types.h"
#include "state_debug.h"

#include "hal-api.h" /* Required for logging */

/**
 * Context for a state condition block.
 */
typedef struct
{
    ehs_sint32 nState;

} EhsStateDebugType;



/******************************************************************************/
/* Define state function block */

EHS_FB_FUNCTIONS_START(state_debug)
EHS_FB_FUNCTION_ENTRY("Run", 0x01, state_debug_Run)
EHS_FB_FUNCTIONS_END

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(state_debug)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(state_debug));
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsStateDebugType);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(state_debug)
{

    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(state_debug));

    EHSH_LOG_ERROR("=== STATE_DEBUG INIT ===");

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(state_debug_Run)
{
//    EhsStateDebugType* pStateDebug = (EhsStateDebugType*)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(state_debug_Run));

    EHSH_LOG_ERROR("=== STATE_DEBUG RUN ===");

    return;
}

