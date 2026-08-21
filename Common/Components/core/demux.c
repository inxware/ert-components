/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/* demux.c
 *
 * src file for demultiplexer functions.  Demultiplexer functions have 2 or more
 * data inputs. The data input appears at the selected output. Any function
 * appearing in this file must also be listed in objRefTable.c for it to be
 * included in the list of functions available to the EHS.
 *
 * For definition of arguments in Ncapsa functions (Identify_, Init_ and Run_)
 * please see types.h.
 *
*/


#include "globals.h"
#include "demux.h"
#include "setCompletes.h"
/* For new function implementations */
#include "ehs_comp_api2.h"

#include "hal-api.h" /* Required for logging */


/******************************************************************************/
/* Define DemultiplexTwoOutputInt function block */

EHS_FB_FUNCTIONS_START(DemultiplexTwoOutputInt)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexTwoOutputInt1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexTwoOutputInt2)
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

EHS_FB_IDENTIFY_FUNCTION(DemultiplexTwoOutputInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexTwoOutputInt)
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
EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputInt1)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputInt2)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define ConvertorBoolToTrigger function block */

EHS_FB_FUNCTIONS_START(DemultiplexThreeOutputInt)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexThreeOutputInt1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexThreeOutputInt2)

EHS_FB_FUNCTION_ENTRY("Run_Output3", 0x03, DemultiplexThreeOutputInt3)
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
EHS_FB_IDENTIFY_FUNCTION(DemultiplexThreeOutputInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexThreeOutputInt)
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
EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputInt1)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputInt2)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputInt3)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define ConvertorBoolToTrigger function block */

EHS_FB_FUNCTIONS_START(DemultiplexFourOutputInt)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexFourOutputInt1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexFourOutputInt2)

EHS_FB_FUNCTION_ENTRY("Run_Output3", 0x03, DemultiplexFourOutputInt3)

EHS_FB_FUNCTION_ENTRY("Run_Output4", 0x04, DemultiplexFourOutputInt4)
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
EHS_FB_IDENTIFY_FUNCTION(DemultiplexFourOutputInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexFourOutputInt)
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputInt1)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputInt2)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputInt3)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputInt4)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define DemultiplexEightOutputInt function block */

EHS_FB_FUNCTIONS_START(DemultiplexEightOutputInt)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexEightOutputInt1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexEightOutputInt2)

EHS_FB_FUNCTION_ENTRY("Run_Output3", 0x03, DemultiplexEightOutputInt3)

EHS_FB_FUNCTION_ENTRY("Run_Output4", 0x04, DemultiplexEightOutputInt4)

EHS_FB_FUNCTION_ENTRY("Run_Output5", 0x05, DemultiplexEightOutputInt5)

EHS_FB_FUNCTION_ENTRY("Run_Output6", 0x06, DemultiplexEightOutputInt6)

EHS_FB_FUNCTION_ENTRY("Run_Output7", 0x07, DemultiplexEightOutputInt7)

EHS_FB_FUNCTION_ENTRY("Run_Output8", 0x08, DemultiplexEightOutputInt8)
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
EHS_FB_IDENTIFY_FUNCTION(DemultiplexEightOutputInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexEightOutputInt)
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt1)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt2)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt3)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt4)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt5)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt6)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt7)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputInt8)
{
    EHS_FB_OUT_I(0) = EHS_FB_IN_I(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}


/******************************************************************************/
/* Define DemultiplexTwoOutputBool function block */

EHS_FB_FUNCTIONS_START(DemultiplexTwoOutputBool)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexTwoOutputBool1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexTwoOutputBool2)
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
EHS_FB_IDENTIFY_FUNCTION(DemultiplexTwoOutputBool)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexTwoOutputBool)
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
EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputBool1)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputBool2)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define ConvertorBoolToTrigger function block */

EHS_FB_FUNCTIONS_START(DemultiplexThreeOutputBool)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexThreeOutputBool1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexThreeOutputBool2)

EHS_FB_FUNCTION_ENTRY("Run_Output3", 0x03, DemultiplexThreeOutputBool3)
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
EHS_FB_IDENTIFY_FUNCTION(DemultiplexThreeOutputBool)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexThreeOutputBool)
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
EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputBool1)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputBool2)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputBool3)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define DemultiplexFourOutputBool function block */

