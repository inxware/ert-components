/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_watchdog2.h
 * Hardware abstraction layer for the hardware watchdog timer peripheral.
 *
 * The *2 suffix distinguishes these new functions from the legacy inline
 * EhsTWatchdog* stubs declared in target_watchdog.h for older system-level use.
 * 
 * TODO2026-we can almost certainly avoid the legacy APIs for WDTs/
 *
 * @author: inx limited
 */

#ifndef _HAL_WATCHDOG2_H
#define _HAL_WATCHDOG2_H

#include "globals.h"

typedef struct
{
    ehs_sint32 timeout_s;
    ehs_bool   enabled;
    ehs_bool   expired;
} ehs_watchdog_state_type;

/** Enable the watchdog with the timeout set in state->timeout_s.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTWatchdogEnable2(ehs_watchdog_state_type *state);

/** Disable the watchdog.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTWatchdogDisable2(ehs_watchdog_state_type *state);

/** Kick (feed/pet) the watchdog to prevent expiry.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTWatchdogKick2(ehs_watchdog_state_type *state);

/** Read the watchdog expiry status into state->expired.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTWatchdogReadStatus2(ehs_watchdog_state_type *state);

#endif /* _HAL_WATCHDOG2_H */
