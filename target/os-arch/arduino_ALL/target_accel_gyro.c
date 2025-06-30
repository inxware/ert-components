
#include "hal_accel_gyro.h"
//#include "hal_process.h"
#include <Arduino_LSM6DS3.h>


ehs_bool gIMU_Enabled = EHS_FALSE;

ehs_bool EnableAccelometerGyrosope()
{
    if(gIMU_Enabled == EHS_FALSE){
        //EhsTPMutex_lock(EhsTPMutex_socketClient);
        int ok = IMU.begin();
        //EhsTPMutex_unlock(EhsTPMutex_socketClient);
        if (!ok) {
            gIMU_Enabled = EHS_FALSE;
        }else{
            gIMU_Enabled = EHS_TRUE;
        }
    }
    return gIMU_Enabled;
}

void DisableAccelometerGyrosope()
{
    if(gIMU_Enabled == EHS_TRUE){
        //EhsTPMutex_lock(EhsTPMutex_socketClient);
        IMU.end();
        //EhsTPMutex_unlock(EhsTPMutex_socketClient);
        gIMU_Enabled = EHS_FALSE;
    }
}

ehs_bool EhsGetAccelometerValues(ehs_float* x, ehs_float* y, ehs_float* z)
{
    ehs_bool success = EHS_FALSE;
    if (gIMU_Enabled && x && y && z) {
        float Ax = 0, Ay = 0, Az = 0;
        //EhsTPMutex_lock(EhsTPMutex_socketClient);
        if(IMU.accelerationAvailable()){
            IMU.readAcceleration(Ax, Ay, Az);
            success = EHS_TRUE;
        }
        //EhsTPMutex_unlock(EhsTPMutex_socketClient);
        *x = (ehs_float)Ax; *y = (ehs_float)Ay; *z = (ehs_float)Az;
    }
    return success;
}

ehs_bool EhsGetGyrosopeValues(ehs_float* x, ehs_float* y, ehs_float* z)
{
    ehs_bool success = EHS_FALSE;
    if (gIMU_Enabled && x && y && z) {
        float Gx = 0, Gy = 0, Gz = 0;
        //EhsTPMutex_lock(EhsTPMutex_socketClient);
        if(IMU.gyroscopeAvailable()){
            IMU.readGyroscope(Gx, Gy, Gz);
            success = EHS_TRUE;
        }
        //EhsTPMutex_unlock(EhsTPMutex_socketClient);
        *x = (ehs_float)Gx; *y = (ehs_float)Gy; *z = (ehs_float)Gz;
    }
    return success;
}
