/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_main.c
 * This file represents the main entry point for the Zephyr RTOS target.
 * Zephyr's main() thread is used to initialise and run the EHS kernel.
 *
 * @author: inx limited
 *
 */

#define EHS_TARGET_CODE

/*****************************************************************************/
/* Included files */
#include <zephyr/kernel.h>
#include <stdio.h>

#include "globals.h"

#ifdef EHS_DEBUG_TCPIP_CONSOLE
  #include "console_server.h"
#endif

#include "ehs_main.h"
#include "targetos_init.h"
#include "hal-api.h"

#ifdef EHS_MQTT_SUPPORT
#include "hal_mqtt.h"
#endif

#include "hal_logger.h"

#ifndef TAG
#define TAG "target_main"
#endif

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/* Stack size for the EHS main kernel thread */
#define EHS_MAIN_THREAD_STACK_SIZE 8192

/* Priority for the EHS main kernel thread (preemptive) */
#define EHS_MAIN_THREAD_PRIORITY 5

/*****************************************************************************/
/* Forward declarations */

static void ehs_main_thread_entry(void *p1, void *p2, void *p3);

/*****************************************************************************/
/* Thread stack and data for the EHS kernel thread */

K_THREAD_STACK_DEFINE(ehs_main_stack, EHS_MAIN_THREAD_STACK_SIZE);
static struct k_thread ehs_main_thread_data;

/*****************************************************************************/
/* Zephyr main entry point */

/**
 * Zephyr main() - runs in the main thread context.
 *
 * Boot sequence:
 * 1. Print boot banner
 * 2. Initialise EHS OS layer (mutexes, etc.)
 * 3. Spawn the EHS kernel thread
 * 4. Main thread becomes idle (or can be used for console/shell)
 *
 * Note: On Zephyr, main() runs as a cooperative thread. We spawn
 * a separate preemptive thread for the EHS kernel to allow proper
 * scheduling with other Zephyr threads and ISRs.
 */
int main(void)
{
    printf("\n--- eRT Zephyr Target Boot ---\n");

    /* Initialise the OS abstraction layer */
    EhsTOsSys_init();

    /* Create and start the EHS kernel thread */
    k_thread_create(&ehs_main_thread_data, ehs_main_stack,
                    K_THREAD_STACK_SIZEOF(ehs_main_stack),
                    ehs_main_thread_entry,
                    NULL, NULL, NULL,
                    EHS_MAIN_THREAD_PRIORITY, 0, K_NO_WAIT);
    k_thread_name_set(&ehs_main_thread_data, "ehs_main");

    printf("EHS main thread started\n");

    /* Main thread can now be used for console/shell or just return.
     * On Zephyr, returning from main() is allowed - the idle thread
     * keeps the system alive. */

    /* TODO: Add serial console command loop here if EHS_HAL_INTERFACE_CONFIG_SUPPORT
     * is enabled (similar to ESP32 target_main.c command_prompt_task) */

    return 0;
}

/**
 * EHS kernel thread entry point.
 * Calls EhsInit() to initialise the kernel, then runs EhsLoop() forever.
 * This mirrors the pattern in the ESP32 target where app_main() spawns
 * the EhsMain task.
 */
static void ehs_main_thread_entry(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    printf("EHS kernel initialising...\n");

    /* Initialise the EHS kernel */
    EhsInit();

    printf("EHS kernel running\n");

    /* Run the EHS main loop - this should not return */
    while (1) {
        EhsLoop();
    }
}
