/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * target_ups.c — stubbed UPS HAL.
 *
 * All operations are no-ops.  ReadStatus always reports mains power with 0 mV.
 * Callbacks are never triggered (no polling thread).
 * Selected when EHS_UPS_SUPPORT=stubbed.
 */

#include "globals.h"
#include "hal_ups.h"

EHS_GLOBAL int EhsTUpsReadStatus(ehs_ups_state_type *state)
{
    if (state)
    {
        state->on_battery = EHS_FALSE;
        state->battery_mv = 0;
    }
    return 0;
}

EHS_GLOBAL int EhsTUpsRegisterCallbacks(ehs_ups_state_type *state)
{
    /* Stubbed: callbacks are never invoked */
    (void)state;
    return 0;
}
