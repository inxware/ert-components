/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

#include "target_adcdac.h"
#include <stdio.h>
#include <time.h>
#ifdef USE_ESP32S3_LEGACY_API
#include "driver/adc.h"
#else
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_continuous.h"
#endif
#include "target.h"
#include "target_config.h"

#include "driver/sdm.h"

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#include "esp_err.h"
#include "esp_check.h"

ehs_adc_config_t g_ehs_adc_configs[EHS_TARGET_ADC_UNIT_NUMBER] = EHS_DEFAULT_ADC_CONFIG();

ehs_uint16 g_ehs_adc_continuous_enabled_bitmask[EHS_TARGET_ADC_UNIT_NUMBER] = EHS_TARGET_ADC_UNIT_DEFAULT(0);

static int32_t adc_conv_results[EHS_TARGET_ADC_CHANNEL_NUMBER] = EHS_TARGET_ADC_CHANNEL_DEFAULT(0);
static bool adc_conv_continuous[EHS_TARGET_ADC_CHANNEL_NUMBER] = EHS_TARGET_ADC_CHANNEL_DEFAULT(false);

static ehs_sint64 adc_conv_data_mean_accum[EHS_TARGET_ADC_UNIT_NUMBER][EHS_TARGET_ADC_CHANNEL_NUMBER] = EHS_TARGET_ADC_UNIT_DEFAULT(EHS_TARGET_ADC_CHANNEL_DEFAULT(0));
static ehs_sint64 adc_conv_data_variance_accum[EHS_TARGET_ADC_UNIT_NUMBER][EHS_TARGET_ADC_CHANNEL_NUMBER] = EHS_TARGET_ADC_UNIT_DEFAULT(EHS_TARGET_ADC_CHANNEL_DEFAULT(0));
static ehs_sint32 adc_conv_data_mean_result[EHS_TARGET_ADC_UNIT_NUMBER][EHS_TARGET_ADC_CHANNEL_NUMBER] = EHS_TARGET_ADC_UNIT_DEFAULT(EHS_TARGET_ADC_CHANNEL_DEFAULT(0));
static int32_t adc_conv_num_samples[EHS_TARGET_ADC_UNIT_NUMBER][EHS_TARGET_ADC_CHANNEL_NUMBER] = EHS_TARGET_ADC_UNIT_DEFAULT(EHS_TARGET_ADC_CHANNEL_DEFAULT(1));
static int64_t adc_conv_data_accum[EHS_TARGET_ADC_UNIT_NUMBER][EHS_TARGET_ADC_CHANNEL_NUMBER] = EHS_TARGET_ADC_UNIT_DEFAULT(EHS_TARGET_ADC_CHANNEL_DEFAULT(0));
static int32_t adc_conv_count[EHS_TARGET_ADC_UNIT_NUMBER][EHS_TARGET_ADC_CHANNEL_NUMBER] = EHS_TARGET_ADC_UNIT_DEFAULT(EHS_TARGET_ADC_CHANNEL_DEFAULT(0));
static int32_t adc_conv_bias[EHS_TARGET_ADC_UNIT_NUMBER][EHS_TARGET_ADC_CHANNEL_NUMBER] = EHS_TARGET_ADC_UNIT_DEFAULT(EHS_TARGET_ADC_CHANNEL_DEFAULT(0));

static DRAM_ATTR adc_cali_handle_t adc1_cali_chan_handle[EHS_TARGET_ADC_UNIT_NUMBER][EHS_TARGET_ADC_CHANNEL_NUMBER] = EHS_TARGET_ADC_UNIT_DEFAULT(EHS_TARGET_ADC_CHANNEL_DEFAULT(NULL));
static bool do_calibration_chan[EHS_TARGET_ADC_UNIT_NUMBER][EHS_TARGET_ADC_CHANNEL_NUMBER] = EHS_TARGET_ADC_UNIT_DEFAULT(EHS_TARGET_ADC_CHANNEL_DEFAULT(false));

