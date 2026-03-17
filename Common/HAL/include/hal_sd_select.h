/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_sd_select.h
 * Hardware abstraction layer for dual SD-card slot selection.
 *
 * @author: inx limited
 */

#ifndef _HAL_SD_SELECT_H
#define _HAL_SD_SELECT_H

#include "globals.h"

typedef struct
{
    ehs_sint32 active_slot;
} ehs_sd_select_state_type;

/** Select the active SD slot.  slot = 0 for main, 1 for secondary.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTSdSelectSlot(ehs_sd_select_state_type *state, ehs_sint32 slot);

/** Read the currently active SD slot into state->active_slot.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTSdReadActiveSlot(ehs_sd_select_state_type *state);

#endif /* _HAL_SD_SELECT_H */
