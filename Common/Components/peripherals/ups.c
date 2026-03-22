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
 *   read_status      (0x01) — poll current power source and battery voltage
 *   power_fail_cb    (0x02) — internal callback fired by HAL on power loss
 *   power_restore_cb (0x03) — internal callback fired by HAL on power restore
 *
 * The two callback functions are InternalPorts at (-1,-1) in the CDF.
 * They are registered with EhsCallbackQueue_register() at init so the HAL
 * can trigger them asynchronously by calling EhsCallbackQueue_execute().
 * This follows the same pattern as the UART recv_cb InternalPort.
 */

#include "inx-component.h"
#include "ups.h"
#include "hal_ups.h"

EHS_FB_FUNCTIONS_START(ups)
EHS_FB_FUNCTION_ENTRY("read_status",      0x01, ups_read_status)
EHS_FB_FUNCTION_ENTRY("power_fail_cb",    0x02, ups_power_fail_cb)
EHS_FB_FUNCTION_ENTRY("power_restore_cb", 0x03, ups_power_restore_cb)
EHS_FB_FUNCTIONS_END

/* Port argument numbers (from CDF <Function argument="N">) */
#define INX_ups_ARG_done         1   /* FinishPort: done (fn1, arg=1)            */
#define INX_ups_ARG_on_battery   1   /* OutputPort: on_battery (fn1, arg=1)      */
#define INX_ups_ARG_battery_mv   2   /* OutputPort: battery_mv (fn1, arg=2)      */
#define INX_ups_ARG_power_fail   1   /* FinishPort: power_fail (fn2, arg=1)      */
#define INX_ups_ARG_power_restored 1 /* FinishPort: power_restored (fn3, arg=1)  */

/* Module-level callback queues — one per InternalPort callback function */
static EhsCallbackQueueType xUpsPowerFailQueue;
static EhsCallbackQueueType xUpsPowerRestoreQueue;


EHS_FB_IDENTIFY_FUNCTION(ups)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_ups_state_type);
}

EHS_FB_INIT_FUNCTION(ups)
{
    ehs_ups_state_type *state = (ehs_ups_state_type *)EHS_FB_INIT_CONTEXT;
    state->on_battery      = EHS_FALSE;
    state->battery_mv      = 0;
    state->pPowerFailQueue    = &xUpsPowerFailQueue;
    state->pPowerRestoreQueue = &xUpsPowerRestoreQueue;

    /* Register callback functions with the eRT callback-queue mechanism.
     * EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1) = first InternalPort (power_fail_cb).
     * EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-2) = second InternalPort (power_restore_cb). */
    EhsCallbackQueue_register(&xUpsPowerFailQueue,
                               EHS_FB_RUN_NAME(ups_power_fail_cb),
                               EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1),
                               &(state->xPowerFailEntry));
    EhsCallbackQueue_register(&xUpsPowerRestoreQueue,
                               EHS_FB_RUN_NAME(ups_power_restore_cb),
                               EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-2),
                               &(state->xPowerRestoreEntry));

    EhsTUpsRegisterCallbacks(state);
    return EHS_TRUE;
}

/* Poll UPS status on demand */
EHS_FB_RUN_FUNCTION(ups_read_status)
{
    ehs_ups_state_type *state = (ehs_ups_state_type *)EHS_FB_RUN_CONTEXT;
    EhsTUpsReadStatus(state);
    EHS_FB_OUT_B_API2(INX_ups_ARG_on_battery) = state->on_battery;
    EHS_FB_OUT_I_API2(INX_ups_ARG_battery_mv) = state->battery_mv;
    EHS_FB_FINISH_API2(INX_ups_ARG_done);
}

/* Fired by the HAL via EhsCallbackQueue_execute(&xUpsPowerFailQueue) */
EHS_FB_RUN_FUNCTION(ups_power_fail_cb)
{
    EHS_FB_FINISH_API2(INX_ups_ARG_power_fail);
}

/* Fired by the HAL via EhsCallbackQueue_execute(&xUpsPowerRestoreQueue) */
EHS_FB_RUN_FUNCTION(ups_power_restore_cb)
{
    EHS_FB_FINISH_API2(INX_ups_ARG_power_restored);
}

EHS_FB_DESTROY_FUNCTION(ups)
{
    return EHS_TRUE;
}
