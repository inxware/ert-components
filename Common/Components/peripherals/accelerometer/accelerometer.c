/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file accelerometer.c
 * @brief Function block implementation for the accelerometer peripheral.
 *
 * Exposes three functions:
 *   enable  (0x01) — enable the accelerometer
 *   disable (0x02) — disable the accelerometer
 *   read    (0x03) — read x/y/z acceleration in mg
 */

#include "globals.h"
#include "accelerometer.h"
#include "hal_accelerometer.h"

EHS_FB_FUNCTIONS_START(accelerometer)
EHS_FB_FUNCTION_ENTRY("enable",  0x01, accelerometer_enable)
EHS_FB_FUNCTION_ENTRY("disable", 0x02, accelerometer_disable)
EHS_FB_FUNCTION_ENTRY("read",    0x03, accelerometer_read)
EHS_FB_FUNCTIONS_END

/* Port argument numbers */
#define INX_accelerometer_ARG_error_id 1   /* Output: error code (enable/disable) */
#define INX_accelerometer_ARG_x       1   /* Output: x acceleration in mg         */
#define INX_accelerometer_ARG_y       2   /* Output: y acceleration in mg         */
#define INX_accelerometer_ARG_z       3   /* Output: z acceleration in mg         */


EHS_FB_IDENTIFY_FUNCTION(accelerometer)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_accelerometer_state_type);
}

EHS_FB_INIT_FUNCTION(accelerometer)
{
    ehs_accelerometer_state_type *state = (ehs_accelerometer_state_type *)EHS_FB_INIT_CONTEXT;
    state->enabled = EHS_FALSE;
    state->x = 0.0f;
    state->y = 0.0f;
    state->z = 0.0f;
    return EHS_TRUE;
}

/* Enable the accelerometer */
EHS_FB_RUN_FUNCTION(accelerometer_enable)
{
    ehs_accelerometer_state_type *state = (ehs_accelerometer_state_type *)EHS_FB_RUN_CONTEXT;
    int rc = EhsTAccelEnable(state);
    EHS_FB_OUT_I_API2(INX_accelerometer_ARG_error_id, rc);
    if (rc == 0)
        state->enabled = EHS_TRUE;
    EHS_FB_FINISH_PORT("enabled");
}

/* Disable the accelerometer */
EHS_FB_RUN_FUNCTION(accelerometer_disable)
{
    ehs_accelerometer_state_type *state = (ehs_accelerometer_state_type *)EHS_FB_RUN_CONTEXT;
    int rc = EhsTAccelDisable(state);
    EHS_FB_OUT_I_API2(INX_accelerometer_ARG_error_id, rc);
    if (rc == 0)
        state->enabled = EHS_FALSE;
    EHS_FB_FINISH_PORT("disabled");
}

/* Read x/y/z acceleration */
EHS_FB_RUN_FUNCTION(accelerometer_read)
{
    ehs_accelerometer_state_type *state = (ehs_accelerometer_state_type *)EHS_FB_RUN_CONTEXT;
    EhsTAccelRead(state);
    EHS_FB_OUT_F_API2(INX_accelerometer_ARG_x, state->x);
    EHS_FB_OUT_F_API2(INX_accelerometer_ARG_y, state->y);
    EHS_FB_OUT_F_API2(INX_accelerometer_ARG_z, state->z);
    EHS_FB_FINISH_PORT("done");
}

EHS_FB_DESTROY_FUNCTION(accelerometer)
{
    ehs_accelerometer_state_type *state = (ehs_accelerometer_state_type *)EHS_FB_DESTROY_CONTEXT;
    if (state->enabled)
        EhsTAccelDisable(state);
    return EHS_TRUE;
}
