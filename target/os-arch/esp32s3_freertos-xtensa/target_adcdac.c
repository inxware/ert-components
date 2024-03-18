/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

#include "target_adcdac.h"
#ifdef USE_ESP32S3_LEGACY_API
#include "driver/adc.h"
#else
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_continuous.h"
#endif
#include "target.h"
#include "target_config.h"

#ifdef USE_ESP32S3_LEGACY_API
/*
any globals needs for the esp ADC to work.
esp_specific_thing x;
*/
// static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1,
// GPIO14 if ADC2 static const adc_bits_width_t width = ADC_WIDTH_BIT_12; static
// const adc_atten_t atten = ADC_ATTEN_DB_0;

ehs_uint8 unit = 0;

static ehs_bool set_ADC_unit(ehs_uint8 value)
{
    switch (value)
    {
    case 1:
        unit = ADC_UNIT_1;
        break;
    case 2:
        unit = ADC_UNIT_2;
        break;
    case 3:
        unit = ADC_UNIT_BOTH;
        break;
    case 7:
        unit = ADC_UNIT_ALTER;
        break;
    default:
        unit = ADC_UNIT_1;
        break;
    }
    return EHS_TRUE;
}

static ehs_bool set_ADC_width(ehs_uint8 width)
{
    adc1_config_width(width);
    return EHS_TRUE;
}

static ehs_bool get_ADC_width(ehs_uint8 *width)
{
    *width = ADC_WIDTH_BIT_12;
    return EHS_TRUE;
}

static ehs_bool set_atten(ehs_uint8 channel, ehs_uint8 atten)
{
    adc1_config_channel_atten(channel, atten);
    return EHS_TRUE;
}

// For ADC1, configure desired precision and attenuation by calling functions
static ehs_bool configure_ADC1(ehs_uint8 channel)
{
    set_ADC_width(ADC_WIDTH_BIT_12);
    set_atten(channel, ADC_ATTEN_DB_0);
    return EHS_TRUE;
}

// Configure the ADC2 channel, including setting attenuation.
static ehs_bool configure_ADC2(ehs_uint8 channel)
{
    adc2_config_channel_atten(channel, ADC_ATTEN_DB_0);
    return EHS_TRUE;
}
#else
#define USE_ESP32S3_ADC_CALIBRATION 0
// implementation for adc version from 5.x
// @todo - implement support for ADC2
adc_oneshot_unit_handle_t adc1_oneshot_handle = NULL;
adc_continuous_handle_t adc1_continuous_handle = NULL;

#if USE_ESP32S3_ADC_CALIBRATION == 1
/*---------------------------------------------------------------
        ADC Calibration
---------------------------------------------------------------*/
adc_cali_handle_t adc1_cali_chan_handle = NULL;
bool do_calibration_chan = false;

static bool adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    if (!calibrated) {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Curve Fitting");
        adc_cali_curve_fitting_config_t cali_config = {
            .unit_id = unit,
            .chan = channel,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }
#elif ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    if (!calibrated) {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }
#endif

    *out_handle = handle;
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Calibration Success");
    } else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated) {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } else {
        ESP_LOGE(TAG, "Invalid arg or no memory");
    }

    return calibrated;
}

static void adc_calibration_deinit(adc_cali_handle_t handle)
{
#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    ESP_LOGI(TAG, "deregister %s calibration scheme", "Curve Fitting");
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_cali_delete_scheme_curve_fitting(handle));

#elif ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    ESP_LOGI(TAG, "deregister %s calibration scheme", "Line Fitting");
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_cali_delete_scheme_line_fitting(handle));
#endif
}
#endif // USE_ESP32S3_ADC_CALIBRATION

/* one shot adc functions */
static void configure_adc_one_shot(adc_oneshot_unit_handle_t* adc_handle, ehs_uint8 channel, const adc_unit_t unit_id, 
                            const adc_bitwidth_t bitwidth, const uint8_t atten)
{
    //-------------ADC Init---------------//
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = unit_id,
    };
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_new_unit(&init_config, adc_handle));
    adc_oneshot_chan_cfg_t chan_config = {
        .bitwidth = bitwidth,
        .atten = atten,
    };
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_config_channel(*adc_handle, channel, &chan_config));
}

static ehs_float read_adc_one_shot(adc_oneshot_unit_handle_t* adc_handle, ehs_uint8 channel)
{
    int raw_value = 0;
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_read(*adc_handle, channel, &raw_value));
#if USE_ESP32S3_ADC_CALIBRATION == 1
    if (do_calibration_chan) {
        int voltage = 0;
        ESP_ERROR_CHECK_WITHOUT_ABORT(adc_cali_raw_to_voltage(adc1_cali_chan_handle, raw_value, &voltage));
        return (ehs_float)voltage;
    }else{
        return (ehs_float)raw_value;
    }
#else
    return (ehs_float)raw_value;
#endif // USE_ESP32S3_ADC_CALIBRATION
}

