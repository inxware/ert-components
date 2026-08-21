/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_time.c
 * Target-specific timer implementation for QNX Neutrino AArch64.
 *
 * QNX Neutrino provides POSIX CLOCK_MONOTONIC and clock_nanosleep(), so
 * the implementation is identical to linux_ALL/target_time.c with the
 * following differences:
 *
 *   - sys/timex.h is Linux-specific and is not included.
 *   - clock_nanosleep() does not need an external forward-declaration on
 *     QNX (it is in <time.h> directly).
 *
 * @author: inx limited
 */

#define EHS_TARGET_CODE

/* sys/timex.h is Linux-only — omit on QNX */
#include <errno.h>
#include <time.h>

#include "globals.h"
#include "ehs_timer.h"
#include "messages.h"
#include "hal_logger.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

#define EHS_TGT_ERROR_TIME_FAILURE(x) "**Error: clock_gettime failed. Errno = %d\n", x

/*****************************************************************************/
/* Variables defined with file-scope */

/** The soonest time that any timer will expire */
EHS_LOCAL EhsTickType EhsTgtTimerExpiryTime;

/*****************************************************************************/
/* Function definitions */

/**
 * Return the current time in EHS ticks (microseconds, CLOCK_MONOTONIC).
 */
EhsTickType EhsTgtTimer_now(void)
{
    struct timespec xtime;
    EhsTickType ret;

    if (clock_gettime(CLOCK_MONOTONIC, &xtime) == -1)
    {
        EHSH_LOG_ERROR(EHS_TGT_ERROR_TIME_FAILURE(errno));
        ret = EHS_TICKTYPE_INVALID;
    }
    else
    {
        ehs_uint32 s  = (ehs_uint32)(xtime.tv_sec);
        ehs_uint32 us = (ehs_uint32)(xtime.tv_nsec) / 1000;
        ret = (((ehs_uint32)EHS_TIME_s(s) & EHS_TICKTYPE_MAX)
               + ((ehs_uint32)EHS_TIME_us(us))) & EHS_TICKTYPE_MAX;
    }
    return ret;
}

/**
 * Sleep the current thread for a specified duration.
 *
 * @param tSleepTime  Time to sleep in target ticks (microseconds).
 */
void EhsSleep(EhsTickType tSleepTime)
{
    struct timespec tSleep;
    ehs_uint32 t;
    t = EhsTgtTimer_tickTous(tSleepTime) / EHS_uS_PER_S;
    tSleep.tv_sec  = (ehs_sint32)(t);
    t = EhsTgtTimer_tickTous(tSleepTime) % EHS_uS_PER_S;
    tSleep.tv_nsec = (ehs_sint32)(t) * 1000;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &tSleep, NULL);
}

void EhsSleepUs(ehs_uint32 tSleepTime)
{
    struct timespec tSleep;
    ehs_uint32 t;
    t = tSleepTime / EHS_uS_PER_S;
    tSleep.tv_sec  = (ehs_sint32)(t);
    t = tSleepTime % EHS_uS_PER_S;
    tSleep.tv_nsec = (ehs_sint32)(t) * 1000;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &tSleep, NULL);
}

/**
 * Initialise the target-specific timer (to be not running).
 */
void EhsTgtTimer_reset(void)
{
    EhsTgtTimer_clear();
}

/**
 * Set the target-specific timer to expire at a specific time.
 */
void EhsTgtTimer_set(EhsTickType tExpiryTime)
{
    EhsTgtTimerExpiryTime = tExpiryTime;
}

/**
 * Cancel any pending timer event.
 */
void EhsTgtTimer_clear(void)
{
    EhsTgtTimerExpiryTime = EHS_TICKTYPE_INVALID;
}

/**
 * Report expiry time for the current timer.
 */
EhsTickType EhsTgtTimer_expiry(void)
{
    return EhsTgtTimerExpiryTime;
}

/**
 * Polling tick check — call EhsTimer_tick() if the timer has expired.
 * @returns EHS_TRUE always.
 */
ehs_bool EhsTgtTimer_tick(void)
{
    if (EhsTgtTimerExpiryTime != EHS_TICKTYPE_INVALID)
    {
        if (EHS_TARGET_TIME_IS_EARLIER(EhsTgtTimerExpiryTime, EHS_CURRENT_TIME))
        {
            EhsTimer_tick();
        }
    }
    return EHS_TRUE;
}
