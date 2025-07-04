/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_time.c
 * Definitions for target-specific time-related functions
 *
 *
 * @author: inx limited
 *
 */

#define EHS_TGT_CODE /* Ensure header files include target-internal values */

#include <windows.h>


#include "hal_time.h"
#include "timer.h"
#include "hal_logger.h"



#ifndef EHS_TARGET_FIXED_TIMER_RESOLUTION
/**
 * Defines the current clock frequency used for this device
 */
EHS_LOCAL LARGE_INTEGER EhsL_clockFrequency;
#endif

/**
 * Convert a long long integer into a double because
 * MinGW can't do this using built in support
 */
double EhsL_int64ToDouble(LARGE_INTEGER a)
{
    union
    {
        LARGE_INTEGER sint64;
        struct
        {
            ehs_uint32 nLo;
            ehs_sint32 nHi;
        } sint32;
    } conv;
    double ret;

    conv.sint64 = a;
    ret = (double)(conv.sint32.nLo) + (double)(conv.sint32.nHi) * 4294967296.0;

    return ret;
}
/**
 * Set up the target timer
 */
ehs_uint32 EhsTgtTimer_init(void)
{
    ehs_uint32 dwError = 0;
#ifndef EHS_TARGET_FIXED_TIMER_RESOLUTION
    LARGE_INTEGER clockFreq;
    if (!QueryPerformanceFrequency(&clockFreq))
    {
        dwError = GetLastError();
        /** @todo Provide error handling if performance clock isn't supported */
    }
    else
    {
        EhsL_clockFrequency = clockFreq;
    }

#endif
    return dwError;
}

/**
 * Return the current time in EHS ticks
 */
EhsTickType EhsTgtTimer_now(void)
{
#ifdef EHS_TARGET_FIXED_TIMER_RESOLUTION
    return (GetTickCount() & EHS_TICKTYPE_MAX);
#else
    EhsTickType ret;

    QueryPerformanceCounter(&ret);

    ret = (EhsTickType)(ret & EHS_TICKTYPE_MAX);

    return ret;
#endif
}

/**
 * Sleep the current thread for a specified duration
 *
 * @param tSleepTime Time to sleep for in target-specific ticks
 */
//todo 2023 - we should rename this function for all targets to be EhsSleepTicks(x) to diffeerntiate it from random c versions of sleep().
void EhsSleep(EhsTickType tSleepTime)
{
    //It seems Windows Sleep is in ms 
    Sleep(EhsTgtTimer_tickTous(tSleepTime)/1000);
}

void EhsSleepUs(ehs_uint32 tSleepTime)
{
    /*
    struct timespec tSleep;
    ehs_uint32 t; / * intermediate variable to simplify the conversion process - required by MISRA * /
    / * convert sleep time to seconds * /
    t = tSleepTime / EHS_uS_PER_S;
    tSleep.tv_sec = (ehs_sint32)(t);
    / * convert sleep time to microseconds * /
    t = tSleepTime % EHS_uS_PER_S;
    tSleep.tv_nsec = (ehs_sint32)(t)*1000;
    */
    //  #error We need to identify what the tick to vTaskDelay time value is and factor this in
    //todo 2023 - do we need to replace this?
    usleep(tSleepTime); /* @todo This can crash ?? */ /* todo this needs to factor in whatthe tick tie of windows is explicitly.
    /* alternative implementation using select:
     * select(0,NULL,NULL,NULL,&tSleep); /*lint !e534 Return value not of interest here * /
     */
}

#ifndef EHS_TARGET_FIXED_TIMER_RESOLUTION

/**
 * convert microseconds to target-specific ticks
 *
 * @param us Time in microseconds
 * @return equivalent time in target-specific ticks
 */
EhsTickType EhsTgtTimer_usToTick(ehs_uint32 us)
{
    double dClockFreq = EhsL_int64ToDouble(EhsL_clockFrequency);
    double dTicksInUs = (double)us;
    double dTicks = (dClockFreq * dTicksInUs) / 1e6;
    //double ret = ((double)(EhsL_clockFrequency)/1e6)*((double)us);
    EhsTickType ret = (EhsTickType)dTicks; //(EhsL_clockFrequency * (EhsTickType)us) / 1000000LL;

    return (EhsTickType)ret;
}

/**
 * convert target-specific ticks to microseconds
 *
 * @param tick Time in microseconds
 * @return equivalent time in target-specific ticks
 */
ehs_uint32 EhsTgtTimer_tickTous(EhsTickType tick)
{
    double ret = (1e6/EhsL_int64ToDouble(EhsL_clockFrequency))*((double)tick);

    return (ehs_uint32)ret;
}

#endif

EhsTickType EhsTgtTimerExpiryTime;

/**
 * Initialise the target-specific timer (to be not running)
 */
void EhsTgtTimer_reset(void)
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
    if ((EhsTgtTimerExpiryTime != EHS_TICKTYPE_INVALID) &&
            EHS_TARGET_TIME_IS_EARLIER(EhsTgtTimerExpiryTime,EHS_CURRENT_TIME))
    {
        EhsTimer_tick();
    }

    return EHS_TRUE;
}
