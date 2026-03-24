/*---------------------------------------------------------------
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
 *---------------------------------------------------------------*/

/*
 * target_config.h — platform-specific compile-time configuration
 * for XMOS xcore.ai FreeRTOS targets.
 */

#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

/* Platform identification */
#define EHS_TARGET_XCORE          1
#define EHS_TARGET_XCORE_AI       1
#define EHS_TARGET_FREERTOS       1

/* XMOS xcore.ai tile/core configuration */
#define EHS_XCORE_NUM_TILES       1
#define EHS_XCORE_NUM_CORES       8    /* hardware threads per tile */

/* Stack and heap sizing — tune per application */
#define EHS_TASK_STACK_SIZE_DEFAULT   (8 * 1024)
#define EHS_HEAP_SIZE_DEFAULT         (256 * 1024)

/* Console output via xscope or UART (set in platform config) */
#ifndef EHS_CONSOLE_UART
#define EHS_CONSOLE_UART          0
#endif

/* Pull in shared base constants (EHS_MAX_TOOLKITS, EHS_MAX_FUNCTION_BLOCKS,
 * etc.) — must come after any local overrides above. */
#include "../base_small/base_config.h"

#endif /* TARGET_CONFIG_H */
