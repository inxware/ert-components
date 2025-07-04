/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/* mux.c
 *
 * src file for multiplexer functions.  Multiplexer functions have 2 or more data inputs.
 * The data input selected appears at the output. Any function appearing in
 * this file must also be listed in objRefTable.c for it to be included in the
 * list of functions available to the EHS.
 *
 * For definition of arguments in Ncapsa functions (Identify_, Init_ and Run_)
 * please see types.h.
 *
*/

#include <string.h>
#include "mux.h"
#include "setCompletes.h"
#include "hal-api.h" /* Required for logging */

// Integer Multiplexers

/**
 * Indexed Event Multiplexer
 */
typedef struct
{
    ehs_bool bHasFired;	// state of FB
    ehs_bool bRequiresResetting;	// Requires manual resetting before can be triggered again
} EhsIndexedEventMultiplexerStruct;

/******************************************************************************/
/* Define xyzzy function block */

EHS_FB_FUNCTIONS_START(MultiplexOneInputInt)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexOneInputInt1)

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
EHS_FB_IDENTIFY_FUNCTION(MultiplexOneInputInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexOneInputInt)
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
EHS_FB_RUN_FUNCTION(MultiplexOneInputInt1)
{
    int* px = (int*)((NCAPSA_pMain)->pIn[0]);
    if(*px != 3)
    {
    }
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define xyzzy function block */

EHS_FB_FUNCTIONS_START(MultiplexTwoInputInt)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexTwoInputInt1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexTwoInputInt2)

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
EHS_FB_IDENTIFY_FUNCTION(MultiplexTwoInputInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexTwoInputInt)
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
EHS_FB_RUN_FUNCTION(MultiplexTwoInputInt1)
{
    int* px = (int*)((NCAPSA_pMain)->pIn[0]);
    if(*px != 3)
    {
    }
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexTwoInputInt2)
{

    int* px = (int*)((NCAPSA_pMain)->pIn[0]);
    if(*px != 3)
    {
    }
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexThreeInputInt function block */

EHS_FB_FUNCTIONS_START(MultiplexThreeInputInt)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexThreeInputInt1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexThreeInputInt2)

EHS_FB_FUNCTION_ENTRY("Run_Input3", 0x03, MultiplexThreeInputInt3)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplexThreeInputInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexThreeInputInt)
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
EHS_FB_RUN_FUNCTION(MultiplexThreeInputInt1)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexThreeInputInt2)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexThreeInputInt3)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexFourInputInt function block */

EHS_FB_FUNCTIONS_START(MultiplexFourInputInt)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexFourInputInt1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexFourInputInt2)

EHS_FB_FUNCTION_ENTRY("Run_Input3", 0x03, MultiplexFourInputInt3)

EHS_FB_FUNCTION_ENTRY("Run_Input4", 0x04, MultiplexFourInputInt4)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplexFourInputInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexFourInputInt)
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
EHS_FB_RUN_FUNCTION(MultiplexFourInputInt1)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexFourInputInt2)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexFourInputInt3)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexFourInputInt4)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexEightInputInt function block */

EHS_FB_FUNCTIONS_START(MultiplexEightInputInt)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexEightInputInt1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexEightInputInt2)

EHS_FB_FUNCTION_ENTRY("Run_Input3", 0x03, MultiplexEightInputInt3)

EHS_FB_FUNCTION_ENTRY("Run_Input4", 0x04, MultiplexEightInputInt4)

EHS_FB_FUNCTION_ENTRY("Run_Input5", 0x05, MultiplexEightInputInt5)

EHS_FB_FUNCTION_ENTRY("Run_Input6", 0x06, MultiplexEightInputInt6)

EHS_FB_FUNCTION_ENTRY("Run_Input7", 0x07, MultiplexEightInputInt7)

EHS_FB_FUNCTION_ENTRY("Run_Input8", 0x08, MultiplexEightInputInt8)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplexEightInputInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexEightInputInt)
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
EHS_FB_RUN_FUNCTION(MultiplexEightInputInt1)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputInt2)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputInt3)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputInt4)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputInt5)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputInt6)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputInt7)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputInt8)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define MultiplexOneInputReal function block */

