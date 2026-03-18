/** @file target_config.h
 * Target configuration for NVIDIA Jetson Orin Nano (NGC build environment).
 *
 * Copyright (c) inx Ltd, 2026. All rights reserved.
 */

#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

/* Default display geometry — adjust to match the panel attached to your Jetson.
 * Common options:
 *   HDMI monitor:   1920x1080
 *   7-inch DSI panel: 1024x600
 *   10-inch HDMI panel: 1280x800 */
#define EHS_CONFIG_DISPLAY_HEIGHT  1080
#define EHS_CONFIG_DISPLAY_WIDTH   1920

#define EHS_TARGET_DEFAULT_ID "(arm64/Linux/Jetson)"

#include "../base_full/base_config.h"

#endif /* TARGET_CONFIG_H */
