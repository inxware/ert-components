#include "hal_pid.h"
#include "pid/inx-PID_isr.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_continuous.h"

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_err.h"
#include "esp_check.h"

#include "target_adcdac.h"

#define ADC_GET_CHANNEL(p_data)             ((p_data)->type2.channel)
#define ADC_GET_DATA(p_data)                ((p_data)->type2.data)
#define EHS_PID_ADC_UNIT                    ADC_UNIT_1
#define EHS_PID_ADC_CONV_MODE               ADC_CONV_SINGLE_UNIT_1
#define EHS_PID_ADC_ATTEN                   ADC_ATTEN_DB_6
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

#ifndef ADC_PT100_CHANNEL1
#define ADC_PT100_CHANNEL1 2
#endif//ADC_PT100_CHANNEL1
#ifndef ADC_PT100_CHANNEL2
#define ADC_PT100_CHANNEL2 3
#endif//ADC_PT100_CHANNEL2
#ifndef ADC_10V
#define ADC_10V 1
#endif//ADC_10V
#ifndef ADC_20MA
#define ADC_20MA 0
#endif//ADC_20MA
#ifndef ADC_THERMOCOUPLE1
#define ADC_THERMOCOUPLE1 4
#endif//ADC_THERMOCOUPLE1
#ifndef ADC_THERMOCOUPLE2
#define ADC_THERMOCOUPLE2 5
#endif//ADC_THERMOCOUPLE2

typedef struct
{
    ehs_bool bSamplerCreated;
    ehs_pid_ctrl_type* pid_ctrl[EHS_PID_CTRL_MAX_COUNT];
    adc_continuous_handle_t adc_continuous_handle;
} ehs_pid_sampler_data;

static ehs_pid_sampler_data samplerData = {0};

static ehs_bool calibrated[EHS_PID_ADC_CHAN_COUNT] = { EHS_FALSE };
static adc_cali_handle_t cali_handle[EHS_PID_ADC_CHAN_COUNT] = { NULL };

static uint32_t gAdcPrevResult[EHS_PID_ADC_CHAN_COUNT] = { 1073741824 };

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
    }
    dig_cfg.adc_pattern = adc_pattern;

    for (int i = 0; i < channel_num; i++)
    {
        if (calibrated[i] == EHS_TRUE || cali_handle[i] != NULL) continue;
        adc_cali_curve_fitting_config_t cali_config = {
            .unit_id = EHS_PID_ADC_UNIT,
            .chan = channel[i] & 0x7,
            .atten = EHS_PID_ADC_ATTEN,
            .bitwidth = EHS_PID_ADC_BIT_WIDTH
        };
        err = adc_cali_create_scheme_curve_fitting(&cali_config, &cali_handle[i]);
        if (err == ESP_OK) calibrated[i] = EHS_TRUE;
    }

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

//static const int coeff_a_scaling = 65536;

// determine if availble sensor is connected
void inx_adc_check_sensor_state(int id, uint32_t data, adcRawConnected_t* pSensorConnected)
{
    bool _connected = true;
    switch (id)
    {
        case ADC_PT100_CHANNEL1:
        case ADC_PT100_CHANNEL2:
        {
            // When the ADC input is less than 50mV (<=-65C) or reaches the maximum
            if (data <= 0x71 || data == 0x0FFF) _connected = false;
            switch (id)
            {
                case ADC_PT100_CHANNEL1:
                    pSensorConnected->bPt100Channel1 = _connected;
                    break;
                case ADC_PT100_CHANNEL2:
                    pSensorConnected->bPt100Channel2 = _connected;
                    break;
            }
            break;
        }
        case ADC_THERMOCOUPLE1:
        case ADC_THERMOCOUPLE2:
        {
            if (data == 0x0FFF) _connected = false;
            switch (id)
            {
                case ADC_THERMOCOUPLE1:
                    pSensorConnected->bThermocouple1 = _connected;
                    break;
                case ADC_THERMOCOUPLE2:
                    pSensorConnected->bThermocouple2 = _connected;
                    break;
            }
            break;
        }
        case ADC_10V:
        {
            if (data <= 0x20) _connected = false;
            pSensorConnected->bZeroTenV = _connected;
            break;
        }
        case ADC_20MA:
        {
            if (data <= 690) _connected = false;
            pSensorConnected->bFourTwentyMa = _connected;
            break;
        }
        default:
        // unkown sensor type
            break;
    }
}

