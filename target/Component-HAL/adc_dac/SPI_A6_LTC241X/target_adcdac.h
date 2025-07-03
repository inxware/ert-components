/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

#ifndef EHS_TARGET_ADCCAD_H
#define EHS_TARGET_ADCCAD_H

#include "ehs_types.h"

#ifndef EHS_TARGET_ADC_UNIT_NUMBER
#define EHS_TARGET_ADC_UNIT_NUMBER 1
#endif//EHS_TARGET_ADC_UNIT_NUMBER 
#ifndef EHS_TARGET_ADC_CHANNEL_NUMBER
#define EHS_TARGET_ADC_CHANNEL_NUMBER 16
#endif//EHS_TARGET_ADC_CHANNEL_NUMBER



#if EHS_TARGET_ADC_UNIT_NUMBER == 1
#define EHS_TARGET_ADC_UNIT_DEFAULT(x) {x}
#elif EHS_TARGET_ADC_UNIT_NUMBER == 2
#define EHS_TARGET_ADC_UNIT_DEFAULT(x) {x, x}
#elif EHS_TARGET_ADC_UNIT_NUMBER == 3
#define EHS_TARGET_ADC_UNIT_DEFAULT(x) {x, x, x}
#elif EHS_TARGET_ADC_UNIT_NUMBER == 4
#define EHS_TARGET_ADC_UNIT_DEFAULT(x) {x, x, x, x}
#elif EHS_TARGET_ADC_UNIT_NUMBER == 5
#define EHS_TARGET_ADC_UNIT_DEFAULT(x) {x, x, x, x, x}
#elif EHS_TARGET_ADC_UNIT_NUMBER == 6
#define EHS_TARGET_ADC_UNIT_DEFAULT(x) {x, x, x, x, x, x}
#elif EHS_TARGET_ADC_UNIT_NUMBER == 7
#define EHS_TARGET_ADC_UNIT_DEFAULT(x) {x, x, x, x, x, x, x}
#elif EHS_TARGET_ADC_UNIT_NUMBER == 8
#define EHS_TARGET_ADC_UNIT_DEFAULT(x) {x, x, x, x, x, x, x, x}
#elif EHS_TARGET_ADC_UNIT_NUMBER == 9
#define EHS_TARGET_ADC_UNIT_DEFAULT(x) {x, x, x, x, x, x, x, x, x}
#elif EHS_TARGET_ADC_UNIT_NUMBER == 10
#define EHS_TARGET_ADC_UNIT_DEFAULT(x) {x, x, x, x, x, x, x, x, x, x}
#endif//EHS_TARGET_ADC_UNIT_NUMBER == n

#if EHS_TARGET_ADC_CHANNEL_NUMBER == 1
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x}
#elif EHS_TARGET_ADC_CHANNEL_NUMBER == 2
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x, x}
#elif EHS_TARGET_ADC_CHANNEL_NUMBER == 3
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x, x, x}
#elif EHS_TARGET_ADC_CHANNEL_NUMBER == 4
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x, x, x, x}
#elif EHS_TARGET_ADC_CHANNEL_NUMBER == 5
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x, x, x, x, x}
#elif EHS_TARGET_ADC_CHANNEL_NUMBER == 6
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x, x, x, x, x, x}
#elif EHS_TARGET_ADC_CHANNEL_NUMBER == 7
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x, x, x, x, x, x, x}
#elif EHS_TARGET_ADC_CHANNEL_NUMBER == 8
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x, x, x, x, x, x, x, x}
#elif EHS_TARGET_ADC_CHANNEL_NUMBER == 9
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x, x, x, x, x, x, x, x, x}
#elif EHS_TARGET_ADC_CHANNEL_NUMBER == 10
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x, x, x, x, x, x, x, x, x, x}
#elif EHS_TARGET_ADC_CHANNEL_NUMBER == 11
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x, x, x, x, x, x, x, x, x, x, x}
#elif EHS_TARGET_ADC_CHANNEL_NUMBER == 12
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x, x, x, x, x, x, x, x, x, x, x, x}
#elif EHS_TARGET_ADC_CHANNEL_NUMBER == 13
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x, x, x, x, x, x, x, x, x, x, x, x, x}
#elif EHS_TARGET_ADC_CHANNEL_NUMBER == 14
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x, x, x, x, x, x, x, x, x, x, x, x, x, x}
#elif EHS_TARGET_ADC_CHANNEL_NUMBER == 15
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x, x, x, x, x, x, x, x, x, x, x, x, x, x, x}
#elif EHS_TARGET_ADC_CHANNEL_NUMBER == 16
#define EHS_TARGET_ADC_CHANNEL_DEFAULT(x) {x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x}
#endif//EHS_TARGET_ADC_CHANNEL_NUMBER == n