static adc_oneshot_unit_handle_t adc1_oneshot_handle[EHS_TARGET_ADC_UNIT_NUMBER] = EHS_TARGET_ADC_UNIT_DEFAULT(NULL);
static adc_continuous_handle_t adc1_continuous_handle[EHS_TARGET_ADC_UNIT_NUMBER] = EHS_TARGET_ADC_CHANNEL_DEFAULT(NULL);

/***************************** ADC START *****************************/
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
#define USE_ESP32S3_ADC_CALIBRATION 1
// implementation for adc version from 5.x
// @todo - implement support for ADC2

#if USE_ESP32S3_ADC_CALIBRATION == 1
/*---------------------------------------------------------------
        ADC Calibration
---------------------------------------------------------------*/

/* ------------------------ Context Structure--------------------------- */
typedef struct adc_cali_scheme_t adc_cali_scheme_t;

/**
 * @brief ADC Calibration Scheme Interface and Context
 */
struct adc_cali_scheme_t {

    /**
     * @brief Convert ADC raw data to calibrated voltage
     *
     * @param[in]  arg        ///< ADC calibration scheme specific context
     * @param[in]  raw        ///< ADC raw data
     * @param[out] voltage    ///< Calibrated ADC voltage (in mV)
     *
     * @return
     *         - ESP_OK:                On success
     *         - ESP_ERR_INVALID_ARG:   Invalid argument
     *         - ESP_ERR_INVALID_STATE: Invalid state, scheme didn't registered
     */
    esp_err_t (*raw_to_voltage)(void *arg, int raw, int *voltage);

    /**
     * @brief ADC calibration specific contexts
     * Can be customized to difference calibration schemes
     */
    void *ctx;

};

typedef struct {
    uint32_t coeff_a;    ///< Gradient of ADC-Voltage curve
    uint32_t coeff_b;    ///< Offset of ADC-Voltage curve
} cali_chars_first_step_t;

typedef struct {
    uint8_t  term_num;                                        ///< Term number of the algorithm formula
    const uint64_t (*coeff)[2];    ///< Coeff of each term. See `adc_error_coef_atten` for details (and the magic number 2)
    const int32_t (*sign);         ///< Sign of each term
} cali_chars_second_step_t;

typedef struct {
    adc_unit_t unit_id;                            ///< ADC unit
    adc_channel_t chan;                            ///< ADC channel
    adc_atten_t atten;                             ///< ADC attenuation
    cali_chars_first_step_t chars_first_step;      ///< Calibration first step characteristics
    cali_chars_second_step_t chars_second_step;    ///< Calibration second step characteristics
} cali_chars_curve_fitting_t;

static int32_t IRAM_ATTR inx_get_reading_error(uint64_t v_cali_1, const cali_chars_second_step_t *param)
{
    if (v_cali_1 == 0 || param->term_num == 0) {
        return 0;
    }

    uint8_t term_num = param->term_num;
    int32_t error = 0;
    uint64_t coeff = 0;
    uint64_t variable[term_num];
    uint64_t term[term_num];

    for (int i = 0; i < term_num; i++) {
        variable[i] = 0;
        term[i] = 0;
    }

    /**
     *  The scheme formula is:
     *  error = (K0 * X^0) + (K1 * X^1)  + (K2 * X^2) + (K3 * X^3) + ... +  (Kn * X^n);
     */
    variable[0] = 1<<10;
    coeff = (param->coeff)[0][0] >> 10;
    term[0] = (variable[0] * coeff) / ((param->coeff)[0][1] >> 10);
    error = (int32_t)term[0] * (param->sign)[0];

    for (int i = 1; i < term_num; i++) {
        variable[i] = (variable[i - 1] * v_cali_1) >> 10;
        coeff = (param->coeff)[i][0] >> 10;
        term[i] = variable[i] * coeff;
        // use 'ets_printf' for logging in ISR
        //ESP_LOGV(TAG, "big coef is %llu, big term%d is %llu, coef_id is %d", coeff, i, term[i], i);

        term[i] = term[i] / ((param->coeff)[i][1] >> 10);
        error += (int32_t)term[i] * (param->sign)[i];
        //ESP_LOGV(TAG, "term%d is %llu, error is %"PRId32, i, term[i], error);
    }

    return error;
}

