/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/
/*if.c
*
* if.c
*
* source file for if functions.
*
*/


//#include "types.h"
#include "setCompletes.h"
#include "if.h"
//#include "EHSMacros.h"

/******************************************************************************/
/* Define xyzzy function block */

EHS_FB_FUNCTIONS_START(If)

EHS_FB_FUNCTION_ENTRY("Run_If", 0x01, If)
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
EHS_FB_IDENTIFY_FUNCTION(If)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(If)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(If)
{
    // Input Declarations
    ehs_bool bIn0;  // Condition

    // Output Declarations

    // Input Assignment
    bIn0 = NCAPSA_bIn(0);  //

//TO DO: Complete with own code

    if(bIn0==EHS_TRUE)
    {
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  True
    }
    else
    {
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  False
    }
    return;

}

