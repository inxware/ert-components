/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/
#include <Arduino.h>
#include <mbed.h>

#include "target_types.h"
#include "timer.h"
#include "hal_logger.h"

/**
 * This represents the soonest time that any timer will expire
 */
EHS_LOCAL EhsTickType EhsTgtTimerExpiryTime;

/* This hould return the counter value in "tick time" */
EhsTickType EhsTgtTimer_now() 
{
    return (EhsTickType)( rtos::Kernel::Clock::now().time_since_epoch().count() * 1000 );
}

/* Initialise the hardware timer */
void EhsTgtTimer_reset()
{
    EhsTgtTimer_clear();
}

/*
 * Set the target-specific timer to expire at a specific time.
 * Upon expiry (or periodically if the target's timing facility
 * is unsophisticated), call EhsTimer_tick()
 * This function does not handle the logic of accounting for all the timers in
 * the queue, it simply sets a ime out to call the next tick, where the expiered
 * timer objects will be found triggered
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
 * @return the time when the timer will call EhsTimer_tick, or
 * EHS_TICKTYPE_INVALID if no timer is running
 */
EhsTickType EhsTgtTimer_expiry()
{
    return EhsTgtTimerExpiryTime;
}

/**
 * Sleep the current thread for a specified duration
 *
 * @param tSleepTime Time to sleep for in target-specific ticks
 */

//todo2023 we need to review the efficiency of doing all these divides!
void EhsSleep(EhsTickType tSleepTime)
{
    // this yields the CPU, allowing other threads to run.
    rtos::ThisThread::sleep_for(EhsTgtTimer_tickTous(tSleepTime)/1000u);
}

void EhsSleepUs(ehs_uint32 tSleepTime)
{
    wait_us(EhsTgtTimer_tickTous(tSleepTime)); // this doesn't yield threads
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

