/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file target_watchdog.c
 * @brief Sfera Labs sysfs watchdog HAL implementation.
 *
 * Supported boards: Strato Pi, Strato Pi Max.
 * All other Sfera Labs boards return -1 (watchdog not available).
 *
 * Sysfs paths (Strato Pi):
 *   /sys/class/stratopi/watchdog/enable   — write "1"/"0"
 *   /sys/class/stratopi/watchdog/timeout  — write integer (tenths of second)
 *   /sys/class/stratopi/watchdog/expired  — read "1"/"0"
 *
 * Sysfs paths (Strato Pi Max):
 *   /sys/class/stratopimax/watchdog/enable   — write "1"/"0"
 *   /sys/class/stratopimax/watchdog/timeout  — write integer (tenths of second)
 *   /sys/class/stratopimax/watchdog/expired  — read "1"/"0"
 */

#include "globals.h"
#include "hal_watchdog2.h"
#include "sferalabs_hal.h"
#include "hal_logger.h"

#if defined(EHS_SFERALABS_BOARD_STRATOPI) || defined(EHS_SFERALABS_BOARD_STRATOPIMAX)

#define SFERALABS_WDT_ENABLE_PATH  SFERALABS_CLASS_PATH "/watchdog/enable"
#define SFERALABS_WDT_TIMEOUT_PATH SFERALABS_CLASS_PATH "/watchdog/timeout"
#define SFERALABS_WDT_EXPIRED_PATH SFERALABS_CLASS_PATH "/watchdog/expired"

EHS_GLOBAL int EhsTWatchdogEnable2(ehs_watchdog_state_type *state)
{
    /* Timeout is in tenths of a second on the sysfs interface */
    long timeout_tenths = (long)(state->timeout_s * 10);
    if (sferalabs_sysfs_write_int(SFERALABS_WDT_TIMEOUT_PATH, timeout_tenths) != 0)
    {
        EHSH_LOG_ERROR("Watchdog: failed to set timeout at %s", SFERALABS_WDT_TIMEOUT_PATH);
        return -1;
    }
    if (sferalabs_sysfs_write(SFERALABS_WDT_ENABLE_PATH, "1", 1) != 0)
    {
        EHSH_LOG_ERROR("Watchdog: failed to enable at %s", SFERALABS_WDT_ENABLE_PATH);
        return -1;
    }
    EHSH_LOG_INFO("Watchdog: enabled, timeout %ld tenths-of-second", timeout_tenths);
    return 0;
}

EHS_GLOBAL int EhsTWatchdogDisable2(ehs_watchdog_state_type *state)
{
    (void)state;
    if (sferalabs_sysfs_write(SFERALABS_WDT_ENABLE_PATH, "0", 1) != 0)
    {
        EHSH_LOG_ERROR("Watchdog: failed to disable at %s", SFERALABS_WDT_ENABLE_PATH);
        return -1;
    }
    EHSH_LOG_INFO("Watchdog: disabled");
    return 0;
}

EHS_GLOBAL int EhsTWatchdogKick2(ehs_watchdog_state_type *state)
{
    (void)state;
    /* Kick by re-enabling (writing "1" resets the timer on Strato Pi boards) */
    if (sferalabs_sysfs_write(SFERALABS_WDT_ENABLE_PATH, "1", 1) != 0)
    {
        EHSH_LOG_ERROR("Watchdog: failed to kick at %s", SFERALABS_WDT_ENABLE_PATH);
        return -1;
    }
    return 0;
}

EHS_GLOBAL int EhsTWatchdogReadStatus2(ehs_watchdog_state_type *state)
{
    char ch;
    if (sferalabs_sysfs_read_char(SFERALABS_WDT_EXPIRED_PATH, &ch) != 0)
    {
        EHSH_LOG_ERROR("Watchdog: failed to read expired status at %s", SFERALABS_WDT_EXPIRED_PATH);
        return -1;
    }
    state->expired = (ch == '1') ? EHS_TRUE : EHS_FALSE;
    return 0;
}

#else /* board does not have watchdog */

EHS_GLOBAL int EhsTWatchdogEnable2(ehs_watchdog_state_type *state)
{
    (void)state;
    EHSH_LOG_ERROR("Watchdog: not supported on this Sfera Labs board");
    return -1;
}

EHS_GLOBAL int EhsTWatchdogDisable2(ehs_watchdog_state_type *state)
{
    (void)state;
    EHSH_LOG_ERROR("Watchdog: not supported on this Sfera Labs board");
    return -1;
}

EHS_GLOBAL int EhsTWatchdogKick2(ehs_watchdog_state_type *state)
{
    (void)state;
    EHSH_LOG_ERROR("Watchdog: not supported on this Sfera Labs board");
    return -1;
}

EHS_GLOBAL int EhsTWatchdogReadStatus2(ehs_watchdog_state_type *state)
{
    if (state)
        state->expired = EHS_FALSE;
    EHSH_LOG_ERROR("Watchdog: not supported on this Sfera Labs board");
    return -1;
}

#endif /* board selection */
