/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * operator.h
 *
 * src file for basic mathematical functions.  This file lists basic mathematical
 * functons used in the Lucid Event Handling System.
 *
 * @author: inx limited
 *
 */

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <math.h>

#include "globals.h" /* required for trace */
#include "target.h"
#include "operator.h"
#include "setCompletes.h"
#include "hal-api.h" /* Needed for logging */

/*============================================================================*/
//Comparators

/******************************************************************************/
/* Define ComparatorGreaterInt function block */

EHS_FB_FUNCTIONS_START(ComparatorGreaterInt)

EHS_FB_FUNCTION_ENTRY("Run_ComparatorGreaterInt", 0x00, ComparatorGreaterInt)
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
EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ComparatorGreaterInt)
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
EHS_FB_RUN_FUNCTION(ComparatorGreaterInt)
{
    int* pInt0;
    int* pInt1;
    ehs_bool* pBool;
    pInt0 =(int*)((NCAPSA_pMain)->pIn[0]);
    pInt1 =(int*)((NCAPSA_pMain)->pIn[1]);
    pBool =(ehs_bool*)((NCAPSA_pMain)->pOut[0]);


    /* outputs true if In(1) > In(0), false otherwise...*/

    /*	<original>



    	if if (NCAPSA_nIn(1) > NCAPSA_nIn(0))
    	{
    		//		<original>	NCAPSA_bOut(0) = EHS_TRUE; </original>
    		NCAPSA_bOut(0) = EHS_TRUE;
    	}
    	else
    	{
    		//		<original>	NCAPSA_bOut(0) = EHS_FALSE;</original>
    		NCAPSA_bOut(0) = EHS_FALSE;
    	}
     </original> */


//if (*((int*)((NCAPSA_pMain)->pIn[1])) > *((int*)((NCAPSA_pMain)->pIn[0])))
    if ( (*pInt1) > (*pInt0) )
    {
        *pBool = EHS_TRUE;
    }
    else
    {
        *pBool = EHS_FALSE;
    }


    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define ComparatorGreaterReal function block */

EHS_FB_FUNCTIONS_START(ComparatorGreaterReal)

EHS_FB_FUNCTION_ENTRY("Run_ComparatorGreaterReal", 0x00, ComparatorGreaterReal)
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
EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ComparatorGreaterReal)
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
EHS_FB_RUN_FUNCTION(ComparatorGreaterReal)
{
    /* outputs true if In(1) > In(0), false otherwise...*/
    if (NCAPSA_dIn(1) > NCAPSA_dIn(0))
    {
        NCAPSA_bOut(0) = EHS_TRUE;
    }
    else
    {
        NCAPSA_bOut(0) = EHS_FALSE;
    }
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

/******************************************************************************/
/* Define ComparatorGreaterEqualInt function block */

EHS_FB_FUNCTIONS_START(ComparatorGreaterEqualInt)

EHS_FB_FUNCTION_ENTRY("Run_ComparatorGreaterEqualInt", 0x00, ComparatorGreaterEqualInt)
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
EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterEqualInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ComparatorGreaterEqualInt)
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
EHS_FB_RUN_FUNCTION(ComparatorGreaterEqualInt)
{
    /* outputs true if In(1) >= In(0), false otherwise...*/
    if (NCAPSA_nIn(1) >= NCAPSA_nIn(0))
    {
        NCAPSA_bOut(0) = EHS_TRUE;
    }
    else
    {
        NCAPSA_bOut(0) = EHS_FALSE;
    }
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define ComparatorGreaterEqualReal function block */

EHS_FB_FUNCTIONS_START(ComparatorGreaterEqualReal)

EHS_FB_FUNCTION_ENTRY("Run_ComparatorGreaterEqualReal", 0x00, ComparatorGreaterEqualReal)
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
EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterEqualReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ComparatorGreaterEqualReal)
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
EHS_FB_RUN_FUNCTION(ComparatorGreaterEqualReal)
{
    /* outputs true if In(1) >= In(0), false otherwise...*/
    if (NCAPSA_dIn(1) >= NCAPSA_dIn(0))
    {
        NCAPSA_bOut(0) = EHS_TRUE;
    }
    else
    {
        NCAPSA_bOut(0) = EHS_FALSE;
    }
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

/******************************************************************************/
/* Define ComparatorEqualInt function block */

EHS_FB_FUNCTIONS_START(ComparatorEqualInt)

EHS_FB_FUNCTION_ENTRY("Run_ComparatorEqualInt", 0x00, ComparatorEqualInt)
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
EHS_FB_IDENTIFY_FUNCTION(ComparatorEqualInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ComparatorEqualInt)
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
EHS_FB_RUN_FUNCTION(ComparatorEqualInt)
{
    /* outputs true if In(1) == In(0), false otherwise...*/
    if (NCAPSA_nIn(1) == NCAPSA_nIn(0))
    {
        NCAPSA_bOut(0) = EHS_TRUE;
    }
    else
    {
        NCAPSA_bOut(0) = EHS_FALSE;
    }
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define ComparatorEqualReal function block */

EHS_FB_FUNCTIONS_START(ComparatorEqualReal)

EHS_FB_FUNCTION_ENTRY("Run_ComparatorEqualReal", 0x00, ComparatorEqualReal)
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
EHS_FB_IDENTIFY_FUNCTION(ComparatorEqualReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ComparatorEqualReal)
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
EHS_FB_RUN_FUNCTION(ComparatorEqualReal)
{
    /* outputs true if In(1) == In(0), false otherwise...*/
    if (NCAPSA_dIn(1) == NCAPSA_dIn(0))
    {
        NCAPSA_bOut(0) = EHS_TRUE;
    }
    else
    {
        NCAPSA_bOut(0) = EHS_FALSE;
    }
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif
