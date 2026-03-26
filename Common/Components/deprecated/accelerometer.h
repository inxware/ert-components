/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file accelerometer.h
 * Function block declarations for the accelerometer peripheral.
 */

#ifndef _EHS_ACCELEROMETER_H
#define _EHS_ACCELEROMETER_H

#include "ehs_fb_types.h"
#include "inx-component.h"

#define EHS_FB_NAME_accelerometer "accelerometer"
#define EHS_FB_ID_accelerometer   0x69DD

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(accelerometer);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(accelerometer);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(accelerometer_enable);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(accelerometer_disable);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(accelerometer_read);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(accelerometer);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(accelerometer)

#endif /* _EHS_ACCELEROMETER_H */