EHS_FB_FUNCTIONS_START(DemultiplexFourOutputBool)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexFourOutputBool1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexFourOutputBool2)

EHS_FB_FUNCTION_ENTRY("Run_Output3", 0x03, DemultiplexFourOutputBool3)

EHS_FB_FUNCTION_ENTRY("Run_Output4", 0x04, DemultiplexFourOutputBool4)
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
EHS_FB_IDENTIFY_FUNCTION(DemultiplexFourOutputBool)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexFourOutputBool)
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputBool1)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputBool2)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputBool3)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputBool4)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define DemultiplexEightOutputBool function block */

EHS_FB_FUNCTIONS_START(DemultiplexEightOutputBool)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexEightOutputBool1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexEightOutputBool2)

EHS_FB_FUNCTION_ENTRY("Run_Output3", 0x03, DemultiplexEightOutputBool3)

EHS_FB_FUNCTION_ENTRY("Run_Output4", 0x04, DemultiplexEightOutputBool4)

EHS_FB_FUNCTION_ENTRY("Run_Output5", 0x05, DemultiplexEightOutputBool5)

EHS_FB_FUNCTION_ENTRY("Run_Output6", 0x06, DemultiplexEightOutputBool6)

EHS_FB_FUNCTION_ENTRY("Run_Output7", 0x07, DemultiplexEightOutputBool7)

EHS_FB_FUNCTION_ENTRY("Run_Output8", 0x08, DemultiplexEightOutputBool8)
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
EHS_FB_IDENTIFY_FUNCTION(DemultiplexEightOutputBool)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexEightOutputBool)
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool1)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool2)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool3)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool4)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool5)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool6)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool7)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputBool8)
{
    EHS_FB_OUT_B(0) = EHS_FB_IN_B(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define ConvertorBoolToTrigger function block */

EHS_FB_FUNCTIONS_START(DemultiplexTwoOutputString)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexTwoOutputString1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexTwoOutputString2)
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
EHS_FB_IDENTIFY_FUNCTION(DemultiplexTwoOutputString)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexTwoOutputString)
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
EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputString1)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
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
EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputString2)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define DemultiplexThreeOutputString function block */

EHS_FB_FUNCTIONS_START(DemultiplexThreeOutputString)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexThreeOutputString1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexThreeOutputString2)

EHS_FB_FUNCTION_ENTRY("Run_Output3", 0x03, DemultiplexThreeOutputString3)
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
EHS_FB_IDENTIFY_FUNCTION(DemultiplexThreeOutputString)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexThreeOutputString)
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
EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputString1)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
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
EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputString2)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
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
EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputString3)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define DemultiplexFourOutputString function block */

EHS_FB_FUNCTIONS_START(DemultiplexFourOutputString)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexFourOutputString1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexFourOutputString2)

EHS_FB_FUNCTION_ENTRY("Run_Output3", 0x03, DemultiplexFourOutputString3)

EHS_FB_FUNCTION_ENTRY("Run_Output4", 0x04, DemultiplexFourOutputString4)
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
EHS_FB_IDENTIFY_FUNCTION(DemultiplexFourOutputString)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexFourOutputString)
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputString1)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputString2)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputString3)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputString4)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define DemultiplexEightOutputString function block */

EHS_FB_FUNCTIONS_START(DemultiplexEightOutputString)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexEightOutputString1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexEightOutputString2)

EHS_FB_FUNCTION_ENTRY("Run_Output3", 0x03, DemultiplexEightOutputString3)

EHS_FB_FUNCTION_ENTRY("Run_Output4", 0x04, DemultiplexEightOutputString4)

EHS_FB_FUNCTION_ENTRY("Run_Output5", 0x05, DemultiplexEightOutputString5)

EHS_FB_FUNCTION_ENTRY("Run_Output6", 0x06, DemultiplexEightOutputString6)

EHS_FB_FUNCTION_ENTRY("Run_Output7", 0x07, DemultiplexEightOutputString7)

