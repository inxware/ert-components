
/**
 * All the functions need to be ported to each platform target
 * file: target_pwm.c
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 * 
 */

#include "inx_pwm_port.h"
#include "target_gpio.h"
#include "ehs_types.h"

inx_hw_pwm_port_errcode_t EhsTPortPwmSetup(ehs_sint32 channel, ehs_sint32 io_num, ehs_sint32 freq, ehs_sint32 max_val);
inx_hw_pwm_port_errcode_t EhsTPortPwmDeinit(ehs_sint32 channel);
inx_hw_pwm_port_errcode_t EhsTPortPwmSetFreq(ehs_sint32 channel, ehs_sint32 freq);
inx_hw_pwm_port_errcode_t EhsTPortPwmSetDuty(ehs_sint32 channel, ehs_uint32 duty);
inx_hw_pwm_port_errcode_t EhsTPortPwmSetMaxValue(ehs_sint32 channel, ehs_sint32 freq, ehs_sint32 max_val);
inx_hw_pwm_port_errcode_t EhsTPortPwmEnable(ehs_sint32 channel);
inx_hw_pwm_port_errcode_t EhsTPortPwmDisable(ehs_sint32 channel);

inx_hw_pwm_port_errcode_t EhsTPortPwmGetMaxValue(ehs_sint32 channel, ehs_sint32 *max_val);

inx_hw_pwm_port_errcode_t EhsTPortPwmGetFreq(ehs_sint32 channel, ehs_sint32 *freq);
inx_hw_pwm_port_errcode_t EhsTPortPwmGetDuty(ehs_sint32 channel, ehs_sint32 *duty);
inx_hw_pwm_port_errcode_t EhsTPortPwmEnabled(ehs_sint32 channel, ehs_bool *enabled);

// Detect whether more than one platforms are enabled at the same time
#if defined(EHS_PLATFORM_RASPBERRYPI) && defined(EHS_PLATFORM_ESP32)
    #error "Only one platform could be configured!"
#endif

ehs_bool gGPIOInitialised = EHS_FALSE;

#ifdef  EHS_PLATFORM_RASPBERRYPI
#include <pigpio.h>
// TODO left for daemon-based operations
// #include <pigpiod_if2.h>
// 
// 
// static ehs_sint32 gPigpioNo;

typedef struct {
    ehs_bool initialised;
    ehs_uint32 io_num;
    ehs_bool enabled;
    ehs_uint32 duty;
    ehs_uint32 freq;
    ehs_uint32 max_val;
} ehs_rpi_pwm_duty_t;

static ehs_rpi_pwm_duty_t gGPIO[EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX + 1];
static ehs_sint8 gUsedGPIOCount = 0;

// Test whether the GPIO is available for Hardware PWM
#define EHS_RPI_IS_HWPWM(x) ( x == 12 || x == 13 || x == 18 || x == 19 )

/**
 * @brief Return whether the GPIO is used by the PWM driver
 * 
 * @param io_num The GPIO BCM number
 * @return ehs_bool EHS_TRUE for The GPIO is used by the PWM driver
 */