esp_err_t IRAM_ATTR inx_adc_cali_raw_to_voltage(adc_cali_handle_t handle, int raw, int *voltage)
{
    //pointers are checked in the upper layer
    ESP_RETURN_ON_FALSE_ISR(handle && voltage, ESP_ERR_INVALID_ARG, TAG, "invalid argument: null pointer");
    ESP_RETURN_ON_FALSE_ISR(handle->ctx, ESP_ERR_INVALID_STATE, TAG, "no calibration scheme, create a scheme first");

    cali_chars_curve_fitting_t *ctx = handle->ctx;

#if SOC_ADC_CALIB_CHAN_COMPENS_SUPPORTED
    // make sure this function is IRAM_ATTR !
    int chan_compensation = adc_get_hw_calibration_chan_compens(ctx->unit_id, ctx->chan, ctx->atten);
    raw -= chan_compensation;
    /* Limit the range */
    int max_val = (1L << SOC_ADC_RTC_MAX_BITWIDTH) - 1;
    raw = raw <= 0 ? 0 :
          raw > max_val ? max_val : raw;
#endif  // SOC_ADC_CALIB_CHAN_COMPENS_SUPPORTED

    // v_cali_1 is in fixed point precision (*1024 or <<10). raw is already in 16 bit
    uint64_t v_cali_1 = (((uint64_t)raw * ctx->chars_first_step.coeff_a) >> 6) + (ctx->chars_first_step.coeff_b << 10);
    int32_t error = inx_get_reading_error(v_cali_1, &(ctx->chars_second_step));

    *voltage = (int32_t)v_cali_1 - error;

    return ESP_OK;
}

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
static void configure_adc_one_shot(adc_oneshot_unit_handle_t* adc_handle, adc_channel_t channel, const adc_unit_t unit_id, 
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
    printf("channel %d bitwidth %d atten %d\n", channel, bitwidth, atten);
}

static ehs_float read_adc_one_shot(adc_oneshot_unit_handle_t* adc_handle, ehs_uint8 channel)
{
    int raw_value = 0;
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_read(*adc_handle, channel, &raw_value));
#if USE_ESP32S3_ADC_CALIBRATION == 1
    if (do_calibration_chan[0][channel]) {
        int voltage = 0;
        ESP_ERROR_CHECK_WITHOUT_ABORT(inx_adc_cali_raw_to_voltage(adc1_cali_chan_handle[0][channel], raw_value, &voltage));
        return (ehs_float)(voltage + EHS_PID_ADC_CALIBRATION_MAGIC_NUMBER);
    }else{
        return (ehs_float)raw_value;
    }
#else
    return (ehs_float)raw_value;
#endif // USE_ESP32S3_ADC_CALIBRATION
}

