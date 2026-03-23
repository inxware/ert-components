/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file target_gpio.c
 * @brief Sfera Labs GPIO HAL — digital I/O via sysfs.
 *
 * Accesses Sfera Labs hardware through the Linux sysfs virtual filesystem only.
 * No Sfera Labs kernel headers or libraries are used; this is pure POSIX file I/O.
 *
 * pin_id convention (all boards with relay + OC outputs):
 *   1 .. SFERALABS_DO_RELAY_COUNT         → relay digital outputs (o1, o2, ...)
 *   (SFERALABS_OC_PIN_OFFSET+1) and above → open-collector outputs (oc1, oc2, ...)
 *
 * Strato Pi Max only: pin_id = (slot << 8) | channel  (slot 1-4, channel 1-7)
 */

#include "globals.h"
#include "target_gpio.h"
#include "hal_logger.h"

/* -------------------------------------------------------------------------
 * Internal helpers
 * ------------------------------------------------------------------------- */

/**
 * Build the sysfs path for a digital output. Returns EHS_FALSE if the
 * pin_id is out of range or the board has no suitable output type.
 */
static ehs_bool build_output_path(char *buf, ehs_sint32 pin_id)
{
#if defined(EHS_SFERALABS_BOARD_STRATOPI)
    /* Single relay on pin_id 1, LED on pin_id 100 */
    if (pin_id == 1)
    {
        snprintf(buf, SFERALABS_SYSFS_BUF_SIZE, "%s", SFERALABS_DO_RELAY_FMT);
        return EHS_TRUE;
    }
    if (pin_id == 100)
    {
        snprintf(buf, SFERALABS_SYSFS_BUF_SIZE, "%s", SFERALABS_DO_LED_PATH);
        return EHS_TRUE;
    }
    return EHS_FALSE;

#elif defined(EHS_SFERALABS_BOARD_EXOSENSEPI)
    /* Single relay only */
    if (pin_id == 1)
    {
        snprintf(buf, SFERALABS_SYSFS_BUF_SIZE, "%s", SFERALABS_DO_RELAY_FMT);
        return EHS_TRUE;
    }
    return EHS_FALSE;

#elif defined(EHS_SFERALABS_BOARD_STRATOPIMAX)
    int slot = SFERALABS_SLOT_FROM_PINID(pin_id);
    int ch   = SFERALABS_CH_FROM_PINID(pin_id);
    if (slot < 1 || slot > 4 || ch < 1 || ch > SFERALABS_DO_RELAY_COUNT)
        return EHS_FALSE;
    snprintf(buf, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_DO_RELAY_FMT, slot, ch);
    return EHS_TRUE;

#else
    /* Iono Pi / Iono Pi Max: relay on 1..N, OC on (OC_OFFSET+1)..(OC_OFFSET+M) */
    if (pin_id >= 1 && pin_id <= SFERALABS_DO_RELAY_COUNT)
    {
        snprintf(buf, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_DO_RELAY_FMT, pin_id);
        return EHS_TRUE;
    }
#if SFERALABS_DO_OC_COUNT > 0
    int oc_ch = pin_id - SFERALABS_OC_PIN_OFFSET;
    if (oc_ch >= 1 && oc_ch <= SFERALABS_DO_OC_COUNT)
    {
        snprintf(buf, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_DO_OC_FMT, oc_ch);
        return EHS_TRUE;
    }
#endif
    return EHS_FALSE;
#endif
}

/**
 * Build the sysfs path for a digital input. Returns EHS_FALSE if out of range.
 * WARNING: THIS DOESN@T LIMIT THE PATH LENGTH TO THE BUFFER.
 */
static ehs_bool build_input_path(char *buf, ehs_sint32 pin_id)
{
#if defined(EHS_SFERALABS_BOARD_STRATOPI)
    /* Single button on any pin_id */
    snprintf(buf, SFERALABS_SYSFS_BUF_SIZE, "%s", SFERALABS_DI_FMT);
    return EHS_TRUE;

#elif defined(EHS_SFERALABS_BOARD_STRATOPIMAX)
    int slot = SFERALABS_SLOT_FROM_PINID(pin_id);
    int ch   = SFERALABS_CH_FROM_PINID(pin_id);
    if (slot < 1 || slot > 4 || ch < 1 || ch > SFERALABS_DI_COUNT)
        return EHS_FALSE;
    snprintf(buf, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_DI_FMT, slot, ch);
    return EHS_TRUE;

#else
    if (pin_id < 1 || pin_id > SFERALABS_DI_COUNT)
        return EHS_FALSE;
    snprintf(buf, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_DI_FMT, pin_id);
    return EHS_TRUE;
#endif
}