#define EHS_PID_ADC_IIR_COEFF_L ((uint32_t)1)
#define EHS_PID_ADC_IIR_COEFF_H ((uint32_t)1023)
#define EHS_PID_ADC_IIR_COEFF_TOTAL ((uint32_t)1024) // TODO Kamil this should be a dynamic parameter to change the filter bandwidth and response time

static bool IRAM_ATTR s_pid_ctrl_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t* edata, void* user_data)
{
    BaseType_t mustYield = pdFALSE;
    //Notify that ADC continuous driver has done enough number of conversions
    vTaskNotifyGiveFromISR(s_pid_ctrl_task_handle, &mustYield);
    adcRawConnected_t sensorConnected = {0};
    uint32_t pid_adc_raw[EHS_PID_ADC_CHAN_COUNT] = {0};
    uint32_t pid_adc_avr[EHS_PID_ADC_CHAN_COUNT] = {0};
    for (int i = 0; i < edata->size; i += SOC_ADC_DIGI_RESULT_BYTES) {
        // Loop through all continuous data stored in a buffer
        adc_digi_output_data_t* p = (void*)&(edata->conv_frame_buffer[i]);
        uint32_t chan_num = ADC_GET_CHANNEL(p);
        uint32_t data = ADC_GET_DATA(p);
        // TODO Kamil why not replacing this with the ADC-channel LUT?
        for (int j = 0; j < EHS_PID_ADC_CHAN_COUNT; j++) {
            if(pid_channel[j] == chan_num){
                // Loop through all the sensor channels and find the mapped channel for the current result
                if (cali_handle[j] != NULL)
                {
                    uint32_t cali_data;
                    inx_adc_cali_raw_to_voltage(cali_handle[j], data, &cali_data);
                    // Avoid the random starting value of the averaging
                    if (gAdcPrevResult[j] < (1U << 13))
                        cali_data = (EHS_PID_ADC_IIR_COEFF_L * cali_data + EHS_PID_ADC_IIR_COEFF_H * gAdcPrevResult[j]) / EHS_PID_ADC_IIR_COEFF_TOTAL;
                    gAdcPrevResult[j] = cali_data;
                    // determine if the sensor is connected
                    inx_adc_check_sensor_state(j, data, &sensorConnected); // pass in raw data needed for checking if the sensor is connected
                    // The magic number is added to handle the constant difference
                    //  between the calibration and actual measurements
                    // pid_adc_raw[j] += cali_data + EHS_PID_ADC_CALIBRATION_MAGIC_NUMBER;
                    pid_adc_raw[j] = cali_data + EHS_PID_ADC_CALIBRATION_MAGIC_NUMBER;
                    // adc_cali_raw_to_voltage(cali_handle[j], data, &cali_data);
                    // pid_adc_raw[j] += cali_data * 1024;
                }
                else pid_adc_raw[j] += data;
                pid_adc_avr[j]++;
                break;
            }
        }
    }
    // map adv values to the sensors
    // adcRawValues_t adc_raw = {
    //     (pid_adc_avr[ADC_PT100_CHANNEL1]) ? pid_adc_raw[ADC_PT100_CHANNEL1] / pid_adc_avr[ADC_PT100_CHANNEL1] : pid_adc_raw[ADC_PT100_CHANNEL1], // PT100_CHANNEL1
    //     (pid_adc_avr[ADC_PT100_CHANNEL2]) ? pid_adc_raw[ADC_PT100_CHANNEL2] / pid_adc_avr[ADC_PT100_CHANNEL2] : pid_adc_raw[ADC_PT100_CHANNEL2], // PT100_CHANNEL2
    //     0, // CPU TEMP (not used)
    //     (pid_adc_avr[ADC_10V]) ? pid_adc_raw[ADC_10V] / pid_adc_avr[ADC_10V] : pid_adc_raw[ADC_10V], // 10V
    //     (pid_adc_avr[ADC_20MA]) ? pid_adc_raw[ADC_20MA] / pid_adc_avr[ADC_20MA] : pid_adc_raw[ADC_20MA], // 20MA
    //     (pid_adc_avr[ADC_THERMOCOUPLE1]) ? pid_adc_raw[ADC_THERMOCOUPLE1] / pid_adc_avr[ADC_THERMOCOUPLE1] : pid_adc_raw[ADC_THERMOCOUPLE1],  // THERMOCOUPLE1
    //     (pid_adc_avr[ADC_THERMOCOUPLE2]) ? pid_adc_raw[ADC_THERMOCOUPLE2] / pid_adc_avr[ADC_THERMOCOUPLE2] : pid_adc_raw[ADC_THERMOCOUPLE2],  // THERMOCOUPLE2
    //     sensorConnected
    // };
    adcRawValues_t adc_raw = {
        pid_adc_raw[ADC_PT100_CHANNEL1], // PT100_CHANNEL1
        pid_adc_raw[ADC_PT100_CHANNEL2], // PT100_CHANNEL2
        0, // CPU TEMP (not used)
        pid_adc_raw[ADC_10V], // 10V
        pid_adc_raw[ADC_20MA], // 20MA
        pid_adc_raw[ADC_THERMOCOUPLE1],  // THERMOCOUPLE1
        pid_adc_raw[ADC_THERMOCOUPLE2],  // THERMOCOUPLE2
        sensorConnected
    };
    inx_run_adc_pid(&adc_raw);

    return (mustYield == pdTRUE);
}

