/**
 * ltc241x.c
 * 2022 October 21
 *
 * Description:
 *  This is the SPI API source file. The default SPI config is defined here. 
 *  This only contains necessary functions for LTC2414/LTC2418 ADC communications.
 *  Noted that when the chip is LTC2414. Just read the first 8 channels.
 *   Another 8 channels will return a garbage value.
 *
 * Copyright 2022 inx limited
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT.
 *
 **/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ltc241x.h"
#include "spi.h"

// static const char *addresses[16] = {
// 	"10000", "11000", "10001", "11001", "10010", "11010", "10011", "11011", "10100", "11100", "10101", "11101", "10110", "11110", "10111", "11111"
// };

static const uint8_t addresses_single[16] = {
	16, 24, 17, 25, 18, 26, 19, 27, 20, 28, 21, 29, 22, 30, 23, 31
};

static const uint8_t topValue = 160; // 0b10100000

// uint8_t fromBinary(const char *s) {
//   return (uint8_t) strtoul(s, NULL, 2);
// }

/**
 * Init the SPI bus for LTC241X ADC IC.
 *
 * `config` is the empty LTC241X_config_t variable that would be set by the two 
 * `internal` determines whether the internal oscillator is used or not. If not, pull-down the SCK line.
 * `differential` determines whether the channel input is differential.
 **/
int LTC241X_init(LTC241X_config_t *config, const char* device, bool internal, bool differential)
{
	int ret;
	config->internal_osc = internal;
	if (internal)
	{
		config->frequency = INTERNAL_LTC241X_SCK_FREQ;
		config->conversion_time = INTERNAL_MAX_CONVERSION_TIME_MS;
	}
	else
	{
		config->frequency = CALC_LTC241X_SCK_FREQ(EXT_OSC_FREQ);
		config->conversion_time = CALC_MAX_CONVERSION_TIME_MS(EXT_OSC_FREQ);
	}
	config->differential = differential;
	memset(config->calibration, 0, sizeof(config->calibration));
	
	*(config->spi) = SPI_get_default_config();
	strcpy(config->spi->device, device);
	config->spi->lsb = false;
	config->spi->block_size = 4;
	config->spi->frequency = config->frequency;
	
	ret = SPI_init(config->spi);
	#ifdef _SPI_VERBOSE_
	if (ret == 0)
	{
		printf("LTC241X ADC initialised\n");
	}
	#endif

	return ret;
}

/**
 * This is to decode the input to the LTC2418 according to the requested channel and pre-defined configs
 *
 **/
uint8_t LTC241X_encodeDataIn(const LTC241X_config_t *config, int channel)
{
	uint8_t ret;
	
	if (!config->differential)	// Single-ended
	{
		ret = topValue | addresses_single[channel];
	}
	else						// Differential
	{
		// TODO: to be implemented...
	}

	#ifdef _SPI_VERBOSE_
	printf("Channel; %d, Data in: %x\n", channel, ret);
	#endif

	return ret;
}

/**
 * Check 32-bit parity
 *  Credit: https://graphics.stanford.edu/~seander/bithacks.html##ParityMultiply
 *
 * Return 0 if even number of 1.
 * Return 1 if odd number of 1.
 **/
unsigned int check_parity(unsigned int v)
{
	v ^= v >> 1;
	v ^= v >> 2;
	v = (v & 0x11111111U) * 0x11111111U;
	return (v >> 28) & 1;
}

/**
 * Perform a single read on a specific channel from 0 to 15. The output data is loaded into `output` argument.
 * The `attempts` parameter determines how many times of recursion maximum shall be called before returning error code.
 *
 * Return -2 if the channel address does not match.
 * Return -1 if Even parity is not met.
 * Return 0 if OK.
 **/
int LTC241X_readSingle(LTC241X_config_t *config, int channel, int32_t *output)
{
	uint8_t *tx_buf;// {LTC241X_encodeDataIn(config, channel), 0, 0, 0};
	uint8_t *rx_buf;
	uint32_t result;
	int32_t sign = -1, uvalue_out;
	uint8_t address;
	
	tx_buf = malloc(4 * sizeof(uint8_t));
	rx_buf = malloc(4 * sizeof(uint8_t));
	
	memset(tx_buf, LTC241X_encodeDataIn(config, channel), sizeof(uint8_t));
	memset(tx_buf + 1, 0, 3 * sizeof(uint8_t));
	
	memset(rx_buf, 0, 4 * sizeof(uint8_t));
	
	SPI_transfer(config->spi, tx_buf, rx_buf);
	
	result = ((uint32_t)rx_buf[0] << 24) | ((uint32_t)rx_buf[1] << 16) | ((uint32_t)rx_buf[2] << 8) | ((uint32_t)rx_buf[3] << 0);

	#ifdef _SPI_VERBOSE_
	printf("raw: %x\n", result);
	#endif
	
	if (check_parity(result) == 1)
	{
		return -1;
	}
	
	if (result & (1 << 29)) {sign = 1;}
	
	address = (uint8_t)((result >> 1) & 31); // 0b11111
	if (address != addresses_single[channel])
	{
		return -2;
	}
	uvalue_out = (int32_t)((result >> 6) & 8388607); // 0b11111111111111111111111 -> 23-bit
	*output = sign * uvalue_out - config->calibration[channel];
	
	#ifdef _SPI_VERBOSE_
	printf("output: %d\n", *output);
	#endif
	
	return 0;
}
