/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file hal_accel_gyro.h
 * In this file, all of the hardware abstraction layer functions relating to accelometer and gyroscope are given.
 *
 * @author: inx limited
 *
 */


#ifndef _EHS_HAL_ACCEL_GYRO_H
#define _EHS_HAL_ACCEL_GYRO_H

/*****************************************************************************/
/* Included files */
#include "ehs_types.h"

ehs_bool EnableAccelometerGyrosope();
void DisableAccelometerGyrosope();

ehs_bool EhsGetAccelometerValues(ehs_float* x, ehs_float* y, ehs_float* z);
ehs_bool EhsGetGyrosopeValues(ehs_float* x, ehs_float* y, ehs_float* z);

#endif // _EHS_HAL_ACCEL_GYRO_H