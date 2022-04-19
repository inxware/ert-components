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
 * Lucid project stage two - NcapsaLtd - May 2005 - SDG
*/

#include "target.h"
//#include "types.h"
//#include "EHSMacros.h"
#include "demux.h"
#include "setCompletes.h"
/* For new function implementations */
#include "ehs_comp_api2.h"

#include "hal-api.h" /* Required for logging */


/******************************************************************************/
/* Define DemultiplexTwoOutputInt function block */

EHS_FB_FUNCTIONS_START(DemultiplexTwoOutputInt)
EHS_FB_FUNCTION_ENTRY("Run_Output1", DemultiplexTwoOutputInt1)
EHS_FB_FUNCTION_ENTRY("Run_Output2", DemultiplexTwoOutputInt2)
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
EHS_FB_FUNCTION_ENTRY("Run_Output1", DemultiplexThreeOutputInt1)
EHS_FB_FUNCTION_ENTRY("Run_Output2", DemultiplexThreeOutputInt2)
EHS_FB_FUNCTION_ENTRY("Run_Output3", DemultiplexThreeOutputInt3)
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
EHS_FB_FUNCTION_ENTRY("Run_Output1", DemultiplexFourOutputInt1)
EHS_FB_FUNCTION_ENTRY("Run_Output2", DemultiplexFourOutputInt2)
EHS_FB_FUNCTION_ENTRY("Run_Output3", DemultiplexFourOutputInt3)
EHS_FB_FUNCTION_ENTRY("Run_Output4", DemultiplexFourOutputInt4)
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
/* Define DemultiplexTwoOutputBool function block */

EHS_FB_FUNCTIONS_START(DemultiplexTwoOutputBool)
EHS_FB_FUNCTION_ENTRY("Run_Output1", DemultiplexTwoOutputBool1)
EHS_FB_FUNCTION_ENTRY("Run_Output2", DemultiplexTwoOutputBool2)
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
EHS_FB_FUNCTION_ENTRY("Run_Output1", DemultiplexThreeOutputBool1)
EHS_FB_FUNCTION_ENTRY("Run_Output2", DemultiplexThreeOutputBool2)
EHS_FB_FUNCTION_ENTRY("Run_Output3", DemultiplexThreeOutputBool3)
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
EHS_FB_FUNCTION_ENTRY("Run_Output1", DemultiplexFourOutputBool1)
EHS_FB_FUNCTION_ENTRY("Run_Output2", DemultiplexFourOutputBool2)
EHS_FB_FUNCTION_ENTRY("Run_Output3", DemultiplexFourOutputBool3)
EHS_FB_FUNCTION_ENTRY("Run_Output4", DemultiplexFourOutputBool4)
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
/* Define ConvertorBoolToTrigger function block */

EHS_FB_FUNCTIONS_START(DemultiplexTwoOutputString)
EHS_FB_FUNCTION_ENTRY("Run_Output1", DemultiplexTwoOutputString1)
EHS_FB_FUNCTION_ENTRY("Run_Output2", DemultiplexTwoOutputString2)
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
	strcpy(EHS_FB_OUT_S(0), EHS_FB_IN_S(0));
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
	strcpy(EHS_FB_OUT_S(0), EHS_FB_IN_S(0));
	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
	return;
}

/******************************************************************************/
/* Define DemultiplexThreeOutputString function block */

EHS_FB_FUNCTIONS_START(DemultiplexThreeOutputString)
EHS_FB_FUNCTION_ENTRY("Run_Output1", DemultiplexThreeOutputString1)
EHS_FB_FUNCTION_ENTRY("Run_Output2", DemultiplexThreeOutputString2)
EHS_FB_FUNCTION_ENTRY("Run_Output3", DemultiplexThreeOutputString3)
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
	strcpy(EHS_FB_OUT_S(0), EHS_FB_IN_S(0));
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
	strcpy(EHS_FB_OUT_S(0), EHS_FB_IN_S(0));
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
	strcpy(EHS_FB_OUT_S(0), EHS_FB_IN_S(0));
	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
	return;
}

