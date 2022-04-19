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
 * Lucid project stage two - NcapsaLtd - May 2005 - SDG
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

EHS_FB_FUNCTIONS_START(MultiplexTwoInputInt)
EHS_FB_FUNCTION_ENTRY("Run_Input1", MultiplexTwoInputInt1)
EHS_FB_FUNCTION_ENTRY("Run_Input2", MultiplexTwoInputInt2)
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
	{	//printf(" MuxInput1 value=%d, add=%p \n",*px,(NCAPSA_pMain)->pIn[0]);
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
	{	//printf(" MuxInput2 value=%d, add=%p \n",*px,(NCAPSA_pMain)->pIn[0]);
	}
	NCAPSA_nOut(0) = NCAPSA_nIn(0);
	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
	return;
}

/******************************************************************************/
/* Define MultiplexThreeInputInt function block */

EHS_FB_FUNCTIONS_START(MultiplexThreeInputInt)
EHS_FB_FUNCTION_ENTRY("Run_Input1", MultiplexThreeInputInt1)
EHS_FB_FUNCTION_ENTRY("Run_Input2", MultiplexThreeInputInt2)
EHS_FB_FUNCTION_ENTRY("Run_Input3", MultiplexThreeInputInt3)
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
EHS_FB_FUNCTION_ENTRY("Run_Input1", MultiplexFourInputInt1)
EHS_FB_FUNCTION_ENTRY("Run_Input2", MultiplexFourInputInt2)
EHS_FB_FUNCTION_ENTRY("Run_Input3", MultiplexFourInputInt3)
EHS_FB_FUNCTION_ENTRY("Run_Input4", MultiplexFourInputInt4)
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

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Define MultiplexTwoInputReal function block */

EHS_FB_FUNCTIONS_START(MultiplexTwoInputFloat)
EHS_FB_FUNCTION_ENTRY("Run_Input1", MultiplexTwoInputFloat1)
EHS_FB_FUNCTION_ENTRY("Run_Input2", MultiplexTwoInputFloat2)
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
EHS_FB_FUNCTION_ENTRY("Run_Input1", MultiplexThreeInputFloat1)
EHS_FB_FUNCTION_ENTRY("Run_Input2", MultiplexThreeInputFloat2)
EHS_FB_FUNCTION_ENTRY("Run_Input3", MultiplexThreeInputFloat3)
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
EHS_FB_FUNCTION_ENTRY("Run_Input1", MultiplexFourInputFloat1)
EHS_FB_FUNCTION_ENTRY("Run_Input2", MultiplexFourInputFloat2)
EHS_FB_FUNCTION_ENTRY("Run_Input3", MultiplexFourInputFloat3)
EHS_FB_FUNCTION_ENTRY("Run_Input4", MultiplexFourInputFloat4)
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

#endif /* EHS_TARGET_FP_SUPPORT */
/******************************************************************************/
/* Define MultiplexTwoInputBool function block */

