/** @file timer.h
 * Declarations for the timer functions
 *
 * @author: inx limited
 * @version: $Revision: 1617 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
 *
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_TIMER_H
#define EHS_TIMER_H

#include "target.h"
#include "callback_queue.h"

/**
 * Handles the configuration of a timer. Operates in a linked list ordered by "tNext".
 *
 */
typedef struct EhsTimerStruct {
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

