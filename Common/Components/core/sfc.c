/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * sfc.h
 *
 * function prototypes for sfc.c - for state flow control?
 * Not really sure.
 * @author: inx limited
 *
 */

#include "globals.h"
#include "sfc.h"
#include "setCompletes.h"
#include "hal-api.h" /* Required for logging */

/******************************************************************************/
/* Define xyzzy function block */

EHS_FB_FUNCTIONS_START(SFCBar)

EHS_FB_FUNCTION_ENTRY("Run_SFCBarPreviousStateComplete", 0x01, SFCBarPreviousStateComplete)

EHS_FB_FUNCTION_ENTRY("Run_SFCBarConditionUpdate", 0x02, SFCBarConditionUpdate)
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
EHS_FB_IDENTIFY_FUNCTION(SFCBar)
{
    /* reserve state data for a ehs_bool..*/
    EHS_FB_IDENTIFY_MEMORY =  sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SFCBar)
{
    ehs_bool* pBOOL = NULL;

    /* cast the pointer to type ehs_bool...*/
    pBOOL = (ehs_bool*)EHS_FB_INIT_CONTEXT;



    /* make sure  ehs_bool is initially false...*/
    *pBOOL = EHS_FALSE;

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(SFCBarPreviousStateComplete)
{
    ehs_bool* waiting = NULL;


    waiting = (ehs_bool*)EHS_FB_RUN_CONTEXT; //#define EHS_FB_RUN_CONTEXT (BYTE*)*pByte

    /*  */
    if ( NCAPSA_bIn(0) == EHS_TRUE)
    {
        *waiting = EHS_FALSE;
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        *waiting = EHS_TRUE;
    }
    return;


}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(SFCBarConditionUpdate)
{
    ehs_bool* waiting = NULL;


    waiting = (ehs_bool*)EHS_FB_RUN_CONTEXT;




    if( ( NCAPSA_bIn(0) == EHS_TRUE) && (*waiting == EHS_TRUE) )
    {
        *waiting = EHS_FALSE;
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }

    return;

}