EHS_FB_FUNCTION_ENTRY("Run_Output8", 0x08, DemultiplexEightOutputString8)
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
EHS_FB_IDENTIFY_FUNCTION(DemultiplexEightOutputString)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexEightOutputString)
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString1)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString2)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString3)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString4)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString5)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString6)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString7)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputString8)
{
    EHS_FB_OUT_S_SET(0, EHS_FB_IN_S(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/

/* common index demux initialisation functions */

typedef struct
{
    ehs_sint32 offset;
} EhsIndexedDemuxParms;


/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(IndexedDemultiplexer)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsIndexedDemuxParms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(IndexedDemultiplexer)
{
    EhsIndexedDemuxParms *pCtx = (EhsIndexedDemuxParms*) EHS_FB_INIT_CONTEXT;
    pCtx->offset = atoi(EHS_FB_INIT_PARAMETERS);
    return EHS_TRUE; /* initialisation always succeeds */
}

/******************************************************************************/
/* Define IndexedDemultiplexer_String function block */
//@todo - this code refers to function in Components sandbox, move/remove?
EHS_FB_FUNCTIONS_START(IndexedDemultiplexer_String)

EHS_FB_FUNCTION_ENTRY("Run", 0x01, IndexedDemultiplexer_String)
EHS_FB_FUNCTIONS_END

/**
 * Copy the input to the output correpsonding to the input value
 * Also set the event corresponding to the input value.
 *
 * If the input value is out of range, set the overflow.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(IndexedDemultiplexer_String)
{
    EhsIndexedDemuxParms *pCtx = (EhsIndexedDemuxParms*)EHS_FB_RUN_CONTEXT;

    EhsDataflowIntType index = EHS_FB_IN_I(1)+1-pCtx->offset;

    if (index>8)
    {
        EHS_FB_OUT_I(8)=index-8; /**< shouldn't this be EHS_FB_OUT_I(1) @todo check this */
        EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,9);
    }
    else if (index<1)
    {
    }
    else   //Everything OK !
    {
        EHS_FB_OUT_S_SET(index-1, EHS_FB_IN_S(0)); // index-1+1: the output is offset by one as the offset is first output)
        EHS_FB_OUT_I(8)=0;
        EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,index); /** only trigger the appropriate output @todo these are not 0 indexed */
    }
    return;
}

/******************************************************************************/
/* Define IndexedDemultiplexer_Int function block */
//@todo - this code refers to function in Components sandbox, move/remove?
EHS_FB_FUNCTIONS_START(IndexedDemultiplexer_Int)

EHS_FB_FUNCTION_ENTRY("Run", 0x01, IndexedDemultiplexer_Int)
EHS_FB_FUNCTIONS_END

/**
 * Copy the input to the output correpsonding to the input value
 * Also set the event corresponding to the input value.
 *
 * If the input value is out of range, set the overflow.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(IndexedDemultiplexer_Int)
{
    EhsIndexedDemuxParms *pCtx = (EhsIndexedDemuxParms*)EHS_FB_RUN_CONTEXT;

    EhsDataflowIntType index = EHS_FB_IN_I(1)+1-pCtx->offset;

    EhsDataflowIntType value = EHS_FB_IN_I(0);

    if (index>8)
    {
        EHS_FB_OUT_I(8)=index-8; /**< shouldn't this be EHS_FB_OUT_I(1) @todo check this */
        EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,9);
    }
    else if (index<1)
    {
    }
    else   //Everything OK !
    {
        EHS_FB_OUT_I(index-1) = value; // index-1+1: the output is offset by one as the offset is first output)
        EHS_FB_OUT_I(8)=0;
        EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,index); /** only trigger the appropriate output @todo these are not 0 indexed */
    }
    return;
}

/******************************************************************************/
/* Define IndexedDemultiplexer_Bool function block */
//@todo - this code refers to function in Components sandbox, move/remove?
EHS_FB_FUNCTIONS_START(IndexedDemultiplexer_Bool)

EHS_FB_FUNCTION_ENTRY("Run", 0x01, IndexedDemultiplexer_Bool)
EHS_FB_FUNCTIONS_END

