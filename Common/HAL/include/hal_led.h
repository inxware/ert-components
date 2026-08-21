/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file ehs_hal_gpio.h
 * In this file, all of the hardware abstraction layer functions relating to LED are given.
 *
 * @author: inx limited
 *
 */


#ifndef _EHS_HAL_LED_H
#define _EHS_HAL_LED_H

/*****************************************************************************/
/* Included files */
#include "globals.h"


// Handle GPIO input
ehs_bool EhsEnableLED(ehs_uint8 id);
ehs_bool EhsDisableLED(ehs_uint8 id);
ehs_bool EhsSetLED(ehs_uint8 id, ehs_bool state, ehs_uint8 brightness);


#endif /* _EHS_HAL_LED_H */