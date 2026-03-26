/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/
 
/** @file target_adcdac.h
 * The target-specific declarations required to support the HAL for ADC/DAC services
 * are defined here.
 *
 * @author: inx limited
 *
 */
/**
 * @page Verification Verification report
 * @subsection misra MISRA compliance:
 * Demonstrated MISRA compliant on
 * Last modified on $Date$
 *
 * This file contained the following derogations to the MISRA standard.
 * - Rule 20.9 "Stdio.h shall not be used in production code". Acceptable to use stdio.h on this platform
 */


#ifndef EHS_TARGET_ADCDAC_H
#define EHS_TARGET_ADCDAC_H

/*****************************************************************************/
/* Included files */

#include <ehs_types.h>
#include "../ehs_adc_errors.h"

#define EHS_TARGET_ADC_COMMA ,

#ifndef EHS_TARGET_ADC_UNIT_NUMBER
#define EHS_TARGET_ADC_UNIT_NUMBER 1
#endif//EHS_TARGET_ADC_UNIT_NUMBER 
#ifndef EHS_TARGET_ADC_CHANNEL_NUMBER
#define EHS_TARGET_ADC_CHANNEL_NUMBER 4
#endif//EHS_TARGET_ADC_CHANNEL_NUMBER

#if EHS_TARGET_ADC_UNIT_NUMBER <= 0
#error EHS_TARGET_ADC_UNIT_NUMBER must be a positive number!
#endif
#if EHS_TARGET_ADC_CHANNEL_NUMBER <= 0
#error EHS_TARGET_ADC_CHANNEL_NUMBER must be a positive number!
#endif
#if EHS_TARGET_ADC_UNIT_NUMBER > 10
#error EHS_TARGET_ADC_UNIT_NUMBER must be less than 11!
#endif
#if EHS_TARGET_ADC_CHANNEL_NUMBER > 16
#error EHS_TARGET_ADC_CHANNEL_NUMBER must be less than 17!
#endif

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
#define EHS_TARGET_ADC_FREQ_MAX 2000 /* @TODO */
#endif//EHS_TARGET_ADC_FREQ_MAX
#ifndef EHS_TARGET_ADC_FREQ_MIN
#define EHS_TARGET_ADC_FREQ_MIN 100 /* @TODO */
#endif//EHS_TARGET_ADC_FREQ_MIN

struct ehs_adc_unit_config_s {
    ehs_uint32  f_s_hz;     // Sampling frequency of the ADC unit in Hz
    ehs_bool    mode : 1;   // ADC conversion mode. EHS_FALSE for one-shot. EHS_TRUE for continuous.
    ehs_sint16  channel[EHS_TARGET_ADC_CHANNEL_NUMBER];// Pin mapping of ADC channel
    ehs_bool    init : 1;   // Whether this unit is initialised
};
typedef struct ehs_adc_unit_config_s ehs_adc_unit_config_t;
#define EHS_DEFAULT_ADC_UNIT_CONFIG(...) ((ehs_adc_unit_config_t) { \
    .f_s_hz = 1000 EHS_TARGET_ADC_COMMA \
    .mode = EHS_FALSE EHS_TARGET_ADC_COMMA \
    .channel = EHS_TARGET_ADC_CHANNEL_DEFAULT(-1) EHS_TARGET_ADC_COMMA \
    .init=0 EHS_TARGET_ADC_COMMA \
    ##__VA_ARGS__ \
    })

struct ehs_adc_channel_config_s {
    ehs_uint8  fixed_point;    // Number of bits shifted left for higher precision
    ehs_uint32 decimation;
    ehs_uint32 decimate_average;
    ehs_sint32 bias;
};
typedef struct ehs_adc_channel_config_s ehs_adc_channel_config_t;
#define EHS_DEFAULT_ADC_CHANNEL_CONFIG(...) ((ehs_adc_channel_config_t) { \
    .fixed_point=10 EHS_TARGET_ADC_COMMA \
    .decimation=1 EHS_TARGET_ADC_COMMA \
    .decimate_average=1 EHS_TARGET_ADC_COMMA \
    .bias=0 EHS_TARGET_ADC_COMMA \
    ##__VA_ARGS__ \
    })

struct ehs_adc_config_s {
    ehs_adc_unit_config_t unit_config;
    ehs_adc_channel_config_t channel_configs[EHS_TARGET_ADC_CHANNEL_NUMBER];
};
typedef struct ehs_adc_config_s ehs_adc_config_t;
#define EHS_DEFAULT_ADC_CONFIG(...) EHS_TARGET_ADC_UNIT_DEFAULT( \
    { \
        .unit_config = EHS_DEFAULT_ADC_UNIT_CONFIG() EHS_TARGET_ADC_COMMA \
        .channel_configs = EHS_TARGET_ADC_CHANNEL_DEFAULT(EHS_DEFAULT_ADC_CHANNEL_CONFIG()) \
    } \
)
/* Common code needs access to the adc config array*/
extern ehs_adc_config_t* g_ehs_adc_configs;

// The Bitmask of all enabled channels of each ADC unit. EHS_TARGET_ADC_UNIT_NUMBER (max 10) Units with EHS_TARGET_ADC_CHANNEL_NUMBER (max 16) channels
extern ehs_uint16* g_ehs_adc_continuous_enabled_bitmask;


#define EHS_SET_BIT_N(x, n) x |= (ehs_uint16)1 << n
#define EHS_CLEAR_BIT_N(x, n) x &= ~((ehs_uint16)1 << n)
#define EHS_IS_BIT_N_SET(x, n) ((x & (1 << n)) != 0)

#define EHS_SHIFT_LEFT(x, n) x = n > 0 ? x << n : x >> (-n)

/* This function is used for doing target specific initalisation
 * it must not be used anywhere outside of this traget.
 */
void EhsTgtAdcDacInit();

/***************************** ADC START *****************************/


ehs_bool legacy_target_read_adc_sample(ehs_uint8 channel, ehs_float *value, ehs_uint8 config);
ehs_bool legacy_configure_adc(ehs_uint8 channel, ehs_bool continuous, ehs_float f_s, ehs_sint32 num_samples, 
                       ehs_float bias, ehs_uint8 configuration, ehs_uint8 *config);
/*
declare the adc init function needed to configure the cahnnel/adc.
pass in specific primitive values for now rather than a struct.
target_read_adc_init(....);
*/

ehs_bool legacy_destroy_adc(ehs_uint8 channel);

ehs_sint32 EhsTAdcUnitConfigure(ehs_uint8 unit);

ehs_uint32 EhsTAdcChannelSingleRead(ehs_uint8 unit, ehs_uint8 channel);

// This is called by the ADC API. This function should be defined by the calling instance (i.e. function blocks, PID controller, etc)
EHS_MEMORY_ATTRIB void EhsTHAdcChannelContinuousConvertCB(ehs_uint8 unit, ehs_uint8 channel, ehs_sint32 mean, ehs_uint32 variance, ehs_uint32 mean_squared);

ehs_bool EhsTAdcUnitDestroy(ehs_uint8 unit);

/***************************** ADC ENDED *****************************/

/***************************** DAC START *****************************/

ehs_bool EhsTDacConfigure(ehs_uint8 channel, ehs_sint32 max, ehs_sint32 sample_rate);
ehs_bool EhsTDacSet(ehs_uint8 channel, ehs_sint32 value);
ehs_bool EhsTDacDestroy(ehs_uint8 channel);

/***************************** DAC ENDED *****************************/


#endif /* EHS_TARGET_ADCDAC_H */
