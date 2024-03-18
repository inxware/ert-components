/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/



#include "target_adcdac.h"
#include "target.h"
#include "target_config.h"

#include <sys/utsname.h>
#include <stdlib.h>
#include <stdio.h>

#include "ltc241x.h"

const static char DEVICE_NAME[] =  "/dev/spidev1.0";
const static float Vref = 4.096;

static volatile ehs_uint8 configured = 0;
LTC241X_config_t configuration;
static volatile float prev = 0;

/*
any globals needs for the esp ADC to work.
esp_specific_thing x;
*/
// static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1,
// GPIO14 if ADC2 static const adc_bits_width_t width = ADC_WIDTH_BIT_12; static
// const adc_atten_t atten = ADC_ATTEN_DB_0;

/* todo 
1. Driver code must be non-blocking and return an error if timing or just return random values - we will manage this in this file to avoif sleeps etc.
2. 

Longer term:
1. We need an error conidition for too fast sampling
2. errors for misconfiguration (e.g.) channel number out of range.
*/

ehs_uint8 unit = 0;

static ehs_bool set_ADC_unit(ehs_uint8 value)
{
   
    return EHS_TRUE;
}

static ehs_bool set_ADC_width(ehs_uint8 width)
{
  
    return EHS_TRUE;
}

static ehs_bool get_ADC_width(ehs_uint8 *width)
{

    return EHS_TRUE;
}

static ehs_bool set_atten(ehs_uint8 channel, ehs_uint8 atten)
{
  
    return EHS_TRUE;
}

// todo - what is the *config needed for in generic code?
EHS_GLOBAL ehs_bool configure_adc(ehs_uint8 channel, ehs_uint8 configure,
                                  ehs_uint8 *config)
{
    ehs_bool ret = EHS_TRUE;
    if (configured == 0)
    {
        LTC241X_init(&configuration, DEVICE_NAME, true, false);
        configured = 1;
    }
   
    return EHS_TRUE;
}

/* Probably don't need this : */
EHS_GLOBAL ehs_bool target_read_adc_sample(ehs_uint8 channel, ehs_float *value,
        ehs_uint8 config)
{
    /// todo2022 write the esp32 specific code to get a value and return and error
    /// status.
    ehs_uint8 width;
    int32_t output;
    int ret;

    ret = LTC241X_readSingle(&configuration, channel, &output); 

    if (ret == -2)
    {
        *value = prev;
        printf("The data received is not for the channel %d\n", channel);
        return EHS_FALSE;
    }
    else if (ret == -1)
    {
        *value = prev;
        printf("Data parity check failed.\n");
        return EHS_FALSE;
    }
    else if (ret == 0)
    {
        *value = output / 8388607.0 * Vref;
        prev = *value;
    }

    

    // set_ADC_unit(unit);
    if (config == 1)
    {
 
    }
    else
    {

    }
    return EHS_TRUE;
}

ehs_bool destroy_adc()
{
    return EHS_TRUE;
}


