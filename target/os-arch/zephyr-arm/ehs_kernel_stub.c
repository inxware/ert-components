/***************************************************************
 * Copyright (C) 2025 inx limited, UK - All Rights Reserved
 * LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
 ***************************************************************/

/**
 * @file ehs_kernel_stub.c
 *
 * Placeholder implementations of the eRT kernel entry points for Zephyr ARM.
 *
 * These stubs exist because the eRT kernel (libehs_ehrt1.a) is not yet
 * available as a pre-built Zephyr ARM cross-compiled library.  They allow
 * the firmware image to link and boot so that the surrounding Zephyr HAL
 * layer can be tested.
 *
 * TODO: Remove this file once the real eRT kernel source is integrated into
 * the Zephyr CMake build or a pre-built arm-zephyr-eabi libehs_ehrt1.a is
 * available under ert-build-support/support_libs/target_libs/arm-zephyr/.
 */

#include <zephyr/kernel.h>
#include <stdio.h>
#include "globals.h"
#include "ehs_main.h"

/**
 * Stub: kernel initialisation.
 * The real EhsInit() sets up data tables, loads the boot app, and prepares
 * the kernel state machine.
 */
ehs_bool EhsInit(void)
{
    printf("[STUB] EhsInit called — real eRT kernel not yet available for Zephyr\n");
    return EHS_FALSE;
}

/**
 * Stub: main kernel loop.
 * The real EhsMain() runs the eRT event/scheduling loop and does not return.
 */
void EhsMain(Ehs_ConsoleCommand_Type (*target_loop_iteration)(void *),
             void *target_env_blob)
{
    (void)target_loop_iteration;
    (void)target_env_blob;
    printf("[STUB] EhsMain called — entering idle loop (real kernel not integrated)\n");
    while (1) {
        k_sleep(K_MSEC(1000));
    }
}
