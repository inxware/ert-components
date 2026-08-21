//#include "hal_pwm_legacy.h" - why do we have this???
#include "hal_pwm.h"
#include "inx_pwm_port.h"

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

/* Target port PWM API stubs — required by inx_pwm.c.
 * The gui backend simulates GPIO on-screen; there is no PWM hardware to
 * drive, so these report EHS_HW_PWM_PORT_ENOT_SUPPORTED like the stubbed
 * backend rather than pretending a channel was set up.
 * TODO: simulate duty/freq on-screen like the rest of the gui GPIO backend,
 * instead of just reporting not-supported. */

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
