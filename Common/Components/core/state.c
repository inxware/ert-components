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
 * function definitions for state.c
 *
 * @author: inx limited
 *
 */


#include "ehs_fb_types.h"
#include "state.h"
#include "setCompletes.h"

#include "hal-api.h" /* Required for logging */


/******************************************************************************/
/* Define state function block */

EHS_FB_FUNCTIONS_START(state)
EHS_FB_FUNCTION_ENTRY("state", 0x00, state)
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
EHS_FB_IDENTIFY_FUNCTION(state)
{
    EHS_FB_IDENTIFY_MEMORY = (ehs_uint32)EhsStrlen(EHS_FB_IDENTIFY_PARAMETERS) + 1;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(state)
{
    EhsStrcpy(EHS_FB_INIT_CONTEXT, EHS_FB_INIT_PARAMETERS);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(state)
{
    // Output Assignment
    EhsStrcpy(NCAPSA_szOut(0),(char*)EHS_FB_RUN_CONTEXT );
    SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  OnEntry
    return;

}

