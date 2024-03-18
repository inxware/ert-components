#include "hal_pid.h"
//#include "hal_logger.h"


EHS_GLOBAL ehs_bool EhsInitPIDCtrl(ehs_pid_ctrl_type* pid_ctrl)
{
    printf("EhsInitPIDCtrl (stubbed) \n");
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsDestroyPIDCtrl(ehs_pid_ctrl_type* pid_ctrl)
{
    printf("EhsDestroyPIDCtrl (stubbed) \n");
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsConfigurePIDCtrl(ehs_pid_ctrl_type* pid_ctrl)
{
    printf("EhsConfigurePIDCtrl (stubbed) \n");
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsSetMeasuredValuePIDCtrl(ehs_pid_ctrl_type* pid_ctrl)
{
    printf("EhsSetMeasuredValuePIDCtrl = %d (stubbed) \n", pid_ctrl->nMeasuredValue);
    //EhsCallbackQueue_execute(pid_ctrl->pCallback);
    return EHS_TRUE;
}

EHS_GLOBAL ehs_sint32 EhsGetAdcValuePIDCtrl(const ehs_pid_ctrl_type* pid_ctrl)
{
    printf("EhsGetAdcValuePIDCtrl = %d (stubbed) \n");
    return 0;
}
