/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file ups.c
 * @brief Function block implementation for UPS / power-fail monitoring.
 *
 * Exposes three functions:
 *   read_status     (0x01) — poll current power source and battery voltage
 *   power_fail_cb   (0x02) — internal trigger fired by the HAL on power loss
 *   power_restore_cb(0x03) — internal trigger fired by the HAL on power restore
 *
 * The callbacks are registered at init via EhsTUpsRegisterCallbacks().
 * In the stubbed HAL the callbacks are never triggered (no polling thread).
 * A real implementation should start a background poller or use an interrupt.
 */

#include "globals.h"
#include "ups.h"
#include "hal_ups.h"

EHS_FB_FUNCTIONS_START(ups)
EHS_FB_FUNCTION_ENTRY("read_status",      0x01, ups_read_status)
EHS_FB_FUNCTION_ENTRY("power_fail_cb",    0x02, ups_power_fail_cb)
EHS_FB_FUNCTION_ENTRY("power_restore_cb", 0x03, ups_power_restore_cb)
EHS_FB_FUNCTIONS_END

/* Port argument numbers */
#define INX_ups_ARG_on_battery   1   /* Output: on_battery bool  */
#define INX_ups_ARG_battery_mv   2   /* Output: battery_mv int   */

/* Forward declarations for callback helpers */
static void ups_on_fail_callback(void *ctx);
static void ups_on_restore_callback(void *ctx);


EHS_FB_IDENTIFY_FUNCTION(ups)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_ups_state_type);
}

EHS_FB_INIT_FUNCTION(ups)
{
    ehs_ups_state_type *state = (ehs_ups_state_type *)EHS_FB_INIT_CONTEXT;
    state->on_battery    = EHS_FALSE;
    state->battery_mv    = 0;
    state->on_fail_cb    = ups_on_fail_callback;
    state->on_restore_cb = ups_on_restore_callback;
    state->cb_ctx        = EHS_FB_INIT_CONTEXT; /* pass state itself as context */
    EhsTUpsRegisterCallbacks(state);
    return EHS_TRUE;
}

/* Poll UPS status */
EHS_FB_RUN_FUNCTION(ups_read_status)
{
    ehs_ups_state_type *state = (ehs_ups_state_type *)EHS_FB_RUN_CONTEXT;
    EhsTUpsReadStatus(state);
    EHS_FB_OUT_B_API2(INX_ups_ARG_on_battery, state->on_battery);
    EHS_FB_OUT_I_API2(INX_ups_ARG_battery_mv, state->battery_mv);
    EHS_FB_FINISH_PORT("done");
}

/* Fired by HAL callback when mains power is lost */
EHS_FB_RUN_FUNCTION(ups_power_fail_cb)
{
    EHS_FB_FINISH_PORT("power_fail");
}

/* Fired by HAL callback when mains power is restored */
EHS_FB_RUN_FUNCTION(ups_power_restore_cb)
{
    EHS_FB_FINISH_PORT("power_restored");
}

EHS_FB_DESTROY_FUNCTION(ups)
{
    return EHS_TRUE;
}

/* HAL callback: called from polling thread / interrupt when power fails */
static void ups_on_fail_callback(void *ctx)
{
    /* ctx is ehs_ups_state_type* — trigger the internal port to dispatch power_fail_cb */
    EHS_FB_TRIGGER_INTERNAL_PORT(ctx, "_power_fail_trigger");
}

/* HAL callback: called when power is restored */
static void ups_on_restore_callback(void *ctx)
{
    EHS_FB_TRIGGER_INTERNAL_PORT(ctx, "_power_restore_trigger");
}
