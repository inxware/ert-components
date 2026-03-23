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
 * @brief Arduino LSM6DS3 HAL for accel_gyro function block.
 *
 * Uses the Arduino_LSM6DS3 library.  Both accelerometer and gyroscope
 * are supported on this platform.
 */

#include "hal_accel_gyro.h"
#include <Arduino_LSM6DS3.h>

static ehs_bool gIMU_Enabled = EHS_FALSE;

ehs_sint32 EhsTAccelGyroEnable(void)
{
    if (gIMU_Enabled == EHS_FALSE) {
        if (IMU.begin()) {
            gIMU_Enabled = EHS_TRUE;
        }
    }
    return gIMU_Enabled ? EHS_PERIPH_OK : EHS_PERIPH_ERR_GENERAL;
}

void EhsTAccelGyroDisable(void)
{
    if (gIMU_Enabled == EHS_TRUE) {
        IMU.end();
        gIMU_Enabled = EHS_FALSE;
    }
}

ehs_bool EhsTAccelRead(ehs_float *x, ehs_float *y, ehs_float *z)
{
    if (!gIMU_Enabled || !x || !y || !z)
        return EHS_FALSE;
    float Ax = 0, Ay = 0, Az = 0;
    if (!IMU.accelerationAvailable())
        return EHS_FALSE;
    IMU.readAcceleration(Ax, Ay, Az);
    *x = (ehs_float)Ax; *y = (ehs_float)Ay; *z = (ehs_float)Az;
    return EHS_TRUE;
}

ehs_bool EhsTGyroRead(ehs_float *x, ehs_float *y, ehs_float *z)
{
    if (!gIMU_Enabled || !x || !y || !z)
        return EHS_FALSE;
    float Gx = 0, Gy = 0, Gz = 0;
    if (!IMU.gyroscopeAvailable())
        return EHS_FALSE;
    IMU.readGyroscope(Gx, Gy, Gz);
    *x = (ehs_float)Gx; *y = (ehs_float)Gy; *z = (ehs_float)Gz;
    return EHS_TRUE;
}
