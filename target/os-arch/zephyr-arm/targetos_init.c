/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file targetos_init.c
 * Initialization/reset/termination code for the Zephyr RTOS target.
 *
 * @author: inx limited
 *
 */

/*****************************************************************************/
/* Included files */
#define EHS_TARGET_CODE /* Ensure header files include target-internal values */

#include "globals.h"
#include "hal.h"
#include "hal_string.h"
#include "hal_logger.h"
#include "hal_time.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>

#include <stdio.h>
#include <string.h>

#ifndef TAG
#define TAG "targetos_init"
#endif

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */

/**
 * Allocate the backing store for the Lucid console's ring buffers.
 *
 * EhsConsoleQueueType holds a POINTER (xQueue) rather than an inline array, and
 * neither EhsConsoleQueue_reset() nor the bsdsockets EhsTCommsSys_init() ever
 * allocates it - every target does this itself in its own targetos_init.c
 * (esp32/esp32s3, nxp-redlib, arduino, qnx all have the identical block). Miss
 * it and the console dereferences NULL the first time the queue is measured:
 * EhsConsoleQueue_push() happens to guard against a NULL queue, but
 * EhsConsoleQueue_length()/_space() do not. Nothing catches this at build time.
 */
#ifdef EHS_DEBUG_TCPIP_CONSOLE
    #include "console_queue.h"
    #include "target_mem.h"

    extern EhsConsoleQueueType EhsTgtConsoleInputQueue;
    extern EhsConsoleQueueType EhsTgtConsoleOutputQueue;

    static void EhsTOS_ConsoleQueue_init(void)
    {
        /* MUST be idempotent. Unlike the other targets, this port calls
         * EhsTOsSys_init() twice: once from main() at boot (so the Wi-Fi HAL
         * has mutexes before the kernel exists) and again from hal.c's
         * EhsHSys_init(). Allocating unconditionally would leak the first pair
         * of buffers and swap them out from under a console server that may
         * already hold the old pointers. */
        if (EhsTgtConsoleInputQueue.xQueue != NULL && EhsTgtConsoleOutputQueue.xQueue != NULL)
        {
            return;
        }

        /* EhsTMem_alloc is plain malloc here, so this is safe on the early
         * call too - it does not depend on EhsHMem_init() having run. */
        if (EhsTgtConsoleInputQueue.xQueue == NULL)
        {
            EhsTgtConsoleInputQueue.xQueue  = (ehs_uint8*)EhsTMem_alloc(EHS_DEBUG_CONSOLE_BUFFER_SIZE);
        }
        if (EhsTgtConsoleOutputQueue.xQueue == NULL)
        {
            EhsTgtConsoleOutputQueue.xQueue = (ehs_uint8*)EhsTMem_alloc(EHS_DEBUG_CONSOLE_BUFFER_SIZE);
        }

        if (EhsTgtConsoleInputQueue.xQueue == NULL || EhsTgtConsoleOutputQueue.xQueue == NULL)
        {
            /* Out of heap. Say so rather than faulting later inside the console
             * server thread, where the backtrace points at the queue code and
             * not at the real cause. */
            EHSH_LOG_ERROR("Console queue allocation failed (%d bytes x2) - "
                           "the Lucid console will not work",
                           (int)EHS_DEBUG_CONSOLE_BUFFER_SIZE);
        }
    }
#else
    static void EhsTOS_ConsoleQueue_init(void)
    {
    }
#endif /* EHS_DEBUG_TCPIP_CONSOLE */

/**
 * Perform necessary Operating system setup upon system initialisation.
 * Initialises mutexes and any Zephyr-specific subsystems.
 */
EHS_GLOBAL void EhsTOsSys_init(void)
{
    EhsTPMutex_init();

    EhsTOS_ConsoleQueue_init();

    /* TODO: Add Zephyr-specific init here:
     * - NVS / settings subsystem init
     * - RTC init if hardware supports it
     * - Watchdog configuration
     */
}

/**
 * Called to shutdown the target prior to shutting down EHS
 */
EHS_GLOBAL void EhsTOsSys_term(void)
{
    EhsTPMutex_term();
}

/**
 * Perform setup before loading in a new application
 */
EHS_GLOBAL void EhsTOsApp_init(void)
{
    /* No per-app init needed on MCU targets currently */
}

/**
 * Called to clean up after the execution of an application
 */
EHS_GLOBAL void EhsTOsApp_term(void)
{
    /* No per-app teardown needed on MCU targets currently */
}

/**
 * Perform necessary target setup per application execution
 */
EHS_GLOBAL void EhsTOsApp_reset(void)
{
    /* No per-app reset needed on MCU targets currently */
}

/**
 * Update environment metadata (MAC, IP, RAM usage, etc.)
 *
 * @param pEhsMetaData Pointer to metadata storage structure
 * @param what 0: everything, 1: static only, 2: dynamic only
 * @return EHS_TRUE on success
 */
EHS_GLOBAL ehs_bool EhsTOsSys_UpdateEnvironment(EhsMetaDataType *pEhsMetaData,
        ehs_uint8 what)
{
    if (pEhsMetaData == NULL) {
        return EHS_FALSE;
    }

    /* Static information (what == 0 or 1) */
    if (what == 0 || what == 1) {
        /* TODO: Populate device-specific static metadata:
         * - Device unique ID (from Zephyr hwinfo API)
         * - MAC address (if networking enabled)
         * - Firmware version string
         */
    }

    /* Dynamic information (what == 0 or 2) */
    if (what == 0 || what == 2) {
        /* TODO: Populate dynamic metadata:
         * - Free heap: k_mem_slab or k_heap stats
         * - Uptime: k_uptime_get()
         * - CPU temperature (if sensor available via Zephyr sensor API)
         */
    }

    return EHS_TRUE;
}

/**
 * Function to add EHS's canonical bin to the search path.
 * No-op on MCU targets (no filesystem PATH concept).
 */
void EhsBinSearchPath(void)
{
    /* Not applicable on Zephyr MCU targets */
}
