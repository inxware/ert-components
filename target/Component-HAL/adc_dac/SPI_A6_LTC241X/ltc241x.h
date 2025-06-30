/**
 * ltc241x.h
 * 2022 October 21
 *
 * Description:
 *  This is the LTC2418 ADC header file. It contains all the functions to work with internal oscillators (SCK floating or pull-up)
 *  Can also be used for LTC2414 only with the first 8 channels.
 *
 * Copyright 2022 inx limited
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT.
 *
 **/

#ifndef _LTC241X_H_
#define _LTC241X_H_

#include <stdint.h>
#include <stdbool.h>

#include "spi.h"

#define LTC2414 1
#define LTC2418 2

#define LTC241x LTC2414

// External Oscillator frequency in Hz
#define EXT_OSC_FREQ 500000

// According to the datasheet, the SCK frequency with internal oscillator is 19.2kHz.
#define INTERNAL_LTC241X_SCK_FREQ 19200

// Calculate the SCK frequency in Hz. Input is in Hz.
#define CALC_LTC241X_SCK_FREQ(x) x/8

// With internal oscillator, the worst-case ADC conversion time is 170ms.
#define INTERNAL_MAX_CONVERSION_TIME_MS 170

// Calculate the max ADC conversion time in ms. Input is in Hz.
#define CALC_MAX_CONVERSION_TIME_MS(x) 20510000/x

typedef struct LTC241X_config
{
	bool		internal_osc;	// Whether using the internal oscillator for communication. If false, pull-down SCK line.
	uint32_t	frequency;		// SCK frequency
	bool		differential;	// Whether this is the differential channel reading. If false, then individual ADC channel (single-ended) will be read.
	uint16_t	conversion_time;// The maximum conversion time for a single read
	int32_t		calibration[16];// The calibration data for all 16 channels
	spi_config_t spi[1];			// The configuration of SPIdev
} LTC241X_config_t;

int LTC241X_init(LTC241X_config_t *config, const char* device, bool internal, bool differential);
int LTC241X_readSingle(LTC241X_config_t *config, int channel, int32_t *output);

#endif // #ifndef _LTC241X_H_