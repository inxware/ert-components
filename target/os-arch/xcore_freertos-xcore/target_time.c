/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_time.c
 * Timer implementation for XMOS xcore.ai FreeRTOS targets.
 *
 * EhsTickType resolution is 1 μs (EHS_TICKS_PER_S = 1 000 000).
 * EhsTgtTimer_now() uses xTaskGetTickCount() scaled to microseconds.
 * Accuracy is bounded by configTICK_RATE_HZ (typically 1 ms at 1000 Hz).
 *
 * Timer expiry is driven by a FreeRTOS one-shot software timer that calls
 * EhsTimer_tick() when the next EHS timer event is due.
 *
 * @author: inx limited
 */

#include "globals.h"
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/timers.h"
#include "timer.h"
#include "hal_logger.h"

#define TAG "target_time"

/* Microseconds per FreeRTOS tick (e.g. 1000 us at 1000 Hz) */
#define EHS_XCORE_US_PER_FREERTOS_TICK  (1000000u / configTICK_RATE_HZ)

static EhsTickType s_expiry_time = EHS_TICKTYPE_INVALID;
static TimerHandle_t s_ehs_timer = NULL;

/**
 * Return current time in EHS microsecond ticks.
 * Resolution is limited to 1 FreeRTOS tick.
 */
EhsTickType EhsTgtTimer_now(void)
{
    return (EhsTickType)xTaskGetTickCount() * EHS_XCORE_US_PER_FREERTOS_TICK;
}

static void prv_ehs_timer_cb(TimerHandle_t xTimer)
{
    (void)xTimer;
    if (s_expiry_time != EHS_TICKTYPE_INVALID)
    {
        EhsTimer_tick();
    }
}

/**
 * Initialise the target timer facility (called once at startup).
 */
void EhsTgtTimer_reset(void)
{
    s_expiry_time = EHS_TICKTYPE_INVALID;
    if (s_ehs_timer == NULL)
    {
        /* Period of 1 tick; will be changed via EhsTgtTimer_set() each time */
        s_ehs_timer = xTimerCreate("EhsTgt",
                                   pdMS_TO_TICKS(1),
                                   pdFALSE,       /* one-shot */
                                   NULL,
                                   prv_ehs_timer_cb);
    }
}

/**
 * Arm the timer to fire at tExpiryTime (in EHS microsecond ticks).
 */
void EhsTgtTimer_set(EhsTickType tExpiryTime)
{
    s_expiry_time = tExpiryTime;
    if (s_ehs_timer != NULL)
    {
        EhsTickType now      = EhsTgtTimer_now();
        EhsTickType delta_us = (tExpiryTime > now) ? (tExpiryTime - now) : 1u;

        /* Convert microseconds to FreeRTOS ticks (round up, minimum 1) */
        TickType_t ticks = (TickType_t)((delta_us + EHS_XCORE_US_PER_FREERTOS_TICK - 1u)
                                        / EHS_XCORE_US_PER_FREERTOS_TICK);
        if (ticks < 1u) ticks = 1u;

        xTimerChangePeriod(s_ehs_timer, ticks, 0);
        xTimerStart(s_ehs_timer, 0);
    }
}

/**
 * Cancel any pending timer event.
 */
void EhsTgtTimer_clear(void)
{
    s_expiry_time = EHS_TICKTYPE_INVALID;
    if (s_ehs_timer != NULL)
    {
        xTimerStop(s_ehs_timer, 0);
    }
}

/**
 * Return the scheduled expiry time, or EHS_TICKTYPE_INVALID if none.
 */
EhsTickType EhsTgtTimer_expiry(void)
{
    return s_expiry_time;
}

/**
 * Polling tick — returns EHS_TRUE.  The real tick is driven by the
 * FreeRTOS software timer callback above.
 */
ehs_bool EhsTgtTimer_tick(void)
{
    return EHS_TRUE;
}

/**
 * Sleep the calling task for tSleepTime EHS ticks.
 */
void EhsSleep(EhsTickType tSleepTime)
{
    TickType_t ticks = (TickType_t)(tSleepTime / EHS_XCORE_US_PER_FREERTOS_TICK);
    if (ticks < 1u) ticks = 1u;
    vTaskDelay(ticks);
}

void EhsSleepUs(ehs_uint32 tSleepTime)
{
    EhsSleep(EhsTgtTimer_usToTick(tSleepTime));
}
