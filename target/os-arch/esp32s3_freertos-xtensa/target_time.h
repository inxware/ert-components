/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_time.h
 * The target-specific declarations required to support the HAL for time
 * services are defined here. This file should only be included by hal_time.h
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_TIME_H
#define EHS_TARGET_TIME_H

#ifndef EHS_HAL_TIME_H
#error "This file should only be included by hal_time.h"
#endif

#include <time.h>      //Needed for the tm struct used in time_hal.h
#include <sdkconfig.h> //esp32 SDK config

#include "globals.h"



/*****************************************************************************/
/* Included files */
/*****************************************************************************/
/* Define macros  */

/* @TODO if you change the following the time wrap around will break and timers
 * will become dead after a while! - Look into if changing this can reduce
 * processing*/
#define EHS_TICKS_PER_S                                                        \
  (1000000u) /**< Defines the resolution of the system clock in Hz */
#define EHS_uS_PER_TICK (1000000u/EHS_TICKS_PER_S) // (1000u)

#define EHS_FREERTOS_TICKS_PER_S CONFIG_FREERTOS_HZ
#define EHS_TICKS_PER_EHS_FREERTOS_TICK (EHS_TICKS_PER_S / EHS_FREERTOS_TICKS_PER_S)

/**
 * Defines the maximum value for EhsTickType. In this target, we reserve the top
 * bit in order to allow EHS_TICKTYPE_INVALID to have a distinct value
 */
#define EHS_TICKTYPE_MAX 0x7ffffffffffffffful
#define EHS_TICKTYPE_INVALID                                                   \
  0xfffffffffffffffful /**< Used as a rogue value for time functions */

/**
 * convert microseconds to target-specific ticks
 *
 * @param us Time in microseconds
 * @return equivalent time in target-specific ticks
 */

#define EhsTgtTimer_usToTick(us)                                               \
  us >= EHS_uS_PER_TICK ? ((EhsTickType)((EhsTickType)(us) / (EhsTickType)EHS_uS_PER_TICK)) : 1
#define EhsTgtTimer_msToTick(ms)                                               \
  ((EhsTickType)((EhsTickType)(ms)*1000 / (EhsTickType)EHS_uS_PER_TICK))

/**
 * Converts target-specific times to microseconds
 *
 * @param ticks time in target-specific ticks
 * @return equivalent time in microseconds
 */
#ifndef EHS_MINGW
#define EhsTgtTimer_tickTous(tick) ((ehs_uint64)((tick)*EHS_uS_PER_TICK))
#else
#define EhsTgtTimer_tickTous(tick) (tick)
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
 * However, problems occur when we get clock roll-over: in this instance,
 * laterTime has a smaller value than earlierTime.
 * @p
 * To deal with roll-over, we assume that no time difference will be greater
 * than half of the largest value that can be represented by EhsTickType (half
 * of EHS_TICKTYPE_MAX). We further assume that unsigned integer arithmetic
 * handles overflows silently: for example (1 - 0xf) % 0x10 should produce 2.
 * @p
 * Given these assumptions, we can implement our opeEHS_TIME_srations as
 * follows:
 * @li Difference is simply (later - earlier) % EHS_TICKTYPE_MAX.
 * @li Determining which time is earlier becomes a matter of checking whether
 * the difference between the two values is less than or equal to half of
 * EHS_TICKTYPE_MAX.
 */
typedef ehs_uint64 EhsTickType;

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

/* Override the generic TRUSTED_CLIENT macro so hot-path callers bypass the
 * readiness gate in EhsTgtTimer_now() and read the timer register directly. */
#define EHS_CURRENT_TIME_TRUSTED_CLIENT (EhsTgtTimer_now_trusted())

#endif /* EHS_TARGET_TIME_H */
