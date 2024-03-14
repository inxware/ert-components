/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/* EventCounter1.c
 *
 * Counts events up to a specified threshold.
 *
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
#include "fid.h"
#include "app_data.h"
#include "hal-api.h" /* Required for logging */
#include "event_counter1.h"

/**
 * Event counter controller
 */
typedef struct
{
    ehs_bool bInitialise;					// is first time run or when reset
    ehs_bool bEventCounterRunning;			// is counter enabled
    ehs_bool bEnableAtInit;					// enable counter at init time
    EhsDataflowIntType nInitialValue;		// initial counter value
    EhsDataflowIntType nStepSize;			// stpe size of count
    EhsDataflowIntType nMaxThreshold;		// Max. threshold value from object params. Nb no analysis performed for choice of 16 or 32 bit counter.
    EhsDataflowIntType nMinThreshold;		// Min. threshold value from object params.
    EhsDataflowIntType nCount;				// actual count at any time.
} EhsEventCounterStruct;

/******************************************************************************/
/* Define DemultiplexTwoOutputInt function block */

EHS_FB_FUNCTIONS_START(EventCounterVersatile1)

EHS_FB_FUNCTION_ENTRY("Run_StartEventCounter", 0x01, EventCounterVersatile1_Enable)

EHS_FB_FUNCTION_ENTRY("Run_CountEventCounter", 0x02, EventCounterVersatile1_Count)

EHS_FB_FUNCTION_ENTRY("Run_DecrementEventCounter", 0x03, EventCounterVersatile1_Decrement)

EHS_FB_FUNCTION_ENTRY("Run_ResetEventCounter", 0x04, EventCounterVersatile1_Reset)

EHS_FB_FUNCTION_ENTRY("Run_StopEventCounter", 0x05, EventCounterVersatile1_Disable)

EHS_FB_FUNCTIONS_END

#define EHS_FB_EVENTCOUNTER_MAX_THRESHOLD 0
#define EHS_FB_EVENTCOUNTER_MIN_THRESHOLD 1
#define EHS_FB_EVENTCOUNTER_INI_VALUE 2
#define EHS_FB_EVENTCOUNTER_STEP_VALUE 3
/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(EventCounterVersatile1)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(EventCounterVersatile1));
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsEventCounterStruct);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(EventCounterVersatile1)
{
    int nCount;
    EhsDataflowIntType nMaxThreshold, nMinThreshold;
    const ehs_char* pParams = EHS_FB_INIT_PARAMETERS;
    ehs_bool bRet = EHS_TRUE; /* assume success */
    EhsEventCounterStruct* pEventCounterObj = (EhsEventCounterStruct*)EHS_FB_INIT_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(EventCounterVersatile1));

    pEventCounterObj->bInitialise = EHS_TRUE;
    pEventCounterObj->bEventCounterRunning = EHS_FALSE;
    nCount = 0;

    if (pParams)
    {
        pParams = EhsGetUint8FromString(&(pEventCounterObj->bEnableAtInit), pParams);
    }

    if (pParams)
    {
        pParams = EhsGetSint32FromString(&(pEventCounterObj->nInitialValue), pParams);
    }

    if (pParams)
    {
        pParams = EhsGetSint32FromString(&(pEventCounterObj->nStepSize), pParams);
    }

    if (pParams)
    {
        pParams = EhsGetSint32FromString(&(pEventCounterObj->nMaxThreshold), pParams);
    }

    if (pParams)
    {
        pParams = EhsGetSint32FromString(&(pEventCounterObj->nMinThreshold), pParams);
    }
    else
    {
        /* we haven't got an initial parameter */
        bRet = EHS_FALSE;
    }
    /*
    */
    if (bRet)
    {
        // initialise counter according to params
        if (pEventCounterObj->bEnableAtInit == EHS_TRUE)
        {
            pEventCounterObj->bEventCounterRunning = EHS_TRUE;
        }
        nCount = pEventCounterObj->nInitialValue;

        nMaxThreshold = pEventCounterObj->nMaxThreshold;
        if( !nMaxThreshold )            // if parameter is zero set max pos integer value as threshold.
        {
            nMaxThreshold = EHS_DATAFLOW_INT_MAX;
        }

        nMinThreshold = pEventCounterObj->nMinThreshold;

        // ensure initial count is within bounds
        if (nCount > nMaxThreshold)
        {
            nCount = nMaxThreshold;
        }
        if (nCount < nMinThreshold)
        {
            nCount = nMinThreshold;
        }
        pEventCounterObj->nCount = nCount;
        pEventCounterObj->nMaxThreshold = nMaxThreshold;
        pEventCounterObj->nMinThreshold = nMinThreshold;
    }


    return bRet; /* initialisation always succeeds */
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventCounterVersatile1_Enable)
{
    EhsEventCounterStruct* pEventCounterObj = (EhsEventCounterStruct*)EHS_FB_RUN_CONTEXT;

    // always fire enabled completed
    pEventCounterObj->bEventCounterRunning = EHS_TRUE;
    SetCompletes1((structFuncArg *)EHS_FB_RUN_CONTEXT_REF);           // set start event.
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventCounterVersatile1_Disable)
{
    EhsEventCounterStruct* pEventCounterObj = (EhsEventCounterStruct*)EHS_FB_RUN_CONTEXT;

    // always fire disabled completed
    pEventCounterObj->bEventCounterRunning = EHS_FALSE;
    SetCompletes1((structFuncArg *)EHS_FB_RUN_CONTEXT_REF);           // set stop event.
    return;
}


