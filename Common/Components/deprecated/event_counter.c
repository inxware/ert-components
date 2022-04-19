/* EventCounter.c
 *
 * Counts events up to a specified threshold.
 *
 *  * Lucid project stage one - NcapsaLtd - May 2005 - ADS
*/

/**
 * @page FunctionReference Lucid Function Block Reference Guide
 * @section VersatileEventCounter
 * @anchor VersatileEventCounter
 * @subsection appliesTo Applies To:
 * Event &gt; Counter Versatile
 * @subsection properties Properties:
 * <b>Threshold.</b> This is the upper limit for the counter. If the counter
 * exceeds this value, an overflow event is generated. If this value is zero, the threshold input is
 * read.
 *
 * @subsection description Description:
 * The <em>start</em> event enables the <em>inc</em> and <em>dec</em> events. Once enabled, each occurrence of 
 * <em>inc</em> increments and <em>dec</em> decrements an internal counter, whose value is output as an integer.
 *
 * If the counter exceeds the threshold value (which is either set as a property or via the <em>thresh</em> input,
 * The <em>ovf</em> event is asserted. The threshold is checked every time a counter is incremented or decremented.
 * 
 */
#include "globals.h"
#include "event_counter.h"
#include "fid.h"
#include "target.h"
#include "hal-api.h" /* Needed for logging */

/******************************************************************************/
/* Define DemultiplexTwoOutputInt function block */

EHS_FB_FUNCTIONS_START(EventCounterVersatile)
EHS_FB_FUNCTION_ENTRY("Run_StartEventCounter", EventCounterVersatile_Start)
EHS_FB_FUNCTION_ENTRY("Run_ResetEventCounter", EventCounterVersatile_Reset)
EHS_FB_FUNCTION_ENTRY("Run_CountEventCounter", EventCounterVersatile_Count)
EHS_FB_FUNCTION_ENTRY("Run_DecrementEventCounter", EventCounterVersatile_Decrement)
EHS_FB_FUNCTION_ENTRY("Run_StopEventCounter", EventCounterVersatile_Stop)
EHS_FB_FUNCTIONS_END

#define EHS_FB_EVENTCOUNTER_THRESHOLD 0
/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(EventCounterVersatile)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(structEventCounterObj);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventCounterVersatile)
{
	int nThreshold;
	int nRet=1;
	structEventCounterObj* pEventCounterObj = (structEventCounterObj*)EHS_FB_INIT_CONTEXT;

	pEventCounterObj->bEventCounterRunning = EHS_FALSE;
	pEventCounterObj->nCount = 0;
	/* parse parameter string */
	//nRet = sscanf(pParams, "%d", &nThreshold );Uses loads of memory
	//sgetInt(&nThreshold, pParams); // don't need the end bit
	nThreshold=atoi(EHS_FB_INIT_PARAMETERS);
	if( nRet == 1 )
	{
		pEventCounterObj->nThreshold = nThreshold;
	}
	else
	{
		//printf("Parameter Parsing Error\n");
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
EHS_FB_RUN_FUNCTION(EventCounterVersatile_Start)
{
	structEventCounterObj* pEventCounterObj = (structEventCounterObj*)EHS_FB_RUN_CONTEXT;

	if( !pEventCounterObj->bEventCounterRunning )
	{
		pEventCounterObj->bEventCounterRunning = EHS_TRUE;
		
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
EHS_FB_RUN_FUNCTION(EventCounterVersatile_Stop)
{
	structEventCounterObj* pEventCounterObj = (structEventCounterObj*)EHS_FB_RUN_CONTEXT;
	if( pEventCounterObj->bEventCounterRunning )
	{
		pEventCounterObj->bEventCounterRunning = EHS_FALSE;
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
EHS_FB_RUN_FUNCTION(EventCounterVersatile_Reset)
{
	structEventCounterObj* pEventCounterObj = (structEventCounterObj*)EHS_FB_RUN_CONTEXT;

	if( pEventCounterObj->bEventCounterRunning )
	{
		pEventCounterObj->nCount = 0;
		NCAPSA_nOut(0) = 0;
		//SetCompletes1(pByte);           // set increment event.
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
EHS_FB_RUN_FUNCTION(EventCounterVersatile_Count)
{
	int nCount;
	EhsDataflowIntType nThreshold;
	structEventCounterObj* pEventCounterObj = (structEventCounterObj*)EHS_FB_RUN_CONTEXT;
	EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(EventCounterVersatile_Count));
	if( pEventCounterObj->bEventCounterRunning )
	{
		nCount = pEventCounterObj->nCount;
		nThreshold = pEventCounterObj->nThreshold;
		if( !nThreshold )    // if threshold count is not defined by a parameter.
		{
			nThreshold = NCAPSA_nIn(EHS_FB_EVENTCOUNTER_THRESHOLD);  // take threshold from integer input.
			if( !nThreshold )            // if integer input is also zero set max pos integer value as threshold.
			{
				nThreshold = EHS_DATAFLOW_INT_MAX;
			}
		}
		nCount++;					// increment count.
		if( nCount < 0 )
			nCount = 0;				// PD changed to saturate - this should be an wrap/saturate option put for future - previously wrap to zero when integer goes negative.
		if( nCount > nThreshold )   // PD ditto
		{
			nCount = nThreshold;
			SetCompletes2((structFuncArg *)EHS_FB_RUN_CONTEXT_REF);   // set overflow event.
		}
		pEventCounterObj->nCount = nCount;
		NCAPSA_nOut(0) = nCount;
		SetCompletes1((structFuncArg *)EHS_FB_RUN_CONTEXT_REF);           // set increment event.
	}
	return;
}

EHS_FB_RUN_FUNCTION(EventCounterVersatile_Decrement)
{
	signed int nCount;
	EhsDataflowIntType nThreshold;
	structEventCounterObj* pEventCounterObj = (structEventCounterObj*)EHS_FB_RUN_CONTEXT;
	EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(EventCounterVersatile_Count));
	if( pEventCounterObj->bEventCounterRunning )
	{
		nCount = pEventCounterObj->nCount;
		nThreshold = pEventCounterObj->nThreshold;
		if( !nThreshold )    // if threshold count is not defined by a parameter.
		{
			nThreshold = NCAPSA_nIn(EHS_FB_EVENTCOUNTER_THRESHOLD);  // take threshold from integer input.
			if( !nThreshold )            // if integer input is also zero set max pos integer value as threshold.
			{
				nThreshold = EHS_DATAFLOW_INT_MAX;
			}
		}
		nCount--;					// increment count.
		if( nCount < 0 )
			nCount = 0;				// PD changed to saturate - this should be an wrap/saturate option put for future - previously wrap to zero when integer goes negative.
		if( nCount > nThreshold )   // PD ditto
		{
			nCount = nThreshold;
			SetCompletes2((structFuncArg *)EHS_FB_RUN_CONTEXT_REF);   // set overflow event.
		}
		pEventCounterObj->nCount = nCount;
		NCAPSA_nOut(0) = nCount;
		SetCompletes1((structFuncArg *)EHS_FB_RUN_CONTEXT_REF);           // set increment event.
	}
	return;
}