/* -------------------------------------------------------------------------
 * Public HAL API — output
 * ------------------------------------------------------------------------- */

EHS_GLOBAL ehs_bool EhsInitOutputGPIO(ehs_gpio_out_state_type *pGPIO)
{
    /* Sfera Labs outputs are always available via sysfs — no export step needed.
       Verify the path is reachable by attempting to write the initial state. */
    char path[SFERALABS_SYSFS_BUF_SIZE];
    if (!build_output_path(path, pGPIO->pin_id))
    {
        EHSH_LOG_ERROR("SferaLabs GPIO: pin_id %d is not a valid output", pGPIO->pin_id);
        return EHS_FALSE;
    }

    const char *val = (pGPIO->initial_state) ? "1" : "0";
    if (sferalabs_sysfs_write(path, val, 1) != 0)
    {
        EHSH_LOG_ERROR("SferaLabs GPIO: failed to init output pin_id %d at %s", pGPIO->pin_id, path);
        return EHS_FALSE;
    }

    EHSH_LOG_INFO("SferaLabs GPIO: init output pin_id %d -> %s", pGPIO->pin_id, path);
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsWriteOutputGPIO(ehs_gpio_out_state_type *pGPIO)
{
    char path[SFERALABS_SYSFS_BUF_SIZE];
    if (!build_output_path(path, pGPIO->pin_id))
    {
        EHSH_LOG_ERROR("SferaLabs GPIO: pin_id %d is not a valid output", pGPIO->pin_id);
        return EHS_FALSE;
    }

    const char *val = pGPIO->pin_value ? "1" : "0";
    if (sferalabs_sysfs_write(path, val, 1) != 0)
    {
        EHSH_LOG_ERROR("SferaLabs GPIO: failed to write pin_id %d", pGPIO->pin_id);
        return EHS_FALSE;
    }
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsDestroyOutputGPIO(ehs_gpio_out_state_type *pGPIO)
{
    /* Drive low on destroy for safety */
    char path[SFERALABS_SYSFS_BUF_SIZE];
    if (build_output_path(path, pGPIO->pin_id))
        sferalabs_sysfs_write(path, "0", 1);
    EHSH_LOG_INFO("SferaLabs GPIO: destroy output pin_id %d", pGPIO->pin_id);
    return EHS_TRUE;
}

/* -------------------------------------------------------------------------
 * Public HAL API — input
 * ------------------------------------------------------------------------- */

EHS_GLOBAL ehs_bool EhsInitInputGPIO(ehs_gpio_in_state_type *pGPIO)
{
    char path[SFERALABS_SYSFS_BUF_SIZE];
    if (!build_input_path(path, pGPIO->pin_id))
    {
        EHSH_LOG_ERROR("SferaLabs GPIO: pin_id %d is not a valid input", pGPIO->pin_id);
        printf("SferaLabs GPIO: pin_id %d is not a valid input\n", pGPIO->pin_id);
        return EHS_FALSE;
    }
    /* Verify readable */
    char ch;
    if (sferalabs_sysfs_read_char(path, &ch) != 0)
    {
        printf("SferaLabs GPIO: input pin_id %d not readable at %s\n", pGPIO->pin_id, path);
        EHSH_LOG_ERROR("SferaLabs GPIO: input pin_id %d not readable at %s", pGPIO->pin_id, path);
        return EHS_FALSE;
    }
    EHSH_LOG_INFO("SferaLabs GPIO: init input pin_id %d -> %s", pGPIO->pin_id, path);
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsReadInputGPIO(ehs_gpio_in_state_type *pGPIO)
{
    char path[SFERALABS_SYSFS_BUF_SIZE];
    if (!build_input_path(path, pGPIO->pin_id))
    {
        EHSH_LOG_ERROR("SferaLabs GPIO: pin_id %d is not a valid input", pGPIO->pin_id);
        return EHS_FALSE;
    }

    char ch;
    if (sferalabs_sysfs_read_char(path, &ch) != 0)
    {
        EHSH_LOG_ERROR("SferaLabs GPIO: failed to read input pin_id %d", pGPIO->pin_id);
        return EHS_FALSE;
    }

    pGPIO->pin_value = (ch == '1') ? EHS_TRUE : EHS_FALSE;
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsDestroyInputGPIO(ehs_gpio_in_state_type *pGPIO)
{
    EHSH_LOG_INFO("SferaLabs GPIO: destroy input pin_id %d", pGPIO->pin_id);
    return EHS_TRUE;
}
