/** @file target_config.h
 * Base Target config to be included by target specific configs
 * INCLUDE AFTER DEFINING OVERRIDE PARAMETERS
 * @author: inx limited
 *
 * Copyright (c) inx Ltd, 2010. All rights reserved.
 */


#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

/* Overrides go here */
#define EHS_CONFIG_DISPLAY_HEIGHT 900	/**< Height for the OSD part of the display */
#define EHS_CONFIG_DISPLAY_WIDTH 1100	/**< Width for the OSD part of the display */
#define EHS_TIME_BETWEEN_FRAMES 100
#define EHS_TARGET_DEFAULT_ID "(arm/Arduino)"

/* Number of ADC Channels 
 * ADC nina in RP4020 allows for using extra adc (A4-A7)
 */
#define EHS_TARGET_ADC_CHANNEL_NUMBER 8 

#include "../base_small/base_config.h"


#endif /* TARGET_CONFIG_H */
