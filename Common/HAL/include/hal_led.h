/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file hal_gpio.h
 * In this file, all of the hardware abstraction layer functions relating to LED are given.
 *
 * @author: inx limited
 *
 */


#ifndef _EHS_HAL_LED_H
#define _EHS_HAL_LED_H

/*****************************************************************************/
/* Included files */
#include "ehs_types.h"


// Handle GPIO input
EHS_GLOBAL ehs_bool EhsEnableLED(ehs_uint8 id);
EHS_GLOBAL ehs_bool EhsDisableLED(ehs_uint8 id);
EHS_GLOBAL ehs_bool EhsSetLED(ehs_uint8 id, ehs_bool state, ehs_uint8 brightness);


#endif /* _EHS_HAL_LED_H */