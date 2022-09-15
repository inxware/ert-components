/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

#include "target_adcdac.h"
#include "driver/adc.h"
#include "target.h"
#include "target_config.h"

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
    *width = ADC_WIDTH_BIT_9;
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
}

EHS_GLOBAL ehs_bool configure_adc(ehs_uint8 channel, ehs_uint8 configure,
                                  ehs_uint8 *config)
{
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
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool target_read_adc_sample(ehs_uint8 channel, ehs_float *value,
        ehs_uint8 config)
{
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
    return EHS_TRUE;
}
/*
#include "target_time.h"
//#undef TEST_ADCDAC
#ifdef TEST_ADCDAC
void app_main(void)
{
    static const adc_channel_t channel = ADC_CHANNEL_6;
    ehs_uint8 config;
    configure_adc(channel, 1, &config);
    ehs_float raw_value = 0;
    while (1)
    {
        if (target_read_adc_sample((ehs_uint8)channel, &raw_value, config) ==
                EHS_TRUE)
        {
            printf("Config\t%d\tResult_value\t%f\n", config, raw_value);
        }
        EhsSleep(100);
    }
}
#endif*/