/**
 * Copy the input to the output correpsonding to the input value
 * Also set the event corresponding to the input value.
 *
 * If the input value is out of range, set the overflow.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(IndexedDemultiplexer_Bool)
{
    EhsIndexedDemuxParms *pCtx = (EhsIndexedDemuxParms*)EHS_FB_RUN_CONTEXT;

    EhsDataflowIntType index = EHS_FB_IN_I(1)+1-pCtx->offset;

    EhsDataflowBoolType value = EHS_FB_IN_B(0);

    if (index>8)
    {
        EHS_FB_OUT_I(8)=index-8; /**< shouldn't this be EHS_FB_OUT_I(1) @todo check this */
        EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,9);
    }
    else if (index<1)
    {
    }
    else   //Everything OK !
    {
        EHS_FB_OUT_B(index-1) = value; // index-1+1: the output is offset by one as the offset is first output)
        EHS_FB_OUT_I(8)=0;
        EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,index); /** only trigger the appropriate output @todo these are not 0 indexed */
    }
    return;
}

/******************************************************************************/
/* Define IndexedDemultiplexer_Event function block */
// other indexed demuxs have been moved to Components sandbox as can be replaced by other components combinations, this one can't so we're keeping it
//@todo - move code to events
EHS_FB_FUNCTIONS_START(IndexedDemultiplexer_Event)

EHS_FB_FUNCTION_ENTRY("Run", 0x01, IndexedDemultiplexer_Event)
EHS_FB_FUNCTIONS_END

/**
 * Set the event corresponding to the input value.
 *
 * If the input value is out of range, set the overflow.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(IndexedDemultiplexer_Event)
{
    EhsIndexedDemuxParms *pCtx = (EhsIndexedDemuxParms*)EHS_FB_RUN_CONTEXT;
    EhsDataflowIntType index = EHS_FB_IN_I(0)+1-pCtx->offset;

    // always trigger pass through
    EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,10);

    if (index>8)
    {
        EHS_FB_OUT_I(0)=index-8;
        EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,9);
    }
    else if (index<1)
    {
    }
    else   //Everything OK !
    {
        EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,index); /** only trigger the appropriate output @todo these are not 0 indexed */
    }
    return;
}

/*************************************************************************************
 *  Text Case Event Demux
 *  ************************************************************************************/


typedef struct
{
    ehs_char * parmCases[8]; //allocated dynamically (auto deallocated) where entries are available
    ehs_bool bSearchMode; // When set to 'true' we filer event if string exists in the text
} EhsTextCaseDemultiplexerParms;
/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */

EHS_FB_IDENTIFY_FUNCTION(TextCaseDemultiplexer_Event)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsTextCaseDemultiplexerParms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 *
 *  todo - we might want this to do some regexp or wildcard processing at some point?
 */
EHS_FB_INIT_FUNCTION(TextCaseDemultiplexer_Event)
{
    int i;
    const ehs_char * pParams = (ehs_char *) EHS_FB_INIT_PARAMETERS;
    ehs_char temp[EHS_STRING_LENGTH_MAX];
    EhsTextCaseDemultiplexerParms *pCtx = (EhsTextCaseDemultiplexerParms*) EHS_FB_INIT_CONTEXT;
    pCtx->bSearchMode = EHS_FALSE;
    for (i = 0; i< 8 && pParams ; i++)
    {
        temp[0]='\0';
        pParams = EhsStrTrimL(pParams);
        if (EhsStrncmp(pParams,"NULL",4) == 0)
        {
            pCtx->parmCases[i] = NULL;
            pParams+=4;
        }
        else
        {
            if (pParams[0] == '"')
            {
                pParams = EhsGetQuoteDelimFromString(temp, pParams, EHS_STRING_LENGTH_MAX);
            }
            else   // assume we have some bare text
            {
                pParams = EhsGetWordFromString(temp, pParams, sizeof(temp));
            }

            const ehs_sint32 len = EhsStrlen(temp);

            if (len > 0 && len < EHS_STRING_LENGTH_MAX)
            {
                pCtx->parmCases[i] = EhsHMem_writeableAlloc(len + 1);
                if (pCtx->parmCases[i]){
                    EhsStrcpy(pCtx->parmCases[i],temp);
                    pCtx->parmCases[i][len] = '\0';
                }
            }
            else
            {
                pCtx->parmCases[i] = NULL;
            }
        }
    }
    pParams = EhsGetUint8FromString(&(pCtx->bSearchMode), pParams);
    return EHS_TRUE; /* initialisation always succeeds */
}

