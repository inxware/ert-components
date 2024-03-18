#include "hal_pid.h"
#include <pthread.h>
//#include "hal_logger.h"
#include "inx-PID_heatrod.h"

#define EHS_PID_SAMPLE_SLEEP 100000

static void* Ehs_pid_sampler(void* pData);

typedef struct
{
    pthread_t samplerThreadId;
    ehs_bool bSamplerThreadCreated;
    ehs_bool bSamplerThreadRunning;
    adcRawValues_t adc_raw;
    ehs_pid_ctrl_type* pid_ctrl[EHS_PID_CTRL_MAX_COUNT];
} ehs_pid_sampler_data;

static ehs_pid_sampler_data samplerData = {0};

EHS_GLOBAL ehs_bool EhsInitPIDCtrl(ehs_pid_ctrl_type* pid_ctrl)
{
    if(samplerData.bSamplerThreadCreated == EHS_FALSE) {
        for(int i = 0; i < EHS_PID_CTRL_MAX_COUNT; i++){
            samplerData.pid_ctrl[i] = NULL;
        }
        memset(&samplerData.samplerThreadId, 0, sizeof(pthread_t));
        samplerData.bSamplerThreadCreated = EHS_TRUE;
        samplerData.bSamplerThreadRunning = EHS_FALSE;
        pthread_create(&samplerData.samplerThreadId, NULL, Ehs_pid_sampler, (void*)&samplerData);
    }
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsDestroyPIDCtrl(ehs_pid_ctrl_type* pid_ctrl)
{
    if(samplerData.bSamplerThreadCreated == EHS_TRUE){
        samplerData.bSamplerThreadRunning = EHS_FALSE;
        pthread_join(samplerData.samplerThreadId, NULL);
        samplerData.bSamplerThreadCreated = EHS_FALSE;
    }
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsConfigurePIDCtrl(ehs_pid_ctrl_type* pid_ctrl)
{
    ehs_sint32 pid_ctrl_index;
    if(!inx_get_pid_index(pid_ctrl, &pid_ctrl_index)){
        pid_ctrl->nError = PID_CTRL_BAD_PID_INDEX;
        return EHS_FALSE;
    }
    samplerData.pid_ctrl[pid_ctrl_index] = pid_ctrl;
    printf("PID = [%f %f %f] \n", samplerData.pid_ctrl[pid_ctrl_index]->P, samplerData.pid_ctrl[pid_ctrl_index]->I, samplerData.pid_ctrl[pid_ctrl_index]->D);
    inx_set_pid_config(samplerData.pid_ctrl[pid_ctrl_index]);
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsSetMeasuredValuePIDCtrl(ehs_pid_ctrl_type* pid_ctrl)
{
    ehs_sint32 pid_ctrl_index;
    if(!inx_get_pid_index(pid_ctrl, &pid_ctrl_index)){
        pid_ctrl->nError = PID_CTRL_BAD_PID_INDEX;
        return EHS_FALSE;
    }
    samplerData.pid_ctrl[pid_ctrl_index]->nMeasuredValue = pid_ctrl->nMeasuredValue;
    samplerData.pid_ctrl[pid_ctrl_index]->nSetValue = pid_ctrl->nSetValue;
    if(samplerData.pid_ctrl[pid_ctrl_index]->nChannelADC==INX_CONTROLLER_SENSORS_PT100_CHANNEL1){
        samplerData.adc_raw.pt100Channel1 = pid_ctrl->nMeasuredValue;
    }
    else if(samplerData.pid_ctrl[pid_ctrl_index]->nChannelADC==INX_CONTROLLER_SENSORS_PT100_CHANNEL2){
        samplerData.adc_raw.pt100Channel2 = pid_ctrl->nMeasuredValue;
    }
    else if(samplerData.pid_ctrl[pid_ctrl_index]->nChannelADC==INX_CONTROLLER_SENSORS_10V){
        samplerData.adc_raw.zeroTenV = pid_ctrl->nMeasuredValue;
    }
    else if(samplerData.pid_ctrl[pid_ctrl_index]->nChannelADC==INX_CONTROLLER_SENSORS_20MA){
        samplerData.adc_raw.fourTwentyMa = pid_ctrl->nMeasuredValue;
    }
    else if(samplerData.pid_ctrl[pid_ctrl_index]->nChannelADC==INX_CONTROLLER_SENSORS_THERMOCOUPLE){
        samplerData.adc_raw.thermocouple1 = pid_ctrl->nMeasuredValue;
    }
    inx_set_pid_value(samplerData.pid_ctrl[pid_ctrl_index]);
    return EHS_TRUE;
}

EHS_GLOBAL ehs_sint32 EhsGetAdcValuePIDCtrl(const ehs_pid_ctrl_type* pid_ctrl)
{
    if(pid_ctrl->ISR_mode == EHS_FALSE){
		return pid_ctrl->nMeasuredValue;
	}else{
        // todo - ISR_mode is not implemented for gnu atm
    }
    return 0;
}

static void duty_update_callback(int pin, int proc)
{
    ehs_sint32 pid_index, pid_out_index;
    if(inx_get_pid_output_index_for_pin(pin, &pid_index, &pid_out_index) == EHS_TRUE){
         ehs_pid_ctrl_type* ppid_ctrl = samplerData.pid_ctrl[pid_index];
        if(ppid_ctrl){
            printf("pin = %d , duty = %d \n", pin, proc);
            ppid_ctrl->nOutputPercent = proc;
            EhsCallbackQueue_execute(ppid_ctrl->pCallback);
        }
    }
}

static void gpio_state_callback(int pin, int state)
{
    ehs_sint32 pid_index, pid_out_index;
    if(inx_get_pid_output_index_for_pin(pin, &pid_index, &pid_out_index) == EHS_TRUE){
        ehs_pid_ctrl_type* ppid_ctrl = samplerData.pid_ctrl[pid_index];
        if(ppid_ctrl){
            printf("pin = %d , state = %d \n", pin, state);
            switch (pid_out_index)
            {
                case 0:
                    ppid_ctrl->bPWMOut1 = (state == 1) ? EHS_TRUE : EHS_FALSE;
                    break;
                case 1:
                    ppid_ctrl->bPWMOut2 = (state == 1) ? EHS_TRUE : EHS_FALSE;
                    break;
                case 2:
                    ppid_ctrl->bPWMOut3 = (state == 1) ? EHS_TRUE : EHS_FALSE;
                    break;
                default:
                    return;
            }
            EhsCallbackQueue_execute(ppid_ctrl->pCallback);
        }
    }
}

static void* Ehs_pid_sampler(void* pData)
{
    ehs_pid_sampler_data* pThreadData = (ehs_pid_sampler_data*)pData;
    if(pThreadData != NULL){
        pThreadData->bSamplerThreadRunning = EHS_TRUE;
        inx_set_duty_update_callback(duty_update_callback);
        inx_set_gpio_state_callback(gpio_state_callback);
        while(pThreadData->bSamplerThreadRunning == EHS_TRUE){
            
            inx_run_adc_pid(&pThreadData->adc_raw);

            EhsSleepUs(EHS_PID_SAMPLE_SLEEP);
        }
        inx_set_duty_update_callback(NULL);
        inx_set_gpio_state_callback(NULL);
    }
    return NULL;
}
