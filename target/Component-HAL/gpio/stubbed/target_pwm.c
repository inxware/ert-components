//#include "hal_pwm_legacy.h" - why do we have this???
#include "hal_pwm.h"

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


#if 0 
// these are target functions that should be stubbed in the stubbed implementation (we don't need to stub them here)
ehs_sint32 EhsTPwmSetup(ehs_sint32 channel, ehs_sint32 io_num, ehs_sint32 freq, ehs_sint32 max_val, ehs_bool hw, inx_hw_pwm_t **obj) { return 0; }
ehs_sint32 EhsTPwmDeinit(inx_hw_pwm_t *obj) { return 0; }
ehs_sint32 EhsTPwmSetFreq(inx_hw_pwm_t *obj, ehs_sint32 freq) { return 0; }
ehs_sint32 EhsTPwmSetDuty(inx_hw_pwm_t *obj, ehs_uint32 duty) { return 0; }
ehs_sint32 EhsTPwmSetMaxRes(inx_hw_pwm_t *obj, ehs_uint32 duty) { return 0; }
ehs_sint32 EhsTPwmEnable( inx_hw_pwm_t *obj ) { return 0; }
ehs_sint32 EhsTPwmDisable( inx_hw_pwm_t *obj ) { return 0; }
ehs_sint32 EhsTPwmSetStatus(inx_hw_pwm_t *obj, inx_hw_pwm_status_t status) { return 0; }
ehs_sint32 EhsTPwmGetStatus(inx_hw_pwm_t *obj, inx_hw_pwm_status_t *pStatus) { return 0; }
#endif