EHS_FB_FUNCTIONS_START(MultiplexTwoInputBool)
EHS_FB_FUNCTION_ENTRY("Run_Input1", MultiplexTwoInputBool1)
EHS_FB_FUNCTION_ENTRY("Run_Input2", MultiplexTwoInputBool2)
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
EHS_FB_FUNCTION_ENTRY("Run_Input1", MultiplexThreeInputBool1)
EHS_FB_FUNCTION_ENTRY("Run_Input2", MultiplexThreeInputBool2)
EHS_FB_FUNCTION_ENTRY("Run_Input3", MultiplexThreeInputBool3)
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
EHS_FB_FUNCTION_ENTRY("Run_Input1", MultiplexFourInputBool1)
EHS_FB_FUNCTION_ENTRY("Run_Input2", MultiplexFourInputBool2)
EHS_FB_FUNCTION_ENTRY("Run_Input3", MultiplexFourInputBool3)
EHS_FB_FUNCTION_ENTRY("Run_Input4", MultiplexFourInputBool4)
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
EHS_FB_RUN_FUNCTION(MultiplexFourInputBool2){
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
EHS_FB_RUN_FUNCTION(MultiplexFourInputBool3){
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
EHS_FB_RUN_FUNCTION(MultiplexFourInputBool4){
	NCAPSA_bOut(0) = NCAPSA_bIn(0);
	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
	return;
}

/******************************************************************************/
/* Define MultiplexTwoInputString function block */

EHS_FB_FUNCTIONS_START(MultiplexTwoInputString)
EHS_FB_FUNCTION_ENTRY("Run_Input1", MultiplexTwoInputString1)
EHS_FB_FUNCTION_ENTRY("Run_Input2", MultiplexTwoInputString2)
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
	//printf("Output string 1=%s\n",NCAPSA_szOut(0));
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
	//printf("Output string 2=%s\n",NCAPSA_szOut(0));
	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
	return;
}

/******************************************************************************/
/* Define MultiplexThreeInputString function block */

EHS_FB_FUNCTIONS_START(MultiplexThreeInputString)
EHS_FB_FUNCTION_ENTRY("Run_Input1", MultiplexThreeInputString1)
EHS_FB_FUNCTION_ENTRY("Run_Input2", MultiplexThreeInputString2)
EHS_FB_FUNCTION_ENTRY("Run_Input3", MultiplexThreeInputString3)
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
EHS_FB_FUNCTION_ENTRY("Run_Input1", MultiplexFourInputString1)
EHS_FB_FUNCTION_ENTRY("Run_Input2", MultiplexFourInputString2)
EHS_FB_FUNCTION_ENTRY("Run_Input3", MultiplexFourInputString3)
EHS_FB_FUNCTION_ENTRY("Run_Input4", MultiplexFourInputString4)
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
/* Define IndexedMultiplexer_Event function block */

EHS_FB_FUNCTIONS_START(IndexedMultiplexer_Event)
EHS_FB_FUNCTION_ENTRY("Run_Input1", IndexedMultiplexer_Event_1)
EHS_FB_FUNCTION_ENTRY("Run_Input2", IndexedMultiplexer_Event_2)
EHS_FB_FUNCTION_ENTRY("Run_Input3", IndexedMultiplexer_Event_3)
EHS_FB_FUNCTION_ENTRY("Run_Input4", IndexedMultiplexer_Event_4)
EHS_FB_FUNCTION_ENTRY("Run_Input5", IndexedMultiplexer_Event_5)
EHS_FB_FUNCTION_ENTRY("Run_Input6", IndexedMultiplexer_Event_6)
EHS_FB_FUNCTION_ENTRY("Run_Input7", IndexedMultiplexer_Event_7)
EHS_FB_FUNCTION_ENTRY("Run_Input8", IndexedMultiplexer_Event_8)
EHS_FB_FUNCTION_ENTRY("Reset", IndexedMultiplexer_Event_Reset)
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

	if (pParams) {
		pParams = EhsGetUint8FromString(&nByte, pParams);
		pIndexedEventObj->bRequiresResetting = (ehs_bool)nByte;
	} else {
		/* we haven't got an initial parameter */
		bRet = EHS_FALSE;
	}

//	printf("\nbRequiresResetting=[%d]\n",pIndexedEventObj->bRequiresResetting);
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
//	printf("bRequiresResetting=[%d],bHasFired=[%d]\n",pIndexedEventObj->bRequiresResetting,pIndexedEventObj->bHasFired);

	// if not manual resetting mode or has not been triggered since last reset, trigger
	if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired) {
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
	if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired) {
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
	if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired) {
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
	if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired) {
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
	if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired) {
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
	if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired) {
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
	if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired) {
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
	if (!pIndexedEventObj->bRequiresResetting || !pIndexedEventObj->bHasFired) {
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
