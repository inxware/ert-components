/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/* logic.h
 *
 * logic functions that operate on Boolean data.
 *
 * For definition of arguments in Ncapsa functions (Identify_, Init_ and Run_)
 * please see types.h.
 *
 */
#include "logic.h"
#include "setCompletes.h"
#include "hal-api.h" /* Required for logging */

/******************************************************************************/
/* Define xyzzy function block */

EHS_FB_FUNCTIONS_START(LogicNot)

EHS_FB_FUNCTION_ENTRY("Run_LogicNot", 0x01, LogicNot)
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
EHS_FB_IDENTIFY_FUNCTION(LogicNot)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogicNot)
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
EHS_FB_RUN_FUNCTION(LogicNot)
{
    /* output != input...*/
    ehs_bool in;
    in = NCAPSA_bIn(0);
    NCAPSA_bOut(0) = (ehs_bool)!in;
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define LogicTwoInputAnd function block */

EHS_FB_FUNCTIONS_START(LogicTwoInputAnd)

EHS_FB_FUNCTION_ENTRY("Run_LogicTwoInputAnd", 0x01, LogicTwoInputAnd)
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
EHS_FB_IDENTIFY_FUNCTION(LogicTwoInputAnd)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogicTwoInputAnd)
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
EHS_FB_RUN_FUNCTION(LogicTwoInputAnd)
{
    if (NCAPSA_bIn(0) && NCAPSA_bIn(1))
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

/******************************************************************************/
/* Define LogicThreeInputAnd function block */

EHS_FB_FUNCTIONS_START(LogicThreeInputAnd)

EHS_FB_FUNCTION_ENTRY("Run_LogicThreeInputAnd", 0x01, LogicThreeInputAnd)
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
EHS_FB_IDENTIFY_FUNCTION(LogicThreeInputAnd)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogicThreeInputAnd)
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
EHS_FB_RUN_FUNCTION(LogicThreeInputAnd)
{
    if (NCAPSA_bIn(0) && NCAPSA_bIn(1) && NCAPSA_bIn(2))
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

/******************************************************************************/
/* Define LogicFourInputAnd function block */

EHS_FB_FUNCTIONS_START(LogicFourInputAnd)

EHS_FB_FUNCTION_ENTRY("Run_LogicFourInputAnd", 0x01, LogicFourInputAnd)
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
EHS_FB_IDENTIFY_FUNCTION(LogicFourInputAnd)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogicFourInputAnd)
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
EHS_FB_RUN_FUNCTION(LogicFourInputAnd)
{
    if (NCAPSA_bIn(0) && NCAPSA_bIn(1) && NCAPSA_bIn(2)  && NCAPSA_bIn(3))
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

/******************************************************************************/
/* Define LogicTwoInputOr function block */

EHS_FB_FUNCTIONS_START(LogicTwoInputOr)

EHS_FB_FUNCTION_ENTRY("Run_LogicTwoInputOr", 0x01, LogicTwoInputOr)
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
EHS_FB_IDENTIFY_FUNCTION(LogicTwoInputOr)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogicTwoInputOr)
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
EHS_FB_RUN_FUNCTION(LogicTwoInputOr)
{
    if (NCAPSA_bIn(0) || NCAPSA_bIn(1))
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


/******************************************************************************/
/* Define LogicThreeInputOr function block */

EHS_FB_FUNCTIONS_START(LogicThreeInputOr)

EHS_FB_FUNCTION_ENTRY("Run_LogicThreeInputOr", 0x01, LogicThreeInputOr)
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
EHS_FB_IDENTIFY_FUNCTION(LogicThreeInputOr)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogicThreeInputOr)
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
EHS_FB_RUN_FUNCTION(LogicThreeInputOr)
{
    if (NCAPSA_bIn(0) || NCAPSA_bIn(1) || NCAPSA_bIn(2))
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


/******************************************************************************/
/* Define xyzzy function block */

EHS_FB_FUNCTIONS_START(LogicFourInputOr)

EHS_FB_FUNCTION_ENTRY("Run_LogicFourInputOr", 0x01, LogicFourInputOr)
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
EHS_FB_IDENTIFY_FUNCTION(LogicFourInputOr)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogicFourInputOr)
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
EHS_FB_RUN_FUNCTION(LogicFourInputOr)
{
    if (NCAPSA_bIn(0) || NCAPSA_bIn(1) || NCAPSA_bIn(2) || NCAPSA_bIn(3))
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


/******************************************************************************/
/* Define LogicTwoInputNor function block */

EHS_FB_FUNCTIONS_START(LogicTwoInputNor)

EHS_FB_FUNCTION_ENTRY("Run_LogicTwoInputNor", 0x01, LogicTwoInputNor)
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
EHS_FB_IDENTIFY_FUNCTION(LogicTwoInputNor)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogicTwoInputNor)
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
EHS_FB_RUN_FUNCTION(LogicTwoInputNor)
{
    NCAPSA_bOut(0) = (ehs_bool)!(NCAPSA_bIn(0) || NCAPSA_bIn(1));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define LogicThreeInputNor function block */

EHS_FB_FUNCTIONS_START(LogicThreeInputNor)

EHS_FB_FUNCTION_ENTRY("Run_LogicThreeInputNor", 0x01, LogicThreeInputNor)
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
EHS_FB_IDENTIFY_FUNCTION(LogicThreeInputNor)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogicThreeInputNor)
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
EHS_FB_RUN_FUNCTION(LogicThreeInputNor)
{
    NCAPSA_bOut(0) = (ehs_bool)!(NCAPSA_bIn(0) || NCAPSA_bIn(1) || NCAPSA_bIn(2));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define xyzzy function block */

EHS_FB_FUNCTIONS_START(LogicFourInputNor)

EHS_FB_FUNCTION_ENTRY("Run_LogicFourInputNor", 0x01, LogicFourInputNor)
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
EHS_FB_IDENTIFY_FUNCTION(LogicFourInputNor)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogicFourInputNor)
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
EHS_FB_RUN_FUNCTION(LogicFourInputNor)
{
    NCAPSA_bOut(0) = (ehs_bool)!(NCAPSA_bIn(0) || NCAPSA_bIn(1) || NCAPSA_bIn(2) || NCAPSA_bIn(3));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define xyzzy function block */

EHS_FB_FUNCTIONS_START(LogicTwoInputNand)

EHS_FB_FUNCTION_ENTRY("Run_LogicTwoInputNand", 0x01, LogicTwoInputNand)
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
EHS_FB_IDENTIFY_FUNCTION(LogicTwoInputNand)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogicTwoInputNand)
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
EHS_FB_RUN_FUNCTION(LogicTwoInputNand)
{
    NCAPSA_bOut(0) = (ehs_bool)!(NCAPSA_bIn(0) && NCAPSA_bIn(1));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define xyzzy function block */

EHS_FB_FUNCTIONS_START(LogicThreeInputNand)

EHS_FB_FUNCTION_ENTRY("Run_LogicThreeInputNand", 0x01, LogicThreeInputNand)
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
EHS_FB_IDENTIFY_FUNCTION(LogicThreeInputNand)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogicThreeInputNand)
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
EHS_FB_RUN_FUNCTION(LogicThreeInputNand)
{
    NCAPSA_bOut(0) = (ehs_bool)!(NCAPSA_bIn(0) && NCAPSA_bIn(1) && NCAPSA_bIn(2));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define LogicFourInputNand function block */

EHS_FB_FUNCTIONS_START(LogicFourInputNand)

EHS_FB_FUNCTION_ENTRY("Run_LogicFourInputNand", 0x01, LogicFourInputNand)
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
EHS_FB_IDENTIFY_FUNCTION(LogicFourInputNand)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogicFourInputNand)
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
EHS_FB_RUN_FUNCTION(LogicFourInputNand)
{
    NCAPSA_bOut(0) = (ehs_bool)!(NCAPSA_bIn(0) && NCAPSA_bIn(1) && NCAPSA_bIn(2) && NCAPSA_bIn(3));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define xyzzy function block */

EHS_FB_FUNCTIONS_START(LogicXor)

EHS_FB_FUNCTION_ENTRY("Run_LogicXor", 0x01, LogicXor)
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
EHS_FB_IDENTIFY_FUNCTION(LogicXor)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogicXor)
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
EHS_FB_RUN_FUNCTION(LogicXor)
{
    NCAPSA_bOut(0) = (ehs_bool)(NCAPSA_bIn(0) ^ NCAPSA_bIn(1));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}