/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 */
EHS_FB_RUN_FUNCTION(UpdateThresholds)
{
    /* Note that this code relies on all run functions that call this, to have the same port enumeration for EHS_FB_EVENTCOUNTER_MAX_THRESHOLD and EHS_FB_EVENTCOUNTER_MIN_THRESHOLD */

    EhsDataflowIntType nMaxThreshold, nMinThreshold, nStepSize, nCount;
    EhsEventCounterStruct* pEventCounterObj = (EhsEventCounterStruct*)EHS_FB_RUN_CONTEXT;

    if (EHS_FB_IN_CONNECTED(EHS_FB_EVENTCOUNTER_MAX_THRESHOLD))
    {
        nMaxThreshold = EHS_FB_IN_I(EHS_FB_EVENTCOUNTER_MAX_THRESHOLD);  // take threshold from integer input.
    }
    else
    {
        nMaxThreshold = pEventCounterObj->nMaxThreshold;
        if( !nMaxThreshold )            // NOTE - if parameter is zero set max pos integer value as threshold.
        {
            nMaxThreshold = EHS_DATAFLOW_INT_MAX;
        }
    }

    if (EHS_FB_IN_CONNECTED(EHS_FB_EVENTCOUNTER_MIN_THRESHOLD))
    {
        nMinThreshold = EHS_FB_IN_I(EHS_FB_EVENTCOUNTER_MIN_THRESHOLD);  // take threshold from integer input.
    }
    else
    {
        nMinThreshold = pEventCounterObj->nMinThreshold;
    }

    if (EHS_FB_IN_CONNECTED(EHS_FB_EVENTCOUNTER_STEP_VALUE))
    {
        nStepSize = EHS_FB_IN_I(EHS_FB_EVENTCOUNTER_STEP_VALUE);  // take step size from integer input.
    }
    else
    {
        nStepSize = pEventCounterObj->nStepSize;
    }
    // SENSIBILITY CHECK - if step size is less than 1, set to 1
    if(nStepSize < 1)
    {
        nStepSize = 1;
    }

    // if initialise then initialise counter value
    if (pEventCounterObj->bInitialise == EHS_TRUE)
    {
        if (EHS_FB_IN_CONNECTED(EHS_FB_EVENTCOUNTER_INI_VALUE))
        {
            nCount = EHS_FB_IN_I(EHS_FB_EVENTCOUNTER_INI_VALUE);
        }
        else
        {
            nCount = pEventCounterObj->nInitialValue;
        }

        // ensure initial count is within bounds
        if (nCount > nMaxThreshold)
        {
            nCount = nMaxThreshold;
        }
        if (nCount < nMinThreshold)
        {
            nCount = nMinThreshold;
        }

        pEventCounterObj->bInitialise = EHS_FALSE;
        pEventCounterObj->nCount = nCount;
    }


    pEventCounterObj->nMaxThreshold = nMaxThreshold;
    pEventCounterObj->nMinThreshold = nMinThreshold;
    pEventCounterObj->nStepSize = nStepSize;
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventCounterVersatile1_Reset)
{
    int nCount;
    EhsDataflowIntType nMaxThreshold, nMinThreshold;
    EhsEventCounterStruct* pEventCounterObj = (EhsEventCounterStruct*)EHS_FB_RUN_CONTEXT;
// - always reset, even if counter not running
//	if( pEventCounterObj->bEventCounterRunning )
//	{

    EHS_FB_START_RUN_FUNCTION(UpdateThresholds);

    nMaxThreshold = pEventCounterObj->nMaxThreshold;
    nMinThreshold = pEventCounterObj->nMinThreshold;
    nCount = pEventCounterObj->nInitialValue;
    if (EHS_FB_IN_CONNECTED(EHS_FB_EVENTCOUNTER_INI_VALUE))
    {
        nCount = EHS_FB_IN_I(EHS_FB_EVENTCOUNTER_INI_VALUE);
    }


    // ensure count is within bounds
    if (nCount > nMaxThreshold)
    {
        nCount = nMaxThreshold;
    }
    if (nCount < nMinThreshold)
    {
        nCount = nMinThreshold;
    }

    pEventCounterObj->nCount = nCount;
    NCAPSA_nOut(0) = nCount;
    SetCompletes1((structFuncArg *)EHS_FB_RUN_CONTEXT_REF);           // set reset event.
    //SetCompletes1(pByte);           // set increment event.
//	}
    return;
}

