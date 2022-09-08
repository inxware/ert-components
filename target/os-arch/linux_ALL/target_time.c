/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_time.c
 * Definitions for target-specific time-related functions. The timer code contained
 * in here implements a very simple timer model. The functions provided by the timer
 * are:
 * - EhsTgtTimer_set(). This sets the absolute time when the timer is due to expire.
 * - EhsTgtTimer_clear(). This indicates that the timer will never expire.
 * - EhsTgtTimer_tick(). Checks if the expiry time has happened. If so, it calls EhsTimer_tick(). This could be called:
 *   - By a periodic timer interrupt
 *   - By a smart timer interrupt (configured by EhsTgtTimer_set to interrupt at the specified time)
 *   - By polling
 * - EhsTgtTimer_init(). Sets up the timer.
 *
 * @author: inx limited
 *
 */


/**
 * @page Verification Verification report
 * @section target_timer
 * @anchor target_timer
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on 2007-10-12
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */


/**
 * Provides access to the target-specific declarations of header files
 */
#define EHS_TARGET_CODE

/*****************************************************************************/
/* Included files */
#include "target.h"
#include "timer.h"

#ifndef EHS_MINGW
#ifndef EHS_BSD
#include <sys/timex.h>
#endif
#endif

//#include <sys/time.h>

#include "messages.h"
#include <errno.h>
#include <time.h>

#ifdef EHS_USE_ORDINARY_NANOSLEEP

#else //#ifdef EHS_USE_ORDINARY_NANOSLEEP
int clock_nanosleep(clockid_t clockid, int flags,
                    const struct timespec *request,
                    struct timespec *remain);
#endif //#else #ifdef EHS_USE_ORDINARY_NANOSLEEP


/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/**
 * Included to make it easier to understand why we're multiplying by a million in some places.
 */


#define EHS_TGT_ERROR_TIME_FAILURE(x) "**Error: ntp_gettime failed. Errno = %d\n",x

/*****************************************************************************/
/* Declare prototypes of local functions */
/*****************************************************************************/
/* Variables defined with file-scope */

/**
 * This represents the soonest time that any timer will expire
 */
EHS_LOCAL EhsTickType EhsTgtTimerExpiryTime;

/*****************************************************************************/
/* Variables defined with global-scope */#include <time.h>
/*****************************************************************************/
/* Function definitions */


/**ntp
 * Return the current time in EHS ticks
 */
EhsTickType EhsTgtTimer_now()
{
    //struct ntptimeval xtime;
    struct timespec xtime;
    EhsTickType ret;

    if (clock_gettime(CLOCK_MONOTONIC, &xtime) == -1)
    {
        EhsError(EHS_TGT_ERROR_TIME_FAILURE(errno));
        ret = EHS_TICKTYPE_INVALID;
    }
    else
    {
        //ehs_uint32 s = (ehs_uint32)(xtime.time.tv_sec); // this will br truncated
        //ehs_uint32 us = (ehs_uint32)(xtime.time.tv_usec); // this shouldn't be
        ehs_uint32 s = (ehs_uint32)(xtime.tv_sec); // this will br truncated
        ehs_uint32 us = (ehs_uint32)(xtime.tv_nsec)/1000; // this shouldn't be
        /*@TODO: Replace tick timing system with something more robust. Utter shite here - masks in one place integer sizes elsewhere..*/
        ret = ( (((ehs_uint32) EHS_TIME_s(s) & EHS_TICKTYPE_MAX) + (((ehs_uint32) EHS_TIME_us(us))) &  EHS_TICKTYPE_MAX) ); // why truncate?
        //ret = ( ((EHS_TIME_s(s)) + (( EHS_TIME_us(us)))) );
    }
    return ret;
}

/**
 * Sleep the current thread for a specified duration
 *
 * @param tSleepTime Time to sleep for in target-specific ticks
 */
void EhsSleep(EhsTickType tSleepTime)
{
    struct timespec tSleep;
    ehs_uint32 t; /* intermediate variable to simplify the conversion process - required by MISRA */
    /* convert sleep time to seconds */
    t = EhsTgtTimer_tickTous(tSleepTime) / EHS_uS_PER_S;
    tSleep.tv_sec = (ehs_sint32)(t);
    /* convert sleep time to microseconds */
    t = EhsTgtTimer_tickTous(tSleepTime) % EHS_uS_PER_S;
    tSleep.tv_nsec = (ehs_sint32)(t)*1000;
#ifdef EHS_USE_ORDINARY_NANOSLEEP
    nanosleep(&tSleep,NULL);
#else
    clock_nanosleep(CLOCK_MONOTONIC,0,&tSleep,NULL); /*  This may not use hrtimer?*/
#endif
    /* alternative implementation using select:
     * select(0,NULL,NULL,NULL,&tSleep); //lint !e534 Return value not of interest here
     */
}

void EhsSleepUs(ehs_uint32 tSleepTime)
{
    struct timespec tSleep;
    ehs_uint32 t; /* intermediate variable to simplify the conversion process - required by MISRA */
    /* convert sleep time to seconds */
    t = tSleepTime / EHS_uS_PER_S;
    tSleep.tv_sec = (ehs_sint32)(t);
    /* convert sleep time to microseconds */
    t = tSleepTime % EHS_uS_PER_S;
    tSleep.tv_nsec = (ehs_sint32)(t)*1000;
#ifdef EHS_USE_ORDINARY_NANOSLEEP
    nanosleep(&tSleep,NULL);
#else
    clock_nanosleep(CLOCK_MONOTONIC,0,&tSleep,NULL); /*  This may not use hrtimer?*/
#endif
    /* alternative implementation using select:
     * select(0,NULL,NULL,NULL,&tSleep); //lint !e534 Return value not of interest here
     */
}

/**
 * Initialise the target-specific timer (to be not running)
 */
void EhsTgtTimer_reset()
{
    EhsTgtTimer_clear();
}

/*
 * Set the target-specific timer to expire at a specific time.
 * Upon expiry (or periodically if the target's timing facility
 * is unsophisticated), call EhsTimer_tick()
 */
void EhsTgtTimer_set(EhsTickType tExpiryTime)
{
    EhsTgtTimerExpiryTime = tExpiryTime;
}

/**
 * Stop the target-specific timer from calling EhsTimer_tick(), cancel
 * any pending events
 */
void EhsTgtTimer_clear()
{
    EhsTgtTimerExpiryTime = EHS_TICKTYPE_INVALID;
}

/**
 * Report expiry time for current timer
 *
 * @return the time when the timer will call EhsTimer_tick, or EHS_TICKTYPE_INVALID
 * if no timer is running
 */
EhsTickType EhsTgtTimer_expiry()
{
    return EhsTgtTimerExpiryTime;
}

/**
 * This is only needed if we've gone for the application polling method.
 * @returns it *must* return true.
 */
ehs_bool EhsTgtTimer_tick()
{
    if (EhsTgtTimerExpiryTime != EHS_TICKTYPE_INVALID)
    {
        if (EHS_TARGET_TIME_IS_EARLIER(EhsTgtTimerExpiryTime,EHS_CURRENT_TIME))
            //if (EhsTimer_earlier(EhsTgtTimerExpiryTime,EHS_CURRENT_TIME))
        {
            EhsTimer_tick();
        }
    }
    return EHS_TRUE;
}
