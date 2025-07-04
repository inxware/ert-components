/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * trigger.h
 *
 * functions that operate on completes/triggers.  These typically take the
 * form of small logic functions, often with a latching property.  They are
 * used to combine mutiple triggers into one which can be used to trigger a
 * function.
 *
 * @author: inx limited
 *
 */
#include "setCompletes.h"
#include "target.h"
#include "hal-api.h" /* Required for logging */
#include "trigger.h"


/******************************************************************************/
/* Define EventLatchingAnd function block */

EHS_FB_FUNCTIONS_START(EventLatchingAnd)

EHS_FB_FUNCTION_ENTRY("And1", 0x01, EventLatchingAnd1)

EHS_FB_FUNCTION_ENTRY("And2", 0x02, EventLatchingAnd2)

EHS_FB_FUNCTION_ENTRY("Reset", 0x03, EventLatchingAndReset)
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
EHS_FB_IDENTIFY_FUNCTION(EventLatchingAnd)
{
    /* reserve state data for two BOOLs...*/
    EHS_FB_IDENTIFY_MEMORY = 2 * sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventLatchingAnd)
{
    ehs_bool* pBOOL = NULL;

    /* cast the pointer to type ehs_bool...*/
    pBOOL = (ehs_bool*)EHS_FB_INIT_CONTEXT;
    /* make sure both BOOLS are initially false...*/
    pBOOL[0] = EHS_FALSE; // this indicates the 
    pBOOL[1] = EHS_FALSE;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventLatchingAnd1)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as BOOLs...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[0] = EHS_TRUE;
    if (p[1] == EHS_TRUE)
    {
        EHS_FB_FINISH(1);
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
EHS_FB_RUN_FUNCTION(EventLatchingAnd2)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[1] = EHS_TRUE;
    if (p[0] == EHS_TRUE)
    {
        EHS_FB_FINISH(1);
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
EHS_FB_RUN_FUNCTION(EventLatchingAndReset)
{
    ehs_bool* p = NULL;

    /* simply reset the the latches whatever state they are in...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[0] = EHS_FALSE;
    p[1] = EHS_FALSE;
    EHS_FB_FINISH(1);
    return;
}

/******************************************************************************/
/* Define Start function block */

EHS_FB_FUNCTIONS_START(Start)

EHS_FB_FUNCTION_ENTRY("Run_Start", 0x01, Start)
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
EHS_FB_IDENTIFY_FUNCTION(Start)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(Start)
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
EHS_FB_RUN_FUNCTION(Start)
{
    /* simply set the start completes in their proper locations...*/
    EHS_FB_FINISH(1);
    return;
}

/******************************************************************************/
/* Define EventOr function block */

EHS_FB_FUNCTIONS_START(EventOr)

EHS_FB_FUNCTION_ENTRY("Run_TriggerOr", 0x01, EventOr)
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
EHS_FB_IDENTIFY_FUNCTION(EventOr)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventOr)
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
EHS_FB_RUN_FUNCTION(EventOr)
{
    ehs_bool bIn1;
    ehs_bool bIn2;
    /* output tigger is asserted if either of the inputs is high...*/
    bIn1 = NCAPSA_bIn(0);
    bIn2 = NCAPSA_bIn(1);

    if (bIn1 || bIn2)
    {
        EHS_FB_FINISH(1);
    }
}

/******************************************************************************/
/* Define EventBooleanControl function block */

EHS_FB_FUNCTIONS_START(EventBooleanControl)

EHS_FB_FUNCTION_ENTRY("Update", 0x01, EventBooleanControl)
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
EHS_FB_IDENTIFY_FUNCTION(EventBooleanControl)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventBooleanControl)
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
EHS_FB_RUN_FUNCTION(EventBooleanControl)
{
    ehs_bool bIn1;
    //ehs_bool bIn2;
    /* output tigger is asserted if either of the inputs is high...*/
    bIn1 = NCAPSA_bIn(0);
    //bIn2 = NCAPSA_bIn(1);
    if (bIn1)
    {
        EHS_FB_FINISH(1);
    }
    else
    {
        EHS_FB_FINISH(2);
    }
    return;
}

/******************************************************************************/
/* Define EventTwoInputLatchingAnd function block */

EHS_FB_FUNCTIONS_START(EventTwoInputLatchingAnd)

EHS_FB_FUNCTION_ENTRY("And1", 0x01, EventTwoInputLatchingAnd1)

EHS_FB_FUNCTION_ENTRY("And2", 0x02, EventTwoInputLatchingAnd2)

EHS_FB_FUNCTION_ENTRY("Reset", 0x03, EventTwoInputLatchingAndReset)
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
EHS_FB_IDENTIFY_FUNCTION(EventTwoInputLatchingAnd)
{
    /* reserve state data for two BOOLs...*/
    EHS_FB_IDENTIFY_MEMORY = 2 * sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventTwoInputLatchingAnd)
{
    ehs_bool* 	p = (ehs_bool*)EHS_FB_INIT_CONTEXT;
    /* make sure both BOOLS are initially false...*/
    p[0] = EHS_FALSE;
    p[1] = EHS_FALSE;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventTwoInputLatchingAnd1)
{
    ehs_bool* p = (ehs_bool*)EHS_FB_RUN_CONTEXT;

    /* the triggers are stored as BOOLs...*/
    p[0] = EHS_TRUE;
    if (p[1] == EHS_TRUE)
    {
        EHS_FB_FINISH(1);
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
EHS_FB_RUN_FUNCTION(EventTwoInputLatchingAnd2)
{
    ehs_bool* p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[1] = EHS_TRUE;
    if (p[0] == EHS_TRUE)
    {
        EHS_FB_FINISH(1);
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
EHS_FB_RUN_FUNCTION(EventTwoInputLatchingAndReset)
{
    ehs_bool* p = NULL;
    /* simply reset the the latches whatever state they are in...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[0] = EHS_FALSE;
    p[1] = EHS_FALSE;
    EHS_FB_FINISH(1);
    return;
}

// Three input latching
/******************************************************************************/
/* Define EventThreeInputLatchingAnd function block */

EHS_FB_FUNCTIONS_START(EventThreeInputLatchingAnd)

EHS_FB_FUNCTION_ENTRY("And1", 0x01, EventThreeInputLatchingAnd1)

EHS_FB_FUNCTION_ENTRY("And2", 0x02, EventThreeInputLatchingAnd2)

EHS_FB_FUNCTION_ENTRY("And3", 0x03, EventThreeInputLatchingAnd3)

EHS_FB_FUNCTION_ENTRY("Reset", 0x04, EventThreeInputLatchingAndReset)
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
EHS_FB_IDENTIFY_FUNCTION(EventThreeInputLatchingAnd)
{
    /* reserve state data for two BOOLs...*/
    EHS_FB_IDENTIFY_MEMORY = 3 * sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventThreeInputLatchingAnd)
{
    ehs_bool* pBOOL = NULL;

    /* cast the pointer to type ehs_bool...*/
    pBOOL = (ehs_bool*)EHS_FB_INIT_CONTEXT;
    /* make sure all three BOOLS are initially false...*/
    pBOOL[0] = EHS_FALSE;
    pBOOL[1] = EHS_FALSE;
    pBOOL[2] = EHS_FALSE;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventThreeInputLatchingAnd1)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as BOOLs...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[0] = EHS_TRUE;
    if (p[1] == EHS_TRUE && p[2] == EHS_TRUE)
    {
        EHS_FB_FINISH(1);
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
EHS_FB_RUN_FUNCTION(EventThreeInputLatchingAnd2)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[1] = EHS_TRUE;
    if (p[0] == EHS_TRUE && p[2] == EHS_TRUE)
    {
        EHS_FB_FINISH(1);
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
EHS_FB_RUN_FUNCTION(EventThreeInputLatchingAnd3)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[2] = EHS_TRUE;
    if (p[0] == EHS_TRUE && p[1] == EHS_TRUE)
    {
        EHS_FB_FINISH(1);
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
EHS_FB_RUN_FUNCTION(EventThreeInputLatchingAndReset)
{
    ehs_bool* p = NULL;

    /* simply reset the the latches whatever state they are in...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[0] = EHS_FALSE;
    p[1] = EHS_FALSE;
    p[2] = EHS_FALSE;
    EHS_FB_FINISH(1);
    return;
}

/******************************************************************************/
/* Define EventFourInputLatchingAnd function block */

EHS_FB_FUNCTIONS_START(EventFourInputLatchingAnd)

EHS_FB_FUNCTION_ENTRY("And1", 0x01, EventFourInputLatchingAnd1)

EHS_FB_FUNCTION_ENTRY("And2", 0x02, EventFourInputLatchingAnd2)

EHS_FB_FUNCTION_ENTRY("And3", 0x03, EventFourInputLatchingAnd3)

EHS_FB_FUNCTION_ENTRY("And4", 0x04, EventFourInputLatchingAnd4)

EHS_FB_FUNCTION_ENTRY("Reset", 0x05, EventFourInputLatchingAndReset)
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
EHS_FB_IDENTIFY_FUNCTION(EventFourInputLatchingAnd)
{
    /* reserve state data for two BOOLs...*/
    EHS_FB_IDENTIFY_MEMORY = 4 * sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventFourInputLatchingAnd)
{
    ehs_bool* pBOOL = NULL;

    /* cast the pointer to type ehs_bool...*/
    pBOOL = (ehs_bool*)EHS_FB_INIT_CONTEXT;
    /* make sure both BOOLS are initially false...*/
    pBOOL[0] = EHS_FALSE;
    pBOOL[1] = EHS_FALSE;
    pBOOL[2] = EHS_FALSE;
    pBOOL[3] = EHS_FALSE;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventFourInputLatchingAnd1)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as BOOLs...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[0] = EHS_TRUE;
    if (p[1] == EHS_TRUE && p[2] == EHS_TRUE && p[3] == EHS_TRUE)
    {
        EHS_FB_FINISH(1);
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
EHS_FB_RUN_FUNCTION(EventFourInputLatchingAnd2)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[1] = EHS_TRUE;
    if (p[0] == EHS_TRUE && p[2] == EHS_TRUE && p[3] == EHS_TRUE)
    {
        EHS_FB_FINISH(1);
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
EHS_FB_RUN_FUNCTION(EventFourInputLatchingAnd3)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[2] = EHS_TRUE;
    if (p[0] == EHS_TRUE && p[1] == EHS_TRUE && p[3] == EHS_TRUE)
    {
        EHS_FB_FINISH(1);
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
EHS_FB_RUN_FUNCTION(EventFourInputLatchingAnd4)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[3] = EHS_TRUE;
    if (p[0] == EHS_TRUE && p[1] == EHS_TRUE && p[2] == EHS_TRUE)
    {
        EHS_FB_FINISH(1);
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
EHS_FB_RUN_FUNCTION(EventFourInputLatchingAndReset)
{
    ehs_bool* p = NULL;

    /* simply reset the the latches whatever state they are in...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[0] = EHS_FALSE;
    p[1] = EHS_FALSE;
    p[2] = EHS_FALSE;
    p[3] = EHS_FALSE;
    EHS_FB_FINISH(1);
    return;
}

/******************************************************************************/
/* Define EventOneInputManRstAnd function block */

EHS_FB_FUNCTIONS_START(EventOneInputManRstAnd)

EHS_FB_FUNCTION_ENTRY("And1", 0x01, EventOneInputManRstAnd1)

EHS_FB_FUNCTION_ENTRY("Reset", 0x02, EventOneInputManRstAndReset)
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
EHS_FB_IDENTIFY_FUNCTION(EventOneInputManRstAnd)
{
    /* reserve state data for two BOOLs...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventOneInputManRstAnd)
{
    ehs_bool* 	p = (ehs_bool*)EHS_FB_INIT_CONTEXT;
    /* make sure both BOOLS are initially false...*/
    p[0] = EHS_FALSE;	// 'has fired' bool
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function asserts output only once when all inputs have been asserted.
 * The reset must be asserted before any further outputs are emitted. 
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventOneInputManRstAnd1)
{
    ehs_bool* p = (ehs_bool*)EHS_FB_RUN_CONTEXT;

    /* a one input resetting AND acts as a gate, will only fire if reset and will not fire again until reset again */
    if (p[0] == EHS_FALSE)
    {
        p[0] = EHS_TRUE; // set 'has fired' to true
        EHS_FB_FINISH(1);
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
EHS_FB_RUN_FUNCTION(EventOneInputManRstAndReset)
{
    ehs_bool* p = NULL;
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[0] = EHS_FALSE;	// reset 'has fired' flag
    EHS_FB_FINISH(1);
    return;
}

/******************************************************************************/
/* Define EventTwoInputManRstAnd function block */

EHS_FB_FUNCTIONS_START(EventTwoInputManRstAnd)

EHS_FB_FUNCTION_ENTRY("And1", 0x01, EventTwoInputManRstAnd1)

EHS_FB_FUNCTION_ENTRY("And2", 0x02, EventTwoInputManRstAnd2)

EHS_FB_FUNCTION_ENTRY("Reset", 0x03, EventTwoInputManRstAndReset)
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
EHS_FB_IDENTIFY_FUNCTION(EventTwoInputManRstAnd)
{
    /* reserve state data for two BOOLs...*/
    EHS_FB_IDENTIFY_MEMORY = 2 * sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventTwoInputManRstAnd)
{
    ehs_bool* 	p = (ehs_bool*)EHS_FB_INIT_CONTEXT;
    /* make sure both BOOLS are initially false...*/
    p[0] = EHS_FALSE;	
    p[1] = EHS_FALSE;   
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function asserts output only once when all inputs have been asserted.
 * The reset must be asserted before any further outputs are emitted. 
 * 
 * Coded for speed over code size
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventTwoInputManRstAnd1)
{
    ehs_bool* p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    /* the triggers are stored as BOOLs...*/
    if ((p[0] == EHS_FALSE) && (p[1] == EHS_TRUE))
    {
        p[0] = EHS_TRUE; 
        EHS_FB_FINISH(1);
    }
    else {
        p[0] = EHS_TRUE; 
    }
    return;
}

/**
 * Run the function See above.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventTwoInputManRstAnd2)
{
    ehs_bool* p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if ((p[0] == EHS_TRUE) && (p[1] == EHS_FALSE))
    {
        p[1] = EHS_TRUE; 
        EHS_FB_FINISH(1);
    }
    else {
        p[1] = EHS_TRUE;  
    }
    return;
}

/**
 * Run the function : See above.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventTwoInputManRstAndReset)
{
    ehs_bool* p = NULL;
    /* simply reset the the latches whatever state they are in...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[0] = EHS_FALSE;	// reset 'has fired' flag
    p[1] = EHS_FALSE;
    EHS_FB_FINISH(1);
    return;
}

// Three input latching
/******************************************************************************/
/* Define EventThreeInputManRstAnd function block */

EHS_FB_FUNCTIONS_START(EventThreeInputManRstAnd)

EHS_FB_FUNCTION_ENTRY("And1", 0x01, EventThreeInputManRstAnd1)

EHS_FB_FUNCTION_ENTRY("And2", 0x02, EventThreeInputManRstAnd2)

EHS_FB_FUNCTION_ENTRY("And3", 0x03, EventThreeInputManRstAnd3)

EHS_FB_FUNCTION_ENTRY("Reset", 0x04, EventThreeInputManRstAndReset)
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
EHS_FB_IDENTIFY_FUNCTION(EventThreeInputManRstAnd)
{
    /* reserve state data for two BOOLs...*/
    EHS_FB_IDENTIFY_MEMORY = 3 * sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventThreeInputManRstAnd)
{
    ehs_bool* p = NULL;

    /* cast the pointer to type ehs_bool...*/
    p = (ehs_bool*)EHS_FB_INIT_CONTEXT;
    /* make sure all three BOOLS are initially false...*/
    p[0] = EHS_FALSE;	// 'has fired' bool
    p[1] = EHS_FALSE;
    p[2] = EHS_FALSE;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * (See above))
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventThreeInputManRstAnd1)
{
    ehs_bool* p = NULL;
    /* the triggers are stored as BOOLs...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[0] == EHS_FALSE && (p[1] && p[2]))
    {
        p[0] = EHS_TRUE; // set 'has fired' to true
        EHS_FB_FINISH(1);
    }
    else {
        p[0] = EHS_TRUE;
    }
    return;
}

/**
 * see above.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventThreeInputManRstAnd2)
{
    ehs_bool* p = NULL;
    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[1] == EHS_FALSE && (p[0] && p[2]))
    {
        p[1] = EHS_TRUE; // set 'has fired' to true
        EHS_FB_FINISH(1);
    }
    else {
        p[1] = EHS_TRUE;
    }
    return;
}

/**
 * see above
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventThreeInputManRstAnd3)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[2] == EHS_FALSE && (p[0] && p[1]))
    {
        p[2] = EHS_TRUE; // set 'has fired' to true
        EHS_FB_FINISH(1);
    }
    else {
        p[2] = EHS_TRUE;
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
EHS_FB_RUN_FUNCTION(EventThreeInputManRstAndReset)
{
    ehs_bool* p = NULL;

    /* simply reset the the latches whatever state they are in...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[0] = EHS_FALSE;	// reset 'has fired' bool
    p[1] = EHS_FALSE;
    p[2] = EHS_FALSE;
    EHS_FB_FINISH(1);
    return;
}

/******************************************************************************/
/* Define EventFourInputManRstAnd function block */

EHS_FB_FUNCTIONS_START(EventFourInputManRstAnd)

EHS_FB_FUNCTION_ENTRY("And1", 0x01, EventFourInputManRstAnd1)

EHS_FB_FUNCTION_ENTRY("And2", 0x02, EventFourInputManRstAnd2)

EHS_FB_FUNCTION_ENTRY("And3", 0x03, EventFourInputManRstAnd3)

EHS_FB_FUNCTION_ENTRY("And4", 0x04, EventFourInputManRstAnd4)

EHS_FB_FUNCTION_ENTRY("Reset", 0x05, EventFourInputManRstAndReset)
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
EHS_FB_IDENTIFY_FUNCTION(EventFourInputManRstAnd)
{
    /* reserve state data for 4 BOOLs...*/
    EHS_FB_IDENTIFY_MEMORY = 4 * sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventFourInputManRstAnd)
{
    ehs_bool* p = NULL;

    /* cast the pointer to type ehs_bool...*/
    p = (ehs_bool*)EHS_FB_INIT_CONTEXT;
    /* make sure all three BOOLS are initially false...*/
    p[0] = EHS_FALSE;	// 'has fired' bool
    p[1] = EHS_FALSE;
    p[2] = EHS_FALSE;
    p[3] = EHS_FALSE;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventFourInputManRstAnd1)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as BOOLs...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[0] == EHS_FALSE && (p[1] && p[2] && p[3]))
    {
        p[0] = EHS_TRUE; // set 'has fired' to true
        EHS_FB_FINISH(1);
    }
    else {
        p[0] = EHS_TRUE;
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
EHS_FB_RUN_FUNCTION(EventFourInputManRstAnd2)
{
    ehs_bool* p = NULL;
    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[1] == EHS_FALSE && ( p[0] && p[2] && p[3] ))
    {
        p[1] = EHS_TRUE;
        EHS_FB_FINISH(1);
    }
    else {
        p[1] = EHS_TRUE;
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
EHS_FB_RUN_FUNCTION(EventFourInputManRstAnd3)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[2] == EHS_FALSE && ( p[0] && p[1] && p[3] ))
    {
        p[2] = EHS_TRUE; 
        EHS_FB_FINISH(1);
    }
    else {
        p[2] = EHS_TRUE;
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
EHS_FB_RUN_FUNCTION(EventFourInputManRstAnd4)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[3] == EHS_FALSE && (p[0] && p[1] && p[2] ))
    {
        p[3] = EHS_TRUE; // set 'has fired' to true
        EHS_FB_FINISH(1);
    }
    else {
         p[3] = EHS_TRUE;
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
EHS_FB_RUN_FUNCTION(EventFourInputManRstAndReset)
{
    ehs_bool* p = NULL;

    /* simply reset the the latches whatever state they are in...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    p[0] = EHS_FALSE; 	// reset 'has fired' bool
    p[1] = EHS_FALSE;
    p[2] = EHS_FALSE;
    p[3] = EHS_FALSE;
    EHS_FB_FINISH(1);
    return;
}

/******************************************************************************/
/* Define EventSelfResetTwoInputLatchingAnd function block */

EHS_FB_FUNCTIONS_START(EventSelfResetTwoInputLatchingAnd)

EHS_FB_FUNCTION_ENTRY("And1", 0x01, EventSelfResetTwoInputLatchingAnd1)

EHS_FB_FUNCTION_ENTRY("And2", 0x02, EventSelfResetTwoInputLatchingAnd2)
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
EHS_FB_IDENTIFY_FUNCTION(EventSelfResetTwoInputLatchingAnd)
{
    /* reserve state data for two BOOLs...*/
    EHS_FB_IDENTIFY_MEMORY = 2 * sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventSelfResetTwoInputLatchingAnd)
{
    ehs_bool* pBOOL = NULL;
    /* cast the pointer to type ehs_bool...*/
    pBOOL = (ehs_bool*)EHS_FB_INIT_CONTEXT;
    /* make sure both BOOLS are initially false...*/
    pBOOL[0] = EHS_FALSE;
    pBOOL[1] = EHS_FALSE;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventSelfResetTwoInputLatchingAnd1)
{
    ehs_bool* p = NULL;
    /* the triggers are stored as BOOLs...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[1] == EHS_TRUE)
    {
        p[0] = EHS_FALSE;
        p[1] = EHS_FALSE;
        EHS_FB_FINISH(1);
    }
    else
    {
        p[0] = EHS_TRUE;
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
EHS_FB_RUN_FUNCTION(EventSelfResetTwoInputLatchingAnd2)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[0] == EHS_TRUE)
    {
        p[0] = EHS_FALSE;
        p[1] = EHS_FALSE;
        EHS_FB_FINISH(1);
    }
    else
    {
        p[1] = EHS_TRUE;
    }
    return;
}

/******************************************************************************/
/* Define EventSelfResetThreeInputLatchingAnd function block */

EHS_FB_FUNCTIONS_START(EventSelfResetThreeInputLatchingAnd)

EHS_FB_FUNCTION_ENTRY("And1", 0x01, EventSelfResetThreeInputLatchingAnd1)

EHS_FB_FUNCTION_ENTRY("And2", 0x02, EventSelfResetThreeInputLatchingAnd2)

EHS_FB_FUNCTION_ENTRY("And3", 0x03, EventSelfResetThreeInputLatchingAnd3)
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
EHS_FB_IDENTIFY_FUNCTION(EventSelfResetThreeInputLatchingAnd)
{
    /* reserve state data for two BOOLs...*/
    EHS_FB_IDENTIFY_MEMORY = 3 * sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventSelfResetThreeInputLatchingAnd)
{
    ehs_bool* pBOOL = NULL;

    /* cast the pointer to type ehs_bool...*/
    pBOOL = (ehs_bool*)EHS_FB_INIT_CONTEXT;
    /* make sure all three BOOLS are initially false...*/
    pBOOL[0] = EHS_FALSE;
    pBOOL[1] = EHS_FALSE;
    pBOOL[2] = EHS_FALSE;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventSelfResetThreeInputLatchingAnd1)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as BOOLs...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[1] == EHS_TRUE && p[2] == EHS_TRUE)
    {
        p[0] = EHS_FALSE;
        p[1] = EHS_FALSE;
        p[2] = EHS_FALSE;
        EHS_FB_FINISH(1);
    }
    else
    {
        p[0] = EHS_TRUE;
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
EHS_FB_RUN_FUNCTION(EventSelfResetThreeInputLatchingAnd2)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[0] == EHS_TRUE && p[2] == EHS_TRUE)
    {
        p[0] = EHS_FALSE;
        p[1] = EHS_FALSE;
        p[2] = EHS_FALSE;
        EHS_FB_FINISH(1);
    }
    else
    {
        p[1] = EHS_TRUE;

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
EHS_FB_RUN_FUNCTION(EventSelfResetThreeInputLatchingAnd3)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[0] == EHS_TRUE && p[1] == EHS_TRUE)
    {
        p[0] = EHS_FALSE;
        p[1] = EHS_FALSE;
        p[2] = EHS_FALSE;
        EHS_FB_FINISH(1);
    }
    else
    {
        p[2] = EHS_TRUE;
    }
    return;
}

/******************************************************************************/
/* Define EventSelfResetFourInputLatchingAnd function block */

EHS_FB_FUNCTIONS_START(EventSelfResetFourInputLatchingAnd)

EHS_FB_FUNCTION_ENTRY("And1", 0x01, EventSelfResetFourInputLatchingAnd1)

EHS_FB_FUNCTION_ENTRY("And2", 0x02, EventSelfResetFourInputLatchingAnd2)

EHS_FB_FUNCTION_ENTRY("And3", 0x03, EventSelfResetFourInputLatchingAnd3)

EHS_FB_FUNCTION_ENTRY("And4", 0x04, EventSelfResetFourInputLatchingAnd4)
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
EHS_FB_IDENTIFY_FUNCTION(EventSelfResetFourInputLatchingAnd)
{
    /* reserve state data for two BOOLs...*/
    EHS_FB_IDENTIFY_MEMORY = 4 * sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventSelfResetFourInputLatchingAnd)
{
    ehs_bool* pBOOL = NULL;

    /* cast the pointer to type ehs_bool...*/
    pBOOL = (ehs_bool*)EHS_FB_INIT_CONTEXT;
    /* make sure both BOOLS are initially false...*/
    pBOOL[0] = EHS_FALSE;
    pBOOL[1] = EHS_FALSE;
    pBOOL[2] = EHS_FALSE;
    pBOOL[3] = EHS_FALSE;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventSelfResetFourInputLatchingAnd1)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as BOOLs...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[1] == EHS_TRUE && p[2] == EHS_TRUE && p[3] == EHS_TRUE)
    {
        p[0] = EHS_FALSE;
        p[1] = EHS_FALSE;
        p[2] = EHS_FALSE;
        p[3] = EHS_FALSE;
        EHS_FB_FINISH(1);
    }
    else
    {
        p[0] = EHS_TRUE;
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
EHS_FB_RUN_FUNCTION(EventSelfResetFourInputLatchingAnd2)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[0] == EHS_TRUE && p[2] == EHS_TRUE && p[3] == EHS_TRUE)
    {
        p[0] = EHS_FALSE;
        p[1] = EHS_FALSE;
        p[2] = EHS_FALSE;
        p[3] = EHS_FALSE;
        EHS_FB_FINISH(1);
    }
    else
    {
        p[1] = EHS_TRUE;
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
EHS_FB_RUN_FUNCTION(EventSelfResetFourInputLatchingAnd3)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[0] == EHS_TRUE && p[1] == EHS_TRUE && p[3] == EHS_TRUE)
    {
        p[0] = EHS_FALSE;
        p[1] = EHS_FALSE;
        p[2] = EHS_FALSE;
        p[3] = EHS_FALSE;
        EHS_FB_FINISH(1);
    }
    else
    {
        p[2] = EHS_TRUE;
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
EHS_FB_RUN_FUNCTION(EventSelfResetFourInputLatchingAnd4)
{
    ehs_bool* p = NULL;

    /* the triggers are stored as ehs_bool...*/
    p = (ehs_bool*)EHS_FB_RUN_CONTEXT;
    if (p[0] == EHS_TRUE && p[1] == EHS_TRUE && p[2] == EHS_TRUE)
    {
        p[0] = EHS_FALSE;
        p[1] = EHS_FALSE;
        p[2] = EHS_FALSE;
        p[3] = EHS_FALSE;
        EHS_FB_FINISH(1);
    }
    else
    {
        p[3] = EHS_TRUE;
    }
    return;
}

/******************************************************************************/
/* Define EventTwoInputOr function block */

EHS_FB_FUNCTIONS_START(EventTwoInputOr)

EHS_FB_FUNCTION_ENTRY("Or1", 0x01, EventInputOr)

EHS_FB_FUNCTION_ENTRY("Or2", 0x02, EventInputOr)
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
EHS_FB_IDENTIFY_FUNCTION(EventTwoInputOr)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventTwoInputOr)
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
EHS_FB_RUN_FUNCTION(EventInputOr)
{
    /* if this is triggered at all set the completes...*/
    EHS_FB_FINISH(1);
    return;
}


/******************************************************************************/
/* Define EventThreeInputOr function block */

EHS_FB_FUNCTIONS_START(EventThreeInputOr)

EHS_FB_FUNCTION_ENTRY("Or1", 0x01, EventInputOr)

EHS_FB_FUNCTION_ENTRY("Or2", 0x02, EventInputOr)

EHS_FB_FUNCTION_ENTRY("Or3", 0x03, EventInputOr)
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
EHS_FB_IDENTIFY_FUNCTION(EventThreeInputOr)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventThreeInputOr)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/******************************************************************************/
/* Define EventFourInputOr function block */

EHS_FB_FUNCTIONS_START(EventFourInputOr)

EHS_FB_FUNCTION_ENTRY("Or1", 0x01, EventInputOr)

EHS_FB_FUNCTION_ENTRY("Or2", 0x02, EventInputOr)

EHS_FB_FUNCTION_ENTRY("Or3", 0x03, EventInputOr)

EHS_FB_FUNCTION_ENTRY("Or4", 0x04, EventInputOr)
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
EHS_FB_IDENTIFY_FUNCTION(EventFourInputOr)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventFourInputOr)
{
    return EHS_TRUE; /* initialisation always succeeds */
}

/******************************************************************************/
/* Define EventTwoInputBooleanOr function block */

EHS_FB_FUNCTIONS_START(EventTwoInputBooleanOr)

EHS_FB_FUNCTION_ENTRY("BoolOrTrig", 0x01, EventTwoInputBooleanOr)
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
EHS_FB_IDENTIFY_FUNCTION(EventTwoInputBooleanOr)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventTwoInputBooleanOr)
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
EHS_FB_RUN_FUNCTION(EventTwoInputBooleanOr)
{
    ehs_bool bIn1;
    ehs_bool bIn2;
    /* output tigger is asserted if either of the inputs is high...*/
    bIn1 = NCAPSA_bIn(0);
    bIn2 = NCAPSA_bIn(1);
    if (bIn1 || bIn2)
    {
        EHS_FB_FINISH(1);
    }
}

/******************************************************************************/
/* Define EventThreeInputBooleanOr function block */

EHS_FB_FUNCTIONS_START(EventThreeInputBooleanOr)

EHS_FB_FUNCTION_ENTRY("BoolOrTrig", 0x01, EventThreeInputBooleanOr)
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
EHS_FB_IDENTIFY_FUNCTION(EventThreeInputBooleanOr)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventThreeInputBooleanOr)
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
EHS_FB_RUN_FUNCTION(EventThreeInputBooleanOr)
{
    ehs_bool bIn1;
    ehs_bool bIn2;
    ehs_bool bIn3;
    /* output tigger is asserted if either of the inputs is high...*/
    bIn1 = NCAPSA_bIn(0);
    bIn2 = NCAPSA_bIn(1);
    bIn3 = NCAPSA_bIn(2);
    if (bIn1 || bIn2 ||bIn3)
    {
        EHS_FB_FINISH(1);
    }
}

/******************************************************************************/
/* Define EventFourInputBooleanOr function block */

EHS_FB_FUNCTIONS_START(EventFourInputBooleanOr)

EHS_FB_FUNCTION_ENTRY("BoolOrTrig", 0x01, EventFourInputBooleanOr)
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
EHS_FB_IDENTIFY_FUNCTION(EventFourInputBooleanOr)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventFourInputBooleanOr)
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
EHS_FB_RUN_FUNCTION(EventFourInputBooleanOr)
{
    ehs_bool bIn1;
    ehs_bool bIn2;
    ehs_bool bIn3;
    ehs_bool bIn4;
    /* output tigger is asserted if either of the inputs is high...*/
    bIn1 = NCAPSA_bIn(0);
    bIn2 = NCAPSA_bIn(1);
    bIn3 = NCAPSA_bIn(2);
    bIn4 = NCAPSA_bIn(3);

    if (bIn1 || bIn2 ||bIn3||bIn4)
    {
        EHS_FB_FINISH(1);
    }
}

/******************************************************************************/
/* Define EventToggleBoolean function block */

EHS_FB_FUNCTIONS_START(EventToggleBoolean)

EHS_FB_FUNCTION_ENTRY("Toggle", 0x01, EventToggleBoolean)
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
EHS_FB_IDENTIFY_FUNCTION(EventToggleBoolean)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventToggleBoolean)
{
    /* make sure both BOOLS are initially false...*/
    *((ehs_bool*)(EHS_FB_INIT_CONTEXT)) = EHS_FALSE;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventToggleBoolean)
{
    structFuncArg* sFunc = (structFuncArg*)EHS_FB_RUN_CONTEXT_REF;
    ehs_bool* lastState = (ehs_bool*)(sFunc->pObjData);
    if ((*(lastState)) == EHS_TRUE )
    {
        *(lastState) = EHS_FALSE;
    }
    else
    {
        *(lastState) = EHS_TRUE;
    }
    EHS_FB_OUT_B(0) = *(lastState);
    /* output tigger is asserted if either of the inputs is high...*/
    EHS_FB_FINISH(1);
}

/******************************************************************************/
/* Define EventSetReset function block */

EHS_FB_FUNCTIONS_START(EventSetReset)

EHS_FB_FUNCTION_ENTRY("Reset", 0x01, EventSetReset_Reset)

EHS_FB_FUNCTION_ENTRY("Set", 0x02, EventSetReset_Set)

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
EHS_FB_IDENTIFY_FUNCTION(EventSetReset)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventSetReset)
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
EHS_FB_RUN_FUNCTION(EventSetReset_Set)
{
    NCAPSA_bOut(0)=EHS_TRUE;
    EHS_FB_FINISH(1);
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventSetReset_Reset)
{
    NCAPSA_bOut(0)=EHS_FALSE;
    EHS_FB_FINISH(1);
}


/******************************************************************************/
/* Define EventPositiveEdgeDetect function block */

EHS_FB_FUNCTIONS_START(EventPositiveEdgeDetect)
EHS_FB_FUNCTION_ENTRY("Test", 0x01, EventPositiveEdgeDetect)
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
EHS_FB_IDENTIFY_FUNCTION(EventPositiveEdgeDetect)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventPositiveEdgeDetect)
{
    /* make sure both BOOLS are initially false...*/
    *((ehs_bool*)(EHS_FB_INIT_CONTEXT)) = EHS_FALSE;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventPositiveEdgeDetect)
{
    ehs_bool bIn1;
    /* output tigger is asserted if either of the inputs is high...*/
    bIn1 = NCAPSA_bIn(0);
    if (bIn1 && !*(ehs_bool*)(EHS_FB_RUN_CONTEXT) )
    {
        *(ehs_bool*)(EHS_FB_RUN_CONTEXT) =EHS_TRUE;
        EHS_FB_FINISH(1);
    }
    else
    {
        *(ehs_bool*)(EHS_FB_RUN_CONTEXT) =bIn1;
    }

    /* output tigger is asserted if either of the inputs is high...*/
}


/******************************************************************************/
/* Define EventNegativeEdgeDetect function block */

EHS_FB_FUNCTIONS_START(EventNegativeEdgeDetect)

EHS_FB_FUNCTION_ENTRY("Test", 0x01, EventNegativeEdgeDetect)
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
EHS_FB_IDENTIFY_FUNCTION(EventNegativeEdgeDetect)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventNegativeEdgeDetect)
{
    /* make sure both BOOLS are initially false...*/
    *((ehs_bool*)(EHS_FB_INIT_CONTEXT)) = EHS_FALSE;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventNegativeEdgeDetect)
{
    ehs_bool bIn1;
    /* output tigger is asserted if either of the inputs is high...*/
    bIn1 = NCAPSA_bIn(0);
    if (!bIn1 && *(ehs_bool*)(EHS_FB_RUN_CONTEXT) )
    {
        *(ehs_bool*)(EHS_FB_RUN_CONTEXT) =EHS_FALSE;
        EHS_FB_FINISH(1);
    }
    else
    {
        *(ehs_bool*)(EHS_FB_RUN_CONTEXT) =bIn1;
    }
    /* output tigger is asserted if either of the inputs is high...*/
}





/******************************************************************************/
/* Define EventEdgeDetect function block */

EHS_FB_FUNCTIONS_START(EventEdgeDetect)

EHS_FB_FUNCTION_ENTRY("Test", 0x01, EventEdgeDetect)
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
EHS_FB_IDENTIFY_FUNCTION(EventEdgeDetect)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_bool);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventEdgeDetect)
{
    /* make sure both BOOLS are initially false...*/
    *((ehs_bool*)(EHS_FB_INIT_CONTEXT)) = (ehs_bool)(EHS_FB_INIT_PARAMETERS[0]!='0');
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventEdgeDetect)
{
    ehs_bool bIn1;
    /* output tigger is asserted if either of the inputs are high...*/
    bIn1 = NCAPSA_bIn(0);
    if ((bIn1  ^ *(ehs_bool*)(EHS_FB_RUN_CONTEXT)))
    {
        *(ehs_bool*)(EHS_FB_RUN_CONTEXT) =bIn1;
        EHS_FB_FINISH(1);
    }

    /* output tigger is asserted if either of the inputs is high...*/
}


/******************************************************************************/
/* Define EventCounter function block */

EHS_FB_FUNCTIONS_START(EventCounter)

EHS_FB_FUNCTION_ENTRY("Count", 0x01, EventCounter)

EHS_FB_FUNCTION_ENTRY("Reset", 0x02, EventCounter_Reset)
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
EHS_FB_IDENTIFY_FUNCTION(EventCounter)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_uint32);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventCounter)
{
    /* make sure the counter is reset...*/
    *(ehs_uint32*)EHS_FB_INIT_CONTEXT = 0;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventCounter)
{
    int nCount;
    /* simply increment the object state data...*/
    nCount = *(int*)EHS_FB_RUN_CONTEXT;
    nCount++;
    /* copy to state data and output...*/
    *(int*)EHS_FB_RUN_CONTEXT = nCount;
    NCAPSA_nOut(0) = nCount;
    EHS_FB_FINISH(1);
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventCounter_Reset)
{
    /* reset state data and output...*/
    *(int*)EHS_FB_RUN_CONTEXT = 0;
    NCAPSA_nOut(0) = 0;
    //EHS_FB_FINISH(1);
    return;
}



