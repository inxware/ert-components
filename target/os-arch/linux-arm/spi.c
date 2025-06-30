/**
 * spi.c
 * 2022 October 21
 *
 * Description:
 *  This is the SPI API source file. The default SPI config is defined here. 
 *  This only contains necessary functions for LTC2418 ADC communications.
 *
 * Copyright 2022 inx limited
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT.
 *
 **/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

#include "spi.h"

const static spi_config_t spi_config_default = 
{
	.device			= "/dev/spidev1.0",
	.fd				= -1,
	.mode			= SPI_MODE_0,	// 0
	.lsb			= false,		// MSB first
	.bits_per_word	= 8,			// 8 bits per word
	.block_size		= 1,			// 1 word to be transferred
	.frequency		= 10000000U,	// 10MHz SCK Frequency
	.ready			= false			// SPI not ready
};

/**
 * Get the default SPI configuration.
 *
 **/
spi_config_t SPI_get_default_config( void )
{
	return spi_config_default;
}

/**
 * Init the SPI bus
 *
 * The SPI bus will be configured as described by the `config` input. The device file is opened to get the device file descriptor.
 **/
int SPI_init(spi_config_t *config)
{
	uint8_t temp8;
	uint32_t temp32;
	if ((config->fd = open(config->device, O_RDONLY)) < 0)
	{
		printf("%s: ", config->device);
		perror("Failed to open SPI device\n");
		return -1;
	}
	
	temp8 = config->mode;// | SPI_READY;
	if (ioctl(config->fd, SPI_IOC_WR_MODE, &temp8) < 0)
	{
		perror("Failed to write SPI mode\n");
		return -2;
	}
	if (ioctl(config->fd, SPI_IOC_RD_MODE, &temp8) < 0)
	{
		perror("Failed to read SPI mode\n");
		return -2;
	}
	config->mode = temp8;
	
	temp8 = (uint8_t)config->lsb;
	if (ioctl(config->fd, SPI_IOC_WR_LSB_FIRST, &temp8) < 0)
	{
		perror("Failed to write SPI LSB first config\n");
		return -3;
	}
	if (ioctl(config->fd, SPI_IOC_RD_LSB_FIRST, &temp8) < 0)
	{
		perror("Failed to read SPI LSB first config\n");
		return -3;
	}
	config->lsb = (bool)temp8;
	
	temp8 = config->bits_per_word;
	if (ioctl(config->fd, SPI_IOC_WR_BITS_PER_WORD, &temp8) < 0)
	{
		perror("Failed to write SPI bits per word config\n");
		return -4;
	}
	if (ioctl(config->fd, SPI_IOC_RD_BITS_PER_WORD, &temp8) < 0)
	{
		perror("Failed to read SPI bits per word config\n");
		return -4;
	}
	config->bits_per_word = temp8;
	
	temp32 = config->frequency;
	if (ioctl(config->fd, SPI_IOC_WR_MAX_SPEED_HZ, &temp32) < 0)
	{
		perror("Failed to write max SPI speed config\n");
		return -5;
	}
	if (ioctl(config->fd, SPI_IOC_RD_MAX_SPEED_HZ, &temp32) < 0)
	{
		perror("Failed to read max SPI speed config\n");
		return -5;
	}
	config->frequency = temp32;
	
	config->ready = true;
	
	#ifdef _SPI_VERBOSE_
	printf("\nSPI init success!\n%s:\n Mode: %d,\n Frequency: %d Hz,\n %s First,\n Bits per word: %d,\n block size: %d\n\n", config->device, config->mode, config->frequency, config->lsb ? "LSB" : "MSB", config->bits_per_word, config->block_size);
	#endif
	
	return 0;
}

/**
 * Transfer the data between the SPI master and slave
 *
 * This function transfers the data according to the `config` input. 
 * `tx_buf` is the buffer to be transmitted to the slave. This needs to have memory pre-allocated.
 * `rx_buf` is the buffer to be received from the slave. This needs to have memory pre-allocated.
 **/
int SPI_transfer(spi_config_t *config, void *tx_buf, void *rx_buf)
{
	struct spi_ioc_transfer sit = {
		.tx_buf = (unsigned long)tx_buf,
		.rx_buf = (unsigned long)rx_buf,
		.len = config->block_size,
		.delay_usecs = 0,
		.speed_hz = config->frequency,
		.bits_per_word = config->bits_per_word,
	};
	
	if (ioctl(config->fd, SPI_IOC_MESSAGE(1), &sit) < 0)
	{
		perror("SPI message transfer failed\n");
		return -1;
	}
	return 0;
}