static const adc_channel_t DRAM_ATTR adc_channels[EHS_TARGET_ADC_CHANNEL_NUMBER] = {
    ADC_CHANNEL_0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_7,
    ADC_CHANNEL_8,
    ADC_CHANNEL_9
};
static adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0};
static bool g_b_adc_init = false;
static bool g_b_adc_configured[SOC_ADC_PATT_LEN_MAX] = {0};
static int g_i_adc_dig_pattern_num = 0;
static bool IRAM_ATTR s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t* edata, void* user_data);
/* continuous adc functions */
static void configure_adc_continuous(adc_continuous_handle_t* adc_handle, adc_continuous_callback_t on_conv_done, adc_channel_t channel,
                                     const adc_unit_t unit_id, const adc_bitwidth_t bitwidth, const uint8_t atten)
{
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = 256,//1024,
        .conv_frame_size = 64,//256,
    };
    if (*adc_handle == NULL) ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_new_handle(&adc_config, adc_handle));
    if (g_b_adc_init) ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_stop(*adc_handle));

    adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = 1000,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE2,
    };

    if (!g_b_adc_configured[channel]) g_i_adc_dig_pattern_num++;
    g_b_adc_configured[channel] = true;
    dig_cfg.pattern_num = g_i_adc_dig_pattern_num;
    // The sampling frequency is divided by the number of active channels
    dig_cfg.sample_freq_hz *= g_i_adc_dig_pattern_num;
    
    adc_pattern[channel].atten = atten;
    adc_pattern[channel].channel = adc_channels[channel] & 0x7;
    adc_pattern[channel].unit = unit_id;
    adc_pattern[channel].bit_width = bitwidth;

    adc_digi_pattern_config_t _adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0};
    int i = 0;
    int j = 0;

    for (; i < SOC_ADC_PATT_LEN_MAX ; i++)
    {
        if (!g_b_adc_configured[i]) continue;
        _adc_pattern[j] = adc_pattern[i];
        j++;
    }

    dig_cfg.adc_pattern = _adc_pattern;
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_config(*adc_handle, &dig_cfg));

    if (!g_b_adc_init)
    {
        adc_continuous_evt_cbs_t cbs = {
            .on_conv_done = on_conv_done,
        };
        ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_register_event_callbacks(*adc_handle, &cbs, NULL));
    }
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_start(*adc_handle));
    g_b_adc_init = true;
}
// @TODO - fix continuous option
static ehs_float EhsTGetAdcValue(ehs_uint8 channel)
{
    return sqrt((ehs_float)(adc_conv_results[channel]));
}
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
static TaskHandle_t s_task_handle;
static bool IRAM_ATTR s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t* edata, void* user_data)
{
    BaseType_t mustYield = pdFALSE;
    //Notify that ADC continuous driver has done enough number of conversions
    // vTaskNotifyGiveFromISR(s_task_handle, &mustYield);

    for (int i = 0 ; i < edata->size ; i += SOC_ADC_DIGI_RESULT_BYTES)
    {
        // Loop through all continuous data stored in a buffer
        adc_digi_output_data_t* p = (void *) &(edata->conv_frame_buffer[i]);
        uint32_t chan_num = p->type2.channel;
        if (chan_num >= EHS_TARGET_ADC_CHANNEL_NUMBER) continue;
        uint32_t data = p->type2.data;
        uint32_t cali_data = 0;
        if (adc1_cali_chan_handle[0][chan_num] != NULL)
        {
            inx_adc_cali_raw_to_voltage(adc1_cali_chan_handle[0][chan_num], data, &cali_data);
        }
        adc_conv_data_accum[0][chan_num] += ((((int32_t)cali_data - adc_conv_bias[0][chan_num]) / 1024) * (((int32_t)cali_data - adc_conv_bias[0][chan_num]) / 1024));
        adc_conv_count[0][chan_num] += 1;
        if (adc_conv_count[0][chan_num] == adc_conv_num_samples[0][chan_num])
        {
            adc_conv_results[chan_num] = adc_conv_data_accum[0][chan_num] / adc_conv_count[0][chan_num];
            adc_conv_data_accum[0][chan_num] = 0;
            adc_conv_count[0][chan_num] = 0;
        }
    }
    
    return (mustYield == pdTRUE);
}

#endif

