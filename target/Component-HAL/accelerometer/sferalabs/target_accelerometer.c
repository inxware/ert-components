/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file target_accelerometer.c
 * @brief Sfera Labs sysfs accelerometer HAL implementation.
 *
 * Supported boards: Exo Sense Pi only.
 * All other Sfera Labs boards return -1.
 *
 * Sysfs paths (Exo Sense Pi):
 *   /sys/class/exosensepi/accelerometer/enabled — write "1"/"0"
 *   /sys/class/exosensepi/accelerometer/x       — read integer in mg
 *   /sys/class/exosensepi/accelerometer/y       — read integer in mg
 *   /sys/class/exosensepi/accelerometer/z       — read integer in mg
 */

#include "globals.h"
#include "hal_accelerometer.h"
#include "sferalabs_hal.h"
#include "hal_logger.h"

#if defined(EHS_SFERALABS_BOARD_EXOSENSEPI)

#define SFERALABS_ACCEL_ENABLED_PATH  SFERALABS_CLASS_PATH "/accelerometer/enabled"
#define SFERALABS_ACCEL_X_PATH        SFERALABS_CLASS_PATH "/accelerometer/x"
#define SFERALABS_ACCEL_Y_PATH        SFERALABS_CLASS_PATH "/accelerometer/y"
#define SFERALABS_ACCEL_Z_PATH        SFERALABS_CLASS_PATH "/accelerometer/z"

EHS_GLOBAL int EhsTAccelEnable(ehs_accelerometer_state_type *state)
{
    if (sferalabs_sysfs_write(SFERALABS_ACCEL_ENABLED_PATH, "1", 1) != 0)
    {
        EHSH_LOG_ERROR("Accelerometer: failed to enable at %s", SFERALABS_ACCEL_ENABLED_PATH);
        return -1;
    }
    if (state)
        state->enabled = EHS_TRUE;
    EHSH_LOG_INFO("Accelerometer: enabled");
    return 0;
}

EHS_GLOBAL int EhsTAccelDisable(ehs_accelerometer_state_type *state)
{
    if (sferalabs_sysfs_write(SFERALABS_ACCEL_ENABLED_PATH, "0", 1) != 0)
    {
        EHSH_LOG_ERROR("Accelerometer: failed to disable at %s", SFERALABS_ACCEL_ENABLED_PATH);
        return -1;
    }
    if (state)
        state->enabled = EHS_FALSE;
    EHSH_LOG_INFO("Accelerometer: disabled");
    return 0;
}

EHS_GLOBAL int EhsTAccelRead(ehs_accelerometer_state_type *state)
{
    long x = 0, y = 0, z = 0;

    if (sferalabs_sysfs_read_int(SFERALABS_ACCEL_X_PATH, &x) != 0)
    {
        EHSH_LOG_ERROR("Accelerometer: failed to read x at %s", SFERALABS_ACCEL_X_PATH);
        return -1;
    }
    if (sferalabs_sysfs_read_int(SFERALABS_ACCEL_Y_PATH, &y) != 0)
    {
        EHSH_LOG_ERROR("Accelerometer: failed to read y at %s", SFERALABS_ACCEL_Y_PATH);
        return -1;
    }
    if (sferalabs_sysfs_read_int(SFERALABS_ACCEL_Z_PATH, &z) != 0)
    {
        EHSH_LOG_ERROR("Accelerometer: failed to read z at %s", SFERALABS_ACCEL_Z_PATH);
        return -1;
    }

    if (state)
    {
        state->x = (ehs_float)x;
        state->y = (ehs_float)y;
        state->z = (ehs_float)z;
    }
    return 0;
}

#else /* board does not have accelerometer */

EHS_GLOBAL int EhsTAccelEnable(ehs_accelerometer_state_type *state)
{
    (void)state;
    EHSH_LOG_ERROR("Accelerometer: not supported on this Sfera Labs board");
    return -1;
}

EHS_GLOBAL int EhsTAccelDisable(ehs_accelerometer_state_type *state)
{
    (void)state;
    EHSH_LOG_ERROR("Accelerometer: not supported on this Sfera Labs board");
    return -1;
}

EHS_GLOBAL int EhsTAccelRead(ehs_accelerometer_state_type *state)
{
    if (state)
    {
        state->x = 0.0f;
        state->y = 0.0f;
        state->z = 0.0f;
    }
    EHSH_LOG_ERROR("Accelerometer: not supported on this Sfera Labs board");
    return -1;
}

#endif /* board selection */
