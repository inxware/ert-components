/** @file target_config.h
 * Target configuration for QNX Neutrino 8.0 on Raspberry Pi 4 (Cortex-A72 / AArch64)
 * INCLUDE AFTER DEFINING OVERRIDE PARAMETERS
 * @author: inx limited
 *
 * Copyright (c) inx Ltd, 2026. All rights reserved.
 */

#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

/* Display geometry — RPi4 HDMI default 1080p; override as needed */
#define EHS_CONFIG_DISPLAY_HEIGHT 1080
#define EHS_CONFIG_DISPLAY_WIDTH  1920
#define EHS_TARGET_DEFAULT_ID "(aarch64/QNX8)"

#include "../base_full/base_config.h"

#endif /* TARGET_CONFIG_H */
