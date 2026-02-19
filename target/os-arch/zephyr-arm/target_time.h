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

#include <time.h>
#include "globals.h"

/*****************************************************************************/
/* Included files */
/*****************************************************************************/
/* Define macros  */

/* Zephyr kernel tick is configurable (typically 1kHz = 1ms).
 * We use microsecond resolution for EHS ticks, matching the ESP32 pattern.
 * Zephyr's k_uptime_get() returns milliseconds, k_cycle_get_32() gives
 * hardware cycle counts. We use k_uptime_ticks() / k_ticks_to_us_floor64()
 * for microsecond resolution where the hardware supports it.
 */
#define EHS_TICKS_PER_S                                                        \
  (1000000u) /**< Defines the resolution of the system clock in Hz */
#define EHS_uS_PER_TICK (1000000u/EHS_TICKS_PER_S)

/* Zephyr kernel tick rate - CONFIG_SYS_CLOCK_TICKS_PER_SEC is set in Kconfig.
 * Default to 1000 Hz if not defined (typical Zephyr default). */
#ifndef CONFIG_SYS_CLOCK_TICKS_PER_SEC
#define CONFIG_SYS_CLOCK_TICKS_PER_SEC 1000
#endif
#define EHS_ZEPHYR_TICKS_PER_S CONFIG_SYS_CLOCK_TICKS_PER_SEC
#define EHS_TICKS_PER_ZEPHYR_TICK (EHS_TICKS_PER_S / EHS_ZEPHYR_TICKS_PER_S)

/**
 * Defines the maximum value for EhsTickType. We reserve the top
 * bit to allow EHS_TICKTYPE_INVALID to have a distinct value.
 */
#define EHS_TICKTYPE_MAX 0x7ffffffffffffffful
#define EHS_TICKTYPE_INVALID                                                   \
  0xfffffffffffffffful /**< Used as a rogue value for time functions */

/**
 * Convert microseconds to target-specific ticks
 */
#define EhsTgtTimer_usToTick(us)                                               \
  us >= EHS_uS_PER_TICK ? ((EhsTickType)((EhsTickType)(us) / (EhsTickType)EHS_uS_PER_TICK)) : 1
#define EhsTgtTimer_msToTick(ms)                                               \
  ((EhsTickType)((EhsTickType)(ms)*1000 / (EhsTickType)EHS_uS_PER_TICK))

/**
 * Converts target-specific times to microseconds
 */
#define EhsTgtTimer_tickTous(tick) ((ehs_uint64)((tick)*EHS_uS_PER_TICK))

/*****************************************************************************/
/* Define types */

/**
 * Target specific representation of time.
 * Uses 64-bit unsigned for maximum range before rollover.
 * See ESP32 target_time.h for detailed rollover handling documentation.
 */
typedef ehs_uint64 EhsTickType;

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

#endif /* EHS_TARGET_TIME_H */
