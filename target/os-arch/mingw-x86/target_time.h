/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_time.h
 * The target-specific declarations required to support the HAL for time services
 * are defined here. This file should only be included by hal_time.h
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_TIME_H
#define EHS_TARGET_TIME_H

#ifndef EHS_HAL_TIME_H
#error "This file should only be included by hal_time.h"
#endif

#include <time.h> //Needed for the tm struct used in time_hal.h

/*****************************************************************************/
/* Included files */

#include "target_types.h"

/*****************************************************************************/
/* Define macros  */

/* EHS_TARGET_FIXED_TIMER_RESOLUTION is provided in case we need to switch from
 * the windows fine timer, which depends on target hardware to a timer that is the
 * same on all windows targets. It isn't defined anywhere as we don't use the fixed
 * timer resolution. */

#ifdef EHS_TARGET_FIXED_TIMER_RESOLUTION
#define EHS_TICKS_PER_S 1000 /**< Defines the resolution of the system clock in Hz */
#define EHS_uS_PER_TICK 1000
#else
#define EHS_TICKS_PER_S (EhsTicksPerS) /**< Defines the resolution of the system clock in Hz */
#endif

/**
 * Maximum value of EhsTickType is half maximum integer value. This is required to guarantee
 * correct calculation of ordering (e.g. time X before time Y).
 * MUST BE ONE LESS THAN A POWER OF TWO! Otherwise EhsCurrentTime() won't work correctly.
 */
#define EHS_TICKTYPE_MAX 0x7fffffffffffffff
#define EHS_TICKTYPE_MAX_HALF 0x8000000
#define EHS_TICKTYPE_INVALID -1 /**< Used as a rogue value for time functions */

/**
 * convert microseconds to target-specific ticks
 *
 * @param us Time in microseconds
 * @return equivalent time in target-specific ticks
 */
#ifdef EHS_TARGET_FIXED_TIMER_RESOLUTION
__declspec(dllimport) ehs_uint32 __stdcall GetTickCount(void);
#define EhsusToTick(us) ((EhsTickType)((EhsTickType)(us) / EHS_uS_PER_TICK))
#endif

/**
 * Converts target-specific times to microseconds
 *
 * @param ticks time in target-specific ticks
 * @return equivalent time in microseconds
 */
#ifdef EHS_TARGET_FIXED_TIMER_RESOLUTION
#define EhsTickTous(tick) ((ehs_uint32)((tick) * EHS_uS_PER_TICK))
#endif

/*****************************************************************************/
/* Define types */

/**
 * Target specific representation of time.
 * There are two major operations that we need to perform when handling time:
 * @li determining which time is earlier.
 * @li calculating the difference between two times.
 * Normally, this is simple: laterTime - earlierTime gives the difference. The
 * difference is greater than zero if the earlierTime was before laterTime.
 * However, problems occur when we get clock roll-over: in this instance, laterTime has a
 * smaller value than earlierTime.
 * @p
 * To deal with roll-over, we assume that no time difference will be greater than
 * half of the largest value that can be represented by EhsTickType (half of EHS_TICKTYPE_MAX).
 * We further assume that unsigned integer arithmetic handles overflows silently: for
 * example (1 - 0xf) % 0x10 should produce 2.
 * @p
 * Given these assumptions, we can implement our operations as follows:
 * @li Difference is simply (later - earlier) % EHS_TICKTYPE_MAX.
 * @li Determining which time is earlier becomes a matter of checking whether the difference
 * between the two values is less than or equal to half of EHS_TICKTYPE_MAX.
 */
typedef long long EhsTickType; /**< Target specific representation of time */

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

#ifdef EHS_TGT_CODE
/**
 * Initialise the target timer
 */
EHS_GLOBAL ehs_uint32 EhsTgtTimer_init(void);
#endif

#endif /* EHS_TARGET_TIME_H */


/******************************************************************************/
/* Target-specific time handling functions */




