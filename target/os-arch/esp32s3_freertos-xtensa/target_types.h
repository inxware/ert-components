/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_types.h
 * In this file, all of the target-specifc type definitions required by EHS are
 * given. This file is normally used by being included by ehs_types.h. This
 * applies to the win_x86 target.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_TYPES_H
#define EHS_TARGET_TYPES_H

/*Overrides Here! */

#include "esp_attr.h" // needed for IRAM_ATTR

/* Memory specific compiler attribute e.g. 'IRAM_ATTR' on esp32s3 */
//use this for function declerations
#define EHS_MEMORY_ATTRIB IRAM_ATTR
//use this for data that must be in IRAM/DRAM (not PSRAM)
#define EHS_DATA_MEMORY_ATTRIB DRAM_ATTR

/* ESP32-S3 IDF uses pthread wrappers over FreeRTOS mutexes which include full
 * memory barriers — volatile is not needed for inter-thread shared variables. */
#define EHS_VOLATILE

#include "../base_small/base_types.h"

#endif
