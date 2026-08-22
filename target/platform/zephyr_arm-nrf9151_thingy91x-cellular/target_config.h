/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

/** @file target_config.h
 * Platform constants for the Thingy:91 X CELLULAR variant.
 *
 * Everything is inherited from the shared baseline; this variant has no
 * constant of its own to override. Kept as a file rather than deleted because
 * the build resolves target_config.h relative to EHS_PLATFORM_PATH, which is
 * always the active target's own directory — there is no search fallback the
 * way ERT_ZEPHYR_BASE_PLATFORM provides for the Zephyr asset files.
 *
 * To override a constant, #define it BEFORE the include below: base_config.h
 * guards every value with #ifndef.
 */

#ifndef TARGET_CONFIG_ZEPHYR_NRF9151_THINGY91X_CELLULAR_H
#define TARGET_CONFIG_ZEPHYR_NRF9151_THINGY91X_CELLULAR_H

#include "../zephyr_arm-nrf9151_thingy91x-base/target_config.h"

#endif /* TARGET_CONFIG_ZEPHYR_NRF9151_THINGY91X_CELLULAR_H */
