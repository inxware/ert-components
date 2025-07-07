/*
 * target_gpio.c
 */
#include "target.h"
#include "target_gpio.h"
#include "target_config.h"
#include "hal_file.h"
#include "hal_logger.h"

#include <pigpio.h>

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
            ret = gpioSetMode(io_num, PI_OUTPUT);
            switch (ret) {
                case 0:             break;
                case PI_BAD_GPIO:   return EHS_FALSE;
                case PI_BAD_MODE:   return EHS_FALSE;
                default:            return EHS_FALSE;
            }
            break;
        }
        case 0b11:
        {
            ret = gpioSetMode(io_num, PI_INPUT);
            switch (ret) {
                case 0:             break;
                case PI_BAD_GPIO:   return EHS_FALSE;
                case PI_BAD_MODE:   return EHS_FALSE;
                default:            return EHS_FALSE;
            }
            break;
        }
        default: break;
    }
    switch (expression) {
       case 0b00:
       case 0b01:
       case 0b10:
       {
           ret = gpioWrite(io_num, value);
           switch (ret) {
               case 0:             break;
               case PI_BAD_GPIO:   return EHS_FALSE;
               case PI_BAD_LEVEL:  return EHS_FALSE;
               default:            return EHS_FALSE;
           }
           break;
       }
       default: break;
    }

    return EHS_TRUE;
}

// Handle GPIO output
EHS_GLOBAL ehs_bool EhsInitOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    if (pGPIO->pin_id < 0 || pGPIO->pin_id >EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return EHS_FALSE;
    // The GPIO is used by PWM driver
    if (isTargetPwmGpioUsed(pGPIO->pin_id)) return EHS_FALSE;
    ehs_bool ret = EHS_TRUE;
    if (!gGPIOInitialised)
    {
        if (gpioInitialise() < 0) return EHS_FALSE;
        gGPIOInitialised = EHS_TRUE;
    }

    ret = set_output_value_GPIO(pGPIO->pin_id, pGPIO->initial_state > 0, pGPIO->open_drain_mode | (pGPIO->initial_state == 2));

    pGPIO->error_state = ret;
    return ret;
}

EHS_GLOBAL ehs_bool EhsWriteOutputGPIO(ehs_gpio_out_state_type* pGPIO)
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

EHS_GLOBAL ehs_bool EhsDestroyOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    if (!gGPIOInitialised) return EHS_FALSE;
    if (pGPIO->pin_id < 0 || pGPIO->pin_id >EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return EHS_FALSE;
    if (!gGPIOInitialised_ionum[pGPIO->pin_id]) return EHS_FALSE;
    // The GPIO is used by PWM driver
    if (isTargetPwmGpioUsed(pGPIO->pin_id)) return EHS_FALSE;
    ehs_bool ret = EHS_TRUE;
    ehs_uint8 _i = 0;
    ret = gpioSetMode(pGPIO->pin_id, PI_INPUT);
    switch (ret) {
        case 0:             break;
        case PI_BAD_GPIO:   return EHS_FALSE;
        case PI_BAD_MODE:   return EHS_FALSE;
        default:            return EHS_FALSE;
    }
    gGPIOInitialised_ionum[pGPIO->pin_id] = EHS_FALSE;
    for (_i = 0 ; _i < EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX ; _i++)
    {
        // There's still one GPIO in use. Do not terminate the entire GPIO instance
        if (gGPIOInitialised_ionum[_i]) return EHS_TRUE;
    }

    // gpioTerminate();
    return EHS_TRUE;
}

// Handle GPIO input
EHS_GLOBAL ehs_bool EhsInitInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    if (pGPIO->pin_id < 0 || pGPIO->pin_id >EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX)
    // The GPIO is used by PWM driver
    if (isTargetPwmGpioUsed(pGPIO->pin_id)) return EHS_FALSE;
    int ret= 0;
    if (!gGPIOInitialised)
    {
        if (gpioInitialise() < 0) return EHS_FALSE;
        gGPIOInitialised = EHS_TRUE;
    }
    ret = gpioSetMode(pGPIO->pin_id, PI_INPUT);
    switch (ret) {
        case 0:             break;
        case PI_BAD_GPIO:   return EHS_FALSE;
        case PI_BAD_MODE:   return EHS_FALSE;
        default:            return EHS_FALSE;
    }

    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsReadInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    if (!gGPIOInitialised) return EHS_FALSE;
    if (pGPIO->pin_id < 0 || pGPIO->pin_id >EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return EHS_FALSE;
    // The GPIO is used by PWM driver
    if (isTargetPwmGpioUsed(pGPIO->pin_id)) return EHS_FALSE;
    ehs_bool ret = EHS_TRUE;
    ret = gpioSetMode(pGPIO->pin_id, PI_INPUT);
    switch (ret) {
        case 0:             break;
        case PI_BAD_GPIO:   return EHS_FALSE;
        case PI_BAD_MODE:   return EHS_FALSE;
        default:            return EHS_FALSE;
    }
    ret = gpioRead(pGPIO->pin_id);
    switch (ret) {
        case PI_BAD_GPIO:   return EHS_FALSE;
        default:            pGPIO->pin_value = ret; break;
    }

    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsDestroyInputGPIO(ehs_gpio_in_state_type* pGPIO)
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
