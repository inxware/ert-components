/** @file target_config.h
 * Base Target config to be included by target specific configs
 * INCLUDE AFTER DEFINING OVERRIDE PARAMETERS
 * @author: inx limited
 *
 * Copyright (c) inx Ltd, 2020. All rights reserved.
 */

#ifndef TARGET_CONFIG_XTENSA_EXTRAM_BASE_H
#define TARGET_CONFIG_XTENSA_EXTRAM_BASE_H

/* Overrides go here */
#ifndef EHS_CONFIG_DISPLAY_HEIGHT
#define EHS_CONFIG_DISPLAY_HEIGHT 480 /**< Height for the OSD part of the display */
#endif//EHS_CONFIG_DISPLAY_HEIGHT
#ifndef EHS_CONFIG_DISPLAY_WIDTH
#define EHS_CONFIG_DISPLAY_WIDTH 800 /**< Width for the OSD part of the display */
#endif//EHS_CONFIG_DISPLAY_WIDTH
#ifndef EHS_TARGET_DEFAULT_ID
#define EHS_TARGET_DEFAULT_ID "(xtensa/esp32s3)"
#endif//EHS_TARGET_DEFAULT_ID

#define LV_ATTRIBUTE_LARGE_RAM_ARRAY EXT_RAM_BSS_ATTR

#include "../base_small/base_config.h"

/* Overides must go here */

#include "../esp32s3_freertos-xtensa-base/target_config.h"

#endif /* TARGET_CONFIG_XTENSA_EXTRAM_BASE_H */
