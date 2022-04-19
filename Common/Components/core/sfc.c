/**
 * sfc.h
 *
 * function prototypes for sfc.c - for state flow control? Not really sure.
 *
 * @author: ??
 * @version: $Revision: 2780 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#include "globals.h"
#include "sfc.h"
#include "setCompletes.h"
#include "hal-api.h" /* Required for logging */

/******************************************************************************/
/* Define xyzzy function block */

EHS_FB_FUNCTIONS_START(SFCBar)
EHS_FB_FUNCTION_ENTRY("Run_SFCBarPreviousStateComplete", SFCBarPreviousStateComplete)
EHS_FB_FUNCTION_ENTRY("Run_SFCBarConditionUpdate", SFCBarConditionUpdate)
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
EHS_FB_IDENTIFY_FUNCTION(SFCBar)
{
	/* reserve state data for a ehs_bool..*/
	EHS_FB_IDENTIFY_MEMORY =  sizeof(ehs_bool);
	return;
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(SFCBar)
{
	ehs_bool* pBOOL = NULL;

	/* cast the pointer to type ehs_bool...*/
	pBOOL = (ehs_bool*)EHS_FB_INIT_CONTEXT;
	
	
	
	/* make sure  ehs_bool is initially false...*/
	*pBOOL = EHS_FALSE;
	
	return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(SFCBarPreviousStateComplete)
{
	ehs_bool* waiting = NULL;


	waiting = (ehs_bool*)EHS_FB_RUN_CONTEXT; //#define EHS_FB_RUN_CONTEXT (BYTE*)*pByte
	
/*  */
	if ( NCAPSA_bIn(0) == EHS_TRUE)
	{		
		*waiting = EHS_FALSE;
		SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
	}
	else
	{
		*waiting = EHS_TRUE;
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
EHS_FB_RUN_FUNCTION(SFCBarConditionUpdate)
{
	ehs_bool* waiting = NULL;


	waiting = (ehs_bool*)EHS_FB_RUN_CONTEXT;
	
	
	
	
	if( ( NCAPSA_bIn(0) == EHS_TRUE) && (*waiting == EHS_TRUE) )
	{		
		*waiting = EHS_FALSE;
		SetCompletes((structFuncArg*)EHS_FB_RUN_CONTEXT_REF);
	}

	return;
	
}