/******************************************************************************/
/* Define IndexedDemultiplexer_Event function block */
// other indexed demuxs have been moved to Components sandbox as can be replaced by other components combinations, this one can't so we're keeping it
//@todo - move code to events
EHS_FB_FUNCTIONS_START(TextCaseDemultiplexer_Event)

EHS_FB_FUNCTION_ENTRY("Run", 0x01, TextCaseDemultiplexer_Event)
EHS_FB_FUNCTIONS_END

/**
 * Set the event corresponding to the input value.
 *
 * If the input value is out of range, set the overflow.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */

#define EHS_PORTNUM_TextCaseDemultiplexer_Test 1
#define EHS_PORTNUM_TextCaseDemultiplexer_Case_1 2
#define EHS_PORTNUM_TextCaseDemultiplexer_Case_Last 9
#define EHS_PORTNUM_TextCaseDemultiplexer_EventDefault 9
#define EHS_PORTNUM_TextCaseDemultiplexer_EventCase1 1
#define EHS_PORTNUM_TextCaseDemultiplexer_EventCaseLast 8


EHS_FB_RUN_FUNCTION(TextCaseDemultiplexer_Event)
{
    EhsTextCaseDemultiplexerParms *pCtx = (EhsTextCaseDemultiplexerParms*)EHS_FB_RUN_CONTEXT;
    int i;
    ehs_char * psZcase[8] = {0};
    ehs_bool bDefault = EHS_TRUE;
    /* Choose where we get the cases from */
    for (i = 0; i< 8 ; i++)
    {
        if (EHS_FB_IN_CONNECTED_API2(i + EHS_PORTNUM_TextCaseDemultiplexer_Case_1)) psZcase[i] = EHS_FB_IN_S_API2( i + EHS_PORTNUM_TextCaseDemultiplexer_Case_1 );
        else  psZcase[i] = pCtx->parmCases[i];
    }

    //return;
    /*  Check for matches */
    if (EHS_FB_IN_CONNECTED_API2(EHS_PORTNUM_TextCaseDemultiplexer_Test))
    {
        for (i = 0; i< 8 ; i++)
        {
            if (psZcase[i])
            {
                if(pCtx->bSearchMode == EHS_TRUE){
                    char* s1; char* s2; char* s3;
                    s1 = EHS_FB_IN_S_API2(EHS_PORTNUM_TextCaseDemultiplexer_Test);
                    s2 = psZcase[i];
                    s3 = EhsStrstr(s1, s2);
                    if(s3){
                        EHS_FB_FINISH_API2(EHS_PORTNUM_TextCaseDemultiplexer_EventCase1+i);
                        bDefault = EHS_FALSE;
                        break; // we only fire the first one found
                    }
                }else{
                    if (EhsStrcmp(psZcase[i],EHS_FB_IN_S_API2(EHS_PORTNUM_TextCaseDemultiplexer_Test)) == 0)
                    {
                        EHS_FB_FINISH_API2(EHS_PORTNUM_TextCaseDemultiplexer_EventCase1+i);
                        bDefault = EHS_FALSE;
                        break; // we only fire the first one found
                    }
                }
            }
        }
        if (bDefault == EHS_TRUE) EHS_FB_FINISH_API2(EHS_PORTNUM_TextCaseDemultiplexer_EventDefault);
    }
    else EHS_FB_FINISH_API2(EHS_PORTNUM_TextCaseDemultiplexer_EventDefault);

    return;
}


#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define DemultiplexTwoOutputFloat function block */

EHS_FB_FUNCTIONS_START(DemultiplexTwoOutputFloat)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexTwoOutputFloat1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexTwoOutputFloat2)
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
EHS_FB_IDENTIFY_FUNCTION(DemultiplexTwoOutputFloat)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexTwoOutputFloat)
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
EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputFloat1)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexTwoOutputFloat2)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define DemultiplexThreeOutputFloat function block */