ehs_bool isTargetPwmGpioUsed(ehs_sint32 io_num)
{
    if (io_num < 0 || io_num > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return EHS_FALSE;
    ehs_uint8 _i = 0;
    for (_i = 0 ; _i < gUsedGPIOCount; _i++)
    {
        if (gGPIO[_i].io_num == io_num && gGPIO[_i].initialised == EHS_TRUE) return EHS_TRUE;
    }
    return EHS_FALSE;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmSetup(ehs_sint32 channel, ehs_sint32 io_num, ehs_sint32 freq, ehs_sint32 max_val)
{
    int ret = 0;
    if (io_num < 0 || io_num > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_IO_NUM;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIOInitialised)
    {
        if (gpioInitialise() < 0) return INX_HW_PWM_PORT_ESETUP;
        gGPIOInitialised = EHS_TRUE;
    }
    ret = gpioSetMode(io_num, PI_OUTPUT);
    switch (ret) {
        case 0:             break;
        case PI_BAD_GPIO:   return INX_HW_PWM_PORT_EINVALID_IO_NUM;
        case PI_BAD_MODE:   return INX_HW_PWM_PORT_EGENRIC;
        default:            return INX_HW_PWM_PORT_EGENRIC;
    }

    ret = gpioSetPWMfrequency(io_num, freq);
    switch (ret) {
        case 0:                 break;
        case PI_BAD_USER_GPIO:  return INX_HW_PWM_PORT_EINVALID_IO_NUM;
        default:                break;
    }
    
    ret = gpioSetPWMrange(io_num, max_val);
    switch (ret) {
        case 0:                 break;
        case PI_BAD_USER_GPIO:  return INX_HW_PWM_PORT_EINVALID_IO_NUM;
        case PI_BAD_DUTYRANGE:  return INX_HW_PWM_PORT_EINVALID_MAX_VAL;
        default:                break;
    }
    
    gpioPWM(io_num, 0);

    gGPIO[channel].initialised = EHS_TRUE;
    gGPIOInitialised_ionum[io_num] = EHS_TRUE;
    // Reset the parameter to 0 if the count is below 0
    if (gUsedGPIOCount < 0) gUsedGPIOCount = 0;
    gUsedGPIOCount++;
    gGPIO[channel].io_num = io_num;
    gGPIO[channel].enabled = EHS_FALSE;
    gGPIO[channel].duty = 0;
    gGPIO[channel].freq = freq;
    gGPIO[channel].max_val = max_val;

    return INX_HW_PWM_PORT_EOK;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmDeinit(ehs_sint32 channel)
{
    if (!gGPIOInitialised) return INX_HW_PWM_PORT_ESETUP;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIO[channel].initialised) return INX_HW_PWM_PORT_ESETUP;
    ehs_uint32 io_num = gGPIO[channel].io_num;
    ehs_uint8 _i = 0;
    // Set to input. Terminate only if all channels are destroyed.
    gpioSetMode(io_num, PI_INPUT);
    gGPIO[channel].initialised = EHS_FALSE;
    gUsedGPIOCount--;
    // Reset the parameter to 0 if the count is below 0
    if (gUsedGPIOCount < 0) gUsedGPIOCount = 0;
    gGPIOInitialised_ionum[io_num] = EHS_FALSE;
    for (_i = 0 ; _i < EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX ; _i++)
    {
        // Found initialised GPIO. Directly return without terminate.
        if (gGPIO[_i].initialised) goto lReturn;
    }
    // gpioTerminate();
lReturn:
    return INX_HW_PWM_PORT_EOK;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmSetFreq(ehs_sint32 channel, ehs_sint32 freq)
{
    if (!gGPIOInitialised) return INX_HW_PWM_PORT_ESETUP;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIO[channel].initialised) return INX_HW_PWM_PORT_ESETUP;
    ehs_uint32 io_num = gGPIO[channel].io_num;
    int ret = 0;
    gGPIO[channel].freq = freq;
    if (EHS_RPI_IS_HWPWM(io_num))
    {
        ret = gpioHardwarePWM(io_num, gGPIO[channel].freq, (ehs_uint32)(((ehs_uint64)gGPIO[channel].duty * 1000000) / gGPIO[channel].max_val));
        switch (ret) {
            case 0:                 break;
            case PI_BAD_GPIO:       return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            case PI_NOT_HPWM_GPIO:  return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            case PI_BAD_HPWM_DUTY:  return INX_HW_PWM_PORT_ESET_DUTY;
            case PI_BAD_HPWM_FREQ:  return INX_HW_PWM_PORT_EINTERNAL_CLK;
            case PI_HPWM_ILLEGAL:   return INX_HW_PWM_PORT_EGENRIC;
            default:                return INX_HW_PWM_PORT_EGENRIC;
        }
    }
    else {
        ret = gpioSetPWMfrequency(io_num, freq);
        switch (ret) {
            case 0:                 break;
            case PI_BAD_USER_GPIO:  return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            default:                return INX_HW_PWM_PORT_EGENRIC;
        }
    }
    return INX_HW_PWM_PORT_EOK;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmSetDuty(ehs_sint32 channel, ehs_uint32 duty)
{
    if (!gGPIOInitialised) return INX_HW_PWM_PORT_ESETUP;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIO[channel].initialised) return INX_HW_PWM_PORT_ESETUP;
    ehs_uint32 io_num = gGPIO[channel].io_num;
    int ret = 0;
    gGPIO[channel].duty = duty;
    if (EHS_RPI_IS_HWPWM(io_num))
    {
        ret = gpioHardwarePWM(io_num, gGPIO[channel].freq, (ehs_uint32)(((ehs_uint64)gGPIO[channel].duty * 1000000) / gGPIO[channel].max_val));
        switch (ret) {
            case 0:                 break;
            case PI_BAD_GPIO:       return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            case PI_NOT_HPWM_GPIO:  return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            case PI_BAD_HPWM_DUTY:  return INX_HW_PWM_PORT_ESET_DUTY;
            case PI_BAD_HPWM_FREQ:  return INX_HW_PWM_PORT_EINTERNAL_CLK;
            case PI_HPWM_ILLEGAL:   return INX_HW_PWM_PORT_EGENRIC;
            default:                return INX_HW_PWM_PORT_EGENRIC;
        }
    }
    else {
        if (gGPIO[channel].enabled) ret = gpioPWM(io_num, duty);
        switch (ret) {
            case 0:                 break;
            case PI_BAD_USER_GPIO:  return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            case PI_BAD_DUTYCYCLE:  return INX_HW_PWM_PORT_ESET_DUTY;
            default:                return INX_HW_PWM_PORT_EGENRIC;
        }
    }
    return INX_HW_PWM_PORT_EOK;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmSetMaxValue(ehs_sint32 channel, ehs_sint32 freq, ehs_sint32 max_val)
{
    if (!gGPIOInitialised) return INX_HW_PWM_PORT_ESETUP;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIO[channel].initialised) return INX_HW_PWM_PORT_ESETUP;
    ehs_uint32 io_num = gGPIO[channel].io_num;
    int ret = 0;
    gGPIO[channel].max_val = max_val;
    if (EHS_RPI_IS_HWPWM(io_num))
    {
        //
    }
    else {
        ret = gpioSetPWMrange(io_num, max_val);
        switch (ret) {
            case 0:                 break;
            case PI_BAD_USER_GPIO:  return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            case PI_BAD_DUTYRANGE:  return INX_HW_PWM_PORT_EINVALID_MAX_VAL;
            default:                return INX_HW_PWM_PORT_EGENRIC;
        }
    }
    return INX_HW_PWM_PORT_EOK;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmEnable(ehs_sint32 channel)
{
    if (!gGPIOInitialised) return INX_HW_PWM_PORT_ESETUP;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIO[channel].initialised) return INX_HW_PWM_PORT_ESETUP;
    ehs_uint32 io_num = gGPIO[channel].io_num;
    int ret = 0;
    if (EHS_RPI_IS_HWPWM(io_num))
    {
        ret = gpioHardwarePWM(io_num, gGPIO[channel].freq, (ehs_uint32)(((ehs_uint64)gGPIO[channel].duty * 1000000) / gGPIO[channel].max_val));
        switch (ret) {
            case 0:                 break;
            case PI_BAD_GPIO:       return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            case PI_NOT_HPWM_GPIO:  return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            case PI_BAD_HPWM_DUTY:  return INX_HW_PWM_PORT_ESET_DUTY;
            case PI_BAD_HPWM_FREQ:  return INX_HW_PWM_PORT_EINTERNAL_CLK;
            case PI_HPWM_ILLEGAL:   return INX_HW_PWM_PORT_EGENRIC;
            default:                return INX_HW_PWM_PORT_EGENRIC;
        }
    }
    else {
        ret = gpioPWM(io_num, gGPIO[channel].duty);
        switch (ret) {
            case 0:                 break;
            case PI_BAD_USER_GPIO:  return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            case PI_BAD_DUTYCYCLE:  return INX_HW_PWM_PORT_ESET_DUTY;
            default:                return INX_HW_PWM_PORT_EGENRIC;
        }
    }
    gGPIO[channel].enabled = EHS_TRUE;
    return INX_HW_PWM_PORT_EOK;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmDisable(ehs_sint32 channel)
{
    if (!gGPIOInitialised) return INX_HW_PWM_PORT_ESETUP;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIO[channel].initialised) return INX_HW_PWM_PORT_ESETUP;
    ehs_uint32 io_num = gGPIO[channel].io_num;
    int ret = 0;
    if (EHS_RPI_IS_HWPWM(io_num))
    {
        ret = gpioHardwarePWM(io_num, 0, 0);
        switch (ret) {
            case 0:                 break;
            case PI_BAD_GPIO:       return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            case PI_NOT_HPWM_GPIO:  return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            case PI_BAD_HPWM_DUTY:  return INX_HW_PWM_PORT_ESET_DUTY;
            case PI_BAD_HPWM_FREQ:  return INX_HW_PWM_PORT_EINTERNAL_CLK;
            case PI_HPWM_ILLEGAL:   return INX_HW_PWM_PORT_EGENRIC;
            default:                return INX_HW_PWM_PORT_EGENRIC;
        }
    }
    else {
        ret = gpioPWM(io_num, 0);
        switch (ret) {
            case 0:                 break;
            case PI_BAD_USER_GPIO:  return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            case PI_BAD_DUTYCYCLE:  return INX_HW_PWM_PORT_ESET_DUTY;
            default:                return INX_HW_PWM_PORT_EGENRIC;
        }
    }
    gGPIO[channel].enabled = EHS_FALSE;
    return INX_HW_PWM_PORT_EOK;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmGetMaxValue(ehs_sint32 channel, ehs_sint32 *max_val)
{
    if (!gGPIOInitialised) return INX_HW_PWM_PORT_ESETUP;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIO[channel].initialised) return INX_HW_PWM_PORT_ESETUP;
    ehs_uint32 io_num = gGPIO[channel].io_num;
    int ret = 0;
    if (EHS_RPI_IS_HWPWM(io_num))
    {
        *max_val = gGPIO[channel].max_val;
    }
    else {
        ret = gpioGetPWMrange(io_num);
        switch (ret) {
            case PI_BAD_USER_GPIO:  return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            default:                *max_val = ret; break;
        }
    }
    return INX_HW_PWM_PORT_EOK;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmGetFreq(ehs_sint32 channel, ehs_sint32 *freq)
{
    if (!gGPIOInitialised) return INX_HW_PWM_PORT_ESETUP;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIO[channel].initialised) return INX_HW_PWM_PORT_ESETUP;
    ehs_uint32 io_num = gGPIO[channel].io_num;
    int ret = 0;
    if (EHS_RPI_IS_HWPWM(io_num))
    {
        *freq = gGPIO[channel].freq;
    }
    else {
        ret = gpioGetPWMfrequency(io_num);
        switch (ret) {
            case PI_BAD_USER_GPIO:  return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            default:                *freq = ret; break;
        }
    }
    return INX_HW_PWM_PORT_EOK;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmGetDuty(ehs_sint32 channel, ehs_sint32 *duty)
{
    if (!gGPIOInitialised) return INX_HW_PWM_PORT_ESETUP;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIO[channel].initialised) return INX_HW_PWM_PORT_ESETUP;
    ehs_uint32 io_num = gGPIO[channel].io_num;
    int ret = 0;
    if (EHS_RPI_IS_HWPWM(io_num))
    {
        *duty = gGPIO[channel].duty;
    }
    else {
        ret = gpioGetPWMdutycycle(io_num);
        switch (ret) {
            case PI_BAD_USER_GPIO:  return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            case PI_NOT_PWM_GPIO:   return INX_HW_PWM_PORT_EINVALID_IO_NUM;
            default:                *duty = ret; break;
        }
    }
    return INX_HW_PWM_PORT_EOK;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmEnabled(ehs_sint32 channel, ehs_bool *enabled)
{
    if (!gGPIOInitialised) return INX_HW_PWM_PORT_ESETUP;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIO[channel].initialised) return INX_HW_PWM_PORT_ESETUP;
    ehs_uint32 io_num = gGPIO[channel].io_num;
    *enabled = gGPIO[channel].enabled;
    return INX_HW_PWM_PORT_EOK;
}

#endif//EHS_PLATFORM_RASPBERRYPI