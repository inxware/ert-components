/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_accelerometer.h
 * Hardware abstraction layer for the accelerometer peripheral.
 * Acceleration values are in milli-g (mg).
 *
 * @author: inx limited
 */

#ifndef _HAL_ACCELEROMETER_H
#define _HAL_ACCELEROMETER_H

#include "globals.h"

typedef struct
{
    ehs_bool  enabled;
    ehs_float x;   /* acceleration in mg */
    ehs_float y;   /* acceleration in mg */
    ehs_float z;   /* acceleration in mg */
} ehs_accelerometer_state_type;

/** Enable the accelerometer.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTAccelEnable(ehs_accelerometer_state_type *state);

/** Disable the accelerometer.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTAccelDisable(ehs_accelerometer_state_type *state);

/** Read the current x/y/z acceleration in mg into state->x, state->y, state->z.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTAccelRead(ehs_accelerometer_state_type *state);

#endif /* _HAL_ACCELEROMETER_H */
