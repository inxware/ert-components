/**
 * file: target_gpio.h
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 * 
 */

#ifndef TARGET_GPIO_H_
#define TARGET_GPIO_H_

#include "globals.h"
#include "ehs_hal_gpio.h"

//#define EHS_PLATFORM_RASPBERRYPI

extern ehs_bool gGPIOInitialised;

#define EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX 31

extern ehs_bool gGPIOInitialised_ionum[EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX + 1];
ehs_bool isTargetPwmGpioUsed(ehs_sint32 io_num);

#endif /* TARGET_GPIO_H_ */
