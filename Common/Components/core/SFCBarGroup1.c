/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * SFCBarGroup1.cpp
 *
 * function definitions for SFBarGroup function blocks
 *
 * @author inx limited
 *
 */

#include "globals.h"
#include "setCompletes.h"
#include "hal-api.h" /* Required for logging */
#include "sfc.h"


/******************************************************************************/
/* Define SFCBarGroup1 function block */

EHS_FB_FUNCTIONS_START(SFCBarGroup1)

EHS_FB_FUNCTION_ENTRY("Run_OnEntry1", 0x00, SFCBarGroup1)

EHS_FB_FUNCTION_ENTRY("Run_CL1", 0x01, Run_CL)
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
EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup1)
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
EHS_FB_INIT_FUNCTION(SFCBarGroup1)
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
EHS_FB_RUN_FUNCTION(SFCBarGroup1)
{

    // Input Assignment
    ehs_bool bIn1 = NCAPSA_bIn(0);  //
    ehs_bool postStateActive;


    if(bIn1 == EHS_TRUE)
    {
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT);  //  Path 1
        postStateActive=EHS_FALSE;
    }
    else
    {
        postStateActive = EHS_TRUE;
    }


    *((ehs_bool*)EHS_FB_RUN_CONTEXT) = postStateActive;

    return;

}

/******************************************************************************/
/* Define SFCBarGroup2 function block */

EHS_FB_FUNCTIONS_START(SFCBarGroup2)

EHS_FB_FUNCTION_ENTRY("Run_OnEntry2", 0x00, SFCBarGroup2)

EHS_FB_FUNCTION_ENTRY("Run_CL1", 0x01, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL2", 0x02, Run_CL)
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
EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup2)
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
EHS_FB_INIT_FUNCTION(SFCBarGroup2)
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
EHS_FB_RUN_FUNCTION(SFCBarGroup2)
{

    // Input Assignment
    ehs_bool bIn1 = NCAPSA_bIn(0);  //
    ehs_bool bIn2 = NCAPSA_bIn(1);  //
    //ehs_bool bIn3 = NCAPSA_bIn(2);  //
    ehs_bool postStateActive;


    if(bIn1 == EHS_TRUE)
    {
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT);  //  Path 1
        postStateActive=EHS_FALSE;
    }
    else if(bIn2 == EHS_TRUE)
    {
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT);  //  Path 2
        postStateActive=EHS_FALSE;
    }
    else
    {
        postStateActive = EHS_TRUE;
    }


    *((ehs_bool*)EHS_FB_RUN_CONTEXT) = postStateActive;

    return;

}

/******************************************************************************/
/* Define SFCBarGroup3 function block */

EHS_FB_FUNCTIONS_START(SFCBarGroup3)

EHS_FB_FUNCTION_ENTRY("Run_OnEntry3", 0x00, SFCBarGroup3)

