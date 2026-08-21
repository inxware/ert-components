/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

/** @file target_config.h
 * Platform constants for Nordic nRF5340 DK (application core, Cortex-M33, 512 KB RAM).
 * Include after defining any override parameters.
 */

#ifndef TARGET_CONFIG_ZEPHYR_NRF5340_NRF5340DK_H
#define TARGET_CONFIG_ZEPHYR_NRF5340_NRF5340DK_H

#ifndef TARGET_HOSTNAME
#define TARGET_HOSTNAME     "INX-NRF5340DK"
#endif

#ifndef EHS_TARGET_DEFAULT_ID
#define EHS_TARGET_DEFAULT_ID "(zephyr/nrf5340/cpuapp/nrf5340dk)"
#endif

/* nRF5340 cpuapp: 512 KB RAM — slightly larger budget than nRF52840 */
#ifndef EHS_MAX_WIDGET_INSTANCES
#define EHS_MAX_WIDGET_INSTANCES 120
#endif

#ifndef EHS_DEBUG_CONSOLE_BUFFER_SIZE
#define EHS_DEBUG_CONSOLE_BUFFER_SIZE (1u << 9)   /* 512 bytes */
#endif

#include "../base_small/base_config.h"

#endif /* TARGET_CONFIG_ZEPHYR_NRF5340_NRF5340DK_H */
