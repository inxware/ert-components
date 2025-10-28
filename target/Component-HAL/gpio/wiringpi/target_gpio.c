/**
 * file: target_gpio.c
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 * 
 */


 #include "globals.h"
#include "target_gpio.h"
#include "hal_file.h"
#include "hal_logger.h"

#include <wiringPi.h>

ehs_bool gGPIOInitialised_ionum[EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX + 1] = { 0 };

static ehs_bool set_output_value_GPIO(ehs_sint32 io_num, ehs_bool value, ehs_bool openDrain)
{
    int ret = 0;
    ehs_uint8 expression = ((openDrain & 1) << 1) | (value & 1);
    switch (expression) {
        case 0b00: 
        case 0b01:
        case 0b10:
        {
            pinMode(io_num, OUTPUT);
            break;
        }
        case 0b11:
        {
            pinMode(io_num, INPUT);
            break;
        }
        default: break;
    }
    switch (expression) {
       case 0b00:
       case 0b01:
       case 0b10:
       {
           digitalWrite(io_num, value);
           break;
       }
       default: break;
    }

    return EHS_TRUE;
}

// Handle GPIO output
ehs_bool EhsInitOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    if (pGPIO->pin_id < 0 || pGPIO->pin_id >EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return EHS_FALSE;
    // The GPIO is used by PWM driver
    if (isTargetPwmGpioUsed(pGPIO->pin_id)) return EHS_FALSE;
    ehs_bool ret = EHS_TRUE;
    if (!gGPIOInitialised)
    {
        if (wiringPiSetupPinType(WPI_PIN_BCM) < 0) return EHS_FALSE;
        gGPIOInitialised = EHS_TRUE;
    }

    ret = set_output_value_GPIO(pGPIO->pin_id, pGPIO->initial_state > 0, pGPIO->open_drain_mode | (pGPIO->initial_state == 2));

    pGPIO->error_state = ret;
    return ret;
}

ehs_bool EhsWriteOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    if (!gGPIOInitialised) return EHS_FALSE;
    if (pGPIO->pin_id < 0 || pGPIO->pin_id >EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return EHS_FALSE;
    // The GPIO is used by PWM driver
    if (isTargetPwmGpioUsed(pGPIO->pin_id)) return EHS_FALSE;
    ehs_bool ret = EHS_TRUE;
    ret = set_output_value_GPIO(pGPIO->pin_id, pGPIO->pin_value, pGPIO->open_drain_mode);

    pGPIO->error_state = ret;
    return ret;
}

ehs_bool EhsDestroyOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    if (!gGPIOInitialised) return EHS_FALSE;
    if (pGPIO->pin_id < 0 || pGPIO->pin_id >EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return EHS_FALSE;
    if (!gGPIOInitialised_ionum[pGPIO->pin_id]) return EHS_FALSE;
    // The GPIO is used by PWM driver
    if (isTargetPwmGpioUsed(pGPIO->pin_id)) return EHS_FALSE;
    ehs_bool ret = EHS_TRUE;
    ehs_uint8 _i = 0;
    pinMode(pGPIO->pin_id, PM_OFF);
    gGPIOInitialised_ionum[pGPIO->pin_id] = EHS_FALSE;

    return EHS_TRUE;
}

// Handle GPIO input
ehs_bool EhsInitInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    if (pGPIO->pin_id < 0 || pGPIO->pin_id >EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX)
    // The GPIO is used by PWM driver
    if (isTargetPwmGpioUsed(pGPIO->pin_id)) return EHS_FALSE;
    int ret= 0;
    if (!gGPIOInitialised)
    {
        if (wiringPiSetupPinType(WPI_PIN_BCM) < 0) return EHS_FALSE;
        gGPIOInitialised = EHS_TRUE;
    }
    pinMode(pGPIO->pin_id, INPUT);

    return EHS_TRUE;
}

ehs_bool EhsReadInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    if (!gGPIOInitialised) return EHS_FALSE;
    if (pGPIO->pin_id < 0 || pGPIO->pin_id >EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return EHS_FALSE;
    // The GPIO is used by PWM driver
    if (isTargetPwmGpioUsed(pGPIO->pin_id)) return EHS_FALSE;
    pinMode(pGPIO->pin_id, INPUT);
    pGPIO->pin_value = digitalRead(pGPIO->pin_id);

    return EHS_TRUE;
}

ehs_bool EhsDestroyInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    ehs_gpio_out_state_type _pGPIO = {
        .pin_id = pGPIO->pin_id,
        .pin_value = EHS_FALSE,
        .target_data = pGPIO->target_data,
    };
    // The input GPIO destroy process is the same as the output one
    ehs_bool ret = EhsDestroyOutputGPIO(&_pGPIO);

    return ret;
}

