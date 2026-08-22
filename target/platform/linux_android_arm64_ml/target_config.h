/** @file target_config.h
 * Target config for linux_android_arm64_ml
 * @author: inx limited
 *
 * Copyright (c) inx Ltd, 2026. All rights reserved.
 */

#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

/* Overrides go here */
#define EHS_CONFIG_DISPLAY_HEIGHT 768	/**< Height for the OSD part of the display */
#define EHS_CONFIG_DISPLAY_WIDTH 1024	/**< Width for the OSD part of the display */
#define EHS_TARGET_DEFAULT_ID "(arm64/android/ml)"

#define EHS_GFXTARGET_RGBA

#include "../base_full/base_config.h"

#endif /* TARGET_CONFIG_H */