EHS_FB_FUNCTIONS_START(MultiplexOneInputFloat)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexOneInputFloat1)

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
EHS_FB_IDENTIFY_FUNCTION(MultiplexOneInputFloat)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexOneInputFloat)
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
EHS_FB_RUN_FUNCTION(MultiplexOneInputFloat1)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexTwoInputReal function block */

EHS_FB_FUNCTIONS_START(MultiplexTwoInputFloat)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexTwoInputFloat1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexTwoInputFloat2)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplexTwoInputFloat)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexTwoInputFloat)
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
EHS_FB_RUN_FUNCTION(MultiplexTwoInputFloat1)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexTwoInputFloat2)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexThreeInputFloat function block */

EHS_FB_FUNCTIONS_START(MultiplexThreeInputFloat)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexThreeInputFloat1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexThreeInputFloat2)

EHS_FB_FUNCTION_ENTRY("Run_Input3", 0x03, MultiplexThreeInputFloat3)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplexThreeInputFloat)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexThreeInputFloat)
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
EHS_FB_RUN_FUNCTION(MultiplexThreeInputFloat1)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexThreeInputFloat2)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexThreeInputFloat3)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexFourInputFloat function block */

EHS_FB_FUNCTIONS_START(MultiplexFourInputFloat)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexFourInputFloat1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexFourInputFloat2)

EHS_FB_FUNCTION_ENTRY("Run_Input3", 0x03, MultiplexFourInputFloat3)

EHS_FB_FUNCTION_ENTRY("Run_Input4", 0x04, MultiplexFourInputFloat4)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplexFourInputFloat)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexFourInputFloat)
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
EHS_FB_RUN_FUNCTION(MultiplexFourInputFloat1)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexFourInputFloat2)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexFourInputFloat3)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexFourInputFloat4)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexEightInputFloat function block */

EHS_FB_FUNCTIONS_START(MultiplexEightInputFloat)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexEightInputFloat1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexEightInputFloat2)

EHS_FB_FUNCTION_ENTRY("Run_Input3", 0x03, MultiplexEightInputFloat3)

EHS_FB_FUNCTION_ENTRY("Run_Input4", 0x04, MultiplexEightInputFloat4)

EHS_FB_FUNCTION_ENTRY("Run_Input5", 0x05, MultiplexEightInputFloat5)

EHS_FB_FUNCTION_ENTRY("Run_Input6", 0x06, MultiplexEightInputFloat6)

EHS_FB_FUNCTION_ENTRY("Run_Input7", 0x07, MultiplexEightInputFloat7)

EHS_FB_FUNCTION_ENTRY("Run_Input8", 0x08, MultiplexEightInputFloat8)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplexEightInputFloat)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexEightInputFloat)
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
EHS_FB_RUN_FUNCTION(MultiplexEightInputFloat1)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputFloat2)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputFloat3)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputFloat4)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputFloat5)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputFloat6)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputFloat7)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputFloat8)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#endif /* EHS_TARGET_FP_SUPPORT */
/******************************************************************************/
/* Define MultiplexOneInputBool function block */

EHS_FB_FUNCTIONS_START(MultiplexOneInputBool)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexOneInputBool1)

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
EHS_FB_IDENTIFY_FUNCTION(MultiplexOneInputBool)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexOneInputBool)
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
EHS_FB_RUN_FUNCTION(MultiplexOneInputBool1)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexTwoInputBool function block */

EHS_FB_FUNCTIONS_START(MultiplexTwoInputBool)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexTwoInputBool1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexTwoInputBool2)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplexTwoInputBool)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexTwoInputBool)
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
EHS_FB_RUN_FUNCTION(MultiplexTwoInputBool1)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexTwoInputBool2)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexThreeInputBool function block */

EHS_FB_FUNCTIONS_START(MultiplexThreeInputBool)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexThreeInputBool1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexThreeInputBool2)

