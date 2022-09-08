/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * SFCBarGroup1Event.cpp
 *
 * function definitions for SFBarGroup function blocks
 *
 * @author inx limited
 *
 */


#include "ehs_fb_types.h"
#include "setCompletes.h"
#include "sfc.h"


/******************************************************************************/
/* Define SFCBarGroup1Event function block */

EHS_FB_FUNCTIONS_START(SFCBarGroup1Event)

EHS_FB_FUNCTION_ENTRY("Run_OnEntry1Event", 0x00, SFCBarGroup1Event)

EHS_FB_FUNCTION_ENTRY("Run_CL1Event", 0x01, Run_CLEvent)
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
EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup1Event)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_bool);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SFCBarGroup1Event)
{
    *((ehs_bool*)EHS_FB_INIT_CONTEXT) = EHS_FALSE;

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(SFCBarGroup1Event)
{
    *((ehs_bool*)EHS_FB_RUN_CONTEXT) = EHS_TRUE;
}

/******************************************************************************/
/* Define SFCBarGroup2Event function block */

EHS_FB_FUNCTIONS_START(SFCBarGroup2Event)

EHS_FB_FUNCTION_ENTRY("Run_OnEntry2Event", 0x00, SFCBarGroup2Event)

EHS_FB_FUNCTION_ENTRY("Run_CL1Event", 0x01, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL2Event", 0x02, Run_CLEvent)
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
EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup2Event)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_bool);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SFCBarGroup2Event)
{
    *((ehs_bool*)EHS_FB_INIT_CONTEXT) = EHS_FALSE;

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(SFCBarGroup2Event)
{
    //ehs_bool* p2;
    *((ehs_bool*)EHS_FB_RUN_CONTEXT) = EHS_TRUE;

    //p2 = ((ehs_bool*)EHS_FB_RUN_CONTEXT);
}



/******************************************************************************/
/* Define SFCBarGroup3Event function block */

EHS_FB_FUNCTIONS_START(SFCBarGroup3Event)

EHS_FB_FUNCTION_ENTRY("Run_OnEntry3Event", 0x00, SFCBarGroup3Event)

EHS_FB_FUNCTION_ENTRY("Run_CL1Event", 0x01, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL2Event", 0x02, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL3Event", 0x03, Run_CLEvent)
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
EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup3Event)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_bool);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SFCBarGroup3Event)
{
    *((ehs_bool*)EHS_FB_INIT_CONTEXT) = EHS_FALSE;

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(SFCBarGroup3Event)
{
    *((ehs_bool*)EHS_FB_RUN_CONTEXT) = EHS_TRUE;
}



/******************************************************************************/
/* Define SFCBarGroup4Event function block */

EHS_FB_FUNCTIONS_START(SFCBarGroup4Event)

EHS_FB_FUNCTION_ENTRY("Run_OnEntry4Event", 0x00, SFCBarGroup4Event)

EHS_FB_FUNCTION_ENTRY("Run_CL1Event", 0x01, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL2Event", 0x02, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL3Event", 0x03, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL4Event", 0x04, Run_CLEvent)
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
EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup4Event)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_bool);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SFCBarGroup4Event)
{
    *((ehs_bool*)EHS_FB_INIT_CONTEXT) = EHS_FALSE;

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(SFCBarGroup4Event)
{
    *((ehs_bool*)EHS_FB_RUN_CONTEXT) = EHS_TRUE;
}

/******************************************************************************/
/* Define SFCBarGroup8Event function block */

EHS_FB_FUNCTIONS_START(SFCBarGroup8Event)

EHS_FB_FUNCTION_ENTRY("Run_OnEntry8Event", 0x00, SFCBarGroup8Event)

EHS_FB_FUNCTION_ENTRY("Run_CL1Event", 0x01, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL2Event", 0x02, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL3Event", 0x03, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL4Event", 0x04, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL5Event", 0x05, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL6Event", 0x06, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL7Event", 0x07, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL8Event", 0x08, Run_CLEvent)
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
EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup8Event)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_bool);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SFCBarGroup8Event)
{
    *((ehs_bool*)EHS_FB_INIT_CONTEXT) = EHS_FALSE;

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(SFCBarGroup8Event)
{
    *((ehs_bool*)EHS_FB_RUN_CONTEXT) = EHS_TRUE;
}



/******************************************************************************/
/* Define SFCBarGroup12Event function block */

EHS_FB_FUNCTIONS_START(SFCBarGroup12Event)

EHS_FB_FUNCTION_ENTRY("Run_OnEntry12Event", 0x0C, SFCBarGroup12Event)

EHS_FB_FUNCTION_ENTRY("Run_CL1Event", 0x01, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL2Event", 0x03, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL3Event", 0x04, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL4Event", 0x06, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL5Event", 0x08, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL6Event", 0x09, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL7Event", 0x0A, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL8Event", 0x0B, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL9Event", 0x0C, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL10Event", 0x0D, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL11Event", 0x0E, Run_CLEvent)

EHS_FB_FUNCTION_ENTRY("Run_CL12Event", 0x0F, Run_CLEvent)
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
EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup12Event)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_bool);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SFCBarGroup12Event)
{
    *((ehs_bool*)EHS_FB_INIT_CONTEXT) = EHS_FALSE;

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(SFCBarGroup12Event)
{
    *((ehs_bool*)EHS_FB_RUN_CONTEXT) = EHS_TRUE; //Run_SFCBarGroup12Event
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(Run_CLEvent)
{
    ehs_bool postStateActive = *((ehs_bool*)EHS_FB_RUN_CONTEXT);

    if(postStateActive==EHS_TRUE)
    {
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        postStateActive=EHS_FALSE;
        *((ehs_bool*)EHS_FB_RUN_CONTEXT) = postStateActive;
    }
    return;
}
