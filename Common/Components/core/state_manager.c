/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * state_manager.h
 *
 * function definitions for state_manager.c
 *
 * @author: inx limited
 *
 */
#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

#include "ehs_fb_types.h"
#include "state_manager.h"

#include "hal-api.h" /* Required for logging */

#define SM_USER_DATA_OUT_PORT 0 /* state manager output port number */
#define SC_USER_DATA_IN_PORT 0 /* state condition input port number */
#define SM_ENABLED_FINISH_PORT 1
#define SM_STATE_FINISH_PORT 2
#define SM_STATE_INT_OUT_PORT 1

// state manager callbacks
ehs_bool onClearEventConditionCallback(void* self)
{
    EhsFunctionInstanceClearEventsType* pClearEventsData = (EhsFunctionInstanceClearEventsType*)self;
    if(pClearEventsData != NULL && pClearEventsData->pFuncInst != NULL && pClearEventsData->pFuncInstCaller != NULL){
        void* pOut = (pClearEventsData->pFuncInstCaller->nNumOutputs > SM_USER_DATA_OUT_PORT) ? pClearEventsData->pFuncInstCaller->pOut[SM_USER_DATA_OUT_PORT] : NULL;
        void* pIn = (pClearEventsData->pFuncInst->nNumInputs > SC_USER_DATA_IN_PORT) ? pClearEventsData->pFuncInst->pIn[SC_USER_DATA_IN_PORT] : NULL;
        if(pOut != NULL && pIn != NULL && *(EhsDataflowUserType*)pOut == *(EhsDataflowUserType*)pIn){
            return EHS_TRUE; // return true to remove event from queue
        }
    }
    return EHS_FALSE;
}

void onStateChangedCallback(void* self)
{
    EhsStateManagerType* pStateManager = (EhsStateManagerType*) self;
    if(pStateManager != NULL){
        //EHSH_LOG_ERROR("STATE_MAMANGER -> (current state = %d)", pStateManager->nState);

        EhsFunctionInstanceDataType* pFIdata = pStateManager->pFIdata;

        EhsFunctionInstanceClearEventsType clearEventsData;
        clearEventsData.pFuncInstCaller = pFIdata;
        clearEventsData.pFuncInst = NULL;
        clearEventsData.onClearEventCondition = onClearEventConditionCallback;
        EhsFunctionInstanceData_clearEvents(&clearEventsData);

        EHS_FB_OUT_I(SM_STATE_INT_OUT_PORT) = pStateManager->nState; // output state id
        EHS_FB_FINISH(SM_STATE_FINISH_PORT);
    }
}


/******************************************************************************/
/* Define state function block */

EHS_FB_FUNCTIONS_START(state_manager)
EHS_FB_FUNCTION_ENTRY("enabled", 0x01, state_manager_enabled)
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
EHS_FB_IDENTIFY_FUNCTION(state_manager)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(state_manager));
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsStateManagerType);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(state_manager)
{
    EhsStateManagerType* pStateManager = (EhsStateManagerType*)EHS_FB_INIT_CONTEXT;
    ehs_sint32 nInitalState = -1;

    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(state_manager));

    /* parse parameters to get inital state, from the state manager */
    EhsGetSint32FromString(&nInitalState, EHS_FB_INIT_PARAMETERS);
    pStateManager->onStateChanged = onStateChangedCallback;
    pStateManager->nInitalState = nInitalState;
    pStateManager->nState = nInitalState;
    pStateManager->pFIdata = NULL;
    pStateManager->bEnabled = EHS_FALSE;
    //EHSH_LOG_ERROR("STATE_MAMANGER (%p) -> (Inital State = %d)", pStateManager, pStateManager->nInitalState);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(state_manager_enabled)
{
    EhsStateManagerType* pStateManager = (EhsStateManagerType*)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(state_manager_enabled));

    if (EHS_FB_OUT_CONNECTED(SM_USER_DATA_OUT_PORT)){ // check if user output is connected to a state
        if(pStateManager->bEnabled == EHS_FALSE){
            
            pStateManager->pFIdata = EHS_FB_RUN_CONTEXT_REF;
            
            EHS_FB_OUT_U(SM_USER_DATA_OUT_PORT) = (EhsDataflowUserType) pStateManager; // assign this context to the user data
            pStateManager->bEnabled = EHS_TRUE;
            // update state on enabled, as it is in inital state
            if(pStateManager->nState == pStateManager->nInitalState){
                pStateManager->onStateChanged((void*)pStateManager);
            }
            EHS_FB_FINISH(SM_ENABLED_FINISH_PORT); 
        }
    }else{
        EHSH_LOG_ERROR("STATE_MAMANGER is not connected");
    }
    return;
}