// todo - what is the *config needed for in generic code?
EHS_GLOBAL ehs_bool configure_adc(ehs_uint8 channel, ehs_bool continuous, ehs_float f_s, ehs_sint32 num_samples, ehs_float bias, ehs_uint8 configure, ehs_uint8 *config)
{
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
    //s_task_handle = xTaskGetCurrentTaskHandle();
    //-------------ADC1 Config---------------//
    /*
     * ADC_ATTEN_DB_0   - 0dB attenuaton    (0 mV ~ 950 mV)
     * ADC_ATTEN_DB_2_5 - 2.5dB attenuation (0 mV ~ 1250 mV)
     * ADC_ATTEN_DB_6   - 6dB attenuation   (0 mV ~ 1750 mV)
     * ADC_ATTEN_DB_11  - 11dB attenuation  (0 mV ~ 3100 mV)
     */
    if (channel >= EHS_TARGET_ADC_CHANNEL_NUMBER) return EHS_FALSE;
    const uint8_t atten = ADC_ATTEN_DB_6;
    const adc_bitwidth_t bitwidth = ADC_BITWIDTH_12;
    adc_conv_num_samples[0][channel] = num_samples;
    adc_conv_bias[0][channel] = (int32_t)(bias * 1024);
    adc_conv_continuous[channel] = continuous;
    // if(continuous == EHS_FALSE){
    //     configure_adc_one_shot(&adc1_oneshot_handle[channel], adc_channels[channel], ADC_UNIT_1, ADC_BITWIDTH_12, ADC_ATTEN_DB_6);
    // }else{
    configure_adc_continuous(&adc1_continuous_handle[channel], s_conv_done_cb, adc_channels[channel], ADC_UNIT_1, ADC_BITWIDTH_12, ADC_ATTEN_DB_6);
    // }
#if USE_ESP32S3_ADC_CALIBRATION == 1
    //-------------ADC1 Calibration Init---------------//
    do_calibration_chan[0][channel] = adc_calibration_init(ADC_UNIT_1, channel, atten, &adc1_cali_chan_handle[0][channel]);
#endif // USE_ESP32S3_ADC_CALIBRATION

#endif
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool target_read_adc_sample(ehs_uint8 channel, ehs_float *value, ehs_uint8 config)
{
    if (channel >= EHS_TARGET_ADC_CHANNEL_NUMBER) return EHS_FALSE;
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
    // if (adc_conv_continuous[channel])
    // {
    *value = EhsTGetAdcValue(channel);
    // }
    // else
    // {
    //     if(adc1_oneshot_handle[channel] != NULL){
    //         *value = read_adc_one_shot(&adc1_oneshot_handle[channel], channel);
    //     }else{
    //         *value = 0.0f;
    //     }
    // }
#endif
    return EHS_TRUE;
}

ehs_bool destroy_adc(ehs_uint8 channel)
{
#ifdef USE_ESP32S3_LEGACY_API
    // not applying
#else
    if(adc1_oneshot_handle[channel] != NULL){
        ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_del_unit(adc1_oneshot_handle[channel]));
        adc1_oneshot_handle[channel] = NULL;
    }
    if(adc1_continuous_handle[channel] != NULL){
        ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_stop(adc1_continuous_handle[channel]));
        ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_deinit(adc1_continuous_handle[channel]));
        adc1_continuous_handle[channel] = NULL;
    }
#if USE_ESP32S3_ADC_CALIBRATION == 1
    if (adc1_cali_chan_handle[0][channel] != NULL) {
        adc_calibration_deinit(adc1_cali_chan_handle[0][channel]);
        adc1_cali_chan_handle[0][channel] = NULL;
    }
    do_calibration_chan[0][channel] = false;
#endif // USE_ESP32S3_ADC_CALIBRATION

#endif
    return EHS_TRUE;
}