EHS_FB_FUNCTION_ENTRY("Run_CL1", 0x01, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL2", 0x02, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL3", 0x03, Run_CL)
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
EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup3)
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
EHS_FB_INIT_FUNCTION(SFCBarGroup3)
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
EHS_FB_RUN_FUNCTION(SFCBarGroup3)
{

    // Input Assignment
    ehs_bool bIn1 = NCAPSA_bIn(0);  //
    ehs_bool bIn2 = NCAPSA_bIn(1);  //
    ehs_bool bIn3 = NCAPSA_bIn(2);  //
    ehs_bool postStateActive;


    if(bIn1 == EHS_TRUE)
    {
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  Path 1
        postStateActive=EHS_FALSE;
    }
    else if(bIn2 == EHS_TRUE)
    {
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  Path 2
        postStateActive=EHS_FALSE;
    }
    else if(bIn3 == EHS_TRUE)
    {
        SetCompletes3((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  Path 3
        postStateActive=EHS_FALSE;
    }
    else
    {
        postStateActive = EHS_TRUE;
    }


    *((ehs_bool*)EHS_FB_RUN_CONTEXT) = postStateActive;

    return;

}

/******************************************************************************/
/* Define SFCBarGroup4 function block */

EHS_FB_FUNCTIONS_START(SFCBarGroup4)

EHS_FB_FUNCTION_ENTRY("Run_OnEntry4", 0x00, SFCBarGroup4)

EHS_FB_FUNCTION_ENTRY("Run_CL1", 0x01, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL2", 0x02, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL3", 0x03, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL4", 0x04, Run_CL)
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
EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup4)
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
EHS_FB_INIT_FUNCTION(SFCBarGroup4)
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
EHS_FB_RUN_FUNCTION(SFCBarGroup4)
{

    // Input Assignment
    ehs_bool bIn1 = NCAPSA_bIn(0);  //
    ehs_bool bIn2 = NCAPSA_bIn(1);  //
    ehs_bool bIn3 = NCAPSA_bIn(2);  //
    ehs_bool bIn4 = NCAPSA_bIn(3);  //
    ehs_bool postStateActive;


    if(bIn1 == EHS_TRUE)
    {
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  Path 1
        postStateActive=EHS_FALSE;
    }
    else if(bIn2 == EHS_TRUE)
    {
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  Path 2
        postStateActive=EHS_FALSE;
    }
    else if(bIn3 == EHS_TRUE)
    {
        SetCompletes3((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  Path 3
        postStateActive=EHS_FALSE;
    }
    else if(bIn4 == EHS_TRUE)
    {
        SetCompletes4((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  Path 4
        postStateActive=EHS_FALSE;
    }
    else
    {
        postStateActive = EHS_TRUE;
    }


    *((ehs_bool*)EHS_FB_RUN_CONTEXT) = postStateActive;

    return;

}

/******************************************************************************/
/* Define SFCBarGroup8 function block */

EHS_FB_FUNCTIONS_START(SFCBarGroup8)

EHS_FB_FUNCTION_ENTRY("Run_OnEntry8", 0x00, SFCBarGroup8)

EHS_FB_FUNCTION_ENTRY("Run_CL1", 0x01, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL2", 0x02, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL3", 0x03, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL4", 0x04, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL5", 0x05, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL6", 0x06, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL7", 0x07, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL8", 0x08, Run_CL)
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
EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup8)
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
EHS_FB_INIT_FUNCTION(SFCBarGroup8)
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
EHS_FB_RUN_FUNCTION(SFCBarGroup8)
{

    // Input Assignment
    ehs_bool bIn7 = NCAPSA_bIn(7);
    ehs_bool bIn6 = NCAPSA_bIn(6);
    ehs_bool bIn5 = NCAPSA_bIn(5);
    ehs_bool bIn4 = NCAPSA_bIn(4);
    ehs_bool bIn3 = NCAPSA_bIn(3);
    ehs_bool bIn2 = NCAPSA_bIn(2);
    ehs_bool bIn1 = NCAPSA_bIn(1);
    ehs_bool bIn0 = NCAPSA_bIn(0);

    ehs_bool postStateActive;


    if(bIn0 == EHS_TRUE)
    {
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        postStateActive=EHS_FALSE;
    }
    else if(bIn1 == EHS_TRUE)
    {
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        postStateActive=EHS_FALSE;
    }
    else if(bIn2 == EHS_TRUE)
    {
        SetCompletes3((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  Path3
        postStateActive=EHS_FALSE;
    }
    else if(bIn3 == EHS_TRUE)
    {
        SetCompletes4((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  Path4
        postStateActive=EHS_FALSE;
    }
    else if(bIn4 == EHS_TRUE)
    {
        SetCompletes5((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);   //  Path5
        postStateActive=EHS_FALSE;
    }
    else if(bIn5 == EHS_TRUE)
    {
        SetCompletes6((structFuncArg*)EHS_FB_RUN_CONTEXT_REF); //  Path6
        postStateActive=EHS_FALSE;
    }
    else if(bIn6 == EHS_TRUE)
    {
        SetCompletes7((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  Path7
        postStateActive=EHS_FALSE;
    }
    else if(bIn7 == EHS_TRUE)
    {
        SetCompletes8((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        postStateActive=EHS_FALSE;
    }
    else
    {
        postStateActive = EHS_TRUE;
    }
    *((ehs_bool*)EHS_FB_RUN_CONTEXT) = postStateActive;

    return;

}


/******************************************************************************/
/* Define SFCBarGroup12 function block */

EHS_FB_FUNCTIONS_START(SFCBarGroup12)

EHS_FB_FUNCTION_ENTRY("Run_OnEntry12", 0x00, SFCBarGroup12)

EHS_FB_FUNCTION_ENTRY("Run_CL1", 0x01, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL2", 0x02, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL3", 0x03, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL4", 0x04, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL5", 0x05, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL6", 0x06, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL7", 0x07, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL8", 0x08, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL9", 0x09, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL10", 0x0A, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL11", 0x0B, Run_CL)

EHS_FB_FUNCTION_ENTRY("Run_CL12", 0x0C, Run_CL)
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
EHS_FB_IDENTIFY_FUNCTION(SFCBarGroup12)
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
EHS_FB_INIT_FUNCTION(SFCBarGroup12)
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
EHS_FB_RUN_FUNCTION(SFCBarGroup12)
{
    ehs_bool bIn11 = NCAPSA_bIn(11);  //PP: Careful with these the MCU version has limited nos of inputs
    ehs_bool bIn10 = NCAPSA_bIn(10);
    ehs_bool bIn9 = NCAPSA_bIn(9);
    ehs_bool bIn8 = NCAPSA_bIn(8);
    ehs_bool bIn7 = NCAPSA_bIn(7);
    ehs_bool bIn6 = NCAPSA_bIn(6);
    ehs_bool bIn5 = NCAPSA_bIn(5);
    ehs_bool bIn4 = NCAPSA_bIn(4);
    ehs_bool bIn3 = NCAPSA_bIn(3);
    ehs_bool bIn2 = NCAPSA_bIn(2);
    ehs_bool bIn1 = NCAPSA_bIn(1);
    ehs_bool bIn0 = NCAPSA_bIn(0);

    ehs_bool postStateActive;


    if(bIn0 == EHS_TRUE)
    {
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        postStateActive=EHS_FALSE;
    }
    else if(bIn1 == EHS_TRUE)
    {
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        postStateActive=EHS_FALSE;
    }
    else if(bIn2 == EHS_TRUE)
    {
        SetCompletes3((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  Path3
        postStateActive=EHS_FALSE;
    }
    else if(bIn3 == EHS_TRUE)
    {
        SetCompletes4((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  Path4
        postStateActive=EHS_FALSE;
    }
    else if(bIn4 == EHS_TRUE)
    {
        SetCompletes5((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);   //  Path5
        postStateActive=EHS_FALSE;
    }
    else if(bIn5 == EHS_TRUE)
    {
        SetCompletes6((structFuncArg*)EHS_FB_RUN_CONTEXT_REF); //  Path6
        postStateActive=EHS_FALSE;
    }
    else if(bIn6 == EHS_TRUE)
    {
        SetCompletes7((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);  //  Path7
        postStateActive=EHS_FALSE;
    }
    else if(bIn7 == EHS_TRUE)
    {
        SetCompletes8((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        postStateActive=EHS_FALSE;
    }
    else if(bIn7 == EHS_TRUE)
    {
        SetCompletes8((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        postStateActive=EHS_FALSE;
    }
    else if(bIn8 == EHS_TRUE)
    {
        EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,9);
        postStateActive=EHS_FALSE;
    }
    else if(bIn9 == EHS_TRUE)
    {
        EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,10);
        postStateActive=EHS_FALSE;
    }
    else if(bIn10 == EHS_TRUE)
    {
        EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,11);
        postStateActive=EHS_FALSE;
    }
    else if(bIn11 == EHS_TRUE)
    {
        EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,12);
        postStateActive=EHS_FALSE;
    }
    else
    {
        postStateActive = EHS_TRUE;
    }
    *((ehs_bool*)EHS_FB_RUN_CONTEXT) = postStateActive;
    return;

}
/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(Run_CL)
{
    ehs_bool bIn = NCAPSA_bIn(0);
    ehs_bool postStateActive = *((ehs_bool*)EHS_FB_RUN_CONTEXT);
    if((bIn == EHS_TRUE)&&(postStateActive==EHS_TRUE))
    {
        SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
        postStateActive=EHS_FALSE;
        *((ehs_bool*)EHS_FB_RUN_CONTEXT) = postStateActive;
    }
    return;
}
