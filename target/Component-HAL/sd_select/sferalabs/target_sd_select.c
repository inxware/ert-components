/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file target_sd_select.c
 * @brief Sfera Labs sysfs SD slot selection HAL implementation.
 *
 * Supported boards: Strato Pi, Strato Pi Max.
 * All other Sfera Labs boards return EHS_PERIPH_ERR_NOT_SUPPORTED (-3).
 *
 * Sysfs path:
 *   /sys/class/[board]/sd_card/sd_main_enabled
 *     write "1" to select slot 0 (main SD), write "0" to select slot 1 (secondary)
 *     read to get current selection
 */

#include "globals.h"
#include "hal_sd_select.h"
#include "hal_peripheral_errors.h"
#include "sferalabs_hal.h"
#include "hal_logger.h"

#if defined(EHS_SFERALABS_BOARD_STRATOPI) || defined(EHS_SFERALABS_BOARD_STRATOPIMAX)

#define SFERALABS_SD_MAIN_ENABLED_PATH  SFERALABS_CLASS_PATH "/sd_card/sd_main_enabled"

EHS_GLOBAL int EhsTSdSelectSlot(ehs_sd_select_state_type *state, ehs_sint32 slot)
{
    /* slot 0 = main (write "1"), slot 1 = secondary (write "0") */
    const char *val = (slot == 0) ? "1" : "0";
    if (sferalabs_sysfs_write(SFERALABS_SD_MAIN_ENABLED_PATH, val, 1) != 0)
    {
        EHSH_LOG_ERROR("SD select: failed to write slot %d to %s", slot, SFERALABS_SD_MAIN_ENABLED_PATH);
        return EHS_PERIPH_ERR_SYSFS;
    }
    if (state)
        state->active_slot = slot;
    EHSH_LOG_INFO("SD select: selected slot %d", slot);
    return EHS_PERIPH_OK;
}

EHS_GLOBAL int EhsTSdReadActiveSlot(ehs_sd_select_state_type *state)
{
    char ch;
    if (sferalabs_sysfs_read_char(SFERALABS_SD_MAIN_ENABLED_PATH, &ch) != 0)
    {
        EHSH_LOG_ERROR("SD select: failed to read from %s", SFERALABS_SD_MAIN_ENABLED_PATH);
        return EHS_PERIPH_ERR_SYSFS;
    }
    /* "1" means main (slot 0) is active; "0" means secondary (slot 1) */
    if (state)
        state->active_slot = (ch == '1') ? 0 : 1;
    return EHS_PERIPH_OK;
}

#else /* board does not have dual SD select */

EHS_GLOBAL int EhsTSdSelectSlot(ehs_sd_select_state_type *state, ehs_sint32 slot)
{
    (void)state;
    (void)slot;
    EHSH_LOG_ERROR("SD select: not supported on this Sfera Labs board");
    return EHS_PERIPH_ERR_NOT_SUPPORTED;
}

EHS_GLOBAL int EhsTSdReadActiveSlot(ehs_sd_select_state_type *state)
{
    if (state)
        state->active_slot = 0;
    EHSH_LOG_ERROR("SD select: not supported on this Sfera Labs board");
    return EHS_PERIPH_ERR_NOT_SUPPORTED;
}

#endif /* board selection */
