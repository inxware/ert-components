/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_main.c
 * Main entry point for XMOS xcore.ai FreeRTOS targets.
 *
 * main() starts the FreeRTOS scheduler with EhsMain as the primary task.
 * The xcc compiler treats main() on tile[0] as the hardware entry point
 * when linking with fwk_rtos / rtos_support.
 *
 * @author: inx limited
 */

#define EHS_TARGET_CODE

#include <stdio.h>
#include "globals.h"
#include "ehs_main.h"
#include "targetos_init.h"
#include "hal-api.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"

/* EhsMain task stack depth in words.  Increase if stack overflows occur. */
#ifndef EHS_XCORE_MAIN_STACK_DEPTH
#define EHS_XCORE_MAIN_STACK_DEPTH 8192
#endif

/* EhsMain task priority.  Adjust relative to networking tasks as needed. */
#ifndef EHS_XCORE_MAIN_TASK_PRIORITY
#define EHS_XCORE_MAIN_TASK_PRIORITY 2
#endif

/**
 * Called by the EHS kernel to check whether the platform is ready to run.
 * The xcore target is always ready immediately.
 */
ehs_bool EhsTPlatformReady(void (*target_loop_iteration)(void *),
                            void *target_env_blob)
{
    (void)target_loop_iteration;
    (void)target_env_blob;
    return EHS_TRUE;
}

/**
 * Shared-memory initialisation stub — not used on xcore (single address space).
 */
void EhsTargetInitSharedMemory(ehs_bool bUniqueCheck)
{
    (void)bUniqueCheck;
}

/**
 * Hardware entry point.
 * Creates the ERT task and starts the FreeRTOS scheduler.
 */
int main(void)
{
    printf("ERT xcore starting\n");

#if defined(EHS_TEST_FUNC_OVERRIDE) && defined(EHS_TEST_FUNC_NO_ERT_INIT)
    /* Bare-metal test mode: run test function and hang */
    extern void EHS_TEST_FUNC_NAME(void);
    printf("EHS Bare Metal Test: running\n");
    EHS_TEST_FUNC_NAME();
    printf("Test completed\n");
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
#endif

#ifdef EHS_TEST_FUNC_OVERRIDE
    /* Full-init test mode: start test task instead of EhsMain */
    extern void EHS_TEST_FUNC_NAME(void);
    xTaskCreate((TaskFunction_t)EHS_TEST_FUNC_NAME,
                "EhsTest",
                EHS_XCORE_MAIN_STACK_DEPTH,
                NULL,
                EHS_XCORE_MAIN_TASK_PRIORITY,
                NULL);
#else
    /* Normal production mode */
    xTaskCreate((TaskFunction_t)EhsMain,
                "EhsMain",
                EHS_XCORE_MAIN_STACK_DEPTH,
                NULL,
                EHS_XCORE_MAIN_TASK_PRIORITY,
                NULL);
#endif

    vTaskStartScheduler();

    /* Should never reach here */
    for (;;) { }
    return 0;
}

/**
 * SIGTERM handler stub — called by EHS shutdown path on GNU targets;
 * retained for API compatibility.
 */
void EhsTargetHandleTerm(int sig)
{
    (void)sig;
    EhsExit(0);
}
