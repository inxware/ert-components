/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_accel_gyro.h
 * HAL for combined accelerometer / gyroscope IMU.
 *
 * Implementations live in target/Component-HAL/accel_gyro/<variant>/
 *   stubbed   — returns (0,0,0) with success; used for development / platforms without IMU
 *   sferalabs — Exo Sense Pi accelerometer via sysfs; gyro not available (returns EHS_FALSE)
 *   arduino   — Arduino LSM6DS3 (target/os-arch/arduino_ALL/target_accel_gyro.c)
 *
 * Selected by EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT in the platform config.mk.
 */

#ifndef _EHS_HAL_ACCEL_GYRO_H
#define _EHS_HAL_ACCEL_GYRO_H

#include "globals.h"
#include "hal_peripheral_errors.h"

/**
 * Enable the IMU.
 * @return EHS_PERIPH_OK (0) on success, EHS_PERIPH_ERR_SYSFS (-2) if the
 *         sysfs node could not be written, EHS_PERIPH_ERR_NOT_SUPPORTED (-3)
 *         if this platform/board has no IMU.
 */
ehs_sint32 EhsTAccelGyroEnable(void);

/** Disable the IMU. */
void EhsTAccelGyroDisable(void);

/**
 * Read accelerometer values.
 * @param x  Acceleration on X axis (g or mg — unit is implementation-defined)
 * @param y  Acceleration on Y axis
 * @param z  Acceleration on Z axis
 * @return EHS_TRUE if data was available and written, EHS_FALSE otherwise.
 */
ehs_bool EhsTAccelRead(ehs_float *x, ehs_float *y, ehs_float *z);

/**
 * Read gyroscope values.
 * @param x  Angular rate on X axis (deg/s — implementation-defined)
 * @param y  Angular rate on Y axis
 * @param z  Angular rate on Z axis
 * @return EHS_TRUE if data was available and written, EHS_FALSE if gyro is
 *         not available on this platform (e.g. Sfera Labs accel-only hardware).
 */
ehs_bool EhsTGyroRead(ehs_float *x, ehs_float *y, ehs_float *z);

#endif /* _EHS_HAL_ACCEL_GYRO_H */
