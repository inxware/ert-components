/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * target_watchdog.h — Sfera Labs sysfs watchdog header.
 *
 * Provides the legacy inline no-op stubs for EhsTWatchdog* (backward
 * compatibility) and includes hal_watchdog2.h for EhsTWatchdog*2 declarations.
 */

#ifndef _TARGET_WATCHDOG_H_
#define _TARGET_WATCHDOG_H_

#include "ehs_fb_types.h"
#include "hal_watchdog2.h"
#include "sferalabs_hal.h"

/* Legacy inline stubs — kept for backward compatibility */
static inline void EhsTWatchdogEnable(void)                       {}
static inline void EhsTWatchdogDisable(void)                      {}
static inline void EhsTWatchdogKick(void)                         {}
static inline void EhsTWatchdogModify(ehs_uint32 period_us)       { (void)period_us; }

/* New EhsTWatchdog*2 functions are declared in hal_watchdog2.h (included above) */

#endif /* _TARGET_WATCHDOG_H_ */
