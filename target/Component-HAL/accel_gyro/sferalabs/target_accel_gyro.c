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
 * @brief Sfera Labs HAL for accel_gyro function block.
 *
 * Accelerometer: Exo Sense Pi only, via sysfs:
 *   /sys/class/exosensepi/accelerometer/{enabled,x,y,z}  (integer mg)
 *
 * Gyroscope: not available on Sfera Labs hardware.
 *   EhsTGyroRead() always returns EHS_FALSE on this platform.
 *
 * All other Sfera Labs boards return EHS_FALSE from EhsTAccelGyroEnable().
 */

#include "globals.h"
#include "hal_accel_gyro.h"
#include "sferalabs_hal.h"
#include "hal_logger.h"

#if defined(EHS_SFERALABS_BOARD_EXOSENSEPI)

#define SFERALABS_ACCEL_ENABLED_PATH  SFERALABS_CLASS_PATH "/accelerometer/enabled"
#define SFERALABS_ACCEL_X_PATH        SFERALABS_CLASS_PATH "/accelerometer/x"
#define SFERALABS_ACCEL_Y_PATH        SFERALABS_CLASS_PATH "/accelerometer/y"
#define SFERALABS_ACCEL_Z_PATH        SFERALABS_CLASS_PATH "/accelerometer/z"

EHS_GLOBAL ehs_sint32 EhsTAccelGyroEnable(void)
{
    if (sferalabs_sysfs_write(SFERALABS_ACCEL_ENABLED_PATH, "1", 1) != 0)
    {
        EHSH_LOG_ERROR("AccelGyro(sferalabs): failed to enable at %s", SFERALABS_ACCEL_ENABLED_PATH);
        return EHS_PERIPH_ERR_SYSFS;
    }
    EHSH_LOG_INFO("AccelGyro(sferalabs): enabled");
    return EHS_PERIPH_OK;
}

EHS_GLOBAL void EhsTAccelGyroDisable(void)
{
    if (sferalabs_sysfs_write(SFERALABS_ACCEL_ENABLED_PATH, "0", 1) != 0)
        EHSH_LOG_ERROR("AccelGyro(sferalabs): failed to disable at %s", SFERALABS_ACCEL_ENABLED_PATH);
    else
        EHSH_LOG_INFO("AccelGyro(sferalabs): disabled");
}

EHS_GLOBAL ehs_bool EhsTAccelRead(ehs_float *x, ehs_float *y, ehs_float *z)
{
    long lx = 0, ly = 0, lz = 0;
    if (!x || !y || !z)
        return EHS_FALSE;
    if (sferalabs_sysfs_read_int(SFERALABS_ACCEL_X_PATH, &lx) != 0) {
        EHSH_LOG_ERROR("AccelGyro(sferalabs): failed to read x at %s", SFERALABS_ACCEL_X_PATH);
        return EHS_FALSE;
    }
    if (sferalabs_sysfs_read_int(SFERALABS_ACCEL_Y_PATH, &ly) != 0) {
        EHSH_LOG_ERROR("AccelGyro(sferalabs): failed to read y at %s", SFERALABS_ACCEL_Y_PATH);
        return EHS_FALSE;
    }
    if (sferalabs_sysfs_read_int(SFERALABS_ACCEL_Z_PATH, &lz) != 0) {
        EHSH_LOG_ERROR("AccelGyro(sferalabs): failed to read z at %s", SFERALABS_ACCEL_Z_PATH);
        return EHS_FALSE;
    }
    *x = (ehs_float)lx;
    *y = (ehs_float)ly;
    *z = (ehs_float)lz;
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsTGyroRead(ehs_float *x, ehs_float *y, ehs_float *z)
{
    /* Gyroscope not available on Sfera Labs hardware */
    (void)x; (void)y; (void)z;
    EHSH_LOG_WARNING("AccelGyro(sferalabs): gyroscope not supported on this platform");
    return EHS_FALSE;
}

#else /* Sfera Labs board without accelerometer */

EHS_GLOBAL ehs_sint32 EhsTAccelGyroEnable(void)
{
    EHSH_LOG_ERROR("AccelGyro(sferalabs): not supported on this Sfera Labs board");
    return EHS_PERIPH_ERR_NOT_SUPPORTED;
}

EHS_GLOBAL void EhsTAccelGyroDisable(void)
{
    EHSH_LOG_ERROR("AccelGyro(sferalabs): not supported on this Sfera Labs board");
}

EHS_GLOBAL ehs_bool EhsTAccelRead(ehs_float *x, ehs_float *y, ehs_float *z)
{
    if (x && y && z) { *x = 0.0f; *y = 0.0f; *z = 0.0f; }
    EHSH_LOG_ERROR("AccelGyro(sferalabs): not supported on this Sfera Labs board");
    return EHS_FALSE;
}

EHS_GLOBAL ehs_bool EhsTGyroRead(ehs_float *x, ehs_float *y, ehs_float *z)
{
    if (x && y && z) { *x = 0.0f; *y = 0.0f; *z = 0.0f; }
    EHSH_LOG_ERROR("AccelGyro(sferalabs): not supported on this Sfera Labs board");
    return EHS_FALSE;
}

#endif /* board selection */
