/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * target_watchdog.c — stubbed watchdog HAL.
 *
 * All EhsTWatchdog*2 operations are no-ops.  Selected when
 * EHS_WATCHDOG_SUPPORT=stubbed.
 */

#include "globals.h"
#include "hal_watchdog2.h"

EHS_GLOBAL int EhsTWatchdogEnable2(ehs_watchdog_state_type *state)
{
    (void)state;
    return 0;
}

EHS_GLOBAL int EhsTWatchdogDisable2(ehs_watchdog_state_type *state)
{
    (void)state;
    return 0;
}

EHS_GLOBAL int EhsTWatchdogKick2(ehs_watchdog_state_type *state)
{
    (void)state;
    return 0;
}

EHS_GLOBAL int EhsTWatchdogReadStatus2(ehs_watchdog_state_type *state)
{
    if (state)
        state->expired = EHS_FALSE;
    return 0;
}
