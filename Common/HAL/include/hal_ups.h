/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_ups.h
 * Hardware abstraction layer for the UPS / power-fail monitoring peripheral.
 *
 * @author: inx limited
 */

#ifndef _HAL_UPS_H
#define _HAL_UPS_H

#include "globals.h"

typedef struct
{
    ehs_bool    on_battery;
    ehs_sint32  battery_mv;
    void      (*on_fail_cb)(void *);
    void      (*on_restore_cb)(void *);
    void       *cb_ctx;
} ehs_ups_state_type;

/** Read the current UPS status into state->on_battery and state->battery_mv.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTUpsReadStatus(ehs_ups_state_type *state);

/** Register power-fail and power-restore callbacks stored in state.
 *  The implementation is responsible for triggering the callbacks when
 *  the power source changes.  A polling thread or interrupt handler may
 *  be used; the stubbed implementation is a no-op.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTUpsRegisterCallbacks(ehs_ups_state_type *state);

#endif /* _HAL_UPS_H */
