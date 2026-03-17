/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * target_accelerometer.c — stubbed accelerometer HAL.
 * All operations are no-ops.  Read always returns 0.0 mg on all axes.
 * Selected when EHS_ACCELEROMETER_SUPPORT=stubbed.
 */

#include "globals.h"
#include "hal_accelerometer.h"

EHS_GLOBAL int EhsTAccelEnable(ehs_accelerometer_state_type *state)
{
    if (state)
        state->enabled = EHS_TRUE;
    return 0;
}

EHS_GLOBAL int EhsTAccelDisable(ehs_accelerometer_state_type *state)
{
    if (state)
        state->enabled = EHS_FALSE;
    return 0;
}

EHS_GLOBAL int EhsTAccelRead(ehs_accelerometer_state_type *state)
{
    if (state)
    {
        state->x = 0.0f;
        state->y = 0.0f;
        state->z = 0.0f;
    }
    return 0;
}
