/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

/** @file target_config.h
 * Platform constants for RAK Wireless RAK4631 (nRF52840, Cortex-M4F, 256 KB RAM).
 * Include after defining any override parameters.
 */

#ifndef TARGET_CONFIG_ZEPHYR_NRF52840_RAK4631_H
#define TARGET_CONFIG_ZEPHYR_NRF52840_RAK4631_H

#ifndef TARGET_HOSTNAME
#define TARGET_HOSTNAME     "INX-RAK4631"
#endif

#ifndef EHS_TARGET_DEFAULT_ID
#define EHS_TARGET_DEFAULT_ID "(zephyr/nrf52840/rak4631)"
#endif

/* nRF52840: 256 KB RAM — keep widget table small */
#ifndef EHS_MAX_WIDGET_INSTANCES
#define EHS_MAX_WIDGET_INSTANCES 120
#endif

#ifndef EHS_DEBUG_CONSOLE_BUFFER_SIZE
#define EHS_DEBUG_CONSOLE_BUFFER_SIZE (1u << 8)   /* 256 bytes */
#endif

#include "../base_small/base_config.h"

#endif /* TARGET_CONFIG_ZEPHYR_NRF52840_RAK4631_H */
