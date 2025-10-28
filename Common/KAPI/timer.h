/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file timer.h
 * Declarations for the timer functions used by EHH-Kernel to manage a list of timer objects (used by the kernel and function blocks)
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TIMER_H
#define EHS_TIMER_H

#include "globals.h"
#include "callback_queue.h"
#include "hal_time.h"

/**
 * Handles the configuration of a timer. Operates in a linked list ordered by "tNext".
 *
 */
typedef struct EhsTimerStruct
{
    EhsTickType tPeriod;	/**< Timer period. 0 indicates "single-shot" (non-repeating) timer */
    EhsTickType tNext;	/**< Time when a timer event will next occur for this timer */
    struct EhsTimerStruct* pNext;	/**< Next item in the linked list. If a single shot timer and this
									 * points to itself, it means the timer has expired */
    EhsCallbackQueueType* pCallback; /**< Pointer to the callback queue to execute upon timer expiry */
} EhsTimerType;

/**
 * Check whether a timer has expired. This applies only to single shot timers,
 * so periodic timers will never return true.
 * Note we also give the opportunity to manually check the clock to see if
 * the target timer has expired. This allows for porting EHS to "dumb" targets,
 * or ones that cannot (for some reason) provide a sensible interrupt/event/signal.
 *
 * @param pTimer Timer to check
 * @return True if the timer has expired
 */
#define EHS_TIMER_EXPIRED(x) (EHS_TARGET_TIMER_TICK && ((x)->tPeriod == 0) && ((x) == (x)->pNext))

/**
 * Determine whether the timer is still running
 */
EHS_GLOBAL ehs_bool EhsTimer_running(EhsTimerType* pTimer);

/**
 * Determine when the next timer event is due
 */
EHS_GLOBAL EhsTickType EhsTimer_nextEvent(void);

/**
 * Add a timer to the set of currently running timers
 */
EHS_GLOBAL void EhsTimer_set(EhsTimerType* pTimer, EhsTickType tExpiryTime);

/**
 * Remove a timer from the set of currently running timers
 */
EHS_GLOBAL void EhsTimer_clear(EhsTimerType* pTimer);

/**
 * Called by the timer hardware - either on a specific timer expiry, or on every tick
 */
EHS_GLOBAL void EhsTimer_tick(void);

/**
 * Initialise the timer subsystem
 */
EHS_GLOBAL void EhsTimer_init(void);
EHS_GLOBAL ehs_bool EhsTimer_earlier(EhsTickType target, EhsTickType time);
#endif /* EHS_TIMER_H */

