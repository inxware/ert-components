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
 * Perform necessary Operating system setup upon system initialisation.
 * Initialises mutexes and any Zephyr-specific subsystems.
 */
EHS_GLOBAL void EhsTOsSys_init(void)
{
    EhsTPMutex_init();

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
