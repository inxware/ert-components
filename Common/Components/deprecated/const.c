/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/* const.c
 *
 * src file for constant functions.  Constant functions output a constant.
 * Any function appearing in this file must also be listed in objRefTable.c
 * for it to be included in the list of functions available to the EHS.
 *
 * For definition of arguments in Ncapsa functions (Identify_, Init_ and Run_)
 * please see types.h.
 *
*/

#include "target.h"
#include "globals.h" /* required for trace */
#include "const.h"
#include "setCompletes.h"

#include "hal-api.h" /* Needed for logging */

/******************************************************************************/
/* Define Constant Integer function block */

EHS_FB_FUNCTIONS_START(ConstantInt)

EHS_FB_FUNCTION_ENTRY("Run_ConstantInt", 0x01, ConstantInt)
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
EHS_FB_IDENTIFY_FUNCTION(ConstantInt)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(ConstantInt));
    /* we require the space to hold an int */
    EHS_FB_IDENTIFY_MEMORY  = sizeof(EhsDataflowIntType);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConstantInt)
{
    EhsDataflowIntType value;
    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(ConstantInt));
    value = atol(EHS_FB_INIT_PARAMETERS);
    *(EhsDataflowIntType*)EHS_FB_INIT_CONTEXT = value;//atol(pParams);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConstantInt)
{
    /* simply copy the state data to the only output location...*/
    /*NCAPSA_nOut(0) equals *(int*)((structFuncArg*)pByte)->pOut[0]*/
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(ConstantInt));

    *(EhsDataflowIntType*)((structFuncArg*)EHS_FB_RUN_CONTEXT_REF)->pOut[0] = *(ehs_sint32*)((ehs_uint8*)EHS_FB_RUN_CONTEXT);
    /*NCAPSA_nOut(0) = *(ehs_sint32*)((ehs_uint8*)EHS_FB_RUN_CONTEXT);*/
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define Constant string function block */

EHS_FB_FUNCTIONS_START(ConstantString)

EHS_FB_FUNCTION_ENTRY("Run_ConstantString", 0x01, ConstantString)
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
EHS_FB_IDENTIFY_FUNCTION(ConstantString)
{
    /* we need space for the length of the string plus one NULL...*/
    EHS_FB_IDENTIFY_MEMORY = (ehs_uint32)EhsStrlen(EHS_FB_IDENTIFY_PARAMETERS) + 1;
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConstantString)
{
    /** @todo handle processing of parameters - currently not possible as paramaters are constant */
#ifdef EHS_TODO_CONST_FUNCTIONS_H
    if ((EHS_FB_INIT_PARAMETERS[0]=='\\')&&(EHS_FB_INIT_PARAMETERS[1]=='r'))
    {
        //PP:Untested modification for avoiding strlib
        EHS_FB_INIT_PARAMETERS[0] = 13;
        EHS_FB_INIT_PARAMETERS[1] = '\0';
    }
    else if ((EHS_FB_INIT_PARAMETERS[0]=='\\')&&(EHS_FB_INIT_PARAMETERS[1]=='n'))
    {
        //PP:Untested modification for avoiding strlib
        EHS_FB_INIT_PARAMETERS[0] = 10;
        EHS_FB_INIT_PARAMETERS[1] = '\0';
        //pParams[2] = '\0';

    }
#endif /* EHS_TODO_CONST_FUNCTIONS_H */
    EHS_FB_INIT_CONTEXT = EhsStrcpy((char *)EHS_FB_INIT_CONTEXT, EHS_FB_INIT_PARAMETERS);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConstantString)
{
    /* write the string to the location of the output...*/
    EhsStrcpy(NCAPSA_szOut(0), (char*)EHS_FB_RUN_CONTEXT);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
}

/******************************************************************************/
/* Define ConstantBool function block */

EHS_FB_FUNCTIONS_START(ConstantBool)

EHS_FB_FUNCTION_ENTRY("Run_ConstantBool", 0x01, ConstantBool)
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
EHS_FB_IDENTIFY_FUNCTION(ConstantBool)
{
    /* we require the space to hold an int */
    EHS_FB_IDENTIFY_MEMORY  = sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConstantBool)
{
    *(int*)EHS_FB_INIT_CONTEXT = atoi(EHS_FB_INIT_PARAMETERS);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConstantBool)
{
    /* simply copy the state data to the only output location...*/
    NCAPSA_bOut(0) = *(ehs_bool*)EHS_FB_RUN_CONTEXT;
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define ConstantReal function block */

EHS_FB_FUNCTIONS_START(ConstantFloat)

EHS_FB_FUNCTION_ENTRY("Run_ConstantReal", 0x01, ConstantFloat)
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
EHS_FB_IDENTIFY_FUNCTION(ConstantFloat)
{
    /* we require the space to hold a ehs_float */
    EHS_FB_IDENTIFY_MEMORY  = sizeof(ehs_float);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConstantFloat)
{
    *(ehs_float*)EHS_FB_INIT_CONTEXT = (ehs_float)atof(EHS_FB_INIT_PARAMETERS);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ConstantFloat)
{
    /* simply copy the state data to the only output location...*/
    NCAPSA_dOut(0) = *(ehs_float*)EHS_FB_RUN_CONTEXT;
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif
