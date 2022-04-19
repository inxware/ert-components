/* logic.h
 *
 * logic functions that operate on Boolean data.
 *
 * For definition of arguments in Ncapsa functions (Identify_, Init_ and Run_)
 * please see types.h.
 *
 * Lucid project stage one - NcapsaLtd - March 2005 - MDD
 */
#include "logic.h"
#include "setCompletes.h"
#include "hal-api.h" /* Required for logging */

/******************************************************************************/
/* Define xyzzy function block */

EHS_FB_FUNCTIONS_START(LogicNot)
EHS_FB_FUNCTION_ENTRY("Run_LogicNot", LogicNot)
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
	//printf("Notting in:%d, out: %d",in,!in);
	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
	return;
}

/******************************************************************************/
/* Define LogicTwoInputAnd function block */

EHS_FB_FUNCTIONS_START(LogicTwoInputAnd)
EHS_FB_FUNCTION_ENTRY("Run_LogicTwoInputAnd", LogicTwoInputAnd)
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
	//printf("Anding in1:%d, in2: %d",NCAPSA_bIn(0),NCAPSA_bIn(1));
	SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
	return;
}

/******************************************************************************/
/* Define LogicThreeInputAnd function block */

EHS_FB_FUNCTIONS_START(LogicThreeInputAnd)
EHS_FB_FUNCTION_ENTRY("Run_LogicThreeInputAnd", LogicThreeInputAnd)
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
EHS_FB_FUNCTION_ENTRY("Run_LogicFourInputAnd", LogicFourInputAnd)
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
EHS_FB_FUNCTION_ENTRY("Run_LogicTwoInputOr", LogicTwoInputOr)
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
EHS_FB_FUNCTION_ENTRY("Run_LogicThreeInputOr", LogicThreeInputOr)
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
EHS_FB_FUNCTION_ENTRY("Run_LogicFourInputOr", LogicFourInputOr)
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
EHS_FB_FUNCTION_ENTRY("Run_LogicTwoInputNor", LogicTwoInputNor)
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
EHS_FB_FUNCTION_ENTRY("Run_LogicThreeInputNor", LogicThreeInputNor)
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
EHS_FB_FUNCTION_ENTRY("Run_LogicFourInputNor", LogicFourInputNor)
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
EHS_FB_FUNCTION_ENTRY("Run_LogicTwoInputNand", LogicTwoInputNand)
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
EHS_FB_FUNCTION_ENTRY("Run_LogicThreeInputNand", LogicThreeInputNand)
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
EHS_FB_FUNCTION_ENTRY("Run_LogicFourInputNand", LogicFourInputNand)
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
EHS_FB_FUNCTION_ENTRY("Run_LogicXor", LogicXor)
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


