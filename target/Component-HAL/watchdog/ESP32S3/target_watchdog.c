/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * target_watchdog.c — ESP32S3 watchdog HAL.
 *
 * Implements the EhsTWatchdog*2 API by delegating to the register-access
 * inline functions defined in target_watchdog.h (SWD + TIMG0 + TIMG1).
 * Selected when EHS_WATCHDOG_SUPPORT=ESP32S3.
 */

#include "globals.h"
#include "target_watchdog.h"

EHS_GLOBAL int EhsTWatchdogEnable2(ehs_watchdog_state_type *state)
{
    if (state && state->timeout_s > 0)
        EhsTWatchdogModify((ehs_uint32)state->timeout_s * 1000000u);
    EhsTWatchdogEnable();
    return 0;
}

EHS_GLOBAL int EhsTWatchdogDisable2(ehs_watchdog_state_type *state)
{
    (void)state;
    EhsTWatchdogDisable();
    return 0;
}

EHS_GLOBAL int EhsTWatchdogKick2(ehs_watchdog_state_type *state)
{
    (void)state;
    EhsTWatchdogKick();
    return 0;
}

EHS_GLOBAL int EhsTWatchdogReadStatus2(ehs_watchdog_state_type *state)
{
    /* No hardware expired-flag readable via the current register API.
     * The ESP32S3 watchdog resets the chip on expiry, so if we reach here
     * it has not expired. */
    if (state)
        state->expired = EHS_FALSE;
    return 0;
}
