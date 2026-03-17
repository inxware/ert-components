/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file target_adcdac.h
 * @brief Sfera Labs Analog I/O HAL header.
 *
 * Self-contained implementation-specific header following the same convention
 * as the other target_adcdac.h implementations (stubbed, ESP32_IDF, etc.).
 *
 * ADC channel mapping (0-based):
 *   Iono Pi:
 *     ch 0-5  → ai1-ai6 (voltage, mV)
 *   Iono Pi Max:
 *     ch 0-3  → av1-av4 (voltage, mV/100)
 *     ch 4-7  → ai1-ai4 (current, µA)
 *     ch 8-9  → at1-at2 (temperature, °C/100)
 *   Strato Pi Max (expansion slot boards):
 *     ch = (slot_nibble << 4) | ch_nibble  (slot 1-4, ch 1-4)
 *
 * DAC channel mapping (0-based):
 *   Iono Pi Max / Strato Pi Max: ch 0 → ao1, ch 1 → ao2
 *
 * The 'configuration' byte passed to configure_adc selects input mode:
 *   SFERALABS_ADC_MODE_VOLT_UNI   (0x00) = unipolar voltage (default)
 *   SFERALABS_ADC_MODE_VOLT_BI    (0x01) = bipolar voltage
 *   SFERALABS_ADC_MODE_CURR_UNI   (0x10) = unipolar current
 *   SFERALABS_ADC_MODE_CURR_BI    (0x11) = bipolar current
 *   SFERALABS_ADC_MODE_TEMP_PT100 (0x20) = PT100 temperature probe
 *   SFERALABS_ADC_MODE_TEMP_PT1000(0x21) = PT1000 temperature probe
 *   SFERALABS_DAC_MODE_VOLT       (0xD0) = DAC voltage mode
 *   SFERALABS_DAC_MODE_CURR       (0xD1) = DAC current mode
 */

#ifndef EHS_TARGET_ADCCAD_H
#define EHS_TARGET_ADCCAD_H

#include "globals.h"
#include "sferalabs_hal.h"

/* -------------------------------------------------------------------------
 * Common ADC/DAC infrastructure — mirrors the content of stubbed/target_adcdac.h
 * so that this header is fully self-contained.
 * ------------------------------------------------------------------------- */

#define EHS_TARGET_ADC_COMMA ,

#ifndef EHS_TARGET_ADC_UNIT_NUMBER
#define EHS_TARGET_ADC_UNIT_NUMBER 1
#endif
#ifndef EHS_TARGET_ADC_CHANNEL_NUMBER
#define EHS_TARGET_ADC_CHANNEL_NUMBER 1
#endif

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
#endif

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
#endif

#ifndef EHS_TARGET_ADC_FREQ_MAX
#define EHS_TARGET_ADC_FREQ_MAX 1000000000
#endif
#ifndef EHS_TARGET_ADC_FREQ_MIN
#define EHS_TARGET_ADC_FREQ_MIN 100
#endif

struct ehs_adc_unit_config_s {
    ehs_uint32 f_s_hz;
    ehs_bool   mode : 1;
    ehs_sint16 channel[EHS_TARGET_ADC_CHANNEL_NUMBER];
    ehs_bool   init : 1;
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
    ehs_uint8  fixed_point;
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
    ehs_adc_unit_config_t    unit_config;
    ehs_adc_channel_config_t channel_configs[EHS_TARGET_ADC_CHANNEL_NUMBER];
};
typedef struct ehs_adc_config_s ehs_adc_config_t;

#define EHS_DEFAULT_ADC_CONFIG(...) EHS_TARGET_ADC_UNIT_DEFAULT( \
    { \
        .unit_config = EHS_DEFAULT_ADC_UNIT_CONFIG() EHS_TARGET_ADC_COMMA \
        .channel_configs = EHS_TARGET_ADC_CHANNEL_DEFAULT(EHS_DEFAULT_ADC_CHANNEL_CONFIG()) \
    } \
)

extern ehs_adc_config_t g_ehs_adc_configs[EHS_TARGET_ADC_UNIT_NUMBER];
extern ehs_uint16 g_ehs_adc_continuous_enabled_bitmask[EHS_TARGET_ADC_UNIT_NUMBER];

#define EHS_SET_BIT_N(x, n)    x |= (ehs_uint16)1 << n
#define EHS_CLEAR_BIT_N(x, n)  x &= ~((ehs_uint16)1 << n)
#define EHS_IS_BIT_N_SET(x, n) ((x & (1 << n)) != 0)
#define EHS_SHIFT_LEFT(x, n)   x = n > 0 ? x << n : x >> (-n)

#ifndef EHS_TARGET_DAC_NUMBER
#define EHS_TARGET_DAC_NUMBER 100
#endif

/* ADC function declarations */
ehs_bool configure_adc(ehs_uint8 channel, ehs_bool continuous, ehs_float f_s,
                        ehs_sint32 num_samples, ehs_float bias,
                        ehs_uint8 configuration, ehs_uint8 *config);
ehs_bool target_read_adc_sample(ehs_uint8 channel, ehs_float *value, ehs_uint8 config);
ehs_bool destroy_adc(ehs_uint8 channel);
ehs_bool EhsTAdcUnitConfigure(ehs_uint8 unit);
ehs_uint32 EhsTAdcChannelSingleRead(ehs_uint8 unit, ehs_uint8 channel);
EHS_MEMORY_ATTRIB void EhsTHAdcChannelContinuousConvertCB(ehs_uint8 unit, ehs_uint8 channel,
                                                           ehs_sint32 mean, ehs_uint32 variance,
                                                           ehs_uint32 mean_squared);
ehs_bool EhsTAdcUnitDestroy(ehs_uint8 unit);

/* DAC function declarations */
ehs_bool EhsTDacConfigure(ehs_uint8 channel, ehs_sint32 max, ehs_sint32 sample_rate);
ehs_bool EhsTDacSet(ehs_uint8 channel, ehs_sint32 value);
ehs_bool EhsTDacDestroy(ehs_uint8 channel);

/* -------------------------------------------------------------------------
 * Sfera Labs specific — input mode configuration bytes
 * ------------------------------------------------------------------------- */

#define SFERALABS_ADC_MODE_VOLT_UNI     0x00
#define SFERALABS_ADC_MODE_VOLT_BI      0x01
#define SFERALABS_ADC_MODE_CURR_UNI     0x10
#define SFERALABS_ADC_MODE_CURR_BI      0x11
#define SFERALABS_ADC_MODE_TEMP_PT100   0x20
#define SFERALABS_ADC_MODE_TEMP_PT1000  0x21
#define SFERALABS_DAC_MODE_VOLT         0xD0
#define SFERALABS_DAC_MODE_CURR         0xD1

#endif /* EHS_TARGET_ADCCAD_H */
