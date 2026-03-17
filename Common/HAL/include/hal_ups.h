/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_ups.h
 * Hardware abstraction layer for UPS / power-fail monitoring.
 *
 * The state struct embeds two EhsCallbackQueueEntryType entries so that the
 * function block can register async callbacks (power-fail, power-restore) with
 * the eRT callback-queue mechanism, exactly as the UART block does for receive.
 *
 * ups.c stores pointers to the module-level queues in pPowerFailQueue and
 * pPowerRestoreQueue at init.  The HAL implementation calls
 * EhsCallbackQueue_execute() on the appropriate queue when it detects a
 * power-state transition.
 */

#ifndef _HAL_UPS_H
#define _HAL_UPS_H

#include "globals.h"
#include "callback_queue.h"

typedef struct
{
    ehs_bool    on_battery;
    ehs_sint32  battery_mv;
    /* Callback queue entries — populated by ups.c at init */
    EhsCallbackQueueEntryType xPowerFailEntry;
    EhsCallbackQueueEntryType xPowerRestoreEntry;
    /* Queue pointers set by ups.c so the HAL can fire them */
    EhsCallbackQueueType     *pPowerFailQueue;
    EhsCallbackQueueType     *pPowerRestoreQueue;
} ehs_ups_state_type;

/** Read the current UPS status into state->on_battery and state->battery_mv.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTUpsReadStatus(ehs_ups_state_type *state);

/** Register the callback queues with the HAL so it can fire them on
 *  power-state transitions.  The stubbed implementation is a no-op.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTUpsRegisterCallbacks(ehs_ups_state_type *state);

#endif /* _HAL_UPS_H */
