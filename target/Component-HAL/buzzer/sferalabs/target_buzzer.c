/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file target_buzzer.c
 * @brief Sfera Labs sysfs buzzer HAL implementation.
 *
 * Supported boards: Exo Sense Pi only.
 * All other Sfera Labs boards return -1.
 *
 * Sysfs paths (Exo Sense Pi):
 *   /sys/class/exosensepi/buzzer/status — write "on"/"off", read "on"/"off"
 *
 * Note: The Exo Sense Pi buzzer is a fixed-tone hardware buzzer.
 * The freq_hz and duration_ms parameters are accepted by the API but are
 * ignored on this board — the buzzer emits at its fixed frequency until
 * explicitly stopped.
 */

#include "globals.h"
#include "hal_buzzer.h"
#include "sferalabs_hal.h"
#include "hal_logger.h"

#if defined(EHS_SFERALABS_BOARD_EXOSENSEPI)

#define SFERALABS_BUZZER_STATUS_PATH  SFERALABS_CLASS_PATH "/buzzer/status"

EHS_GLOBAL int EhsTBuzzerBeep(ehs_buzzer_state_type *state, ehs_sint32 freq_hz, ehs_sint32 duration_ms)
{
    /*
     * freq_hz and duration_ms are ignored: Exo Sense Pi has a fixed-tone
     * hardware buzzer controlled only by on/off via sysfs.
     */
    (void)state;
    (void)freq_hz;
    (void)duration_ms;

    if (sferalabs_sysfs_write(SFERALABS_BUZZER_STATUS_PATH, "on", 2) != 0)
    {
        EHSH_LOG_ERROR("Buzzer: failed to write 'on' to %s", SFERALABS_BUZZER_STATUS_PATH);
        return -1;
    }
    return 0;
}

EHS_GLOBAL int EhsTBuzzerStop(ehs_buzzer_state_type *state)
{
    (void)state;
    if (sferalabs_sysfs_write(SFERALABS_BUZZER_STATUS_PATH, "off", 3) != 0)
    {
        EHSH_LOG_ERROR("Buzzer: failed to write 'off' to %s", SFERALABS_BUZZER_STATUS_PATH);
        return -1;
    }
    return 0;
}

#else /* board does not have a buzzer */

EHS_GLOBAL int EhsTBuzzerBeep(ehs_buzzer_state_type *state, ehs_sint32 freq_hz, ehs_sint32 duration_ms)
{
    (void)state;
    (void)freq_hz;
    (void)duration_ms;
    EHSH_LOG_ERROR("Buzzer: not supported on this Sfera Labs board");
    return -1;
}

EHS_GLOBAL int EhsTBuzzerStop(ehs_buzzer_state_type *state)
{
    (void)state;
    EHSH_LOG_ERROR("Buzzer: not supported on this Sfera Labs board");
    return -1;
}

#endif /* board selection */
