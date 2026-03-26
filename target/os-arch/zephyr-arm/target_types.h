/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_types.h
 * In this file, all of the target-specific type definitions required by EHS are
 * given. This file is normally used by being included by ehs_types.h.
 * This applies to the Zephyr ARM target.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_TYPES_H
#define EHS_TARGET_TYPES_H

/* Zephyr does not have special memory placement attributes like ESP32's IRAM_ATTR.
 * Code and data placement on Cortex-M is handled via linker scripts and Zephyr's
 * __ramfunc / __noinit attributes if needed. For now we leave these empty.
 */

/* Memory specific compiler attribute - empty for Zephyr ARM (no IRAM equivalent) */
/* #define EHS_MEMORY_ATTRIB */
/* #define EHS_DATA_MEMORY_ATTRIB */

/* Zephyr k_mutex provides full memory barriers —
 * volatile is not needed for inter-thread shared variables. */
#define EHS_VOLATILE

#include "../base_small/base_types.h"

#endif
