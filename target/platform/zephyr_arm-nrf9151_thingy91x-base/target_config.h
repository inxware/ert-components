/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

/** @file target_config.h
 * Platform constants for Nordic Thingy:91 X (nRF9151 application core,
 * Cortex-M33, 256 KB RAM, 1 MB Flash).
 * Include after defining any override parameters.
 */

#ifndef TARGET_CONFIG_ZEPHYR_NRF9151_THINGY91X_H
#define TARGET_CONFIG_ZEPHYR_NRF9151_THINGY91X_H

#ifndef TARGET_HOSTNAME
#define TARGET_HOSTNAME     "INX-THINGY91X"
#endif

#ifndef EHS_TARGET_DEFAULT_ID
#define EHS_TARGET_DEFAULT_ID "(zephyr/nrf9151/thingy91x)"
#endif

/* nRF9151: 256 KB RAM — same budget class as the nRF52840 base_small */
#ifndef EHS_MAX_WIDGET_INSTANCES
#define EHS_MAX_WIDGET_INSTANCES 120
#endif

#ifndef EHS_DEBUG_CONSOLE_BUFFER_SIZE
#define EHS_DEBUG_CONSOLE_BUFFER_SIZE (1u << 9)   /* 512 bytes */
#endif

#include "../base_small/base_config.h"

#endif /* TARGET_CONFIG_ZEPHYR_NRF9151_THINGY91X_H */