/******************************************************************************/
/* Define DemultiplexFourOutputString function block */

EHS_FB_FUNCTIONS_START(DemultiplexFourOutputString)
EHS_FB_FUNCTION_ENTRY("Run_Output1", DemultiplexFourOutputString1)
EHS_FB_FUNCTION_ENTRY("Run_Output2", DemultiplexFourOutputString2)
EHS_FB_FUNCTION_ENTRY("Run_Output3", DemultiplexFourOutputString3)
EHS_FB_FUNCTION_ENTRY("Run_Output4", DemultiplexFourOutputString4)
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
	strcpy(EHS_FB_OUT_S(0), EHS_FB_IN_S(0));
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
	strcpy(EHS_FB_OUT_S(0), EHS_FB_IN_S(0));
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
	strcpy(EHS_FB_OUT_S(0), EHS_FB_IN_S(0));
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
	strcpy(EHS_FB_OUT_S(0), EHS_FB_IN_S(0));
	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
	return;
}


/* common index demux initialisation functions */

typedef struct {
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
EHS_FB_FUNCTION_ENTRY("Run", IndexedDemultiplexer_String)
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

	if (index>8) {
		EHS_FB_OUT_I(8)=index-8; /**< shouldn't this be EHS_FB_OUT_I(1) @todo check this */
		EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,9);
	}
	else if (index<1) {
	}
	else { //Everything OK !
		EhsStrcpy(EHS_FB_OUT_S(index-1), EHS_FB_IN_S(0)); // index-1+1: the output is offset by one as the offset is first output)
		EHS_FB_OUT_I(8)=0;
		EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,index); /** only trigger the appropriate output @todo these are not 0 indexed */
	}
	return;
}

/******************************************************************************/
/* Define IndexedDemultiplexer_Int function block */
//@todo - this code refers to function in Components sandbox, move/remove?
EHS_FB_FUNCTIONS_START(IndexedDemultiplexer_Int)
EHS_FB_FUNCTION_ENTRY("Run", IndexedDemultiplexer_Int)
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

	if (index>8) {
		EHS_FB_OUT_I(8)=index-8; /**< shouldn't this be EHS_FB_OUT_I(1) @todo check this */
		EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,9);
	}
	else if (index<1) {
	}
	else { //Everything OK !
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
EHS_FB_FUNCTION_ENTRY("Run", IndexedDemultiplexer_Bool)
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

	if (index>8) {
		EHS_FB_OUT_I(8)=index-8; /**< shouldn't this be EHS_FB_OUT_I(1) @todo check this */
		EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,9);
	}
	else if (index<1) {
	}
	else { //Everything OK !
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
EHS_FB_FUNCTION_ENTRY("Run", IndexedDemultiplexer_Event)
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

	if (index>8) {
		EHS_FB_OUT_I(0)=index-8;
		EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,9);
	}
	else if (index<1) {
	}
	else { //Everything OK !
		EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,index); /** only trigger the appropriate output @todo these are not 0 indexed */
	}
	return;
}

/*************************************************************************************
 *  Text Case Event Demux
 *  ************************************************************************************/


typedef struct {
	ehs_char * parmCases[8]; //allocated dynamically (auto deallocated) where entries are available
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
	for (i = 0; i< 8 && pParams ; i++) {
		temp[0]='\0';
		pParams = EhsStrTrimL(pParams);
		if (EhsStrncmp(pParams,"NULL",4) == 0) {
			pCtx->parmCases[i] = NULL;
			pParams+=4;
		} else {
			if (pParams[0] == '"') {
				pParams = EhsGetQuoteDelimFromString(temp, pParams, EHS_STRING_LENGTH_MAX);
			}
			else { // assume we have some bare text
				pParams = EhsGetWordFromString(temp, pParams);
			}

			if (EhsStrlen(temp) > 0) {
				pCtx->parmCases[i] = EhsHMem_writeableAlloc(EHS_STRING_LENGTH_MAX);
				if (pCtx->parmCases[i])
					EhsStrcpy(pCtx->parmCases[i],temp);
			}
			else {
				pCtx->parmCases[i] = NULL;
			}
		}
	}
	return EHS_TRUE; /* initialisation always succeeds */
}

