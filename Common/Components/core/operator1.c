/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * operator1.c
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
#include "operator1.h"
#include "setCompletes.h"
#include "app_data.h"
#include "hal-api.h" /* Required for logging */

#ifndef EHS_PI
#ifndef M_PI
#define EHS_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491L
#else
#define EHS_PI M_PI
#endif
#endif


/**
 * integrator_int
 */
typedef struct
{
    EhsDataflowIntType nOffset;		// the offset value to add to the value being integrated.
    EhsDataflowIntType nSum;		// the cumulative value of the integration.
} EhsEventIntegratorIntStruct;

/**
 * integrator_real
 */
typedef struct
{
    EhsDataflowFloatType fOffset;		// the offset value to add to the value being integrated.
    EhsDataflowFloatType fSum;		// the cumulative value of the integration.
} EhsEventIntegratorRealStruct;

//Addition

/******************************************************************************/
/* Define AdditionTwoInputInt function block */

EHS_FB_FUNCTIONS_START(AdditionTwoInputInt)

EHS_FB_FUNCTION_ENTRY("Run_AdditionTwoInputInt", 0x01, AdditionTwoInputInt)
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
EHS_FB_IDENTIFY_FUNCTION(AdditionTwoInputInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(AdditionTwoInputInt)
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
EHS_FB_RUN_FUNCTION(AdditionTwoInputInt)
{
    /* the 2 input integers added together form the single output integer...*/
    NCAPSA_nOut(0) = NCAPSA_nIn(0) + NCAPSA_nIn(1);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define AdditionThreeInputInt function block */

EHS_FB_FUNCTIONS_START(AdditionThreeInputInt)

EHS_FB_FUNCTION_ENTRY("Run_AdditionThreeInputInt", 0x01, AdditionThreeInputInt)
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
EHS_FB_IDENTIFY_FUNCTION(AdditionThreeInputInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(AdditionThreeInputInt)
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
EHS_FB_RUN_FUNCTION(AdditionThreeInputInt)
{
    /* the 3 input integers added together form the single output integer...*/
    NCAPSA_nOut(0) = NCAPSA_nIn(0) + NCAPSA_nIn(1) + NCAPSA_nIn(2);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define AdditionFourInputInt function block */

EHS_FB_FUNCTIONS_START(AdditionFourInputInt)

EHS_FB_FUNCTION_ENTRY("Run_AdditionFourInputInt", 0x01, AdditionFourInputInt)
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
EHS_FB_IDENTIFY_FUNCTION(AdditionFourInputInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(AdditionFourInputInt)
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
EHS_FB_RUN_FUNCTION(AdditionFourInputInt)
{
    /* the 4 input integers added together form the single output integer...*/
    NCAPSA_nOut(0) = NCAPSA_nIn(0) + NCAPSA_nIn(1) + NCAPSA_nIn(2) + NCAPSA_nIn(3);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define AdditionTwoInputReal function block */

EHS_FB_FUNCTIONS_START(AdditionTwoInputReal)

EHS_FB_FUNCTION_ENTRY("Run_AdditionTwoInputReal", 0x01, AdditionTwoInputReal)
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
EHS_FB_IDENTIFY_FUNCTION(AdditionTwoInputReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(AdditionTwoInputReal)
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
EHS_FB_RUN_FUNCTION(AdditionTwoInputReal)
{
    /* the 2 input reals added together form the single output real...*/
    NCAPSA_dOut(0) = NCAPSA_dIn(0) + NCAPSA_dIn(1);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define AdditionThreeInputReal function block */

EHS_FB_FUNCTIONS_START(AdditionThreeInputReal)

EHS_FB_FUNCTION_ENTRY("Run_AdditionThreeInputReal", 0x01, AdditionThreeInputReal)
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
EHS_FB_IDENTIFY_FUNCTION(AdditionThreeInputReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(AdditionThreeInputReal)
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
EHS_FB_RUN_FUNCTION(AdditionThreeInputReal)
{
    /* the 3 input reals added together form the single output real...*/
    NCAPSA_dOut(0) = NCAPSA_dIn(0) + NCAPSA_dIn(1) + NCAPSA_dIn(2);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define AdditionFourInputReal function block */

EHS_FB_FUNCTIONS_START(AdditionFourInputReal)

EHS_FB_FUNCTION_ENTRY("Run_AdditionFourInputReal", 0x01, AdditionFourInputReal)
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
EHS_FB_IDENTIFY_FUNCTION(AdditionFourInputReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(AdditionFourInputReal)
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
EHS_FB_RUN_FUNCTION(AdditionFourInputReal)
{
    /* the 4 input reals added together form the single output real...*/
    NCAPSA_dOut(0) = NCAPSA_dIn(0) + NCAPSA_dIn(1) + NCAPSA_dIn(2) + NCAPSA_dIn(3);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif
/*============================================================================*/
//Subtraction

/******************************************************************************/
/* Define SubtractionInt function block */

EHS_FB_FUNCTIONS_START(SubtractionInt)

EHS_FB_FUNCTION_ENTRY("Run_SubtractionInt", 0x01, SubtractionInt)
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
EHS_FB_IDENTIFY_FUNCTION(SubtractionInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SubtractionInt)
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
EHS_FB_RUN_FUNCTION(SubtractionInt)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0) - NCAPSA_nIn(1);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define SubtractionReal function block */

EHS_FB_FUNCTIONS_START(SubtractionReal)

EHS_FB_FUNCTION_ENTRY("Run_SubtractionReal", 0x01, SubtractionReal)
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
EHS_FB_IDENTIFY_FUNCTION(SubtractionReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SubtractionReal)
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
EHS_FB_RUN_FUNCTION(SubtractionReal)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0) - NCAPSA_dIn(1);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

/*============================================================================*/
//Multiplication

/******************************************************************************/
/* Define MultiplyTwoInputInt function block */

EHS_FB_FUNCTIONS_START(MultiplyTwoInputInt)

EHS_FB_FUNCTION_ENTRY("Run_MultiplyTwoInputInt", 0x01, MultiplyTwoInputInt)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplyTwoInputInt)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(MultiplyTwoInputInt));
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplyTwoInputInt)
{
    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(MultiplyTwoInputInt));
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(MultiplyTwoInputInt)
{
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(MultiplyTwoInputInt));
    NCAPSA_nOut(0) = NCAPSA_nIn(0) * NCAPSA_nIn(1);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplyThreeInputInt function block */

EHS_FB_FUNCTIONS_START(MultiplyThreeInputInt)

EHS_FB_FUNCTION_ENTRY("Run_MultiplyThreeInputInt", 0x01, MultiplyThreeInputInt)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplyThreeInputInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplyThreeInputInt)
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
EHS_FB_RUN_FUNCTION(MultiplyThreeInputInt)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0) * NCAPSA_nIn(1) * NCAPSA_nIn(2);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MultiplyFourInputInt function block */

EHS_FB_FUNCTIONS_START(MultiplyFourInputInt)

EHS_FB_FUNCTION_ENTRY("Run_MultiplyFourInputInt", 0x01, MultiplyFourInputInt)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplyFourInputInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplyFourInputInt)
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
EHS_FB_RUN_FUNCTION(MultiplyFourInputInt)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0) * NCAPSA_nIn(1) * NCAPSA_nIn(2) * NCAPSA_nIn(3);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define MultiplyTwoInputReal function block */

EHS_FB_FUNCTIONS_START(MultiplyTwoInputReal)

EHS_FB_FUNCTION_ENTRY("Run_MultiplyTwoInputReal", 0x01, MultiplyTwoInputReal)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplyTwoInputReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplyTwoInputReal)
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
EHS_FB_RUN_FUNCTION(MultiplyTwoInputReal)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0) * NCAPSA_dIn(1);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define MultiplyThreeInputReal function block */

EHS_FB_FUNCTIONS_START(MultiplyThreeInputReal)

EHS_FB_FUNCTION_ENTRY("Run_MultiplyThreeInputReal", 0x01, MultiplyThreeInputReal)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplyThreeInputReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplyThreeInputReal)
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
EHS_FB_RUN_FUNCTION(MultiplyThreeInputReal)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0) * NCAPSA_dIn(1) * NCAPSA_dIn(2);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define MultiplyFourInputReal function block */

