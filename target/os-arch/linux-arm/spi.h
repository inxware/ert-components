/**
 * spi.h
 * 2022 October 21
 *
 * Description:
 *  This is the SPI API header file. The config type is generally used throughout this API. 
 *  This only contains necessary functions for LTC2418 ADC communications.
 *
 * Copyright 2022 inx limited
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT.
 *
 **/

#ifndef _SPI_H_
#define _SPI_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct spi_config
{
	char		device[20];		// SPIdev name. e.g. "/dev/spidev1.0"
	int			fd;				// Opened device file descriptor
	uint8_t		mode;			// {CPOL, CPHA}
	bool		lsb;			// Whether LSB data first or not1
	uint8_t		bits_per_word;	// How many bits per word
	uint8_t		block_size;		// How many block sizes are transferred continuously
	uint32_t	frequency;		// SPI clock frequency
	bool		ready;			// SPI ready
} spi_config_t;

spi_config_t SPI_get_default_config( void );

int SPI_init(spi_config_t *config);

int SPI_transfer(spi_config_t *config, void *tx_buf, void *rx_buf);

#endif // #ifndef _SPI_H_