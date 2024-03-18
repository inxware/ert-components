#include "hal_pid.h"
#include "pid/inx-PID_heatrod.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_continuous.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define ADC_GET_CHANNEL(p_data)             ((p_data)->type2.channel)
#define ADC_GET_DATA(p_data)                ((p_data)->type2.data)
#define EHS_PID_ADC_UNIT                    ADC_UNIT_1
#define EHS_PID_ADC_CONV_MODE               ADC_CONV_SINGLE_UNIT_1
#define EHS_PID_ADC_ATTEN                   ADC_ATTEN_DB_11
#define EHS_PID_ADC_BIT_WIDTH               ADC_BITWIDTH_12
#define EHS_PID_ADC_FREQ_HZ                 1 * 1000 // [Hz]
#define EHS_PID_MAX_STORE_BUFF_SIZE         256
#define EHS_PID_CONV_FRAME_SIZE             64
#define EHS_PID_ADC_CHAN_COUNT              6
#define EHS_PID_ADC_FRAME_SIZE              (EHS_PID_CONV_FRAME_SIZE/SOC_ADC_DIGI_RESULT_BYTES)/EHS_PID_ADC_CHAN_COUNT // single adc channel frame size
#if EHS_PID_ADC_FRAME_SIZE == 0
    #error "EHS_PID_ADC_FRAME_SIZE must not be 0!"
#endif

static const char *TAG = "pid_ctrl";

static TaskHandle_t s_pid_ctrl_task_handle = NULL;

// adc PID channels
static adc_channel_t pid_channel[EHS_PID_ADC_CHAN_COUNT] = {
    ADC_CHANNEL_0, // 20MA
    ADC_CHANNEL_1, // 10V
    ADC_CHANNEL_2, // PT100_CHANNEL1
    ADC_CHANNEL_3, // PT100_CHANNEL2
    ADC_CHANNEL_4, // THERMOCOUPLE1
    ADC_CHANNEL_5, // THERMOCOUPLE2
};

#define ADC_PT100_CHANNEL1 2
#define ADC_PT100_CHANNEL2 3
#define ADC_10V 1
#define ADC_20MA 0
#define ADC_THERMOCOUPLE1 4
#define ADC_THERMOCOUPLE2 5

typedef struct
{
    ehs_bool bSamplerCreated;
    ehs_pid_ctrl_type* pid_ctrl[EHS_PID_CTRL_MAX_COUNT];
    adc_continuous_handle_t adc_continuous_handle;
} ehs_pid_sampler_data;

static ehs_pid_sampler_data samplerData = {0};


/* continuous adc functions */
static ehs_bool ehs_pid_configure_adc_continuous(adc_continuous_handle_t* adc_handle, adc_continuous_callback_t on_conv_done, 
                                                 adc_channel_t* channel, const uint8_t channel_num, void *user_data)
{
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = EHS_PID_MAX_STORE_BUFF_SIZE,
        .conv_frame_size = EHS_PID_CONV_FRAME_SIZE,
    };
    esp_err_t err = adc_continuous_new_handle(&adc_config, adc_handle);
    if(err != ESP_OK){
        ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        return EHS_FALSE;
    }
    adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = EHS_PID_ADC_FREQ_HZ,
        .conv_mode = EHS_PID_ADC_CONV_MODE,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE2,
    };
    adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0};
    dig_cfg.pattern_num = channel_num;
    for (int i = 0; i < channel_num; i++) {
        adc_pattern[i].atten = EHS_PID_ADC_ATTEN;
        adc_pattern[i].channel = channel[i] & 0x7;
        adc_pattern[i].unit = EHS_PID_ADC_UNIT;
        adc_pattern[i].bit_width = EHS_PID_ADC_BIT_WIDTH;

      //  ESP_LOGI(TAG, "adc_pattern[%d].atten is :%"PRIx8, i, adc_pattern[i].atten);
      //  ESP_LOGI(TAG, "adc_pattern[%d].channel is :%"PRIx8, i, adc_pattern[i].channel);
      //  ESP_LOGI(TAG, "adc_pattern[%d].unit is :%"PRIx8, i, adc_pattern[i].unit);
    }
    dig_cfg.adc_pattern = adc_pattern;

    err = adc_continuous_config(*adc_handle, &dig_cfg);
    if(err != ESP_OK){
        ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        return EHS_FALSE;
    }
    adc_continuous_evt_cbs_t cbs = {
        .on_conv_done = on_conv_done,
    };
    err = adc_continuous_register_event_callbacks(*adc_handle, &cbs, user_data);
    if(err != ESP_OK){
        ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        return EHS_FALSE;
    }
    err = adc_continuous_start(*adc_handle);
    if(err != ESP_OK){
        ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        return EHS_FALSE;
    }
    return EHS_TRUE;
}