#ifndef EHS_TARGET_ADC_FREQ_MAX
#define EHS_TARGET_ADC_FREQ_MAX 1000000000
#endif//EHS_TARGET_ADC_FREQ_MAX
#ifndef EHS_TARGET_ADC_FREQ_MIN
#define EHS_TARGET_ADC_FREQ_MIN 100
#endif//EHS_TARGET_ADC_FREQ_MIN


/* Trying to make this work with new ADC configuration stuff that the common component uses. */

struct ehs_adc_unit_config_s {
    ehs_uint32  f_s_hz;     // Sampling frequency of the ADC unit in Hz
    ehs_bool    mode;   // ADC conversion mode. EHS_FALSE for one-shot. EHS_TRUE for continuous.
    ehs_sint16  channel[EHS_TARGET_ADC_CHANNEL_NUMBER]; // Pin mapping of ADC channel
    ehs_bool    init;   // Whether this unit is initialised
};
typedef struct ehs_adc_unit_config_s ehs_adc_unit_config_t;


#define EHS_DEFAULT_ADC_UNIT_CONFIG(...) ((ehs_adc_unit_config_t) { \
    .channel = EHS_TARGET_ADC_CHANNEL_DEFAULT(-1) EHS_TARGET_ADC_COMMA \
    .init=0 EHS_TARGET_ADC_COMMA \
    ##__VA_ARGS__ \
    })

struct ehs_adc_channel_config_s {
    ehs_uint32 decimation;
    ehs_uint32 decimate_average;
    ehs_sint32 bias;
};

typedef struct ehs_adc_channel_config_s ehs_adc_channel_config_t;

#define EHS_DEFAULT_ADC_CHANNEL_CONFIG(...) ((ehs_adc_channel_config_t) { \
    .decimation=1 EHS_TARGET_ADC_COMMA \
    .decimate_average=1 EHS_TARGET_ADC_COMMA \
    .bias=0 EHS_TARGET_ADC_COMMA \
    ##__VA_ARGS__ \
    })

struct ehs_adc_config_s {
    ehs_adc_unit_config_t unit_config;
    ehs_adc_channel_config_t channel_configs[EHS_TARGET_ADC_CHANNEL_NUMBER];
};


#define EHS_SET_BIT_N(x, n) x |= (ehs_uint16)1 << n
#define EHS_CLEAR_BIT_N(x, n) x &= ~((ehs_uint16)1 << n)
#define EHS_IS_BIT_N_SET(x, n) ((x & (1 << n)) != 0)
#define EHS_SHIFT_LEFT(x, n) x = n > 0 ? x << n : x >> (-n)


typedef struct ehs_adc_config_s ehs_adc_config_t;

#define EHS_DEFAULT_ADC_CONFIG(...) EHS_TARGET_ADC_UNIT_DEFAULT( {  .unit_config = EHS_DEFAULT_ADC_UNIT_CONFIG() EHS_TARGET_ADC_COMMA .channel_configs = EHS_TARGET_ADC_CHANNEL_DEFAULT(EHS_DEFAULT_ADC_CHANNEL_CONFIG()) } )

extern ehs_adc_config_t* g_ehs_adc_configs;

// The Bitmask of all enabled channels of each ADC unit. EHS_TARGET_ADC_UNIT_NUMBER (max 10) Units with EHS_TARGET_ADC_CHANNEL_NUMBER (max 16) channels
extern ehs_uint16* g_ehs_adc_continuous_enabled_bitmask;



//todo2022 do these need to be made public?
#define ADC1 1
#define ADC2 2

ehs_bool target_read_adc_sample(ehs_uint8 channel, ehs_float *value,
                                ehs_uint8 config);

/* THis is the canonical function for readin a single sample */
ehs_uint32 EhsTAdcChannelSingleRead(ehs_uint8 unit, ehs_uint8 channel);

ehs_bool configure_adc(ehs_uint8 channel, ehs_bool continuous, ehs_float f_s, ehs_sint32 num_samples, ehs_float bias, ehs_uint8 configuration,
                       ehs_uint8 *config);
/*
declare the adc init function needed to configure the cahnnel/adc.
pass in specific primitive values for now rather than a struct.
target_read_adc_init(....);
*/

ehs_bool destroy_adc(ehs_uint8 channel);

ehs_bool EhsTAdcUnitConfigure(ehs_uint8 unit);
ehs_bool EhsTAdcUnitDestroy(ehs_uint8 unit);

/***************************** DAC START *****************************/

// DAC macro definitions
#ifndef EHS_TARGET_DAC_NUMBER
#define EHS_TARGET_DAC_NUMBER 100
#endif

ehs_bool EhsTDacConfigure(ehs_uint8 channel, ehs_sint32 max, ehs_sint32 sample_rate);
ehs_bool EhsTDacSet(ehs_uint8 channel, ehs_sint32 value);
ehs_bool EhsTDacDestroy(ehs_uint8 channel);

/***************************** DAC ENDED *****************************/


#endif