EHS_FB_FUNCTIONS_START(MultiplyFourInputReal)

EHS_FB_FUNCTION_ENTRY("Run_MultiplyFourInputReal", 0x01, MultiplyFourInputReal)
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
EHS_FB_IDENTIFY_FUNCTION(MultiplyFourInputReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MultiplyFourInputReal)
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
EHS_FB_RUN_FUNCTION(MultiplyFourInputReal)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0) * NCAPSA_dIn(1) * NCAPSA_dIn(2) * NCAPSA_dIn(3);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

/*============================================================================*/
//Division
/* maybe consider error checking to prevent a divide by zero...*/

/******************************************************************************/
/* Define DivisionInt function block */

EHS_FB_FUNCTIONS_START(DivisionInt)

EHS_FB_FUNCTION_ENTRY("Run_DivisionInt", 0x01, DivisionInt)
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
EHS_FB_IDENTIFY_FUNCTION(DivisionInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DivisionInt)
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
EHS_FB_RUN_FUNCTION(DivisionInt)
{
    EhsDataflowIntType n = NCAPSA_nIn(0);
    EhsDataflowIntType d = NCAPSA_nIn(1);
    if(d == 0){
        EHSH_LOG_ERROR("DivisionInt - dividing by '0' is not allowed.");
        return;
    }
    NCAPSA_nOut(0) =  n / d ;
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define DivisionReal function block */

EHS_FB_FUNCTIONS_START(DivisionReal)

EHS_FB_FUNCTION_ENTRY("Run_DivisionReal", 0x01, DivisionReal)
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
EHS_FB_IDENTIFY_FUNCTION(DivisionReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(DivisionReal)
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
EHS_FB_RUN_FUNCTION(DivisionReal)
{
    double n = (double)NCAPSA_dIn(0);
    double d = (double)NCAPSA_dIn(1);
    if(d == 0.0){
        EHSH_LOG_ERROR("DivisionReal - dividing by '0' is not allowed.");
        return;
    }
    NCAPSA_dOut(0) = (EhsDataflowFloatType)(n / d);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif
/*============================================================================*/
//Modulus

/******************************************************************************/
/* Define ModulusInt function block */

EHS_FB_FUNCTIONS_START(ModulusInt)

EHS_FB_FUNCTION_ENTRY("Run_ModulusInt", 0x01, ModulusInt)
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
EHS_FB_IDENTIFY_FUNCTION(ModulusInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ModulusInt)
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
EHS_FB_RUN_FUNCTION(ModulusInt)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0) % NCAPSA_nIn(1);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define ModulusReal function block */

EHS_FB_FUNCTIONS_START(ModulusReal)

EHS_FB_FUNCTION_ENTRY("Run_ModulusReal", 0x01, ModulusReal)
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
EHS_FB_IDENTIFY_FUNCTION(ModulusReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ModulusReal)
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
EHS_FB_RUN_FUNCTION(ModulusReal)
{
    NCAPSA_dOut(0) = fmod(NCAPSA_dIn(0), NCAPSA_dIn(1));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif
/*============================================================================*/
//Power

/******************************************************************************/
/* Define PowerInt function block */

EHS_FB_FUNCTIONS_START(PowerInt)

EHS_FB_FUNCTION_ENTRY("Run_PowerInt", 0x01, PowerInt)
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
EHS_FB_IDENTIFY_FUNCTION(PowerInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(PowerInt)
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
EHS_FB_RUN_FUNCTION(PowerInt)
{

#ifdef EHS_TARGET_FP_SUPPORT
    double out;
    out = pow((double)NCAPSA_nIn(0), (double)NCAPSA_nIn(1));
    NCAPSA_nOut(0) = (int)out;
#else
    /* calculate NCAPSA_nIn(0) to the power NCAPSA_nIn(1) without using
     * pow function.
     */
    int i;
    int out;

    if (NCAPSA_nIn(1) < 1)
    {
        out = 0;
    }
    else
    {
        out = NCAPSA_nIn(0);
        for (i = 1; i < NCAPSA_nIn(1); i++)
        {
            out *= NCAPSA_nIn(0);
        }
    }
    NCAPSA_nOut(0) = out;
#endif
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define PowerReal function block */

EHS_FB_FUNCTIONS_START(PowerReal)

EHS_FB_FUNCTION_ENTRY("Run_PowerReal", 0x01, PowerReal)
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
EHS_FB_IDENTIFY_FUNCTION(PowerReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(PowerReal)
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
EHS_FB_RUN_FUNCTION(PowerReal)
{
    NCAPSA_dOut(0) = pow(NCAPSA_dIn(0), NCAPSA_dIn(1));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif
/*============================================================================*/
//Square

/******************************************************************************/
/* Define SquareInt function block */

EHS_FB_FUNCTIONS_START(SquareInt)

EHS_FB_FUNCTION_ENTRY("Run_SquareInt", 0x01, SquareInt)
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
EHS_FB_IDENTIFY_FUNCTION(SquareInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SquareInt)
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
EHS_FB_RUN_FUNCTION(SquareInt)
{
    NCAPSA_nOut(0) = NCAPSA_nIn(0) * NCAPSA_nIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define SquareReal function block */

EHS_FB_FUNCTIONS_START(SquareReal)

EHS_FB_FUNCTION_ENTRY("Run_SquareReal", 0x01, SquareReal)
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
EHS_FB_IDENTIFY_FUNCTION(SquareReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SquareReal)
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
EHS_FB_RUN_FUNCTION(SquareReal)
{
    NCAPSA_dOut(0) = NCAPSA_dIn(0) * NCAPSA_dIn(0);
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif
/*============================================================================*/
//Add Accumulate

/******************************************************************************/
/* Define AddAccumulateInt function block */

EHS_FB_FUNCTIONS_START(AddAccumulateInt)

EHS_FB_FUNCTION_ENTRY("Run_AddAccumulateInt", 0x01, AddAccumulateInt)
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
EHS_FB_IDENTIFY_FUNCTION(AddAccumulateInt)
{
    /* the accumulator requires memory for an int...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(int);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(AddAccumulateInt)
{
    /* make sure the accumulator is reset...*/
    *(int*)EHS_FB_INIT_CONTEXT = 0;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(AddAccumulateInt)
{
    int nSum;
    /* simply add the input value to the object state data...*/
    nSum = *(int*)EHS_FB_RUN_CONTEXT;
    nSum += NCAPSA_nIn(0);
    /* copy to state data and output...*/
    *(int*)EHS_FB_RUN_CONTEXT = nSum;
    NCAPSA_nOut(0) = nSum;
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define AddAccumulateReal function block */

EHS_FB_FUNCTIONS_START(AddAccumulateReal)

EHS_FB_FUNCTION_ENTRY("Run_AddAccumulateReal", 0x01, AddAccumulateReal)
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
EHS_FB_IDENTIFY_FUNCTION(AddAccumulateReal)
{
    /* the accumulator requires memory for a ehs_float...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_float);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(AddAccumulateReal)
{
    /* make sure the accumulator is reset...*/
    *(ehs_float*)EHS_FB_INIT_CONTEXT = 0;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(AddAccumulateReal)
{
    ehs_float dSum;
    /* simply add the input value to the object state data...*/
    dSum = *(ehs_float*)EHS_FB_RUN_CONTEXT;
    dSum += NCAPSA_dIn(0);
    /* copy to state data and output...*/
    *(ehs_float*)EHS_FB_RUN_CONTEXT = dSum;
    NCAPSA_dOut(0) = dSum;
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif
/******************************************************************************/
/* Define SubAccumulateInt function block */

EHS_FB_FUNCTIONS_START(SubAccumulateInt)

EHS_FB_FUNCTION_ENTRY("Run_SubAccumulateInt", 0x01, SubAccumulateInt)
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
EHS_FB_IDENTIFY_FUNCTION(SubAccumulateInt)
{
    /* the accumulator requires memory for an int...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(int);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SubAccumulateInt)
{
    /* make sure the accumulator is reset...*/
    *(int*)EHS_FB_INIT_CONTEXT = 0;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(SubAccumulateInt)
{
    int nSum;
    /* simply add the input value to the object state data...*/
    nSum = *(int*)EHS_FB_RUN_CONTEXT;
    nSum -= NCAPSA_nIn(0);
    /* copy to state data and output...*/
    *(int*)EHS_FB_RUN_CONTEXT = nSum;
    NCAPSA_nOut(0) = nSum;
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define SubAccumulateReal function block */

EHS_FB_FUNCTIONS_START(SubAccumulateReal)

EHS_FB_FUNCTION_ENTRY("Run_SubAccumulateReal", 0x01, SubAccumulateReal)
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
EHS_FB_IDENTIFY_FUNCTION(SubAccumulateReal)
{
    /* the accumulator requires memory for a ehs_float...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_float);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SubAccumulateReal)
{
    /* make sure the accumulator is reset...*/
    *(ehs_float*)EHS_FB_INIT_CONTEXT = 0;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(SubAccumulateReal)
{
    ehs_float dSum;
    /* simply add the input value to the object state data...*/
    dSum = *(ehs_float*)EHS_FB_RUN_CONTEXT;
    dSum -= NCAPSA_dIn(0);
    /* copy to state data and output...*/
    *(ehs_float*)EHS_FB_RUN_CONTEXT = dSum;
    NCAPSA_dOut(0) = dSum;
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define SquareRootReal function block */

EHS_FB_FUNCTIONS_START(SquareRootReal)

EHS_FB_FUNCTION_ENTRY("Run_SquareRootReal", 0x01, SquareRootReal)
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
EHS_FB_IDENTIFY_FUNCTION(SquareRootReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SquareRootReal)
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
EHS_FB_RUN_FUNCTION(SquareRootReal)
{
    NCAPSA_dOut(0) = sqrt(NCAPSA_dIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define LogBaseTenReal function block */

EHS_FB_FUNCTIONS_START(LogBaseTenReal)

EHS_FB_FUNCTION_ENTRY("Run_LogBaseTenReal", 0x01, LogBaseTenReal)
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
EHS_FB_IDENTIFY_FUNCTION(LogBaseTenReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogBaseTenReal)
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
EHS_FB_RUN_FUNCTION(LogBaseTenReal)
{
    NCAPSA_dOut(0) = log10(NCAPSA_dIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define LogReal function block */

EHS_FB_FUNCTIONS_START(LogReal)

EHS_FB_FUNCTION_ENTRY("Run_LogReal", 0x01, LogReal)
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
EHS_FB_IDENTIFY_FUNCTION(LogReal)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(LogReal)
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
EHS_FB_RUN_FUNCTION(LogReal)
{
    NCAPSA_dOut(0) = log(NCAPSA_dIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

/*============================================================================*/
//Trigonometric and Hyperbolic Functions

struct  tTrigparms
{
    ehs_bool rads_degs;
};


#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define SineReal function block */

EHS_FB_FUNCTIONS_START(SineReal)

EHS_FB_FUNCTION_ENTRY("Run_SineReal", 0x01, SineReal)
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
EHS_FB_IDENTIFY_FUNCTION(SineReal)
{
    EHS_FB_IDENTIFY_MEMORY  =  sizeof(struct tTrigparms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SineReal)
{
    struct tTrigparms* pUrlGet = (struct tTrigparms *) EHS_FB_INIT_CONTEXT;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhd",&pUrlGet->rads_degs);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(SineReal)
{
    struct tTrigparms* tTrigparms = (struct tTrigparms *) EHS_FB_RUN_CONTEXT;
    NCAPSA_dOut(0) = (tTrigparms->rads_degs ? sinf(NCAPSA_dIn(0)) : sinf( NCAPSA_dIn(0) * EHS_PI / 180.0 ));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define CosineReal function block */

EHS_FB_FUNCTIONS_START(CosineReal)

EHS_FB_FUNCTION_ENTRY("Run_CosineReal", 0x01, CosineReal)
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
EHS_FB_IDENTIFY_FUNCTION(CosineReal)
{
    EHS_FB_IDENTIFY_MEMORY  =  sizeof(struct tTrigparms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(CosineReal)
{
    struct tTrigparms* pUrlGet = (struct tTrigparms *) EHS_FB_INIT_CONTEXT;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhd",&pUrlGet->rads_degs);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(CosineReal)
{

    struct tTrigparms* tTrigparms = (struct tTrigparms *) EHS_FB_RUN_CONTEXT;
    NCAPSA_dOut(0) = tTrigparms->rads_degs ? cosf(NCAPSA_dIn(0)) : cosf(NCAPSA_dIn(0)  * EHS_PI / 180.0 );
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define TanReal function block */

EHS_FB_FUNCTIONS_START(TanReal)

EHS_FB_FUNCTION_ENTRY("Run_TanReal", 0x01, TanReal)
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
EHS_FB_IDENTIFY_FUNCTION(TanReal)
{
    EHS_FB_IDENTIFY_MEMORY  =  sizeof(struct tTrigparms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(TanReal)
{
    struct tTrigparms* pUrlGet = (struct tTrigparms *) EHS_FB_INIT_CONTEXT;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhd",&pUrlGet->rads_degs);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(TanReal)
{
    struct tTrigparms* tTrigparms = (struct tTrigparms *) EHS_FB_RUN_CONTEXT;
    NCAPSA_dOut(0) = tTrigparms->rads_degs ? tanf(NCAPSA_dIn(0)) : tanf(NCAPSA_dIn(0) * EHS_PI / 180.0 );
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define ArcSineReal function block */

EHS_FB_FUNCTIONS_START(ArcSineReal)

EHS_FB_FUNCTION_ENTRY("Run_ArcSineReal", 0x01, ArcSineReal)
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
EHS_FB_IDENTIFY_FUNCTION(ArcSineReal)
{
    EHS_FB_IDENTIFY_MEMORY  =  sizeof(struct tTrigparms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ArcSineReal)
{
    struct tTrigparms* pUrlGet = (struct tTrigparms *) EHS_FB_INIT_CONTEXT;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhd",&pUrlGet->rads_degs);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ArcSineReal)
{
    struct tTrigparms* tTrigparms = (struct tTrigparms *) EHS_FB_RUN_CONTEXT;
    NCAPSA_dOut(0) = tTrigparms->rads_degs ? asinf(NCAPSA_dIn(0)) : asinf(NCAPSA_dIn(0)) * 180.0 / EHS_PI;
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define ArcCosineReal function block */

EHS_FB_FUNCTIONS_START(ArcCosineReal)

EHS_FB_FUNCTION_ENTRY("Run_ArcCosineReal", 0x01, ArcCosineReal)
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
EHS_FB_IDENTIFY_FUNCTION(ArcCosineReal)
{
    EHS_FB_IDENTIFY_MEMORY  =  sizeof(struct tTrigparms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ArcCosineReal)
{
    struct tTrigparms* pUrlGet = (struct tTrigparms *) EHS_FB_INIT_CONTEXT;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhd",&pUrlGet->rads_degs);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ArcCosineReal)
{
    struct tTrigparms* tTrigparms = (struct tTrigparms *) EHS_FB_RUN_CONTEXT;
    NCAPSA_dOut(0) = tTrigparms->rads_degs ? acosf(NCAPSA_dIn(0)) : acosf(NCAPSA_dIn(0)) * 180.0 / EHS_PI;
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define ArcTanReal function block */

EHS_FB_FUNCTIONS_START(ArcTanReal)

EHS_FB_FUNCTION_ENTRY("Run_ArcTanReal", 0x01, ArcTanReal)
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
EHS_FB_IDENTIFY_FUNCTION(ArcTanReal)
{
    EHS_FB_IDENTIFY_MEMORY  =  sizeof(struct tTrigparms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ArcTanReal)
{
    struct tTrigparms* pUrlGet = (struct tTrigparms *) EHS_FB_INIT_CONTEXT;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhd",&pUrlGet->rads_degs);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ArcTanReal)
{
    struct tTrigparms* tTrigparms = (struct tTrigparms *) EHS_FB_RUN_CONTEXT;
    NCAPSA_dOut(0) = tTrigparms->rads_degs ? atanf(NCAPSA_dIn(0)) : atanf(NCAPSA_dIn(0)) * 180.0 / EHS_PI;
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define HyperbolicSineReal function block */

EHS_FB_FUNCTIONS_START(HyperbolicSineReal)

EHS_FB_FUNCTION_ENTRY("Run_HyperbolicSineReal", 0x01, HyperbolicSineReal)
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
EHS_FB_IDENTIFY_FUNCTION(HyperbolicSineReal)
{
    //EHS_FB_IDENTIFY_MEMORY  =  sizeof(struct tTrigparms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(HyperbolicSineReal)
{
    //struct tTrigparms* pUrlGet = (struct tTrigparms *) EHS_FB_INIT_CONTEXT;
    //EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhc",&pUrlGet->rads_degs);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(HyperbolicSineReal)
{
    NCAPSA_dOut(0) = sinhf(NCAPSA_dIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define HyperbolicCosineReal function block */

EHS_FB_FUNCTIONS_START(HyperbolicCosineReal)

EHS_FB_FUNCTION_ENTRY("Run_HyperbolicCosineReal", 0x01, HyperbolicCosineReal)
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
EHS_FB_IDENTIFY_FUNCTION(HyperbolicCosineReal)
{
    //EHS_FB_IDENTIFY_MEMORY  =  sizeof(struct tTrigparms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(HyperbolicCosineReal)
{
    //struct tTrigparms* pUrlGet = (struct tTrigparms *) EHS_FB_INIT_CONTEXT;
    //EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhc",&pUrlGet->rads_degs);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(HyperbolicCosineReal)
{
    NCAPSA_dOut(0) = coshf(NCAPSA_dIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define HyperbolicTanReal function block */

EHS_FB_FUNCTIONS_START(HyperbolicTanReal)

EHS_FB_FUNCTION_ENTRY("Run_HyperbolicTanReal", 0x01, HyperbolicTanReal)
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
EHS_FB_IDENTIFY_FUNCTION(HyperbolicTanReal)
{
    //EHS_FB_IDENTIFY_MEMORY  =  sizeof(struct tTrigparms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(HyperbolicTanReal)
{
    //struct tTrigparms* pUrlGet = (struct tTrigparms *) EHS_FB_INIT_CONTEXT;
    //EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhc",&pUrlGet->rads_degs);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(HyperbolicTanReal)
{
    NCAPSA_dOut(0) = tanhf(NCAPSA_dIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define ArcHyperbolicSineRea function block */

EHS_FB_FUNCTIONS_START(ArcHyperbolicSineReal)

EHS_FB_FUNCTION_ENTRY("Run_ArcHyperbolicSineReal", 0x01, ArcHyperbolicSineReal)
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
EHS_FB_IDENTIFY_FUNCTION(ArcHyperbolicSineReal)
{
    //EHS_FB_IDENTIFY_MEMORY  =  sizeof(struct tTrigparms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ArcHyperbolicSineReal)
{
    //struct tTrigparms* pUrlGet = (struct tTrigparms *) EHS_FB_INIT_CONTEXT;
    //EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhc",&pUrlGet->rads_degs);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ArcHyperbolicSineReal)
{
    NCAPSA_dOut(0) = asinhf(NCAPSA_dIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define ArcHyperbolicCosineReal function block */

EHS_FB_FUNCTIONS_START(ArcHyperbolicCosineReal)

EHS_FB_FUNCTION_ENTRY("Run_ArcHyperbolicCosineReal", 0x01, ArcHyperbolicCosineReal)
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
EHS_FB_IDENTIFY_FUNCTION(ArcHyperbolicCosineReal)
{
    //EHS_FB_IDENTIFY_MEMORY  =  sizeof(struct tTrigparms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ArcHyperbolicCosineReal)
{
    //struct tTrigparms* pUrlGet = (struct tTrigparms *) EHS_FB_INIT_CONTEXT;
    //EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhc",&pUrlGet->rads_degs);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ArcHyperbolicCosineReal)
{
    NCAPSA_dOut(0) = acoshf(NCAPSA_dIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define ArcHyperbolicTanReal function block */

EHS_FB_FUNCTIONS_START(ArcHyperbolicTanReal)

EHS_FB_FUNCTION_ENTRY("Run_ArcHyperbolicTanReal", 0x01, ArcHyperbolicTanReal)
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
EHS_FB_IDENTIFY_FUNCTION(ArcHyperbolicTanReal)
{
    //EHS_FB_IDENTIFY_MEMORY  =  sizeof(struct tTrigparms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ArcHyperbolicTanReal)
{
    //struct tTrigparms* pUrlGet = (struct tTrigparms *) EHS_FB_INIT_CONTEXT;
    //EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhc",&pUrlGet->rads_degs);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ArcHyperbolicTanReal)
{
    NCAPSA_dOut(0) = atanhf(NCAPSA_dIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

/*============================================================================*/
//Absolute value

/******************************************************************************/
/* Define AbsInt function block */

EHS_FB_FUNCTIONS_START(AbsInt)

EHS_FB_FUNCTION_ENTRY("Run_AbsInt", 0x01, AbsInt)
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
EHS_FB_IDENTIFY_FUNCTION(AbsInt)
{
    //EHS_FB_IDENTIFY_MEMORY  =  sizeof(struct tTrigparms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(AbsInt)
{
    //struct tTrigparms* pUrlGet = (struct tTrigparms *) EHS_FB_INIT_CONTEXT;
    //EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhc",&pUrlGet->rads_degs);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(AbsInt)
{
    NCAPSA_nOut(0) = abs(NCAPSA_nIn(0));
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/*============================================================================*/
//Absolute value
/******************************************************************************/
/* Define AbsReal function block */

EHS_FB_FUNCTIONS_START(AbsReal)

EHS_FB_FUNCTION_ENTRY("Run_AbsReal", 0x01, AbsReal)
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
EHS_FB_IDENTIFY_FUNCTION(AbsReal)
{
    //EHS_FB_IDENTIFY_MEMORY  =  sizeof(struct tTrigparms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(AbsReal)
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
EHS_FB_RUN_FUNCTION(AbsReal)
{
#if EHS_FLOAT_AS_FLOAT_TYPE == 1
    NCAPSA_dOut(0) = fabsf(NCAPSA_dIn(0));
#else
    NCAPSA_dOut(0) = fabs(NCAPSA_dIn(0));
#endif
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/*============================================================================*/
//Exponential
/******************************************************************************/
/* Define ExponentialReal function block */

EHS_FB_FUNCTIONS_START(ExponentialReal)

EHS_FB_FUNCTION_ENTRY("Run_ExponentialReal", 0x01, ExponentialReal)
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
EHS_FB_IDENTIFY_FUNCTION(ExponentialReal)
{
    //EHS_FB_IDENTIFY_MEMORY  =  sizeof(struct tTrigparms);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(ExponentialReal)
{
    //struct tTrigparms* pUrlGet = (struct tTrigparms *) EHS_FB_INIT_CONTEXT;
    //EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhc",&pUrlGet->rads_degs);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ExponentialReal)
{
#if EHS_FLOAT_AS_FLOAT_TYPE == 1
    NCAPSA_dOut(0) = expf(NCAPSA_dIn(0));
#else
    NCAPSA_dOut(0) = exp(NCAPSA_dIn(0));
#endif
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
#endif

/*============================================================================*/
//Comparators

/******************************************************************************/
/* Define ComparatorGreaterInt function block */

EHS_FB_FUNCTIONS_START(ComparatorGreaterInt1)

EHS_FB_FUNCTION_ENTRY("Run_ComparatorGreaterInt", 0x01, ComparatorGreaterInt1)
EHS_FB_FUNCTIONS_END

#define EHS_FB_COMPEQUALS_IN_0 0
#define EHS_FB_COMPEQUALS_IN_1 1
/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterInt1)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(ComparatorGreaterInt1));
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
EHS_FB_INIT_FUNCTION(ComparatorGreaterInt1)
{
    EhsDataflowIntType value;
    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(ComparatorGreaterInt1));
    value = atol(EHS_FB_INIT_PARAMETERS);
    *(EhsDataflowIntType*)EHS_FB_INIT_CONTEXT = value;//atol(pParams);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ComparatorGreaterInt1)
{
    EhsDataflowIntType nIn0;
    EhsDataflowIntType nIn1;


    if (EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        nIn0 = NCAPSA_nIn(0);
        nIn1 = NCAPSA_nIn(1);
    }
    else if (EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && !EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        nIn0 = NCAPSA_nIn(0);
        nIn1 = *(ehs_sint32*)((ehs_uint8*)EHS_FB_RUN_CONTEXT);
    }
    else if (!EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        nIn0 = *(ehs_sint32*)((ehs_uint8*)EHS_FB_RUN_CONTEXT);
        nIn1 = NCAPSA_nIn(1);
    }
    // error condition - neither input is connected - force to false for now
    else
    {
        nIn0 = 1;
        nIn1 = 0;
    }

    /* outputs true if In(1) > In(0), false otherwise...*/
    if (nIn1 > nIn0)
    {
        NCAPSA_bOut(0) = EHS_TRUE;
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        NCAPSA_bOut(0) = EHS_FALSE;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }

//if (*((int*)((NCAPSA_pMain)->pIn[1])) > *((int*)((NCAPSA_pMain)->pIn[0])))

    /*
    if ( (*pInt1) > (*pInt0) )
    {
    	*pBool = EHS_TRUE;
    	SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
    	*pBool = EHS_FALSE;
    	SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    */
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define ComparatorGreaterReal1 function block */

EHS_FB_FUNCTIONS_START(ComparatorGreaterReal1)

EHS_FB_FUNCTION_ENTRY("Run_ComparatorGreaterReal", 0x01, ComparatorGreaterReal1)
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
EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterReal1)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(ComparatorGreaterReal1));
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
EHS_FB_INIT_FUNCTION(ComparatorGreaterReal1)
{
    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(ComparatorGreaterReal1));
    *(ehs_float*)EHS_FB_INIT_CONTEXT = (ehs_float)atof(EHS_FB_INIT_PARAMETERS);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ComparatorGreaterReal1)
{
    EhsDataflowFloatType dIn0;
    EhsDataflowFloatType dIn1;

    if (EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        dIn0 = NCAPSA_dIn(0);
        dIn1 = NCAPSA_dIn(1);
    }
    else if (EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && !EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        dIn0 = NCAPSA_dIn(0);
        dIn1 = *(EhsDataflowFloatType*)EHS_FB_RUN_CONTEXT;
    }
    else if (!EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        dIn0 = *(EhsDataflowFloatType*)EHS_FB_RUN_CONTEXT;
        dIn1 = NCAPSA_dIn(1);
    }
    // error condition - neither input is connected - force to false for now
    else
    {
        dIn0 = 1;
        dIn1 = 0;
    }

    /* outputs true if In(1) > In(0), false otherwise...*/
    if (dIn1 > dIn0)
    {
        NCAPSA_bOut(0) = EHS_TRUE;
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        NCAPSA_bOut(0) = EHS_FALSE;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    return;
}
#endif

/******************************************************************************/
/* Define ComparatorGreaterEqualInt1 function block */

EHS_FB_FUNCTIONS_START(ComparatorGreaterEqualInt1)

EHS_FB_FUNCTION_ENTRY("Run_ComparatorGreaterEqualInt", 0x01, ComparatorGreaterEqualInt1)
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
EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterEqualInt1)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(ComparatorGreaterEqualInt1));
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
EHS_FB_INIT_FUNCTION(ComparatorGreaterEqualInt1)
{
    EhsDataflowIntType value;
    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(ComparatorGreaterEqualInt1));
    value = atol(EHS_FB_INIT_PARAMETERS);
    *(EhsDataflowIntType*)EHS_FB_INIT_CONTEXT = value;//atol(pParams);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ComparatorGreaterEqualInt1)
{
    EhsDataflowIntType nIn0;
    EhsDataflowIntType nIn1;

    if (EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        nIn0 = NCAPSA_nIn(0);
        nIn1 = NCAPSA_nIn(1);
    }
    else if (EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && !EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        nIn0 = NCAPSA_nIn(0);
        nIn1 = *(ehs_sint32*)((ehs_uint8*)EHS_FB_RUN_CONTEXT);
    }
    else if (!EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        nIn0 = *(ehs_sint32*)((ehs_uint8*)EHS_FB_RUN_CONTEXT);
        nIn1 = NCAPSA_nIn(1);
    }
    // error condition - neither input is connected - force to false for now
    else
    {
        nIn0 = 1;
        nIn1 = 0;
    }
    /* outputs true if In(1) >= In(0), false otherwise...*/
    if (nIn1 >= nIn0)
    {
        NCAPSA_bOut(0) = EHS_TRUE;
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        NCAPSA_bOut(0) = EHS_FALSE;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define ComparatorGreaterEqualReal function block */

EHS_FB_FUNCTIONS_START(ComparatorGreaterEqualReal1)

EHS_FB_FUNCTION_ENTRY("Run_ComparatorGreaterEqualReal", 0x01, ComparatorGreaterEqualReal1)
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
EHS_FB_IDENTIFY_FUNCTION(ComparatorGreaterEqualReal1)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(ComparatorGreaterEqualReal1));
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
EHS_FB_INIT_FUNCTION(ComparatorGreaterEqualReal1)
{
    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(ComparatorGreaterEqualReal1));
    *(ehs_float*)EHS_FB_INIT_CONTEXT = (ehs_float)atof(EHS_FB_INIT_PARAMETERS);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ComparatorGreaterEqualReal1)
{
    EhsDataflowFloatType dIn0;
    EhsDataflowFloatType dIn1;

    if (EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        dIn0 = NCAPSA_dIn(0);
        dIn1 = NCAPSA_dIn(1);
    }
    else if (EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && !EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        dIn0 = NCAPSA_dIn(0);
        dIn1 = *(EhsDataflowFloatType*)EHS_FB_RUN_CONTEXT;
    }
    else if (!EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        dIn0 = *(EhsDataflowFloatType*)EHS_FB_RUN_CONTEXT;
        dIn1 = NCAPSA_dIn(1);
    }
    // error condition - neither input is connected - force to false for now
    else
    {
        dIn0 = 1;
        dIn1 = 0;
    }

    /* outputs true if In(1) >= In(0), false otherwise...*/
    if (dIn1 >= dIn0)
    {
        NCAPSA_bOut(0) = EHS_TRUE;
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        NCAPSA_bOut(0) = EHS_FALSE;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    return;
}
#endif

/******************************************************************************/
/* Define ComparatorEqualInt1 function block */

EHS_FB_FUNCTIONS_START(ComparatorEqualInt1)

EHS_FB_FUNCTION_ENTRY("Run_ComparatorEqualInt", 0x01, ComparatorEqualInt1)
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
EHS_FB_IDENTIFY_FUNCTION(ComparatorEqualInt1)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(ComparatorEqualInt1));
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
EHS_FB_INIT_FUNCTION(ComparatorEqualInt1)
{
    EhsDataflowIntType value;
    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(ComparatorEqualInt1));
    value = atol(EHS_FB_INIT_PARAMETERS);
    *(EhsDataflowIntType*)EHS_FB_INIT_CONTEXT = value;//atol(pParams);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ComparatorEqualInt1)
{
    EhsDataflowIntType nIn0;
    EhsDataflowIntType nIn1;

    if (EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        nIn0 = NCAPSA_nIn(0);
        nIn1 = NCAPSA_nIn(1);
    }
    else if (EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && !EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        nIn0 = NCAPSA_nIn(0);
        nIn1 = *(ehs_sint32*)((ehs_uint8*)EHS_FB_RUN_CONTEXT);
    }
    else if (!EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        nIn0 = *(ehs_sint32*)((ehs_uint8*)EHS_FB_RUN_CONTEXT);
        nIn1 = NCAPSA_nIn(1);
    }
    // error condition - neither input is connected - force to false for now
    else
    {
        nIn0 = 0;
        nIn1 = 1;
    }

    /* outputs true if In(1) == In(0), false otherwise...*/
    if (nIn1 == nIn0)
    {
        NCAPSA_bOut(0) = EHS_TRUE;
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        NCAPSA_bOut(0) = EHS_FALSE;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    return;
}

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define ComparatorEqualReal function block */

EHS_FB_FUNCTIONS_START(ComparatorEqualReal1)

EHS_FB_FUNCTION_ENTRY("Run_ComparatorEqualReal", 0x01, ComparatorEqualReal1)
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
EHS_FB_IDENTIFY_FUNCTION(ComparatorEqualReal1)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(ComparatorEqualReal1));
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
EHS_FB_INIT_FUNCTION(ComparatorEqualReal1)
{
    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(ComparatorEqualReal1));
    *(ehs_float*)EHS_FB_INIT_CONTEXT = (ehs_float)atof(EHS_FB_INIT_PARAMETERS);
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ComparatorEqualReal1)
{
    EhsDataflowFloatType dIn0;
    EhsDataflowFloatType dIn1;

    if (EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        dIn0 = NCAPSA_dIn(0);
        dIn1 = NCAPSA_dIn(1);
    }
    else if (EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && !EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        dIn0 = NCAPSA_dIn(0);
        dIn1 = *(EhsDataflowFloatType*)EHS_FB_RUN_CONTEXT;
    }
    else if (!EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_0) && EHS_FB_IN_CONNECTED(EHS_FB_COMPEQUALS_IN_1))
    {
        dIn0 = *(EhsDataflowFloatType*)EHS_FB_RUN_CONTEXT;
        dIn1 = NCAPSA_dIn(1);
    }
    // error condition - neither input is connected - force to false for now
    else
    {
        dIn0 = 0;
        dIn1 = 1;
    }

    /* outputs true if In(1) == In(0), false otherwise...*/
    if (dIn1 == dIn0)
    {
        NCAPSA_bOut(0) = EHS_TRUE;
        SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    else
    {
        NCAPSA_bOut(0) = EHS_FALSE;
        SetCompletes2((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    }
    return;
}
#endif

/*============================================================================*/
// Integrator

/******************************************************************************/
/* Define IntegratorInt function block */

EHS_FB_FUNCTIONS_START(IntegratorInt)

EHS_FB_FUNCTION_ENTRY("Run_IntegratorInt", 0x01, IntegratorInt)

EHS_FB_FUNCTION_ENTRY("Run_IntegratorIntReset", 0x02, IntegratorInt_Reset)
EHS_FB_FUNCTIONS_END

#define EHS_FB_INEGRATORINT_OFFSET 0
#define EHS_FB_INEGRATORINT_VAL 1

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(IntegratorInt)
{
    /* the integrator requires memory for an int...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsEventIntegratorIntStruct);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(IntegratorInt)
{
    EhsEventIntegratorIntStruct* pEventIntegrator = (EhsEventIntegratorIntStruct*)EHS_FB_INIT_CONTEXT;
    const ehs_char* pParams = EHS_FB_INIT_PARAMETERS;

    /* make sure the counter is reset...*/
    pEventIntegrator->nOffset = 0;
    pEventIntegrator->nSum = 0;

    if (pParams)
    {
        pParams = EhsGetSint32FromString(&(pEventIntegrator->nOffset), pParams);
    }

    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(IntegratorInt)
{
    EhsEventIntegratorIntStruct* pEventIntegrator = (EhsEventIntegratorIntStruct*)EHS_FB_INIT_CONTEXT;
    int nSum;
    int nRes;

    /* simply add the input value to the object state data...*/
    nSum = pEventIntegrator->nSum;
    if (EHS_FB_IN_CONNECTED(EHS_FB_INEGRATORINT_VAL))
    {
        nSum += NCAPSA_nIn(EHS_FB_INEGRATORINT_VAL);
    }
    /* copy to state data and output...*/
    pEventIntegrator->nSum = nSum; // note - accumulate values before add the offset, in case offset changes whilst integrating

    // add offset to cumulative value
    if (EHS_FB_IN_CONNECTED(EHS_FB_INEGRATORINT_OFFSET))
    {
        pEventIntegrator->nOffset = NCAPSA_nIn(EHS_FB_INEGRATORINT_OFFSET); // take offset from integer input if connected, else use parameter.
    }
    nRes = nSum + pEventIntegrator->nOffset;



    NCAPSA_nOut(0) = nRes;
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
EHS_FB_RUN_FUNCTION(IntegratorInt_Reset)
{
    EhsEventIntegratorIntStruct* pEventIntegrator = (EhsEventIntegratorIntStruct*)EHS_FB_INIT_CONTEXT;
    int nSum;

    /* simply reset the object state data and write to output...*/
    if (EHS_FB_IN_CONNECTED(EHS_FB_INEGRATORINT_OFFSET))
    {
        pEventIntegrator->nOffset = NCAPSA_nIn(EHS_FB_INEGRATORINT_OFFSET); // take offset from integer input if connected, else use parameter.
    }
    pEventIntegrator->nSum = 0;
    nSum = pEventIntegrator->nOffset + pEventIntegrator->nSum;	// reset to offset value

    NCAPSA_nOut(0) = nSum;
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}


#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define IntegratorReal function block */

EHS_FB_FUNCTIONS_START(IntegratorReal)

EHS_FB_FUNCTION_ENTRY("Run_IntegratorReal", 0x01, IntegratorReal)

EHS_FB_FUNCTION_ENTRY("Run_IntegratorRealReset", 0x02, IntegratorReal_Reset)
EHS_FB_FUNCTIONS_END

#define EHS_FB_INEGRATORREAL_OFFSET 0
#define EHS_FB_INEGRATORREAL_VAL 1

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(IntegratorReal)
{
    /* the integrator requires memory for a ehs_float...*/
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsEventIntegratorRealStruct);
    return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(IntegratorReal)
{
    ehs_float dInit;
    EhsEventIntegratorRealStruct* pEventIntegrator = (EhsEventIntegratorRealStruct*)EHS_FB_INIT_CONTEXT;

    /* make sure the counter is reset...*/
    pEventIntegrator->fOffset = 0.0;
    pEventIntegrator->fSum = 0.0;

    dInit = (ehs_float)atof(EHS_FB_INIT_PARAMETERS);
    pEventIntegrator->fOffset = dInit;


    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(IntegratorReal)
{
    EhsEventIntegratorRealStruct* pEventIntegrator = (EhsEventIntegratorRealStruct*)EHS_FB_INIT_CONTEXT;
    ehs_float dSum;
    ehs_float dRes;

    /* simply add the input value to the object state data...*/
    dSum = pEventIntegrator->fSum;
    if (EHS_FB_IN_CONNECTED(EHS_FB_INEGRATORREAL_VAL))
    {
        dSum += NCAPSA_dIn(EHS_FB_INEGRATORREAL_VAL);
    }
    /* copy to state data and output...*/
    pEventIntegrator->fSum = dSum; // note - accumulate values before add the offset, in case offset changes whilst integrating

    // add offset to cumulative value
    if (EHS_FB_IN_CONNECTED(EHS_FB_INEGRATORREAL_OFFSET))
    {
        pEventIntegrator->fOffset = NCAPSA_dIn(EHS_FB_INEGRATORREAL_OFFSET); // take offset from integer input if connected, else use parameter.
    }
    dRes = dSum + pEventIntegrator->fOffset;



    NCAPSA_dOut(0) = dRes;
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
EHS_FB_RUN_FUNCTION(IntegratorReal_Reset)
{
    EhsEventIntegratorRealStruct* pEventIntegrator = (EhsEventIntegratorRealStruct*)EHS_FB_INIT_CONTEXT;
    ehs_float dSum;

    /* simply reset the object state data and write to output...*/
    if (EHS_FB_IN_CONNECTED(EHS_FB_INEGRATORREAL_OFFSET))
    {
        pEventIntegrator->fOffset = NCAPSA_dIn(EHS_FB_INEGRATORREAL_OFFSET); // take offset from integer input if connected, else use parameter.
    }
    pEventIntegrator->fSum = 0;
    dSum = pEventIntegrator->fOffset + pEventIntegrator->fSum;	// reset to offset value

    NCAPSA_dOut(0) = dSum;
    SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);

    return;
}

#endif

/******************************************************************************/
/* Define MaxInt function block */

EHS_FB_FUNCTIONS_START(MaxInt)

EHS_FB_FUNCTION_ENTRY("Run_MaxInt", 0x01, MaxInt)
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
EHS_FB_IDENTIFY_FUNCTION(MaxInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MaxInt)
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
EHS_FB_RUN_FUNCTION(MaxInt)
{
    if (NCAPSA_nIn(0) < NCAPSA_nIn(1))
    {
        NCAPSA_nOut(0) = NCAPSA_nIn(1);
    }
    else
    {
        NCAPSA_nOut(0) = NCAPSA_nIn(0);
    }

    SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}

/******************************************************************************/
/* Define MinInt function block */

EHS_FB_FUNCTIONS_START(MinInt)

EHS_FB_FUNCTION_ENTRY("Run_MinInt", 0x01, MinInt)
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
EHS_FB_IDENTIFY_FUNCTION(MinInt)
{
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(MinInt)
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
EHS_FB_RUN_FUNCTION(MinInt)
{
    if (NCAPSA_nIn(0) > NCAPSA_nIn(1))
    {
        NCAPSA_nOut(0) = NCAPSA_nIn(1);
    }
    else
    {
        NCAPSA_nOut(0) = NCAPSA_nIn(0);
    }

    SetCompletes1((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
    return;
}
