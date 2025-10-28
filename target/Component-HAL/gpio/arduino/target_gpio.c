/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * target_gpio.c
 */



#include "globals.h"
#include "target_gpio.h"
#include "hal_logger.h"

#include "inx_pwm_port.h"
#include <Arduino.h>

////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// GPIO OUTPUT ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

static ehs_bool set_value_GPIO(ehs_sint32 gpio, ehs_bool value, ehs_bool openDrain)
{
    if(value == EHS_TRUE){
        digitalWrite(gpio, HIGH);
    }else{
        digitalWrite(gpio, LOW); 
    }
    return EHS_TRUE;
}

static ehs_bool set_output_GPIO(ehs_sint32 gpio, ehs_bool initialValue, ehs_bool openDrain)
{
    pinMode(gpio, OUTPUT);
    set_value_GPIO(gpio, initialValue, openDrain);
    return EHS_TRUE;
}

ehs_bool EhsInitOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    return set_output_GPIO(pGPIO->pin_id, pGPIO->initial_state, pGPIO->open_drain_mode);
}

ehs_bool EhsWriteOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    return set_value_GPIO(pGPIO->pin_id, pGPIO->pin_value, pGPIO->open_drain_mode);
}

ehs_bool EhsDestroyOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    return EHS_TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// GPIO INPUT ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

static ehs_bool set_input_GPIO(ehs_sint32 gpio)
{
    pinMode(gpio, INPUT);
    return EHS_TRUE;
}

static ehs_bool get_value_GPIO(ehs_sint32 gpio, ehs_bool *value)
{
    if (value == NULL)
    {
        return EHS_FALSE;
    }
    *value = (digitalRead(gpio) == HIGH) ? EHS_TRUE : EHS_FALSE;
    return EHS_TRUE;
}

ehs_bool EhsInitInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    return set_input_GPIO(pGPIO->pin_id);
}

ehs_bool EhsReadInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    return get_value_GPIO(pGPIO->pin_id, &pGPIO->pin_value);
}

ehs_bool EhsDestroyInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    return EHS_TRUE;
}


inx_hw_pwm_port_errcode_t EhsTPortPwmSetup(ehs_sint32 channel, ehs_sint32 io_num, ehs_sint32 freq, ehs_sint32 max_val){
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}
inx_hw_pwm_port_errcode_t EhsTPortPwmDeinit(ehs_sint32 channel){
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}
inx_hw_pwm_port_errcode_t EhsTPortPwmSetFreq(ehs_sint32 channel, ehs_sint32 freq){
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}
inx_hw_pwm_port_errcode_t EhsTPortPwmSetDuty(ehs_sint32 channel, ehs_uint32 duty){
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}
inx_hw_pwm_port_errcode_t EhsTPortPwmSetMaxValue(ehs_sint32 channel, ehs_sint32 freq, ehs_sint32 max_val){
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}
inx_hw_pwm_port_errcode_t EhsTPortPwmEnable(ehs_sint32 channel){
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}
inx_hw_pwm_port_errcode_t EhsTPortPwmDisable(ehs_sint32 channel){
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmGetMaxValue(ehs_sint32 channel, ehs_sint32 *max_val){
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmGetFreq(ehs_sint32 channel, ehs_sint32 *freq){
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}
inx_hw_pwm_port_errcode_t EhsTPortPwmGetDuty(ehs_sint32 channel, ehs_sint32 *duty){
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}
inx_hw_pwm_port_errcode_t EhsTPortPwmEnabled(ehs_sint32 channel, ehs_bool *enabled){
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}