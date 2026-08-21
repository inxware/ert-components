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

#include "globals.h" /* required for trace */
#include "const1.h"
#include "setCompletes.h"
#include "hal-api.h" /* Required for logging */


/******************************************************************************/
/* Define Constant Integer function block */

EHS_FB_FUNCTIONS_START(ConstantInt1)

EHS_FB_FUNCTION_ENTRY("Run_ConstantInt", 0x01, ConstantInt1)
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
EHS_FB_IDENTIFY_FUNCTION(ConstantInt1)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(ConstantInt1));
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
EHS_FB_INIT_FUNCTION(ConstantInt1)
{
    EhsDataflowIntType value;
    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(ConstantInt1));
    value = atol(EHS_FB_INIT_PARAMETERS);
    *(EhsDataflowIntType*)EHS_FB_INIT_CONTEXT = value;//atol(pParams);
    *(EhsDataflowIntType*)((structFuncArg*)EHS_FB_RUN_CONTEXT_REF)->pOut[0] = *(ehs_sint32*)((ehs_uint8*)EHS_FB_INIT_CONTEXT);
    return EHS_TRUE; /* initialisation always succeeds */
}

EHS_FB_RUN_FUNCTION(ConstantInt1)
{
}

/******************************************************************************/
/* Define Constant string function block */

EHS_FB_FUNCTIONS_START(ConstantString1)

EHS_FB_FUNCTION_ENTRY("Run_ConstantString", 0x01, ConstantString1)
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
EHS_FB_IDENTIFY_FUNCTION(ConstantString1)
{
    /* we need space for the length of the string plus one NULL...*/
    EHS_FB_IDENTIFY_MEMORY = (ehs_uint32)(( EhsStrlen(EHS_FB_IDENTIFY_PARAMETERS)+ 1) *sizeof(ehs_char));
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ConstantString1)
{
    ehs_char * obj=(ehs_char *)EHS_FB_INIT_CONTEXT;
    //int i;
    //deleteme=1
    /** @todo handle processing of parameters - currently not possible as parameters are constant */
    if ((EHS_FB_INIT_PARAMETERS[0]=='\\')&&(EHS_FB_INIT_PARAMETERS[1]=='r'))
    {
        obj[0] = 13;
        obj[1] = '\0';
    }
    else if ((EHS_FB_INIT_PARAMETERS[0]=='\\')&&(EHS_FB_INIT_PARAMETERS[1]=='n'))
    {
        obj[0] = 10;
        obj[1] = '\0';
    }
    else if (EhsStrncmp(EHS_FB_INIT_PARAMETERS, "NULL", EhsStrlen("NULL")) == 0)
    {
        obj[0] = '\0';
    }/* if we have two spaces as the first byte then shift everything to the beginning (not sure why, but something Lucid shouldn't e doing and will be replacing spaces with 0xF1s now anyway)*/
    else if (EHS_FB_INIT_PARAMETERS[0] == ' ' && (EHS_FB_INIT_PARAMETERS[1] == ' ' || EHS_FB_INIT_PARAMETERS[1] == '\0' ))
    {
        //for (i=0;i<EnsStrlen(EHS_FB_INIT_PARAMETERS);i++) {if (EHS_FB_INIT_PARAMETERS[i]!=0) deleteme=1;}
        //EhsStrcpy(obj, &EHS_FB_INIT_PARAMETERS[1]);
        //if (NULL == 
            EhsGetRecordFromString(obj, &EHS_FB_INIT_PARAMETERS[1],(ehs_uint32)(( EhsStrlen(EHS_FB_INIT_PARAMETERS)+ 1) *sizeof(ehs_char)) ); 
        // ) {
        //    printf ("Bad constant value - too long?\n");
        //    return EHS_FALSE; // yes we do want to bail if the constant is not right.
        //}
    }
    else
    {
    //    EhsStrcpy(obj, EHS_FB_INIT_PARAMETERS);

        //if (NULL == we can't actually test for this after all - if only one record is present the return value is NULL too.
            EhsGetRecordFromString(obj, EHS_FB_INIT_PARAMETERS,(ehs_uint32)(( EhsStrlen(EHS_FB_INIT_PARAMETERS) + 1) * sizeof(ehs_char)) ) ;
            // ) {
            //printf ("Bad constant value - too long?\n");
            //return EHS_FALSE; // yes we do want to bail if the constant is not right.
        //}
    
    }

    EhsRunConstantString1(&pCallbackTable[0]); // run the callback function now with the callback PFI data supplied to init
    return EHS_TRUE; /* initialisation always succeeds */
}

//called by call-back
EHS_FB_RUN_FUNCTION(ConstantString1)
{
    EHS_FB_OUT_S_SET(0, (ehs_char*)EHS_FB_RUN_CONTEXT);
}

/******************************************************************************/
/* Define ConstantBool function block */

EHS_FB_FUNCTIONS_START(ConstantBool1)

EHS_FB_FUNCTION_ENTRY("Run_ConstantBool", 0x01, ConstantBool1)
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
EHS_FB_IDENTIFY_FUNCTION(ConstantBool1)
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
EHS_FB_INIT_FUNCTION(ConstantBool1) //@todo need to document the init API that call-backs and normal PFI data is presented here. This is limited as only one FI configured call-back can be installed.
{
    *(int*)EHS_FB_INIT_CONTEXT = atoi(EHS_FB_INIT_PARAMETERS);
    NCAPSA_bOut(0) = *(ehs_bool*)EHS_FB_INIT_CONTEXT;
    return EHS_TRUE; /* initialisation always succeeds */
}

EHS_FB_RUN_FUNCTION(ConstantBool1)
{
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define ConstantReal function block */

EHS_FB_FUNCTIONS_START(ConstantFloat1)

EHS_FB_FUNCTION_ENTRY("Run_ConstantReal", 0x01, ConstantFloat1)
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
EHS_FB_IDENTIFY_FUNCTION(ConstantFloat1)
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
EHS_FB_INIT_FUNCTION(ConstantFloat1)
{
    *(ehs_float*)EHS_FB_INIT_CONTEXT = atof(EHS_FB_INIT_PARAMETERS);
    NCAPSA_dOut(0) = *(ehs_float*)EHS_FB_INIT_CONTEXT;
    return EHS_TRUE; /* initialisation always succeeds */
}

EHS_FB_RUN_FUNCTION(ConstantFloat1)
{
}

#endif
