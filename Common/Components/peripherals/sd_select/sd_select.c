/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file sd_select.c
 * @brief Function block implementation for dual SD-card slot selection.
 *
 * Exposes two functions:
 *   select      (0x01) — select the active SD slot (0 = main, 1 = secondary)
 *   read_active (0x02) — read which slot is currently active
 */

#include "inx-component.h"
#include "sd_select.h"
#include "hal_sd_select.h"

EHS_FB_FUNCTIONS_START(sd_select)
EHS_FB_FUNCTION_ENTRY("select",      0x01, sd_select_select)
EHS_FB_FUNCTION_ENTRY("read_active", 0x02, sd_select_read_active)
EHS_FB_FUNCTIONS_END

/* Port argument numbers (from CDF <Function argument="N">) */
#define INX_sd_select_ARG_slot        1   /* InputPort:  slot (fn1, arg=1)                  */
#define INX_sd_select_ARG_done        1   /* FinishPort: done (fn1, arg=1)                  */
#define INX_sd_select_ARG_error_id    1   /* OutputPort: error_id (fn1+fn2 shared, arg=1)   */
#define INX_sd_select_ARG_read_done   1   /* FinishPort: read_done (fn2, arg=1)             */
#define INX_sd_select_ARG_active_slot 2   /* OutputPort: active_slot (fn2, arg=2)           */


EHS_FB_IDENTIFY_FUNCTION(sd_select)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_sd_select_state_type);
}

EHS_FB_INIT_FUNCTION(sd_select)
{
    ehs_sd_select_state_type *state = (ehs_sd_select_state_type *)EHS_FB_INIT_CONTEXT;
    state->active_slot = 0;
    return EHS_TRUE;
}

/* Select the active SD slot */
EHS_FB_RUN_FUNCTION(sd_select_select)
{
    ehs_sd_select_state_type *state = (ehs_sd_select_state_type *)EHS_FB_RUN_CONTEXT;
    ehs_sint32 slot = EHS_FB_IN_I_API2(INX_sd_select_ARG_slot);
    int rc = EhsTSdSelectSlot(state, slot);
    EHS_FB_OUT_I_API2(INX_sd_select_ARG_error_id) = rc;
    EHS_FB_FINISH_API2(INX_sd_select_ARG_done);
}

/* Read the currently active SD slot */
EHS_FB_RUN_FUNCTION(sd_select_read_active)
{
    ehs_sd_select_state_type *state = (ehs_sd_select_state_type *)EHS_FB_RUN_CONTEXT;
    int rc = EhsTSdReadActiveSlot(state);
    EHS_FB_OUT_I_API2(INX_sd_select_ARG_error_id)    = rc;
    EHS_FB_OUT_I_API2(INX_sd_select_ARG_active_slot) = state->active_slot;
    EHS_FB_FINISH_API2(INX_sd_select_ARG_read_done);
}

EHS_FB_DESTROY_FUNCTION(sd_select)
{
    return EHS_TRUE;
}
