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

#include "inx-component.h"
#include "accelerometer.h"
#include "hal_accelerometer.h"
#include "hal_accel_gyro.h"

EHS_FB_FUNCTIONS_START(accelerometer)
EHS_FB_FUNCTION_ENTRY("enable",  0x01, accelerometer_enable)
EHS_FB_FUNCTION_ENTRY("disable", 0x02, accelerometer_disable)
EHS_FB_FUNCTION_ENTRY("read",    0x03, accelerometer_read)
EHS_FB_FUNCTIONS_END

/* Port argument numbers (from CDF <Function argument="N">) */
#define INX_accelerometer_ARG_enabled   1   /* FinishPort: enabled (fn1, arg=1)    */
#define INX_accelerometer_ARG_disabled  1   /* FinishPort: disabled (fn2, arg=1)   */
#define INX_accelerometer_ARG_error_id  1   /* OutputPort: error_id (fn1+fn2, arg=1) */
#define INX_accelerometer_ARG_done      1   /* FinishPort: done (fn3, arg=1)       */
#define INX_accelerometer_ARG_x        1   /* OutputPort: x in mg (fn3, arg=1)    */
#define INX_accelerometer_ARG_y        2   /* OutputPort: y in mg (fn3, arg=2)    */
#define INX_accelerometer_ARG_z        3   /* OutputPort: z in mg (fn3, arg=3)    */


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
    ehs_sint32 rc = EhsTAccelGyroEnable();
    EHS_FB_OUT_I_API2(INX_accelerometer_ARG_error_id) = rc;
    if (rc == EHS_PERIPH_OK)
        state->enabled = EHS_TRUE;
    EHS_FB_FINISH_API2(INX_accelerometer_ARG_enabled);
}

/* Disable the accelerometer */
EHS_FB_RUN_FUNCTION(accelerometer_disable)
{
    ehs_accelerometer_state_type *state = (ehs_accelerometer_state_type *)EHS_FB_RUN_CONTEXT;
    EhsTAccelGyroDisable();
    state->enabled = EHS_FALSE;
    EHS_FB_OUT_I_API2(INX_accelerometer_ARG_error_id) = EHS_PERIPH_OK;
    EHS_FB_FINISH_API2(INX_accelerometer_ARG_disabled);
}

/* Read x/y/z acceleration */
EHS_FB_RUN_FUNCTION(accelerometer_read)
{
    ehs_accelerometer_state_type *state = (ehs_accelerometer_state_type *)EHS_FB_RUN_CONTEXT;
    EhsTAccelRead(&state->x, &state->y, &state->z);
    EHS_FB_OUT_F_API2(INX_accelerometer_ARG_x) = state->x;
    EHS_FB_OUT_F_API2(INX_accelerometer_ARG_y) = state->y;
    EHS_FB_OUT_F_API2(INX_accelerometer_ARG_z) = state->z;
    EHS_FB_FINISH_API2(INX_accelerometer_ARG_done);
}

EHS_FB_DESTROY_FUNCTION(accelerometer)
{
    ehs_accelerometer_state_type *state = (ehs_accelerometer_state_type *)EHS_FB_DESTROY_CONTEXT;
    if (state->enabled)
        EhsTAccelGyroDisable();
    return EHS_TRUE;
}