/* continuous adc functions */
static void configure_adc_continuous(adc_continuous_handle_t* adc_handle, adc_continuous_callback_t on_conv_done, ehs_uint8 channel,
                                     const adc_unit_t unit_id, const adc_bitwidth_t bitwidth, const uint8_t atten)
{
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = 1024,
        .conv_frame_size = 256,
    };
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_new_handle(&adc_config, adc_handle));

    adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = 10 * 1000,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE2,
    };

    adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0};
    dig_cfg.pattern_num = 1;
    adc_pattern[0].atten = atten;
    adc_pattern[0].channel = channel & 0x7;
    adc_pattern[0].unit = unit_id;
    adc_pattern[0].bit_width = bitwidth;
    dig_cfg.adc_pattern = adc_pattern;
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_config(*adc_handle, &dig_cfg));

    adc_continuous_evt_cbs_t cbs = {
        .on_conv_done = on_conv_done,
    };
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_register_event_callbacks(*adc_handle, &cbs, NULL));
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_start(*adc_handle));
}
// @TODO - fix continuous option
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
uint32_t buff_size=0;
uint32_t buff_count=0;
uint32_t buff_val=0;
static TaskHandle_t s_task_handle;
static bool IRAM_ATTR s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t* edata, void* user_data)
{
    buff_size = edata->size;
    buff_count++;
    buff_val=*(edata->conv_frame_buffer);
    BaseType_t mustYield = pdFALSE;
    //Notify that ADC continuous driver has done enough number of conversions
    vTaskNotifyGiveFromISR(s_task_handle, &mustYield);
    
    return (mustYield == pdTRUE);
}

#endif

// todo - what is the *config needed for in generic code?
EHS_GLOBAL ehs_bool configure_adc(ehs_uint8 channel, ehs_uint8 configure, ehs_uint8 *config)
{
    ehs_bool is_continuous = EHS_FALSE;
#ifdef USE_ESP32S3_LEGACY_API
    set_ADC_unit(configure);
    if (unit == ADC_UNIT_1)
    {
        configure_ADC1(channel);
        *config = ADC1;
    }
    else
    {
        ehs_uint8 raw;
        configure_ADC2(channel);
        *config = ADC2;
    }
#else
    s_task_handle = xTaskGetCurrentTaskHandle();
    //-------------ADC1 Config---------------//
    /*
     * ADC_ATTEN_DB_0   - 0dB attenuaton    (0 mV ~ 950 mV)
     * ADC_ATTEN_DB_2_5 - 2.5dB attenuation (0 mV ~ 1250 mV)
     * ADC_ATTEN_DB_6   - 6dB attenuation   (0 mV ~ 1750 mV)
     * ADC_ATTEN_DB_11  - 11dB attenuation  (0 mV ~ 3100 mV)
     */
    const uint8_t atten = ADC_ATTEN_DB_11;
    const adc_bitwidth_t bitwidth = ADC_BITWIDTH_12;
    if(is_continuous == EHS_FALSE){
        configure_adc_one_shot(&adc1_oneshot_handle, channel, ADC_UNIT_1, bitwidth, atten);
    }else{
        configure_adc_continuous(&adc1_continuous_handle, s_conv_done_cb, channel, ADC_UNIT_1, bitwidth, atten);
    }
#if USE_ESP32S3_ADC_CALIBRATION == 1
    //-------------ADC1 Calibration Init---------------//
    do_calibration_chan = adc_calibration_init(unit_id, channel, atten, &adc1_cali_chan_handle);
#endif // USE_ESP32S3_ADC_CALIBRATION

#endif
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool target_read_adc_sample(ehs_uint8 channel, ehs_float *value, ehs_uint8 config)
{
#ifdef USE_ESP32S3_LEGACY_API
    /// todo2022 write the esp32 specific code to get a value and return and error
    /// status.
    ehs_uint8 width;
    // set_ADC_unit(unit);
    if (config == 1)
    {
        *value = adc1_get_raw(channel);
    }
    else
    {
        ehs_uint8 raw;
        get_ADC_width(&width);
        // Take an ADC2 reading on a single channel.
        adc2_get_raw(channel, width, &raw);
        *value = raw;
    }
#else
    if(adc1_oneshot_handle != NULL){
        *value = read_adc_one_shot(&adc1_oneshot_handle, channel);
    }else{
        *value = 0.0f;
    }
    printf("s=%ld c=%ld v=%ld\n", buff_size, buff_count, buff_val);
#endif
    return EHS_TRUE;
}

ehs_bool destroy_adc()
{
#ifdef USE_ESP32S3_LEGACY_API
    // not applying
#else
    if(adc1_oneshot_handle != NULL){
        ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_del_unit(adc1_oneshot_handle));
        adc1_oneshot_handle = NULL;
    }
    if(adc1_continuous_handle != NULL){
        ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_stop(adc1_continuous_handle));
        ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_deinit(adc1_continuous_handle));
        adc1_continuous_handle = NULL;
    }
#if USE_ESP32S3_ADC_CALIBRATION == 1
    if (adc1_cali_chan_handle != NULL) {
        adc_calibration_deinit(adc1_cali_chan_handle);
        adc1_cali_chan_handle = NULL;
    }
    do_calibration_chan = false;
#endif // USE_ESP32S3_ADC_CALIBRATION

#endif
    return EHS_TRUE;
}