/**
 * Run the function <more detail required>.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(EventCounterVersatile1_Count)
{
    int nCount;
    EhsDataflowIntType nMaxThreshold, nMinThreshold;
    EhsEventCounterStruct* pEventCounterObj = (EhsEventCounterStruct*)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(EventCounterVersatile1_Count));
    if( pEventCounterObj->bEventCounterRunning )
    {

        EHS_FB_START_RUN_FUNCTION(UpdateThresholds);
        nMaxThreshold = pEventCounterObj->nMaxThreshold;
        nMinThreshold = pEventCounterObj->nMinThreshold;
        nCount = pEventCounterObj->nCount;

        // check to ensure counter is within bounds as thresholds can change dynamically
        if (nCount < nMinThreshold)
        {
            nCount = nMinThreshold;
        }

        nCount += pEventCounterObj->nStepSize;	// increment count.
        if( nCount > nMaxThreshold )   // PD ditto
        {
            // note - we don't decr count if overflowed
            NCAPSA_nOut(0) = pEventCounterObj->nCount;
            SetCompletes2((structFuncArg *)EHS_FB_RUN_CONTEXT_REF);   // set overflow event.
        }
        else
        {
            pEventCounterObj->nCount = nCount;
            NCAPSA_nOut(0) = pEventCounterObj->nCount;
            SetCompletes1((structFuncArg *)EHS_FB_RUN_CONTEXT_REF);           // set increment event.
        }
    }
    return;
}

EHS_FB_RUN_FUNCTION(EventCounterVersatile1_Decrement)
{
    signed int nCount;
    EhsDataflowIntType nMinThreshold,nMaxThreshold;
    EhsEventCounterStruct* pEventCounterObj = (EhsEventCounterStruct*)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(EventCounterVersatile1_Count));
    if( pEventCounterObj->bEventCounterRunning )
    {

        EHS_FB_START_RUN_FUNCTION(UpdateThresholds);
        nMaxThreshold = pEventCounterObj->nMaxThreshold;
        nMinThreshold = pEventCounterObj->nMinThreshold;
        nCount = pEventCounterObj->nCount;

        // check to ensure counter is within bounds as thresholds can change dynamically
        if (nCount > nMaxThreshold)
        {
            nCount = nMaxThreshold;
        }

        nCount -= pEventCounterObj->nStepSize;	// decrement count.
        if( nCount < nMinThreshold )   // PD ditto
        {
            // note - we don't decr count if overflowed
            NCAPSA_nOut(0) = pEventCounterObj->nCount;
            SetCompletes2((structFuncArg *)EHS_FB_RUN_CONTEXT_REF);   // set overflow event.
        }
        else
        {
            pEventCounterObj->nCount = nCount;
            NCAPSA_nOut(0) = pEventCounterObj->nCount;
            SetCompletes1((structFuncArg *)EHS_FB_RUN_CONTEXT_REF);           // set increment event.
        }
    }
    return;
}