static bool IRAM_ATTR ehs_s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
{
    BaseType_t mustYield = pdFALSE;
    //Notify that ADC continuous driver has done enough number of conversions
    // vTaskNotifyGiveFromISR(s_task_handle, &mustYield);

    for (int i = 0 ; i < edata->size ; i += SOC_ADC_DIGI_RESULT_BYTES)
    {
        // Loop through all continuous data stored in a buffer
        adc_digi_output_data_t* p = (void *) &(edata->conv_frame_buffer[i]);
        uint32_t unit = p->type2.unit;
        uint32_t chan_num = p->type2.channel;
        if (unit >= EHS_TARGET_ADC_UNIT_NUMBER) continue;
        if (chan_num >= EHS_TARGET_ADC_CHANNEL_NUMBER) continue;
        if (!EHS_IS_BIT_N_SET(g_ehs_adc_continuous_enabled_bitmask[unit], chan_num)) continue;
        uint32_t data = p->type2.data;
#if USE_ESP32S3_ADC_CALIBRATION == 1
        uint32_t cali_data = 0;
        if (adc1_cali_chan_handle[unit][chan_num] != NULL)
        {
            inx_adc_cali_raw_to_voltage(adc1_cali_chan_handle[unit][chan_num], data, &cali_data);
        }
        data = cali_data;
#endif // USE_ESP32S3_ADC_CALIBRATION
        uint32_t decimate_average = g_ehs_adc_configs[unit].channel_configs[chan_num].decimate_average >= g_ehs_adc_configs[unit].channel_configs[chan_num].decimation ? g_ehs_adc_configs[unit].channel_configs[chan_num].decimation : g_ehs_adc_configs[unit].channel_configs[chan_num].decimate_average;
        if (adc_conv_count[unit][chan_num]++ >= g_ehs_adc_configs[unit].channel_configs[chan_num].decimation - decimate_average)
        {
            adc_conv_data_accum[unit][chan_num] += (((int64_t)((int32_t)data - g_ehs_adc_configs[unit].channel_configs[chan_num].bias) / 1024) * (((int32_t)data - g_ehs_adc_configs[unit].channel_configs[chan_num].bias) / 1024));
            // TODO Double type-casting is used to temporarily unblock hardware testing
            adc_conv_data_mean_accum[unit][chan_num] += (int64_t)((int32_t)data);
            adc_conv_data_variance_accum[unit][chan_num] += (((int64_t)((int32_t)data -  adc_conv_data_mean_result[unit][chan_num]) / 1024) * (((int32_t)data - adc_conv_data_mean_result[unit][chan_num]) / 1024));
        }
        if (adc_conv_count[unit][chan_num] >= g_ehs_adc_configs[unit].channel_configs[chan_num].decimation)
        {
            adc_conv_data_mean_result[unit][chan_num] = adc_conv_data_mean_accum[unit][chan_num] / decimate_average;
            EhsTHAdcChannelContinuousConvertCB(unit, chan_num, (ehs_sint32)(adc_conv_data_mean_result[unit][chan_num]), (ehs_uint32)(adc_conv_data_variance_accum[unit][chan_num] / decimate_average), (ehs_uint32)(adc_conv_data_accum[unit][chan_num] / decimate_average));
            adc_conv_data_accum[unit][chan_num] = 0;
            adc_conv_data_mean_accum[unit][chan_num] = 0;
            adc_conv_data_variance_accum[unit][chan_num] = 0;
            adc_conv_count[unit][chan_num] = 0;
        }
    }

    // Clear the buffer by reading the ring buffer
    adc_continuous_clear_frame_in_ISR(handle, edata->size, &mustYield);

    return (mustYield == pdTRUE);
}

static void IRAM_ATTR ehs_s_pool_ovf_cb(void *arg) {
    //ets_printf("[%s] DMA pool overflow occured!\n", __func__);
}

