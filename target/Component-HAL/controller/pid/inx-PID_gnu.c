#include "hal_pid.h"
//#include "hal_logger.h"
#include "hal_string.h"
#include "inx-PID_isr.h"

/* This is probably some kind of PID emulator for desktop ??? */


typedef struct
{
    ehs_uint16 created;
    ehs_pid_ctrl_type* pid_ctrl[EHS_PID_CTRL_MAX_COUNT];
} ehs_pid_ctrls;

static ehs_pid_ctrls pid_ctrls = {0};
static adcRawValues_t adc_raw = {0};

EHS_GLOBAL ehs_bool EhsPIDCtrlInit(ehs_pid_ctrl_type* pid_ctrl)
{
    pid_ctrl->nError = PID_CTRL_NO_ERROR;

    if(pid_ctrls.created == 0){

        inx_pid_ctrl_init();
        
        inx_pid_ctrl_gpio_init();

        pid_ctrls.created = 1;
        EhsMemset(&adc_raw, 0, sizeof(adcRawValues_t));
        EhsMemset(&(adc_raw.connected), true, sizeof(adcRawConnected_t));
    }
    // assign pid ctrl
    ehs_sint32 pid_ctrl_index;
    if(!inx_get_pid_index(pid_ctrl, &pid_ctrl_index)){
        pid_ctrl->nError = PID_CTRL_BAD_PID_INDEX;
        return EHS_FALSE;
    }
    pid_ctrls.pid_ctrl[pid_ctrl_index] = pid_ctrl;
    pid_ctrl->nError = inx_pid_ctrl_channel_init(pid_ctrl);
    return (pid_ctrl->nError == PID_CTRL_NO_ERROR) ? EHS_TRUE : EHS_FALSE;
}

EHS_GLOBAL ehs_bool EhsPIDCtrlDestroy(ehs_pid_ctrl_type* pid_ctrl)
{
    if(pid_ctrls.created == 1){
        inx_pid_ctrl_gpio_destroy();
    }
    pid_ctrls.created = 0;
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsPIDCtrlSetMeasuredValue(ehs_pid_ctrl_type* pid_ctrl)
{
    pid_ctrl->nError = PID_CTRL_NO_ERROR;
    if(pid_ctrl->nSensorID==INX_CONTROLLER_SENSORS_PT100_CHANNEL1){
        adc_raw.pt100Channel1 = pid_ctrl->measuredValue;
    }else if(pid_ctrl->nSensorID==INX_CONTROLLER_SENSORS_PT100_CHANNEL2){
        adc_raw.pt100Channel2 = pid_ctrl->measuredValue;
    }else if(pid_ctrl->nSensorID==INX_CONTROLLER_SENSORS_10V){
        adc_raw.zeroTenV = pid_ctrl->measuredValue;
    }else if(pid_ctrl->nSensorID==INX_CONTROLLER_SENSORS_20MA){
        adc_raw.fourTwentyMa = pid_ctrl->measuredValue;
    }else if(pid_ctrl->nSensorID==INX_CONTROLLER_SENSORS_THERMOCOUPLE_CHANNEL1){
        adc_raw.thermocouple1 = pid_ctrl->measuredValue;
    }else if(pid_ctrl->nSensorID==INX_CONTROLLER_SENSORS_THERMOCOUPLE_CHANNEL2){
        adc_raw.thermocouple2 = pid_ctrl->measuredValue;
    }else{
        pid_ctrl->nError = PID_CTRL_INVALID_CTRL_SENSOR_ID;
    }
    if(pid_ctrl->nError == PID_CTRL_NO_ERROR){
        inx_run_adc_pid(&adc_raw);
        return EHS_TRUE;
    }
    return EHS_FALSE;
}