EHS_FB_FUNCTIONS_START(DemultiplexThreeOutputFloat)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexThreeOutputFloat1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexThreeOutputFloat2)

EHS_FB_FUNCTION_ENTRY("Run_Output3", 0x03, DemultiplexThreeOutputFloat3)
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
EHS_FB_IDENTIFY_FUNCTION(DemultiplexThreeOutputFloat)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexThreeOutputFloat)
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
EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputFloat1)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputFloat2)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexThreeOutputFloat3)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define DemultiplexFourOutputFloat function block */

EHS_FB_FUNCTIONS_START(DemultiplexFourOutputFloat)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexFourOutputFloat1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexFourOutputFloat2)

EHS_FB_FUNCTION_ENTRY("Run_Output3", 0x03, DemultiplexFourOutputFloat3)

EHS_FB_FUNCTION_ENTRY("Run_Output4", 0x04, DemultiplexFourOutputFloat4)
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
EHS_FB_IDENTIFY_FUNCTION(DemultiplexFourOutputFloat)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexFourOutputFloat)
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputFloat1)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputFloat2)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputFloat3)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexFourOutputFloat4)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define DemultiplexFourOutputFloat function block */

EHS_FB_FUNCTIONS_START(DemultiplexEightOutputFloat)

EHS_FB_FUNCTION_ENTRY("Run_Output1", 0x01, DemultiplexEightOutputFloat1)

EHS_FB_FUNCTION_ENTRY("Run_Output2", 0x02, DemultiplexEightOutputFloat2)

EHS_FB_FUNCTION_ENTRY("Run_Output3", 0x03, DemultiplexEightOutputFloat3)

EHS_FB_FUNCTION_ENTRY("Run_Output4", 0x04, DemultiplexEightOutputFloat4)

EHS_FB_FUNCTION_ENTRY("Run_Output5", 0x05, DemultiplexEightOutputFloat5)

EHS_FB_FUNCTION_ENTRY("Run_Output6", 0x06, DemultiplexEightOutputFloat6)

EHS_FB_FUNCTION_ENTRY("Run_Output7", 0x07, DemultiplexEightOutputFloat7)

EHS_FB_FUNCTION_ENTRY("Run_Output8", 0x08, DemultiplexEightOutputFloat8)
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
EHS_FB_IDENTIFY_FUNCTION(DemultiplexEightOutputFloat)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DemultiplexEightOutputFloat)
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat1)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat2)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat3)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat4)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat5)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat6)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat7)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
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
EHS_FB_RUN_FUNCTION(DemultiplexEightOutputFloat8)
{
    EHS_FB_OUT_F(0) = EHS_FB_IN_F(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
/******************************************************************************/
/* Define IndexedDemultiplexer_Float function block */
//@todo - this code refers to function in Components sandbox, move/remove?
EHS_FB_FUNCTIONS_START(IndexedDemultiplexer_Float)

EHS_FB_FUNCTION_ENTRY("Run", 0x01, IndexedDemultiplexer_Float)
EHS_FB_FUNCTIONS_END

/**
 * Copy the input to the output correpsonding to the input value
 * Also set the event corresponding to the input value.
 *
 * If the input value is out of range, set the overflow.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(IndexedDemultiplexer_Float)
{
    EhsIndexedDemuxParms *pCtx = (EhsIndexedDemuxParms*)EHS_FB_RUN_CONTEXT;

    EhsDataflowIntType index = EHS_FB_IN_I(1)+1-pCtx->offset;

    EhsDataflowFloatType value = EHS_FB_IN_F(0);

    if (index>8)
    {
        EHS_FB_OUT_I(8)=index-8; /**< shouldn't this be EHS_FB_OUT_I(1) @todo check this */
        EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,9);
    }
    else if (index<1)
    {
    }
    else   //Everything OK !
    {
        EHS_FB_OUT_F(index-1) = value; // index-1+1: the output is offset by one as the offset is first output)
        EHS_FB_OUT_I(8)=0;
        EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,index); /** only trigger the appropriate output @todo these are not 0 indexed */
    }
    return;
}

#endif
