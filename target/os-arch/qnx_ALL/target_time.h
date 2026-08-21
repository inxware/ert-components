/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_time.h
 * Target-specific time declarations for QNX Neutrino AArch64.
 * This file should only be included by hal_time.h.
 *
 * QNX supports CLOCK_MONOTONIC and clock_nanosleep() via POSIX, so the
 * tick resolution and conversion macros are identical to linux_ALL.
 *
 * @author: inx limited
 */

#ifndef EHS_TARGET_TIME_H
#define EHS_TARGET_TIME_H

#ifndef EHS_HAL_TIME_H
#error "This file should only be included by hal_time.h"
#endif

#include <time.h>

#include "globals.h"

/*****************************************************************************/
/* Define macros  */

/* @TODO if you change the following the time wrap around will break! */
#define EHS_TICKS_PER_S (1000000u) /**< System clock resolution in Hz */
#define EHS_uS_PER_TICK (1u)

/**
 * Maximum value for EhsTickType — top bit reserved for EHS_TICKTYPE_INVALID.
 */
#define EHS_TICKTYPE_MAX     0x7fffffffu
#define EHS_TICKTYPE_INVALID 0xffffffffu /**< Rogue value for time functions */

/**
 * Convert microseconds / milliseconds to target ticks.
 */
#define EhsTgtTimer_usToTick(us) ((EhsTickType)((EhsTickType)(us) / (EhsTickType)EHS_uS_PER_TICK))
#define EhsTgtTimer_msToTick(ms) ((EhsTickType)((EhsTickType)(ms) * 1000 / (EhsTickType)EHS_mS_PER_TICK))

/**
 * Convert target ticks to microseconds.
 */
#define EhsTgtTimer_tickTous(tick) ((ehs_uint32)((tick) * EHS_uS_PER_TICK))

/*****************************************************************************/
/* Define types */

typedef ehs_uint32 EhsTickType;

/*****************************************************************************/
/* Declare global variables */
/*****************************************************************************/
/* Declare function prototypes  */

#endif /* EHS_TARGET_TIME_H */
