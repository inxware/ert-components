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
