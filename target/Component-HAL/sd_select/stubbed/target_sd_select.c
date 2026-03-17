/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * target_sd_select.c — stubbed SD select HAL.
 * All operations are no-ops.  Selected when EHS_SD_SELECT_SUPPORT=stubbed.
 */

#include "globals.h"
#include "hal_sd_select.h"

EHS_GLOBAL int EhsTSdSelectSlot(ehs_sd_select_state_type *state, ehs_sint32 slot)
{
    if (state)
        state->active_slot = slot;
    return 0;
}

EHS_GLOBAL int EhsTSdReadActiveSlot(ehs_sd_select_state_type *state)
{
    if (state)
        state->active_slot = 0;
    return 0;
}