EHS_GLOBAL ehs_bool EhsPIDCtrlInit(ehs_pid_ctrl_type* pid_ctrl)
{
    printf("EhsInitPIDCtrl\n");
    pid_ctrl->nError = PID_CTRL_NO_ERROR;
    if(samplerData.bSamplerCreated == EHS_FALSE) {

        inx_pid_ctrl_init();
        
        inx_pid_ctrl_gpio_init();

        samplerData.adc_continuous_handle = NULL;
        for(int i = 0; i < EHS_PID_CTRL_MAX_COUNT; i++){
            samplerData.pid_ctrl[i] = NULL;
        }

        samplerData.bSamplerCreated = EHS_TRUE;
        // check ISR_mode before creating adc_continuous handler
        if(pid_ctrl->ISR_mode == EHS_TRUE){
            s_pid_ctrl_task_handle = xTaskGetCurrentTaskHandle();
            ehs_bool ret = ehs_pid_configure_adc_continuous(&samplerData.adc_continuous_handle, s_pid_ctrl_conv_done_cb, pid_channel, 
                                                            sizeof(pid_channel) / sizeof(adc_channel_t), NULL);
            if(ret == EHS_FALSE){
                pid_ctrl->nError = PID_CTRL_ADC_INIT_FAILED;
                return EHS_FALSE;
            }
        }
    }
    // assign pid ctrl to the sampler struct
    ehs_sint32 pid_ctrl_index;
    if(!inx_get_pid_index(pid_ctrl, &pid_ctrl_index)){
        pid_ctrl->nError = PID_CTRL_BAD_PID_INDEX;
        return EHS_FALSE;
    }
    samplerData.pid_ctrl[pid_ctrl_index] = pid_ctrl;
    
    pid_ctrl->nError = inx_pid_ctrl_channel_init(pid_ctrl);
    return (pid_ctrl->nError == PID_CTRL_NO_ERROR) ? EHS_TRUE : EHS_FALSE;
}

EHS_GLOBAL ehs_bool EhsPIDCtrlDestroy(ehs_pid_ctrl_type* pid_ctrl)
{
    printf("EhsDestroyPIDCtrl\n");

    if(samplerData.bSamplerCreated == EHS_TRUE) {
        inx_pid_ctrl_gpio_destroy();
    }
    
    if(samplerData.adc_continuous_handle != NULL){
        ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_stop(samplerData.adc_continuous_handle));
        ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_deinit(samplerData.adc_continuous_handle));
        samplerData.adc_continuous_handle = NULL;
    }
    
    samplerData.bSamplerCreated = EHS_FALSE;

    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsPIDCtrlSetMeasuredValue(ehs_pid_ctrl_type* pid_ctrl)
{
    //printf("EhsSetMeasuredValuePIDCtrl\n");
    // @TODO - for now esp32s3 only supports ISR mode
    return EHS_FALSE;
}
