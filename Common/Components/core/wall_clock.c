/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * wall_clock.c
 *
 * function declarations for wall_clock
 *
 * @author: inx limited
 *
 */


#include "ehs_fb_types.h"
#include "wall_clock.h"
#include "app_data.h"

#include "hal-api.h" /* Required for logging */


/******************************************************************************/
/* Define state function block */

EHS_FB_FUNCTIONS_START(wall_clock)

EHS_FB_FUNCTION_ENTRY("get", 0x00, wall_clock_get)

EHS_FB_FUNCTION_ENTRY("set", 0x01, wall_clock_set)
EHS_FB_FUNCTIONS_END

/**
 * Context for a timer block. Comprises the timer, the callback queue and the callback queue entry
 */

/*typedef struct {
@todo
} EhsTimeClockType;
*/

#define EHS_FB_WALLCLOCK_GET_DATA 0
#define EHS_FB_WALLCLOCK_GET_MINS 1
#define EHS_FB_WALLCLOCK_GET_SECS 2
#define EHS_FB_WALLCLOCK_GET_DONE 1
#define EHS_FB_WALLCLOCK_SET_DATA 0
#define EHS_FB_WALLCLOCK_SET_DONE 1

/**
 * Identify the function block. Determine the memory required.
 *
 * This function provides access to:
 *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text
 *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this
 *   function block's context
 *
 */
EHS_FB_IDENTIFY_FUNCTION(wall_clock)
{
    EHS_TRACE_FUNCTION(EHS_FB_IDENTIFY_NAME(wall_clock));
    EHS_FB_IDENTIFY_MEMORY = 0;//sizeof(EhsTimeClockType);
}

/**
 * Initialise the function block. Populate the context area for the function block.
 *
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */
EHS_FB_INIT_FUNCTION(wall_clock)
{
    //EhsTimeClockType* pTimeClock = EHS_FB_INIT_CONTEXT;
    //const char* pParams;
    return EHS_TRUE; /* initialisation always succeeds */
}

/**
 * Run the function start. This starts (or restarts) a timer.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(wall_clock_get)
{
    ehs_sint64 time_value;
    EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(wall_clock_));
    time_value = EhsHGetdateTime(EHS_FB_OUT_S(EHS_FB_WALLCLOCK_GET_DATA),EHS_TRUE);
    EHS_FB_OUT_I(EHS_FB_WALLCLOCK_GET_MINS) = time_value/60; //minutes since 1970
    EHS_FB_OUT_I(EHS_FB_WALLCLOCK_GET_SECS) = time_value%60; //and the residual seconds
    //EhsStrcpy(EHS_FB_OUT_S(EHS_FB_WALLCLOCK_GET_DATA), sZtemp);
    EHS_FB_FINISH(EHS_FB_WALLCLOCK_GET_DONE);
    return;
}

/**
 * Run the function 'stop'. This halts a currently running timer.
 *
 * This function provides access to:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(wall_clock_set)
{
    //EhsTimeClockType* pTimeClock = (EhsTimeClockType*)EHS_FB_RUN_CONTEXT;
    return;
}