static bool IRAM_ATTR s_pid_ctrl_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t* edata, void* user_data)
{
    BaseType_t mustYield = pdFALSE;
    //Notify that ADC continuous driver has done enough number of conversions
    vTaskNotifyGiveFromISR(s_pid_ctrl_task_handle, &mustYield);

    uint32_t pid_adc_raw[EHS_PID_ADC_CHAN_COUNT] = {0};
    uint32_t pid_adc_avr[EHS_PID_ADC_CHAN_COUNT] = {0};
    for (int i = 0; i < edata->size; i += SOC_ADC_DIGI_RESULT_BYTES) {
        adc_digi_output_data_t* p = (void*)&(edata->conv_frame_buffer[i]);
        uint32_t chan_num = ADC_GET_CHANNEL(p);
        uint32_t data = ADC_GET_DATA(p);
        for (int j = 0; j < EHS_PID_ADC_CHAN_COUNT; j++) {
            if(pid_channel[j] == chan_num){
                pid_adc_raw[j] += data;
                pid_adc_avr[j]++;
                break;
            }
        }
    }
    // map adv values to the sensors
    adcRawValues_t adc_raw = {
        (pid_adc_avr[ADC_PT100_CHANNEL1]) ? pid_adc_raw[ADC_PT100_CHANNEL1] / pid_adc_avr[ADC_PT100_CHANNEL1] : pid_adc_raw[ADC_PT100_CHANNEL1], // PT100_CHANNEL1
        (pid_adc_avr[ADC_PT100_CHANNEL2]) ? pid_adc_raw[ADC_PT100_CHANNEL2] / pid_adc_avr[ADC_PT100_CHANNEL2] : pid_adc_raw[ADC_PT100_CHANNEL2], // PT100_CHANNEL2
        0, // CPU TEMP (not used)
        (pid_adc_avr[ADC_10V]) ? pid_adc_raw[ADC_10V] / pid_adc_avr[ADC_10V] : pid_adc_raw[ADC_10V], // 10V
        (pid_adc_avr[ADC_20MA]) ? pid_adc_raw[ADC_20MA] / pid_adc_avr[ADC_20MA] : pid_adc_raw[ADC_20MA], // 20MA
        (pid_adc_avr[ADC_THERMOCOUPLE1]) ? pid_adc_raw[ADC_THERMOCOUPLE1] / pid_adc_avr[ADC_THERMOCOUPLE1] : pid_adc_raw[ADC_THERMOCOUPLE1]  // THERMOCOUPLE1
    };
    inx_run_adc_pid(&adc_raw);

    return (mustYield == pdTRUE);
}

static void duty_update_callback(int pin, int proc)
{
    ehs_sint32 pid_index, pid_out_index;
    if(inx_get_pid_output_index_for_pin(pin, &pid_index, &pid_out_index) == EHS_TRUE){
         ehs_pid_ctrl_type* ppid_ctrl = samplerData.pid_ctrl[pid_index];
        if(ppid_ctrl){
           // ets_printf("pin = %d , duty = %d \n", pin, proc);
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
          //  ets_printf("pin = %d , state = %d \n", pin, state);
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

EHS_GLOBAL ehs_bool EhsInitPIDCtrl(ehs_pid_ctrl_type* pid_ctrl)
{
    printf("EhsInitPIDCtrl\n");
    if(samplerData.bSamplerCreated == EHS_FALSE) {
        samplerData.adc_continuous_handle = NULL;
        for(int i = 0; i < EHS_PID_CTRL_MAX_COUNT; i++){
            samplerData.pid_ctrl[i] = NULL;
        }
        inx_set_duty_update_callback(duty_update_callback);
        inx_set_gpio_state_callback(gpio_state_callback);
        samplerData.bSamplerCreated = EHS_TRUE;
        // check ISR_mode before creating
        if(pid_ctrl->ISR_mode == EHS_TRUE){
            s_pid_ctrl_task_handle = xTaskGetCurrentTaskHandle();
            ehs_bool ret = ehs_pid_configure_adc_continuous(&samplerData.adc_continuous_handle, s_pid_ctrl_conv_done_cb, pid_channel, 
                                                            sizeof(pid_channel) / sizeof(adc_channel_t), NULL);
            if(ret == EHS_FALSE){
                pid_ctrl->nError = PID_CTRL_ADC_INIT_FAILED;
            }
        }
    }
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsDestroyPIDCtrl(ehs_pid_ctrl_type* pid_ctrl)
{
    printf("EhsDestroyPIDCtrl\n");
    inx_set_duty_update_callback(NULL);
    inx_set_gpio_state_callback(NULL);
    samplerData.bSamplerCreated = EHS_FALSE;
    if(samplerData.adc_continuous_handle != NULL){
        ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_stop(samplerData.adc_continuous_handle));
        ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_deinit(samplerData.adc_continuous_handle));
        samplerData.adc_continuous_handle = NULL;
    }
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsConfigurePIDCtrl(ehs_pid_ctrl_type* pid_ctrl)
{
    printf("EhsConfigurePIDCtrl\n");
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
    printf("EhsSetMeasuredValuePIDCtrl\n");
    ehs_sint32 pid_ctrl_index;
    if(!inx_get_pid_index(pid_ctrl, &pid_ctrl_index)){
        pid_ctrl->nError = PID_CTRL_BAD_PID_INDEX;
        return EHS_FALSE;
    }
    if(pid_ctrl->ISR_mode == EHS_FALSE){
        samplerData.pid_ctrl[pid_ctrl_index]->nMeasuredValue = pid_ctrl->nMeasuredValue;
    }
    samplerData.pid_ctrl[pid_ctrl_index]->nSetValue = pid_ctrl->nSetValue;
    inx_set_pid_value(samplerData.pid_ctrl[pid_ctrl_index]);

    return EHS_TRUE;
}

EHS_GLOBAL ehs_sint32 EhsGetAdcValuePIDCtrl(const ehs_pid_ctrl_type* pid_ctrl)
{
    ehs_sint32 val = 0;
    if(pid_ctrl->ISR_mode == EHS_FALSE){
		val = pid_ctrl->nMeasuredValue;
	}else{
        val = inx_get_adc_value(pid_ctrl);
    }
    return val;
}
