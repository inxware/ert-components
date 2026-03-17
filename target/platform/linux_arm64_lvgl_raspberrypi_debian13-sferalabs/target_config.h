/** @file target_config.h
 * Target config for linux_arm64_lvgl_raspberrypi_debian13-sferalabs
 * Raspberry Pi CM4/CM5 with Sfera Labs Strato Pi Max industrial I/O board.
 *
 * Copyright (c) inx Ltd, 2026. All rights reserved.
 */

#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

/* Display geometry — same as base RPi LVGL target */
#define EHS_CONFIG_DISPLAY_HEIGHT 720   /**< Height for the OSD part of the display */
#define EHS_CONFIG_DISPLAY_WIDTH  1024  /**< Width for the OSD part of the display */

#define EHS_TARGET_DEFAULT_ID "(arm64/Linux/SferaLabs-StratoPiMax)"

#include "../base_full/base_config.h"

#endif /* TARGET_CONFIG_H */
