/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

#include "hal_pwm.h"
#include "inx_pwm_port.h"

/* Legacy high-level PWM API stubs */

ehs_bool EhsPWMConfig(const ehs_uint8 pin, const ehs_uint32 periodHz)
{
    return EHS_TRUE;
}

ehs_bool EhsPWMEnable(const ehs_uint8 pin, const ehs_bool enable)
{
    return EHS_TRUE;
}

ehs_bool EhsPWMDuty(const ehs_uint8 pin, const ehs_uint8 powerPercent)
{
    return EHS_TRUE;
}

/* Target port PWM API stubs — required by inx_pwm.c */

inx_hw_pwm_port_errcode_t EhsTPortPwmSetup(ehs_sint32 channel, ehs_sint32 io_num, ehs_sint32 freq, ehs_sint32 max_val)
{
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmDeinit(ehs_sint32 channel)
{
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmSetFreq(ehs_sint32 channel, ehs_sint32 freq)
{
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmSetDuty(ehs_sint32 channel, ehs_uint32 duty)
{
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmSetMaxValue(ehs_sint32 channel, ehs_sint32 freq, ehs_sint32 max_val)
{
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmEnable(ehs_sint32 channel)
{
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmDisable(ehs_sint32 channel)
{
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmGetMaxValue(ehs_sint32 channel, ehs_sint32 *max_val)
{
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmGetFreq(ehs_sint32 channel, ehs_sint32 *freq)
{
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmGetDuty(ehs_sint32 channel, ehs_sint32 *duty)
{
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmEnabled(ehs_sint32 channel, ehs_bool *enabled)
{
    return EHS_HW_PWM_PORT_ENOT_SUPPORTED;
}
