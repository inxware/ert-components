/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_bootstate.h
 *
 * Persistent boot-state flag API.
 *
 * The kernel uses this to detect failed app loads that the reset-reason hook
 * cannot capture (silent hangs, power loss during init, hard reset). Just
 * before parsing an app the kernel calls EhsHBootStateSet(); after the app
 * has run for >5 s the kernel calls EhsHBootStateClear(). On the next boot,
 * if the flag is still set, the previous boot didn't survive the confirm
 * window and the kernel can fall back to default and (at threshold) deny.
 *
 * The default implementation persists each state to a small file in the
 * system dir containing a decimal counter (incremented on each Set, deleted
 * on Clear). Targets that need NVRAM/RTC-RAM backing can replace
 * hal_bootstate.c with their own implementation of the same prototypes.
 *
 * The enum-keyed shape is deliberate so future named persistent boot flags
 * can be added without inventing new HAL pairs.
 */

#ifndef EHS_HAL_BOOTSTATE_H
#define EHS_HAL_BOOTSTATE_H

#include "globals.h"

typedef enum {
    /* Set just before SODL parse on initial boot, cleared after >5 s of
     * confirmed runtime. If still set on the next boot, the previous boot
     * never reached the confirm window. */
    EHS_BOOT_STATE_APP_LOAD_STARTED = 0,

    /* Sentinel — also the size of the table in hal_bootstate.c. Keep last. */
    EHS_BOOT_STATE_COUNT
} EhsBootStateType;

/**
 * Record the named state. If already set, increment the persisted counter
 * so the boot-time path can detect repeated unconfirmed loads.
 *
 * @return EHS_TRUE on successful persist, EHS_FALSE if the backing store
 *         could not be written (in which case the kernel continues without
 *         crash protection — failure is non-fatal).
 */
ehs_bool EhsHBootStateSet(EhsBootStateType state);

/**
 * Clear the named state and reset its counter to 0.
 *
 * @return EHS_TRUE if the state is now cleared (whether or not it was set
 *         before), EHS_FALSE on backing-store failure.
 */
ehs_bool EhsHBootStateClear(EhsBootStateType state);

/**
 * @return EHS_TRUE iff the persisted counter for this state is > 0.
 */
ehs_bool EhsHBootStateIsSet(EhsBootStateType state);

/**
 * @return The persisted counter for this state, or 0 if cleared/unset.
 */
ehs_uint32 EhsHBootStateGetCount(EhsBootStateType state);

/**
 * Policy gate, mirroring EhsHShouldDeleteAppForCrashReason().
 *
 * @return EHS_TRUE iff EhsHBootStateGetCount(EHS_BOOT_STATE_APP_LOAD_STARTED)
 *         has reached the configured tolerance. The threshold is built into
 *         the HAL via EHS_APP_FAILED_BOOT_LIMIT (set in the target's config.mk,
 *         default 1) so the same kernel archive can serve targets with
 *         different policies.
 */
ehs_bool EhsHFailedBootShouldDenyApp(void);

#endif /* EHS_HAL_BOOTSTATE_H */
