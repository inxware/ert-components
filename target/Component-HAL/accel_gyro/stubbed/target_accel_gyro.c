/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file target_accel_gyro.c
 * @brief Stub HAL for accel_gyro — no hardware access.
 *
 * Used on platforms without an IMU, or during development.
 * All reads succeed and return (0.0, 0.0, 0.0).
 */

#include "hal_accel_gyro.h"

EHS_GLOBAL ehs_sint32 EhsTAccelGyroEnable(void)
{
    return EHS_PERIPH_OK;
}

EHS_GLOBAL void EhsTAccelGyroDisable(void)
{
}

EHS_GLOBAL ehs_bool EhsTAccelRead(ehs_float *x, ehs_float *y, ehs_float *z)
{
    if (x && y && z) {
        *x = 0.0f; *y = 0.0f; *z = 0.0f;
        return EHS_TRUE;
    }
    return EHS_FALSE;
}

EHS_GLOBAL ehs_bool EhsTGyroRead(ehs_float *x, ehs_float *y, ehs_float *z)
{
    if (x && y && z) {
        *x = 0.0f; *y = 0.0f; *z = 0.0f;
        return EHS_TRUE;
    }
    return EHS_FALSE;
}
