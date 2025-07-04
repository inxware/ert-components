/**
 * All the functions need to be ported to each platform target
 * file: inx_pwm_port.h
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 * 
 */

#ifndef _INX_PWM_PORT_H_ 
#define _INX_PWM_PORT_H_ 

#include "ehs_types.h"

typedef enum {
    INX_HW_PWM_PORT_EOK = 0,
    INX_HW_PWM_PORT_EINVALID_CHANNEL,
    INX_HW_PWM_PORT_EINVALID_IO_NUM,
    INX_HW_PWM_PORT_EINTERNAL_CLK,
    INX_HW_PWM_PORT_EINVALID_MAX_VAL,
    INX_HW_PWM_PORT_ESETUP,
    INX_HW_PWM_PORT_ESET_FREQ,
    INX_HW_PWM_PORT_ESET_DUTY,
    INX_HW_PWM_PORT_EENABLE,
    INX_HW_PWM_PORT_EDISABLE,
    INX_HW_PWM_PORT_EGET_FREQ,
    INX_HW_PWM_PORT_EGET_DUTY,
    INX_HW_PWM_PORT_EGENRIC,
    INX_HW_PWM_PORT_EMAX
} inx_hw_pwm_port_errcode_t;

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

#endif//_INX_PWM_PORT_H_ 