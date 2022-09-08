/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * time_clock.c
 *
 * function declarations for time_clock
 *
 * @author: inx limited
 *
 */


#include "ehs_fb_types.h"
#include "time_clock.h"
#include "fid.h"
#include "timer.h"
#include "app_data.h"

#include "hal-api.h" /* Required for logging */

/**
 * @page FunctionReference Lucid Function Block Reference Guide
 * @section Clock
 * @anchor time_clock
 * @subsection appliesTo Applies To:
 * Event &gt; Clock
 * @subsection properties Properties:
 * <b>Time Interval.</b> This represents the time (in microseconds) between
 * Start being fired and Clock being asserted. If the periodic flag is set,
 * this represents the period between consecutive asserts of Clock.
 * Although it is possible to specify delays with microsecond resolution,
 * the resolution of timers on your target platform is likely to be less
 * than this. Consult the Lucid Target Supplement for more details.
 *
 * <b>Periodic Flag.</b> If set, the timer will run in periodic mode: Clock
 * events will be asserted with a period of Time Interval from the Start
 * event until a Stop event is received. Retriggerable flag is ignored if
 * this flag is set.
 *
 * <b>Retriggerable Flag.</b> If this flag is set, firing Start after the timer has
 * started will reset the timer. If not set, firing Start after the timer has
 * started is ignored.
 *
 * @subsection description Description:
 * The clock function causes <em>Clk</em> events to be asserted with a particular
 * period. The clock is started or stopped by firing <em>Start</em> and <em>Stop</em> respectively.
 * If the <em>interval</em>, <em>periodic</em> or <em>retriggerable</em> inputs are connected up, these values will override
 * the fixed parameters.
 *
 *
 * @par tgt
 * Note that the maximum resolution of time units depends upon the
 * timer units available in your target hardware. See your target�s <em>Lucid
 * Target Supplement</em> for more details.
 * @p
 * The largest period that can be supported by a clock depends upon your hardware, but is
 * unlikely to allow more than a minute to be timed.
 *
 */


/******************************************************************************/
/* Define state function block */

EHS_FB_FUNCTIONS_START(time_clock)

EHS_FB_FUNCTION_ENTRY("start", 0x00, time_clock_start)

EHS_FB_FUNCTION_ENTRY("stop", 0x01, time_clock_stop)

EHS_FB_FUNCTION_ENTRY("tick", 0x02, time_clock_tick)
EHS_FB_FUNCTIONS_END

/**
 * Context for a timer block. Comprises the timer, the callback queue and the callback queue entry
 */
typedef struct
{
    EhsTimerType xTimer;
    EhsCallbackQueueType xQueue;
    EhsCallbackQueueEntryType xEntry;
    EhsTickType tPeriod;
    ehs_bool bRetriggerable;
    ehs_bool bPeriodic;
    ehs_bool bStarted;
} EhsTimeClockType;

#define EHS_FB_TIMECLOCK_PERIOD 0 /**< Input number for connection 'period' */
#define EHS_FB_TIMECLOCK_REPEAT 1 /**< Input number for connection 'repeat' */
#define EHS_FB_TIMECLOCK_RETRIG 2 /**< Input number for connection 'retrig' */
#define EHS_FB_TIMECLOCK_CLK 3 /**< Event number for connection 'clk' */
#define EHS_FB_TIMECLOCK_TICK -1 /**< Event number for callback event */
/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(time_clock)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(time_clock));
    EHS_FB_IDENTIFY_MEMORY = sizeof(EhsTimeClockType);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(time_clock)
{
    EhsTimeClockType* pTimeClock = EHS_FB_INIT_CONTEXT;
    const char* pParams;
    ehs_uint32 nPeriodIn_uS;
    ehs_uint8 nByte;
    EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(time_clock));

    /* set up the callback queue to call the callback function */
    pTimeClock->xQueue = NULL;
    EhsCallbackQueue_register(&(pTimeClock->xQueue),
                              EHS_FB_RUN_NAME(time_clock_tick),
                              EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(EHS_FB_TIMECLOCK_TICK),
                              &(pTimeClock->xEntry));

    /* tell the timer what to callback upon expiry */
    pTimeClock->xTimer.pCallback = &(pTimeClock->xQueue);

    /* parse parameters to get period, periodic flag and retriggerable flag */
    pParams = EhsGetUint32FromString(&nPeriodIn_uS,EHS_FB_INIT_PARAMETERS);
    pTimeClock->tPeriod = EhsTgtTimer_usToTick(nPeriodIn_uS);
    pParams = EhsGetUint8FromString(&nByte, pParams);
    pTimeClock->bPeriodic = (ehs_bool)nByte;
    pParams = EhsGetUint8FromString(&nByte, pParams);
    pTimeClock->bRetriggerable = (ehs_bool)nByte;
    pTimeClock->bStarted=EHS_FALSE;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function start. This starts (or restarts) a timer.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(time_clock_start)
{
    EhsTimeClockType* pTimeClock = (EhsTimeClockType*)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(time_clock_start));

    if (EHS_FB_IN_CONNECTED(EHS_FB_TIMECLOCK_RETRIG)) // this function isn't working = need to xero data tables?
    {
        pTimeClock->bRetriggerable = EHS_FB_IN_B(EHS_FB_TIMECLOCK_RETRIG);
    }

    /* we can't restart a non-retriggerable timer */
    if ((pTimeClock->bRetriggerable) ||(!EhsTimer_running(&(pTimeClock->xTimer)))) //we can restart a running timer to allow for new parameters to e read in
    {
        /* only change parameters if we can restart the timer */
        if (EHS_FB_IN_CONNECTED(EHS_FB_TIMECLOCK_PERIOD))
        {
            pTimeClock->tPeriod = EhsTgtTimer_usToTick(EHS_FB_IN_I(EHS_FB_TIMECLOCK_PERIOD));
        }
        if (EHS_FB_IN_CONNECTED(EHS_FB_TIMECLOCK_REPEAT))
        {
            pTimeClock->bPeriodic = EHS_FB_IN_B(EHS_FB_TIMECLOCK_REPEAT);
        }

        /* set the period (if the timer is periodic) */
        pTimeClock->xTimer.tPeriod = (pTimeClock->bPeriodic)?pTimeClock->tPeriod:0;
        pTimeClock->bStarted=EHS_TRUE;
        /* start the timer */
        EhsTimer_set(&(pTimeClock->xTimer), EHS_CURRENT_TIME_OFFSET(pTimeClock->tPeriod));

    }

    EhsFunctionInstanceData_triggerEvent(EHS_FB_RUN_CONTEXT_REF,1);
    return;

}

/**
 * Run the function 'stop'. This halts a currently running timer.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(time_clock_stop)
{
    EhsTimeClockType* pTimeClock = (EhsTimeClockType*)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(time_clock_stop));

    /* EhsTimer_clear works whether the timer is running or not */
    EhsTimer_clear(&pTimeClock->xTimer); // this doesn't seem to stop pending events so we gate with bStarted
    EhsFunctionInstanceData_triggerEvent(EHS_FB_RUN_CONTEXT_REF,1);
    pTimeClock->bStarted=EHS_FALSE;
    return;
}
/**
 * Run the function 'tick'. This represents the timer callback.
 * @todo Sort this function for the case where we have multiple groups
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(time_clock_tick)
{
    EhsTimeClockType* pTimeClock = (EhsTimeClockType*)EHS_FB_RUN_CONTEXT;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(time_clock_tick));
    if (pTimeClock->bStarted == EHS_TRUE)
        EhsFunctionInstanceData_triggerEvent(EHS_FB_RUN_CONTEXT_REF,1);
}