ehs_bool EhsTAdcUnitConfigure(ehs_uint8 unit)
{
    // Total supported unit count is 2
    if (unit >= EHS_TARGET_ADC_UNIT_NUMBER) return EHS_FALSE;
    if (g_ehs_adc_configs[unit].unit_config.init == 1) return EHS_FALSE;
    switch (g_ehs_adc_configs[unit].unit_config.mode) {
        case 0: // Single-Shot
        {
            // Single Shot Configuration
            adc_oneshot_unit_init_cfg_t init_config = {
                .unit_id = unit
            };
            ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_new_unit(&init_config, &adc1_oneshot_handle[unit]));
            adc_oneshot_chan_cfg_t config = {
                .bitwidth = ADC_BITWIDTH_12,
                .atten = ADC_ATTEN_DB_6,
            };
            for (int i = 0 ; i < EHS_TARGET_ADC_CHANNEL_NUMBER ; i++)
            {
                if (g_ehs_adc_configs[unit].unit_config.channel[i] != -1)
                {
                    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_config_channel(adc1_oneshot_handle[unit], 0, &config));
                #if USE_ESP32S3_ADC_CALIBRATION == 1
                    //-------------ADC1 Calibration Init---------------//
                    do_calibration_chan[unit][i] = adc_calibration_init(unit, i, ADC_ATTEN_DB_6, &adc1_cali_chan_handle[unit][i]);
                #endif // USE_ESP32S3_ADC_CALIBRATION
                }
            }
            break;
        }
        case 1: // Continuous
        {
            // Continuous Configuration
            // ADC continuous mode on ADC unit 2 is not available
            if (unit == 1) return EHS_FALSE;
            int chan_count = 0;
            for (int i = 0 ; i < EHS_TARGET_ADC_CHANNEL_NUMBER ; i++)
            {
                if (g_ehs_adc_configs[unit].unit_config.channel[i] != -1) chan_count++;
            }
            adc_continuous_handle_cfg_t adc_config = {
                .max_store_buf_size = 1024,//1536 * chan_count,
                .conv_frame_size = 32
            };
            ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_new_handle(&adc_config, &adc1_continuous_handle[unit]));
            int adc_count = 0;
            adc_digi_pattern_config_t pattern_adc[SOC_ADC_PATT_LEN_MAX] = {0};
            for (int i = 0 ; i < EHS_TARGET_ADC_CHANNEL_NUMBER ; i++)
            {
                if (g_ehs_adc_configs[unit].unit_config.channel[i] != -1)
                {
                    pattern_adc[adc_count].atten = ADC_ATTEN_DB_6;
                    pattern_adc[adc_count].channel = adc_channels[i] & 0x7;
                    pattern_adc[adc_count].unit = unit;
                    pattern_adc[adc_count].bit_width = ADC_BITWIDTH_12;
                #if USE_ESP32S3_ADC_CALIBRATION == 1
                    //-------------ADC1 Calibration Init---------------//
                    do_calibration_chan[unit][i] = adc_calibration_init(unit, adc_channels[i], ADC_ATTEN_DB_6, &adc1_cali_chan_handle[unit][i]);
                #endif // USE_ESP32S3_ADC_CALIBRATION
                    adc_count++;
                }
            }
            // No ADC channel is enabled
            if (adc_count == 0) return EHS_FALSE;
            adc_continuous_config_t dig_cfg = {
                .sample_freq_hz = g_ehs_adc_configs[unit].unit_config.f_s_hz * adc_count,
                .conv_mode = ADC_CONV_SINGLE_UNIT_1,
                .format = ADC_DIGI_OUTPUT_FORMAT_TYPE2,
                .pattern_num = adc_count,
                .adc_pattern = pattern_adc,
            };
            ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_config(adc1_continuous_handle[unit], &dig_cfg));
            adc_continuous_evt_cbs_t cbs = {
                .on_conv_done = ehs_s_conv_done_cb,
                .on_pool_ovf = ehs_s_pool_ovf_cb
            };
            ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_register_event_callbacks(adc1_continuous_handle[unit], &cbs, NULL));
            ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_start(adc1_continuous_handle[unit]));
            break;
        }
        default:
            return EHS_FALSE;
    }
    g_ehs_adc_configs[unit].unit_config.init = 1;
    return EHS_TRUE;
}

ehs_uint32 EhsTAdcChannelSingleRead(ehs_uint8 unit, ehs_uint8 channel)
{
    // Total supported unit count is 2
    if (unit >= EHS_TARGET_ADC_UNIT_NUMBER) return EHS_FALSE;
    // This ADC unit is not initialised
    if (g_ehs_adc_configs[unit].unit_config.init == 0) return 0;
    // The mode of this ADC is not Single-shot
    if (g_ehs_adc_configs[unit].unit_config.mode != 0) return 0;
    // This ADC channel is not configured
    if (g_ehs_adc_configs[unit].unit_config.channel[channel] == -1) return 0;
    ehs_uint32 value = 0;
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_read(adc1_oneshot_handle[unit], channel, &value));
#if USE_ESP32S3_ADC_CALIBRATION == 1
    ehs_uint32 cali_value = 0;
    ESP_ERROR_CHECK_WITHOUT_ABORT(inx_adc_cali_raw_to_voltage(adc1_cali_chan_handle[unit][channel], value, &cali_value));
    return cali_value;
