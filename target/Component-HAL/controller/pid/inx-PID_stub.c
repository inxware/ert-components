#include "hal_pid.h"
//#include "hal_logger.h"


EHS_GLOBAL ehs_bool EhsPIDCtrlInit(ehs_pid_ctrl_type* pid_ctrl)
{
    //printf("EhsPIDCtrlInit (stubbed) \n");
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsPIDCtrlDestroy(ehs_pid_ctrl_type* pid_ctrl)
{
    //printf("EhsPIDCtrlDestroy (stubbed) \n");
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsPIDCtrlConfigurePID(ehs_pid_ctrl_type* pid_ctrl)
{
    //printf("EhsPIDCtrlConfigurePID (stubbed) \n");
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsPIDCtrlConfigureIO(ehs_pid_ctrl_type* pid_ctrl)
{
    //printf("EhsPIDCtrlConfigureIO (stubbed) \n");
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsPIDCtrlSetSetpointValue(ehs_pid_ctrl_type* pid_ctrl)
{
    //printf("EhsPIDCtrlSetSetpointValue = %.2f (stubbed) \n", pid_ctrl->setpointValue);
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsPIDCtrlSetMeasuredValue(ehs_pid_ctrl_type* pid_ctrl)
{
    //printf("EhsPIDCtrlSetMeasuredValue = %.2f (stubbed) \n", pid_ctrl->measuredValue);
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsPIDCtrlUpdateOutputs(ehs_pid_ctrl_type* pid_ctrl)
{
    //printf("EhsPIDCtrlUpdateOutputs (stubbed) \n");
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsPIDCtrlDisable(ehs_pid_ctrl_type* pid_ctrl)
{
    //printf("EhsPIDCtrlDisable (stubbed) \n");
    return EHS_TRUE;
}

EHS_GLOBAL ehs_float EhsPIDCtrlGetSensorValue(const ehs_pid_ctrl_type* pid_ctrl, ehs_bool* isConnected)
{
    if(isConnected) *isConnected = EHS_TRUE;
    //printf("EhsPIDCtrlGetSensorValue (stubbed) \n");
    return 0;
}

EHS_GLOBAL ehs_sint32 EhsPIDCtrlCalibrate(ehs_sint32 id, ehs_bool bScale, float scale, ehs_bool bOffset, float offset)
{
    //printf("EhsPIDCtrlCalibrate id = %d (stubbed) \n", id);
    return 0;
}

EHS_GLOBAL ehs_bool EhsPIDCtrlConfigureRelays(ehs_pid_ctrl_relay_type_id id, ehs_sint32 idx, ehs_sint32 rel, ehs_float cyc_len)
{
    //printf("EhsPIDCtrlConfigureRelays id = %d, idx = %d (stubbed) \n", (ehs_sint32)id, idx);
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsPIDCtrlConfigureHardware(ehs_sint32 id, ehs_sint32 value, ehs_uint32* error_no)
{
    //printf("EhsPIDCtrlConfigureHardware id = %d, value = %d (stubbed) \n", id, value);
    return EHS_TRUE;
}