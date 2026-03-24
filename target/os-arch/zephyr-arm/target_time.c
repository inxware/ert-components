/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_time.c
 * Zephyr RTOS implementation of target time services.
 * Uses Zephyr kernel timing APIs (k_uptime_get, k_sleep, k_cycle_get_32).
 *
 * @author: inx limited
 */

#include "globals.h"
#include <zephyr/kernel.h>
#include <errno.h>
#include <stdio.h>
#include "ehs_timer.h"
#include "hal_logger.h"

#define TAG "target_time"

/**
 * This represents the soonest time that any timer will expire
 */
EHS_LOCAL EhsTickType EhsTgtTimerExpiryTime;

static struct k_timer ehs_kernel_timer;
static volatile bool ehs_timer_running = false;

/* Timer expiry callback - called from ISR context */
static void ehs_timer_expiry_fn(struct k_timer *timer)
{
    if (EhsTgtTimerExpiryTime != EHS_TICKTYPE_INVALID)
    {
        EhsTimer_tick();
    }
}

/**
 * Return the current time in EHS ticks (microseconds).
 * Uses Zephyr's k_uptime_ticks() converted to microseconds for best resolution.
 */
EhsTickType EhsTgtTimer_now(void)
{
    /* k_uptime_get() returns milliseconds as int64_t.
     * Multiply by 1000 to get microseconds (our tick unit). */
    int64_t ms = k_uptime_get();
    return (EhsTickType)(ms * 1000);
}

/**
 * Initialise the target-specific timer (to be not running)
 */
void EhsTgtTimer_reset(void)
{
    EhsTgtTimerExpiryTime = EHS_TICKTYPE_INVALID;
    k_timer_init(&ehs_kernel_timer, ehs_timer_expiry_fn, NULL);
    ehs_timer_running = false;
}

/**
 * Set the target-specific timer to expire at a specific time.
 * Upon expiry, calls EhsTimer_tick().
 */
void EhsTgtTimer_set(EhsTickType tExpiryTime)
{
    EhsTgtTimerExpiryTime = tExpiryTime;

    /* Calculate duration until expiry in milliseconds */
    EhsTickType now = EhsTgtTimer_now();
    int64_t delta_us;

    if (tExpiryTime > now) {
        delta_us = (int64_t)(tExpiryTime - now);
    } else {
        /* Already expired or very close - fire immediately (1ms minimum) */
        delta_us = 1000;
    }

    /* Convert microseconds to milliseconds (Zephyr k_timer works in ms/ticks) */
    int64_t delta_ms = delta_us / 1000;
    if (delta_ms < 1) {
        delta_ms = 1;
    }

    k_timer_start(&ehs_kernel_timer, K_MSEC(delta_ms), K_NO_WAIT);
    ehs_timer_running = true;
}

/**
 * Stop the target-specific timer from calling EhsTimer_tick()
 */
void EhsTgtTimer_clear(void)
{
    EhsTgtTimerExpiryTime = EHS_TICKTYPE_INVALID;
    if (ehs_timer_running) {
        k_timer_stop(&ehs_kernel_timer);
        ehs_timer_running = false;
    }
}

/**
 * Report expiry time for current timer
 */
EhsTickType EhsTgtTimer_expiry(void)
{
    return EhsTgtTimerExpiryTime;
}

/**
 * Sleep the current thread for a specified duration
 *
 * @param tSleepTime Time to sleep for in EHS ticks (microseconds)
 */
void EhsSleep(EhsTickType tSleepTime)
{
    /* Convert EHS ticks (microseconds) to Zephyr sleep duration.
     * k_sleep(K_MSEC(n)) sleeps for n milliseconds. Minimum 1ms. */
    int64_t ms = (int64_t)(tSleepTime / 1000);
    if (ms < 1) {
        ms = 1;
    }
    k_sleep(K_MSEC(ms));
}

void EhsSleepUs(ehs_uint32 tSleepTime)
{
    /* Zephyr supports k_usleep() for microsecond-resolution sleep */
    if (tSleepTime < 1000) {
        k_usleep(tSleepTime);
    } else {
        EhsSleep(EhsTgtTimer_usToTick(tSleepTime));
    }
}

/**
 * Polling-based timer tick (if needed).
 * @returns EHS_TRUE always
 */
ehs_bool EhsTgtTimer_tick(void)
{
    return EHS_TRUE;
}
