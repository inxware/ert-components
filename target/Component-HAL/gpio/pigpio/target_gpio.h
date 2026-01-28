/*
 * target_gpio.h
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