/******************************************************************************/
/* Define IndexedDemultiplexer_Event function block */
// other indexed demuxs have been moved to Components sandbox as can be replaced by other components combinations, this one can't so we're keeping it
//@todo - move code to events
EHS_FB_FUNCTIONS_START(TextCaseDemultiplexer_Event)
EHS_FB_FUNCTION_ENTRY("Run", TextCaseDemultiplexer_Event)
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
	for (i = 0; i< 8 ; i++) {
		if (EHS_FB_IN_CONNECTED_API2(i + EHS_PORTNUM_TextCaseDemultiplexer_Case_1)) psZcase[i] = EHS_FB_IN_S_API2( i + EHS_PORTNUM_TextCaseDemultiplexer_Case_1 );
		else  psZcase[i] = pCtx->parmCases[i];
	}

	//return;
	/*  Check for matches */
	if (EHS_FB_IN_CONNECTED_API2(EHS_PORTNUM_TextCaseDemultiplexer_Test)) {
		for (i = 0; i< 8 ; i++) {
			if (psZcase[i]) {
				if (EhsStrcmp(psZcase[i],EHS_FB_IN_S_API2(EHS_PORTNUM_TextCaseDemultiplexer_Test)) == 0) {
					EHS_FB_FINISH_API2(EHS_PORTNUM_TextCaseDemultiplexer_EventCase1+i);
					bDefault = EHS_FALSE;
					break; // we only fire the first one found
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
EHS_FB_FUNCTION_ENTRY("Run_Output1", DemultiplexTwoOutputFloat1)
EHS_FB_FUNCTION_ENTRY("Run_Output2", DemultiplexTwoOutputFloat2)
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
EHS_FB_FUNCTION_ENTRY("Run_Output1", DemultiplexThreeOutputFloat1)
EHS_FB_FUNCTION_ENTRY("Run_Output2", DemultiplexThreeOutputFloat2)
EHS_FB_FUNCTION_ENTRY("Run_Output3", DemultiplexThreeOutputFloat3)
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
EHS_FB_FUNCTION_ENTRY("Run_Output1", DemultiplexFourOutputFloat1)
EHS_FB_FUNCTION_ENTRY("Run_Output2", DemultiplexFourOutputFloat2)
EHS_FB_FUNCTION_ENTRY("Run_Output3", DemultiplexFourOutputFloat3)
EHS_FB_FUNCTION_ENTRY("Run_Output4", DemultiplexFourOutputFloat4)
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
/* Define IndexedDemultiplexer_Float function block */
//@todo - this code refers to function in Components sandbox, move/remove?
EHS_FB_FUNCTIONS_START(IndexedDemultiplexer_Float)
EHS_FB_FUNCTION_ENTRY("Run", IndexedDemultiplexer_Float)
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

	if (index>8) {
		EHS_FB_OUT_I(8)=index-8; /**< shouldn't this be EHS_FB_OUT_I(1) @todo check this */
		EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,9);
	}
	else if (index<1) {
	}
	else { //Everything OK !
		EHS_FB_OUT_F(index-1) = value; // index-1+1: the output is offset by one as the offset is first output)
		EHS_FB_OUT_I(8)=0;
		EhsFunctionInstanceData_triggerEvent((structFuncArg*)EHS_FB_RUN_CONTEXT_REF,index); /** only trigger the appropriate output @todo these are not 0 indexed */
	}
	return;
}

#endif