EHS_FB_FUNCTION_ENTRY("Run_Input3", 0x03, MultiplexThreeInputBool3)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplexThreeInputBool)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexThreeInputBool)
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
EHS_FB_RUN_FUNCTION(MultiplexThreeInputBool1)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexThreeInputBool2)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexThreeInputBool3)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexFourInputBool function block */

EHS_FB_FUNCTIONS_START(MultiplexFourInputBool)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexFourInputBool1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexFourInputBool2)

EHS_FB_FUNCTION_ENTRY("Run_Input3", 0x03, MultiplexFourInputBool3)

EHS_FB_FUNCTION_ENTRY("Run_Input4", 0x04, MultiplexFourInputBool4)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplexFourInputBool)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexFourInputBool)
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
EHS_FB_RUN_FUNCTION(MultiplexFourInputBool1)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexFourInputBool2)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexFourInputBool3)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexFourInputBool4)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexEightInputBool function block */

EHS_FB_FUNCTIONS_START(MultiplexEightInputBool)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexEightInputBool1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexEightInputBool2)

EHS_FB_FUNCTION_ENTRY("Run_Input3", 0x03, MultiplexEightInputBool3)

EHS_FB_FUNCTION_ENTRY("Run_Input4", 0x04, MultiplexEightInputBool4)

EHS_FB_FUNCTION_ENTRY("Run_Input5", 0x05, MultiplexEightInputBool5)

EHS_FB_FUNCTION_ENTRY("Run_Input6", 0x06, MultiplexEightInputBool6)

EHS_FB_FUNCTION_ENTRY("Run_Input7", 0x07, MultiplexEightInputBool7)

EHS_FB_FUNCTION_ENTRY("Run_Input8", 0x08, MultiplexEightInputBool8)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplexEightInputBool)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexEightInputBool)
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
EHS_FB_RUN_FUNCTION(MultiplexEightInputBool1)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputBool2)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputBool3)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputBool4)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputBool5)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputBool6)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputBool7)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputBool8)
{
    NCAPSA_bOut(0) = NCAPSA_bIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexOneInputString function block */

EHS_FB_FUNCTIONS_START(MultiplexOneInputString)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexOneInputString1)

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
EHS_FB_IDENTIFY_FUNCTION(MultiplexOneInputString)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexOneInputString)
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
EHS_FB_RUN_FUNCTION(MultiplexOneInputString1)
{
    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexTwoInputString function block */

EHS_FB_FUNCTIONS_START(MultiplexTwoInputString)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexTwoInputString1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexTwoInputString2)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplexTwoInputString)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexTwoInputString)
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
EHS_FB_RUN_FUNCTION(MultiplexTwoInputString1)
{
    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexTwoInputString2)
{
    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexThreeInputString function block */

EHS_FB_FUNCTIONS_START(MultiplexThreeInputString)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexThreeInputString1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexThreeInputString2)

EHS_FB_FUNCTION_ENTRY("Run_Input3", 0x03, MultiplexThreeInputString3)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplexThreeInputString)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexThreeInputString)
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
EHS_FB_RUN_FUNCTION(MultiplexThreeInputString1)
{
    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexThreeInputString2)
{
    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexThreeInputString3)
{
    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexFourInputString function block */

EHS_FB_FUNCTIONS_START(MultiplexFourInputString)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexFourInputString1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexFourInputString2)

EHS_FB_FUNCTION_ENTRY("Run_Input3", 0x03, MultiplexFourInputString3)

EHS_FB_FUNCTION_ENTRY("Run_Input4", 0x04, MultiplexFourInputString4)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplexFourInputString)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexFourInputString)
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
EHS_FB_RUN_FUNCTION(MultiplexFourInputString1)
{
    //char tmp[256] = {'\0'};

    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexFourInputString2)
{
    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexFourInputString3)
{
    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexFourInputString4)
{
    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplexEightInputString function block */

EHS_FB_FUNCTIONS_START(MultiplexEightInputString)

EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, MultiplexEightInputString1)

EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, MultiplexEightInputString2)

