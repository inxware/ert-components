
#include "hal_accel_gyro.h"



ehs_bool EnableAccelometerGyrosope()
{
    return EHS_TRUE;
}

void DisableAccelometerGyrosope()
{

}

ehs_bool EhsGetAccelometerValues(ehs_float* x, ehs_float* y, ehs_float* z)
{
    if(x && y && z){
        *x = 0; *y = 0; *z = 0;
        return EHS_TRUE;
    }
    return EHS_FALSE;
}

ehs_bool EhsGetGyrosopeValues(ehs_float* x, ehs_float* y, ehs_float* z)
{
    if(x && y && z){
        *x = 0; *y = 0; *z = 0;
        return EHS_TRUE;
    }
    return EHS_FALSE;
}