/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
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
#include "state_condition.h"
#include "state_manager.h"

#include "hal-api.h" /* Required for logging */

/**
 * Context for a state condition block.
 */
typedef struct
{
    ehs_sint32 nState;
    ehs_sint32 nNextState;

} EhsStateConditionType;



/******************************************************************************/
/* Define state function block */

EHS_FB_FUNCTIONS_START(state_condition)
EHS_FB_FUNCTION_ENTRY("doCondition", 0x01, state_condition_doCondition)
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
EHS_FB_IDENTIFY_FUNCTION(state_condition)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(state_condition));
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsStateConditionType);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(state_condition)
{
    EhsStateConditionType* pStateCondition = (EhsStateConditionType*)EHS_FB_INIT_CONTEXT;
    const char* pParams;
    ehs_sint32 nState = -1;
    ehs_sint32 nNextState = -1;

    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(state_condition));

    /* parse parameters to get inital state, from the state manager */
    pParams = EhsGetSint32FromString(&nState, EHS_FB_INIT_PARAMETERS);
    pStateCondition->nState = nState;
    pParams = EhsGetSint32FromString(&nNextState, pParams);
    pStateCondition->nNextState = nNextState;

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(state_condition_doCondition)
{
    EhsStateConditionType* pStateCondition = (EhsStateConditionType*)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(state_condition_doCondition));

    if (EHS_FB_IN_CONNECTED(0)){
        EhsStateManagerType* pStateManager = (EhsStateManagerType*)EHS_FB_IN_U(0);
        if(pStateManager){
            if(pStateManager->nState == pStateCondition->nState){ // Check if manager's current state is the same as this condition
                // move to the next state defined by this condition varable.
                pStateManager->nState = pStateCondition->nNextState;

                pStateManager->onStateChanged((void*)pStateManager);

                EHS_FB_FINISH(1);
            }
        }else{
           // Don't report an error with the client's app to the process console-that the debuggers job 
           // EHSH_LOG_WARNING("STATE_CONDITION -> State manager data is not available !");
        }
    }
    return;
}

