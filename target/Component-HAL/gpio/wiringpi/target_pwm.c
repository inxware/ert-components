/**
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
#include "globals.h"

#define PWM_MAXIMUM_FREQUENCY_VALUE 19200000
#define PWM_MAXIMUM_SETTING_FREQ_VALUE 19200
#define PWM_CONSTANT_RANGE (PWM_MAXIMUM_FREQUENCY_VALUE / PWM_MAXIMUM_SETTING_FREQ_VALUE)

ehs_bool gGPIOInitialised = EHS_FALSE;

#include <wiringPi.h>
#include <softPwm.h>

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
static ehs_uint32 gMaxVal = 0;
static ehs_uint32 gClockDivider = 0;

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
    if (io_num < 0 || io_num > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_IO_NUM;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIOInitialised)
    {
        if (wiringPiSetupPinType(WPI_PIN_BCM) < 0) return INX_HW_PWM_PORT_ESETUP;
        gGPIOInitialised = EHS_TRUE;
    }

    if (EHS_RPI_IS_HWPWM(io_num))
    {
        if (freq <= 0 || max_val <= 0 || (freq*max_val) > PWM_MAXIMUM_FREQUENCY_VALUE) return INX_HW_PWM_PORT_ESETUP;
        if (freq > PWM_MAXIMUM_SETTING_FREQ_VALUE) return INX_HW_PWM_PORT_ESET_FREQ;
        pinMode(io_num, PWM_MS_OUTPUT);
        if (gMaxVal == 0) pwmSetRange(PWM_CONSTANT_RANGE);
        gMaxVal = PWM_CONSTANT_RANGE;
        if (gClockDivider == 0) gClockDivider = 19200000 / max_val / freq;
        else if (gClockDivider != (19200000 / max_val / freq)) return INX_HW_PWM_PORT_ESETUP;
        pwmSetClock(19200000 / max_val / freq);
        pwmWrite(io_num, 0);
    }
    else {
        softPwmCreate(io_num, 0, max_val);
    }

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
    pinMode(io_num, PM_OFF);
    gGPIO[channel].initialised = EHS_FALSE;
    gUsedGPIOCount--;
    // Reset the parameter to 0 if the count is below 0
    if (gUsedGPIOCount < 0) gUsedGPIOCount = 0;
    gGPIOInitialised_ionum[io_num] = EHS_FALSE;

    return INX_HW_PWM_PORT_EOK;
}

/* This would override the existing PWM settings */
inx_hw_pwm_port_errcode_t EhsTPortPwmSetFreq(ehs_sint32 channel, ehs_sint32 freq)
{
    if (!gGPIOInitialised) return INX_HW_PWM_PORT_ESETUP;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIO[channel].initialised) return INX_HW_PWM_PORT_ESETUP;

    if (freq <= 0 || (freq*PWM_CONSTANT_RANGE) > PWM_MAXIMUM_FREQUENCY_VALUE) return INX_HW_PWM_PORT_ESET_FREQ;
    if (freq > PWM_MAXIMUM_SETTING_FREQ_VALUE) return INX_HW_PWM_PORT_ESET_FREQ;
    ehs_uint32 _clockDivider = 19200000 / PWM_CONSTANT_RANGE / freq;
    if (_clockDivider <= 0) return INX_HW_PWM_PORT_ESET_FREQ;
    gClockDivider = _clockDivider;
    gGPIO[channel].freq = freq;
    pwmSetClock(gClockDivider);

    return INX_HW_PWM_PORT_EOK;
}
inx_hw_pwm_port_errcode_t EhsTPortPwmSetDuty(ehs_sint32 channel, ehs_uint32 duty)
{
    if (!gGPIOInitialised) return INX_HW_PWM_PORT_ESETUP;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIO[channel].initialised) return INX_HW_PWM_PORT_ESETUP;
    ehs_uint32 io_num = gGPIO[channel].io_num;
    if (duty > gGPIO[channel].max_val) duty = gGPIO[channel].max_val;
    gGPIO[channel].duty = duty;
    if (EHS_RPI_IS_HWPWM(io_num)) pwmWrite(io_num, duty * PWM_CONSTANT_RANGE / gGPIO[channel].max_val);
    else softPwmWrite(io_num, duty);

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

    return INX_HW_PWM_PORT_EOK;
}
inx_hw_pwm_port_errcode_t EhsTPortPwmEnable(ehs_sint32 channel)
{
    if (!gGPIOInitialised) return INX_HW_PWM_PORT_ESETUP;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIO[channel].initialised) return INX_HW_PWM_PORT_ESETUP;
    ehs_uint32 io_num = gGPIO[channel].io_num;
    if (EHS_RPI_IS_HWPWM(io_num))
    {
        pwmWrite(io_num, gGPIO[channel].duty * PWM_CONSTANT_RANGE / gGPIO[channel].max_val);
    }
    else {
        softPwmWrite(io_num, gGPIO[channel].duty);
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
    if (EHS_RPI_IS_HWPWM(io_num))
    {
        pwmWrite(io_num, 0);
    }
    else {
        softPwmWrite(io_num, 0);
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
    *max_val = gGPIO[channel].max_val;

    return INX_HW_PWM_PORT_EOK;   
}

inx_hw_pwm_port_errcode_t EhsTPortPwmGetFreq(ehs_sint32 channel, ehs_sint32 *freq)
{
    if (!gGPIOInitialised) return INX_HW_PWM_PORT_ESETUP;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIO[channel].initialised) return INX_HW_PWM_PORT_ESETUP;
    ehs_uint32 io_num = gGPIO[channel].io_num;
    *freq = gGPIO[channel].freq;

    return INX_HW_PWM_PORT_EOK;
}
inx_hw_pwm_port_errcode_t EhsTPortPwmGetDuty(ehs_sint32 channel, ehs_sint32 *duty)
{
    if (!gGPIOInitialised) return INX_HW_PWM_PORT_ESETUP;
    if (channel < 0 || channel > EHS_RASPBERRYPI_PWM_GPIO_NUM_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    if (!gGPIO[channel].initialised) return INX_HW_PWM_PORT_ESETUP;
    ehs_uint32 io_num = gGPIO[channel].io_num;
    *duty = gGPIO[channel].duty;

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