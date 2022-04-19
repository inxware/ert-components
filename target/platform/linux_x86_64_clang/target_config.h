/** @file target_config.h
 * Base Target config to be included by target specific configs
 * INCLUDE AFTER DEFINING OVERRIDE PARAMETERS
 * @author: Pierre
 * @version: $Revision: 1245 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx Ltd, 2010. All rights reserved.
 */



#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

/* Overrides go here */
#define EHS_CONFIG_DISPLAY_HEIGHT 1024	/**< Height for the OSD part of the display */
#define EHS_CONFIG_DISPLAY_WIDTH 780	/**< Width for the OSD part of the display */
#define EHS_TARGET_DEFAULT_ID "(x86/Linux)"
#define EHS_MQTT_SUPPORT 1
#include "../base_full/base_config.h"


#endif /* TARGET_CONFIG_H */
