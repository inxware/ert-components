/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file watchdog.c
 * @brief Function block implementation for the hardware watchdog timer.
 *
 * Exposes four functions:
 *   enable      (0x01) — enable the watchdog with a configurable timeout
 *   disable     (0x02) — disable the watchdog
 *   kick        (0x03) — kick (feed/pet) the watchdog
 *   read_status (0x04) — read expiry status
 */

#include "inx-component.h"
#include "watchdog.h"
#include "hal_watchdog2.h"

EHS_FB_FUNCTIONS_START(watchdog)
EHS_FB_FUNCTION_ENTRY("enable",      0x01, watchdog_enable)
EHS_FB_FUNCTION_ENTRY("disable",     0x02, watchdog_disable)
EHS_FB_FUNCTION_ENTRY("kick",        0x03, watchdog_kick)
EHS_FB_FUNCTION_ENTRY("read_status", 0x04, watchdog_read_status)
EHS_FB_FUNCTIONS_END

/* Port argument numbers (from CDF <Function argument="N">) */
#define INX_watchdog_ARG_enabled    1   /* FinishPort: enabled (fn1, arg=1)  */
#define INX_watchdog_ARG_disabled   1   /* FinishPort: disabled (fn2, arg=1) */
#define INX_watchdog_ARG_kicked     1   /* FinishPort: kicked (fn3, arg=1)   */
#define INX_watchdog_ARG_error_id   1   /* OutputPort: error_id (fn1+2, arg=1) */
#define INX_watchdog_ARG_status_done 1  /* FinishPort: status_done (fn4, arg=1) */
#define INX_watchdog_ARG_expired    1   /* OutputPort: expired (fn4, arg=1)  */


EHS_FB_IDENTIFY_FUNCTION(watchdog)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_watchdog_state_type);
}

EHS_FB_INIT_FUNCTION(watchdog)
{
    ehs_watchdog_state_type *state = (ehs_watchdog_state_type *)EHS_FB_INIT_CONTEXT;
    state->timeout_s = 60;
    state->enabled   = EHS_FALSE;
    state->expired   = EHS_FALSE;
    EhsSscanf(EHS_FB_INIT_PARAMETERS, "%d", &state->timeout_s);
    return EHS_TRUE;
}

/* Enable the watchdog using the timeout parameter */
EHS_FB_RUN_FUNCTION(watchdog_enable)
{
    ehs_watchdog_state_type *state = (ehs_watchdog_state_type *)EHS_FB_RUN_CONTEXT;
    int rc = EhsTWatchdogEnable2(state);
    EHS_FB_OUT_I_API2(INX_watchdog_ARG_error_id) = rc;
    if (rc == 0)
        state->enabled = EHS_TRUE;
    EHS_FB_FINISH_API2(INX_watchdog_ARG_enabled);
}

/* Disable the watchdog */
EHS_FB_RUN_FUNCTION(watchdog_disable)
{
    ehs_watchdog_state_type *state = (ehs_watchdog_state_type *)EHS_FB_RUN_CONTEXT;
    int rc = EhsTWatchdogDisable2(state);
    EHS_FB_OUT_I_API2(INX_watchdog_ARG_error_id) = rc;
    if (rc == 0)
        state->enabled = EHS_FALSE;
    EHS_FB_FINISH_API2(INX_watchdog_ARG_disabled);
}

/* Kick the watchdog */
EHS_FB_RUN_FUNCTION(watchdog_kick)
{
    ehs_watchdog_state_type *state = (ehs_watchdog_state_type *)EHS_FB_RUN_CONTEXT;
    EhsTWatchdogKick2(state);
    EHS_FB_FINISH_API2(INX_watchdog_ARG_kicked);
}

/* Read watchdog expiry status */
EHS_FB_RUN_FUNCTION(watchdog_read_status)
{
    ehs_watchdog_state_type *state = (ehs_watchdog_state_type *)EHS_FB_RUN_CONTEXT;
    EhsTWatchdogReadStatus2(state);
    EHS_FB_OUT_B_API2(INX_watchdog_ARG_expired) = state->expired;
    EHS_FB_FINISH_API2(INX_watchdog_ARG_status_done);
}

EHS_FB_DESTROY_FUNCTION(watchdog)
{
    return EHS_TRUE;
}