EHS_FB_FUNCTION_ENTRY("Run_Input3", 0x03, MultiplexEightInputString3)

EHS_FB_FUNCTION_ENTRY("Run_Input4", 0x04, MultiplexEightInputString4)

EHS_FB_FUNCTION_ENTRY("Run_Input5", 0x05, MultiplexEightInputString5)

EHS_FB_FUNCTION_ENTRY("Run_Input6", 0x06, MultiplexEightInputString6)

EHS_FB_FUNCTION_ENTRY("Run_Input7", 0x07, MultiplexEightInputString7)

EHS_FB_FUNCTION_ENTRY("Run_Input8", 0x08, MultiplexEightInputString8)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplexEightInputString)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplexEightInputString)
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
EHS_FB_RUN_FUNCTION(MultiplexEightInputString1)
{
    //char tmp[256] = {'\0'};

    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputString2)
{
    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputString3)
{
    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputString4)
{
    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputString5)
{
    //char tmp[256] = {'\0'};

    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputString6)
{
    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputString7)
{
    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplexEightInputString8)
{
    strcpy(NCAPSA_szOut(0), NCAPSA_szIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define IndexedMultiplexer_Event function block */

EHS_FB_FUNCTIONS_START(IndexedMultiplexer_Event)
EHS_FB_FUNCTION_ENTRY("Run_Input1", 0x01, IndexedMultiplexer_Event_1)
EHS_FB_FUNCTION_ENTRY("Run_Input2", 0x02, IndexedMultiplexer_Event_2)
EHS_FB_FUNCTION_ENTRY("Run_Input3", 0x03, IndexedMultiplexer_Event_3)
EHS_FB_FUNCTION_ENTRY("Run_Input4", 0x04, IndexedMultiplexer_Event_4)
EHS_FB_FUNCTION_ENTRY("Run_Input5", 0x05, IndexedMultiplexer_Event_5)
EHS_FB_FUNCTION_ENTRY("Run_Input6", 0x06, IndexedMultiplexer_Event_6)
EHS_FB_FUNCTION_ENTRY("Run_Input7", 0x07, IndexedMultiplexer_Event_7)
EHS_FB_FUNCTION_ENTRY("Run_Input8", 0x08, IndexedMultiplexer_Event_8)
EHS_FB_FUNCTION_ENTRY("Reset", 0x09, IndexedMultiplexer_Event_Reset)
EHS_FB_FUNCTIONS_END

#define EHS_FB_INDEX_EVENT_OUT_INDEX 0		/**< Function block output for index triggered */

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(IndexedMultiplexer_Event)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsIndexedEventMultiplexerStruct);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(IndexedMultiplexer_Event)
{
    const ehs_char* pParams = EHS_FB_INIT_PARAMETERS;
    EhsIndexedEventMultiplexerStruct* pIndexedEventObj = (EhsIndexedEventMultiplexerStruct*)EHS_FB_INIT_CONTEXT;
    //ehs_bool bReqReset = EHS_FALSE;
    ehs_bool bRet = EHS_TRUE; /* assume success */
    ehs_uint8 nByte;

    // initialise state machine
    pIndexedEventObj->bHasFired = EHS_FALSE;

    if (pParams)
    {
        pParams = EhsGetUint8FromString(&nByte, pParams);
        pIndexedEventObj->bRequiresResetting = (ehs_bool)nByte;
    }
    else
    {
        /* we haven't got an initial parameter */
        bRet = EHS_FALSE;
    }

    return bRet;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_1)
{
    EhsIndexedEventMultiplexerStruct* pIndexedEventObj = (EhsIndexedEventMultiplexerStruct*)EHS_FB_RUN_CONTEXT;

    // if not manual resetting mode or has not been triggered since last reset, trigger
    if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired)
    {
        // output the index of the input event that has been triggered
        EHS_FB_OUT_I(EHS_FB_INDEX_EVENT_OUT_INDEX) = 1;
        // set state
        pIndexedEventObj->bHasFired = EHS_TRUE;
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
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
EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_2)
{
    EhsIndexedEventMultiplexerStruct* pIndexedEventObj = (EhsIndexedEventMultiplexerStruct*)EHS_FB_RUN_CONTEXT;

    // if not manual resetting mode or has not been triggered since last reset, trigger
    if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired)
    {
        // output the index of the input event that has been triggered
        EHS_FB_OUT_I(EHS_FB_INDEX_EVENT_OUT_INDEX) = 2;
        // set state
        pIndexedEventObj->bHasFired = EHS_TRUE;
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
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
EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_3)
{
    EhsIndexedEventMultiplexerStruct* pIndexedEventObj = (EhsIndexedEventMultiplexerStruct*)EHS_FB_RUN_CONTEXT;

    // if not manual resetting mode or has not been triggered since last reset, trigger
    if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired)
    {
        // output the index of the input event that has been triggered
        EHS_FB_OUT_I(EHS_FB_INDEX_EVENT_OUT_INDEX) = 3;
        // set state
        pIndexedEventObj->bHasFired = EHS_TRUE;
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
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
EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_4)
{
    EhsIndexedEventMultiplexerStruct* pIndexedEventObj = (EhsIndexedEventMultiplexerStruct*)EHS_FB_RUN_CONTEXT;

    // if not manual resetting mode or has not been triggered since last reset, trigger
    if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired)
    {
        // output the index of the input event that has been triggered
        EHS_FB_OUT_I(EHS_FB_INDEX_EVENT_OUT_INDEX) = 4;
        // set state
        pIndexedEventObj->bHasFired = EHS_TRUE;
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
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
EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_5)
{
    EhsIndexedEventMultiplexerStruct* pIndexedEventObj = (EhsIndexedEventMultiplexerStruct*)EHS_FB_RUN_CONTEXT;

    // if not manual resetting mode or has not been triggered since last reset, trigger
    if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired)
    {
        // output the index of the input event that has been triggered
        EHS_FB_OUT_I(EHS_FB_INDEX_EVENT_OUT_INDEX) = 5;
        // set state
        pIndexedEventObj->bHasFired = EHS_TRUE;
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
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
EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_6)
{
    EhsIndexedEventMultiplexerStruct* pIndexedEventObj = (EhsIndexedEventMultiplexerStruct*)EHS_FB_RUN_CONTEXT;

    // if not manual resetting mode or has not been triggered since last reset, trigger
    if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired)
    {
        // output the index of the input event that has been triggered
        EHS_FB_OUT_I(EHS_FB_INDEX_EVENT_OUT_INDEX) = 6;
        // set state
        pIndexedEventObj->bHasFired = EHS_TRUE;
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
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
EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_7)
{
    EhsIndexedEventMultiplexerStruct* pIndexedEventObj = (EhsIndexedEventMultiplexerStruct*)EHS_FB_RUN_CONTEXT;

    // if not manual resetting mode or has not been triggered since last reset, trigger
    if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired)
    {
        // output the index of the input event that has been triggered
        EHS_FB_OUT_I(EHS_FB_INDEX_EVENT_OUT_INDEX) = 7;
        // set state
        pIndexedEventObj->bHasFired = EHS_TRUE;
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
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
EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_8)
{
    EhsIndexedEventMultiplexerStruct* pIndexedEventObj = (EhsIndexedEventMultiplexerStruct*)EHS_FB_RUN_CONTEXT;

    // if not manual resetting mode or has not been triggered since last reset, trigger
    if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired)
    {
        // output the index of the input event that has been triggered
        EHS_FB_OUT_I(EHS_FB_INDEX_EVENT_OUT_INDEX) = 8;
        // set state
        pIndexedEventObj->bHasFired = EHS_TRUE;
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
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
EHS_FB_RUN_FUNCTION(IndexedMultiplexer_Event_Reset)
{
    EhsIndexedEventMultiplexerStruct* pIndexedEventObj = (EhsIndexedEventMultiplexerStruct*)EHS_FB_RUN_CONTEXT;

    // reset state machine
    pIndexedEventObj->bHasFired = EHS_FALSE;
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
