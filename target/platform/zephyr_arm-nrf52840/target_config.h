/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_config.h
 * Platform target configuration for Zephyr on nRF52840 (Cortex-M4F).
 * INCLUDE AFTER DEFINING OVERRIDE PARAMETERS
 *
 * @author: inx limited
 */

#ifndef TARGET_CONFIG_ZEPHYR_NRF52840_H
#define TARGET_CONFIG_ZEPHYR_NRF52840_H

#ifndef TARGET_HOSTNAME
#define TARGET_HOSTNAME     "INX-ZEPHYR-NRF52840"
#endif

#ifndef EHS_TARGET_DEFAULT_ID
#define EHS_TARGET_DEFAULT_ID "(zephyr/arm-nrf52840)"
#endif

/* nRF52840 has 256KB RAM - constrain resource usage */
#ifndef EHS_MAX_WIDGET_INSTANCES
#define EHS_MAX_WIDGET_INSTANCES 32
#endif

#ifndef EHS_DEBUG_CONSOLE_BUFFER_SIZE
#define EHS_DEBUG_CONSOLE_BUFFER_SIZE (1u<<8) /* 256 bytes */
#endif

/* Include base MCU configuration defaults */
#include "../base_small/base_config.h"

#endif /* TARGET_CONFIG_ZEPHYR_NRF52840_H */
