#include "inx_pwm_port.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "soc/clk_tree_defs.h"
#include "esp_clk_tree.h"
#include "esp_err.h"
#include "soc/soc_caps.h"

/* inx limited (c) 2020 
  hardware PWM implementation for ESP32 S3 (May work for ESP32 too (TBC)?)
*/

#define LEDC_LL_FRACTIONAL_BITS    (8)
#define LEDC_LL_FRACTIONAL_MAX     ((1 << LEDC_LL_FRACTIONAL_BITS) - 1)

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#define LEDC_TIMER_DIV_NUM_MAX    (0x3FFFF)
#define LEDC_IS_DIV_INVALID(div)  ((div) <= LEDC_LL_FRACTIONAL_MAX || (div) > LEDC_TIMER_DIV_NUM_MAX)

static unsigned long upper_power_of_two(unsigned long v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

unsigned int log2_for_power2_only(unsigned int v){
    int i;
    static const unsigned int log2_b[] = {0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0, 
                                    0xFF00FF00, 0xFFFF0000};
    unsigned int r = (v & log2_b[0]) != 0;
    for (i = 4; i > 0; i--) // unroll for speed...
    {
    r |= ((v & log2_b[i]) != 0) << i;
    }
    return r;
}

static inline uint32_t ledc_calculate_divisor(uint32_t src_clk_freq, int freq_hz, uint32_t precision)
{
    /**
     * In order to find the right divisor, we need to divide the source clock
     * frequency by the desired frequency. However, two things to note here:
     * - The lowest LEDC_LL_FRACTIONAL_BITS bits of the result are the FRACTIONAL
     *   part. The higher bits represent the integer part, this is why we need
     *   to right shift the source frequency.
     * - The `precision` parameter represents the granularity of the clock. It
     *   **must** be a power of 2. It means that the resulted divisor is
     *   a multiplier of `precision`.
     *
     * Let's take a concrete example, we need to generate a 5KHz clock out of
     * a 80MHz clock (APB).
     * If the precision is 1024 (10 bits), the resulted multiplier is:
     * (80000000 << 8) / (5000 * 1024) = 4000 (0xfa0)
     * Let's ignore the fractional part to simplify the explanation, so we get
     * a result of 15 (0xf).
     * This can be interpreted as: every 15 "precision" ticks, the resulted
     * clock will go high, where one precision tick is made out of 1024 source
     * clock ticks.
     * Thus, every `15 * 1024` source clock ticks, the resulted clock will go
     * high.
     *
     * NOTE: We are also going to round up the value when necessary, thanks to:
     * (freq_hz * precision / 2)
     */
    return ( ( (uint64_t) src_clk_freq << LEDC_LL_FRACTIONAL_BITS ) + freq_hz * precision / 2 )
           / (freq_hz * precision);
}
static inline uint32_t ilog2(uint32_t i)
{
    // assert(i > 0);
    uint32_t log = 0;
    while (i >>= 1) {
        ++log;
    }
    return log;
}

// https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf#ledpwm
uint32_t ledc_find_suitable_duty_resolution(uint32_t src_clk_freq, uint32_t timer_freq)
{
    // Highest resolution is calculated when LEDC_CLK_DIV = 1 (i.e. div_param = 1 << LEDC_LL_FRACTIONAL_BITS)
    uint32_t div = (src_clk_freq + timer_freq / 2) / timer_freq; // rounded
    uint32_t duty_resolution = MIN(ilog2(div), SOC_LEDC_TIMER_BIT_WIDTH);
    uint32_t div_param = ledc_calculate_divisor(src_clk_freq, timer_freq, 1 << duty_resolution);
    if (LEDC_IS_DIV_INVALID(div_param)) {
        div = src_clk_freq / timer_freq; // truncated
        duty_resolution = MIN(ilog2(div), SOC_LEDC_TIMER_BIT_WIDTH);
        div_param = ledc_calculate_divisor(src_clk_freq, timer_freq, 1 << duty_resolution);
        if (LEDC_IS_DIV_INVALID(div_param)) {
            duty_resolution = 0;
        }
    }
    return duty_resolution;
}

static int gLedcEnabled[LEDC_TIMER_MAX] = { 0 };
static int gPwmMaxVal[LEDC_TIMER_MAX] = { 0 };
static ledc_timer_config_t gLedcTimer[LEDC_TIMER_MAX] = { 0 };

static ehs_sint32 pwmGetMaxValue(ehs_sint32 channel, ehs_sint32 freq, ehs_sint32 *max_val)
{
    inx_hw_pwm_port_errcode_t ret = INX_HW_PWM_PORT_EOK;
    esp_err_t esp_err = ESP_OK;
    uint32_t freq_value = 0;
    if (channel >= LEDC_TIMER_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    esp_err = esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_APB, 0, &freq_value);
    if (esp_err != ESP_OK) return INX_HW_PWM_PORT_EINTERNAL_CLK;
    uint32_t res_value = ledc_find_suitable_duty_resolution(freq_value, freq);
    *max_val = 1 << res_value;
    return ret;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmSetup(ehs_sint32 channel, ehs_sint32 io_num, ehs_sint32 freq, ehs_sint32 max_val)
{
    inx_hw_pwm_port_errcode_t ret = INX_HW_PWM_PORT_EOK;
    esp_err_t esp_err = ESP_OK;
    if (channel >= LEDC_TIMER_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    esp_err = gpio_set_level(io_num, 0);
    if (esp_err != ESP_OK) return INX_HW_PWM_PORT_EINVALID_IO_NUM;
    gpio_reset_pin(io_num);

    // Start configuring the PWM timer and driver
    ehs_sint32 max_ref = 0;
    max_val = upper_power_of_two(max_val);
    ret = pwmGetMaxValue(channel, freq, &max_ref);
    if (max_val > max_ref) return INX_HW_PWM_PORT_EINVALID_MAX_VAL;
    gLedcTimer[channel].speed_mode         = LEDC_LOW_SPEED_MODE;
    gLedcTimer[channel].duty_resolution    = log2_for_power2_only(max_val);
    gLedcTimer[channel].timer_num          = channel;
    gLedcTimer[channel].freq_hz            = freq;
    gLedcTimer[channel].clk_cfg            = LEDC_AUTO_CLK;
    esp_err = ledc_timer_config(&gLedcTimer[channel]);
    if (esp_err != ESP_OK) return INX_HW_PWM_PORT_EINTERNAL_CLK;
    ledc_channel_config_t ledc_channel = {
        .speed_mode         = LEDC_LOW_SPEED_MODE,
        .channel            = channel,
        .timer_sel          = channel,
        .intr_type          = LEDC_INTR_DISABLE,
        .gpio_num           = io_num,
        .duty               = 0,
        .hpoint             = 0
    };
    esp_err = ledc_channel_config(&ledc_channel);
    if (esp_err != ESP_OK) return INX_HW_PWM_PORT_ESETUP;
    gPwmMaxVal[channel] = max_val;
    return ret;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmDeinit(ehs_sint32 channel)
{
    inx_hw_pwm_port_errcode_t ret = INX_HW_PWM_PORT_EOK;
    esp_err_t esp_err = ESP_OK;
    if (channel >= LEDC_TIMER_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    esp_err = ledc_stop(LEDC_LOW_SPEED_MODE, channel, 0);
    if (esp_err != ESP_OK) return INX_HW_PWM_PORT_EDISABLE;
    esp_err = ledc_timer_rst(LEDC_LOW_SPEED_MODE, channel);
    if (esp_err != ESP_OK) return INX_HW_PWM_PORT_EINTERNAL_CLK;
    esp_err = ledc_timer_pause(LEDC_LOW_SPEED_MODE, channel);
    if (esp_err != ESP_OK) return INX_HW_PWM_PORT_EINTERNAL_CLK;
    gLedcEnabled[channel] = 0;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmSetFreq(ehs_sint32 channel, ehs_sint32 freq)
{
    inx_hw_pwm_port_errcode_t ret = INX_HW_PWM_PORT_EOK;
    esp_err_t esp_err = ESP_OK;
    if (channel >= LEDC_TIMER_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    esp_err = ledc_set_freq(LEDC_LOW_SPEED_MODE, channel, freq);
    if (esp_err != ESP_OK) return INX_HW_PWM_PORT_ESET_FREQ;
    return ret;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmSetDuty(ehs_sint32 channel, ehs_uint32 duty)
{
    inx_hw_pwm_port_errcode_t ret = INX_HW_PWM_PORT_EOK;
    esp_err_t esp_err = ESP_OK;
    if (channel >= LEDC_TIMER_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    esp_err = ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE, channel, duty, 0);
    if (esp_err != ESP_OK) return INX_HW_PWM_PORT_ESET_DUTY;
    gLedcEnabled[channel] = 1;
    return ret;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmSetMaxValue(ehs_sint32 channel, ehs_sint32 freq, ehs_sint32 max_val)
{
    inx_hw_pwm_port_errcode_t ret = INX_HW_PWM_PORT_EOK;
    esp_err_t esp_err = ESP_OK;
    if (channel >= LEDC_TIMER_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    ehs_sint32 max_ref = 0;
    max_val = upper_power_of_two(max_val);
    ret = pwmGetMaxValue(channel, freq, &max_ref);
    if (ret != INX_HW_PWM_PORT_EOK) return ret;
    if (max_val > max_ref) return INX_HW_PWM_PORT_EINVALID_MAX_VAL;
    gLedcTimer[channel].duty_resolution = log2_for_power2_only(max_val);
    esp_err = ledc_timer_config(&gLedcTimer[channel]);
    if (esp_err != ESP_OK) return INX_HW_PWM_PORT_ESETUP;
    return ret;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmEnable(ehs_sint32 channel)
{
    inx_hw_pwm_port_errcode_t ret = INX_HW_PWM_PORT_EOK;
    esp_err_t esp_err = ESP_OK;
    if (channel >= LEDC_TIMER_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    esp_err = ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
    if (esp_err != ESP_OK) return INX_HW_PWM_PORT_EENABLE;
    esp_err = ledc_timer_resume(LEDC_LOW_SPEED_MODE, channel);
    if (esp_err != ESP_OK) return INX_HW_PWM_PORT_EINTERNAL_CLK;
    gLedcEnabled[channel] = 1;
    return ret;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmDisable(ehs_sint32 channel)
{
    inx_hw_pwm_port_errcode_t ret = INX_HW_PWM_PORT_EOK;
    esp_err_t esp_err = ESP_OK;
    if (channel >= LEDC_TIMER_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    esp_err = ledc_stop(LEDC_LOW_SPEED_MODE, channel, 0);
    if (esp_err != ESP_OK) return INX_HW_PWM_PORT_EDISABLE;
    gLedcEnabled[channel] = 0;
    return ret;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmGetMaxValue(ehs_sint32 channel, ehs_sint32 *max_val)
{
    inx_hw_pwm_port_errcode_t ret = INX_HW_PWM_PORT_EOK;
    esp_err_t esp_err = ESP_OK;
    if (channel >= LEDC_TIMER_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    *max_val = gPwmMaxVal[channel];
    return ret;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmGetFreq(ehs_sint32 channel, ehs_sint32 *freq)
{
    inx_hw_pwm_port_errcode_t ret = INX_HW_PWM_PORT_EOK;
    if (channel >= LEDC_TIMER_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    *freq = ledc_get_freq(LEDC_LOW_SPEED_MODE, channel);
    if (*freq == 0) return INX_HW_PWM_PORT_EGET_FREQ;
    return ret;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmGetDuty(ehs_sint32 channel, ehs_sint32 *duty)
{
    inx_hw_pwm_port_errcode_t ret = INX_HW_PWM_PORT_EOK;
    if (channel >= LEDC_TIMER_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    *duty = ledc_get_duty(LEDC_LOW_SPEED_MODE, channel);
    if (*duty == LEDC_ERR_DUTY) return INX_HW_PWM_PORT_EGET_DUTY;
    return ret;
}

inx_hw_pwm_port_errcode_t EhsTPortPwmEnabled(ehs_sint32 channel, ehs_bool *enabled)
{
    inx_hw_pwm_port_errcode_t ret = INX_HW_PWM_PORT_EOK;
    if (channel >= LEDC_TIMER_MAX) return INX_HW_PWM_PORT_EINVALID_CHANNEL;
    *enabled = gLedcEnabled[channel] ? EHS_TRUE : EHS_FALSE;
    return ret;
}
