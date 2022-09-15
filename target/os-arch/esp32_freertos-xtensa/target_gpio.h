/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/*
 * target_gpio.h
 */

#ifndef TARGET_GPIO_H_
#define TARGET_GPIO_H_

#include "target_types.h"

// Handle GPIO output
EHS_GLOBAL ehs_bool EhsInitOutputGPIO(ehs_sint32 pin_id);

EHS_GLOBAL ehs_bool EhsWriteOutputGPIO(ehs_sint32 pin_id, ehs_bool value);

EHS_GLOBAL ehs_bool EhsDestroyOutputGPIO(ehs_sint32 pin_id);

// Handle GPIO input
EHS_GLOBAL ehs_bool EhsInitInputGPIO(ehs_sint32 pin_id);

EHS_GLOBAL ehs_bool EhsReadInputGPIO(ehs_sint32 pin_id, ehs_bool *value);

EHS_GLOBAL ehs_bool EhsDestroyInputGPIO(ehs_sint32 pin_id);

#endif /* TARGET_GPIO_H_ */
