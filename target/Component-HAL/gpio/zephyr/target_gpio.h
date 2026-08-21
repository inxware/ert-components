/*---------------------------------------------------------------
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
 *---------------------------------------------------------------*/

/**
 * @file target_gpio.h
 * Zephyr GPIO HAL header.
 *
 * GPIO operations are implemented via the Zephyr device driver API using
 * DTS aliases (inx-gpio0, inx-gpio1) defined in the board overlay.
 * Pin IDs passed to EhsInitOutputGPIO / EhsInitInputGPIO are encoded as
 * (port << 5 | pin) so that pins 0-31 map to port 0 and 32-63 to port 1.
 */

#ifndef TARGET_GPIO_ZEPHYR_H_
#define TARGET_GPIO_ZEPHYR_H_

#include "globals.h"
#include "ehs_hal_gpio.h"

#endif /* TARGET_GPIO_ZEPHYR_H_ */