#endif // USE_ESP32S3_ADC_CALIBRATION
    return value;
}

ehs_bool EhsTAdcUnitDestroy(ehs_uint8 unit)
{
    // Total supported unit count is 2
    if (unit >= EHS_TARGET_ADC_UNIT_NUMBER) return EHS_FALSE;
    if (g_ehs_adc_configs[unit].unit_config.init == 0) return EHS_FALSE;
#if USE_ESP32S3_ADC_CALIBRATION == 1
    for (int i = 0 ; i < EHS_TARGET_ADC_CHANNEL_NUMBER ; i++)
    {
        if (g_ehs_adc_configs[unit].unit_config.channel[i] != -1)
        {
            ESP_ERROR_CHECK_WITHOUT_ABORT(adc_cali_delete_scheme_curve_fitting(adc1_cali_chan_handle[unit][i]));
            do_calibration_chan[unit][i] = false;
        }
    }
#endif // USE_ESP32S3_ADC_CALIBRATION
    switch (g_ehs_adc_configs[unit].unit_config.mode)
    {
        case 0:
        {
            // Single-shot conversion mode
            ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_del_unit(adc1_oneshot_handle[unit]));
            adc1_oneshot_handle[unit] = NULL;
            break;
        }
        case 1:
        {
            // Continuous conversion mode
            ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_stop(adc1_continuous_handle[unit]));
            ESP_ERROR_CHECK_WITHOUT_ABORT(adc_continuous_deinit(adc1_continuous_handle[unit]));
        }
        default:
            return EHS_FALSE;
    }
    return EHS_TRUE;
}

/***************************** ADC ENDED *****************************/

/***************************** DAC START *****************************/

static sdm_channel_handle_t dac_channel_handles[EHS_TARGET_DAC_NUMBER] = { NULL };
static ehs_sint32 dac_channel_max[EHS_TARGET_DAC_NUMBER] = { 0 };

ehs_bool EhsTDacConfigure(ehs_uint8 channel, ehs_sint32 max, ehs_sint32 sample_rate)
{
    if (channel >= EHS_TARGET_DAC_NUMBER) return EHS_FALSE;
    if (dac_channel_handles[channel] != NULL) return EHS_FALSE;
    if (max <= 0) return EHS_FALSE;
    sdm_config_t config = {
        .clk_src = SDM_CLK_SRC_DEFAULT,
        .gpio_num = channel,
        .sample_rate_hz = sample_rate
    };
    ESP_ERROR_CHECK_WITHOUT_ABORT(sdm_new_channel(&config, &(dac_channel_handles[channel])));
    ESP_ERROR_CHECK_WITHOUT_ABORT(sdm_channel_enable(&(dac_channel_handles[channel])));
    dac_channel_max[channel] = max;
    return EHS_TRUE;
}

ehs_bool EhsTDacSet(ehs_uint8 channel, ehs_sint32 value)
{
    if (channel >= EHS_TARGET_DAC_NUMBER) return EHS_FALSE;
    if (dac_channel_handles[channel] == NULL) return EHS_FALSE;
    ehs_sint32 actual_value = ((value > dac_channel_max[channel] ? dac_channel_max[channel] : value < 0 ? 0 : value) << 8) / dac_channel_max[channel] - 128;
    // printf("DAC set value for channel %d: %d\n", channel, actual_value);
    ESP_ERROR_CHECK_WITHOUT_ABORT(sdm_channel_set_pulse_density(dac_channel_handles[channel], actual_value));
    return EHS_TRUE;
}

ehs_bool EhsTDacDestroy(ehs_uint8 channel)
{
    if (channel >= EHS_TARGET_DAC_NUMBER) return EHS_FALSE;
    if (dac_channel_handles[channel] == NULL) return EHS_FALSE;
    ESP_ERROR_CHECK_WITHOUT_ABORT(sdm_del_channel(dac_channel_handles[channel]));
    dac_channel_handles[channel] = NULL;
    dac_channel_max[channel] = 0;
    return EHS_TRUE;
}

/***************************** DAC ENDED *****************************/
