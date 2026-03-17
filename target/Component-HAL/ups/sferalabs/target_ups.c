/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file target_ups.c
 * @brief Sfera Labs sysfs UPS HAL implementation.
 *
 * Supported boards: Strato Pi (UPS variant).
 * Other Sfera Labs boards: ReadStatus returns 0 with on_battery=false;
 * RegisterCallbacks returns -1 (no UPS hardware).
 *
 * Sysfs paths (Strato Pi):
 *   /sys/class/stratopi/ups/battery_v     — integer millivolts
 *   /sys/class/stratopi/ups/power_source  — "ext" (mains) or "bat" (battery)
 *
 * Power-fail and power-restore detection uses sferalabs_poll, a shared
 * background thread that polls all registered sysfs nodes at
 * SFERALABS_POLL_INTERVAL_MS (250 ms) and fires a callback on any change.
 * The callback calls EhsCallbackQueue_execute() to notify the eRT scheduler.
 */

#include "globals.h"
#include "hal_ups.h"
#include "sferalabs_hal.h"
#include "sferalabs_poll.h"
#include "callback_queue.h"
#include "hal_logger.h"
#include <string.h>

#if defined(EHS_SFERALABS_BOARD_STRATOPI)

#define SFERALABS_UPS_BATTERY_V_PATH     SFERALABS_CLASS_PATH "/ups/battery_v"
#define SFERALABS_UPS_POWER_SOURCE_PATH  SFERALABS_CLASS_PATH "/ups/power_source"

/* ------------------------------------------------------------------ */
/*  Poll callback — fired by sferalabs_poll when power_source changes  */
/* ------------------------------------------------------------------ */

static void ups_power_source_changed(void       *ctx,
                                     const char *old_val,
                                     const char *new_val)
{
    ehs_ups_state_type *state = (ehs_ups_state_type *)ctx;

    if (strncmp(new_val, "bat", 3) == 0)
    {
        EHSH_LOG_WARNING("UPS: power fail detected (source: %s -> %s)",
                         old_val, new_val);
        state->on_battery = EHS_TRUE;
        EhsCallbackQueue_execute(state->pPowerFailQueue);
    }
    else if (strncmp(new_val, "ext", 3) == 0)
    {
        EHSH_LOG_INFO("UPS: power restored (source: %s -> %s)",
                      old_val, new_val);
        state->on_battery = EHS_FALSE;
        EhsCallbackQueue_execute(state->pPowerRestoreQueue);
    }
    else
    {
        EHSH_LOG_WARNING("UPS: unexpected power_source value '%s'", new_val);
    }
}

/* ------------------------------------------------------------------ */
/*  HAL API                                                             */
/* ------------------------------------------------------------------ */

EHS_GLOBAL int EhsTUpsReadStatus(ehs_ups_state_type *state)
{
    /* Read battery voltage in mV */
    long mv = 0;
    if (sferalabs_sysfs_read_int(SFERALABS_UPS_BATTERY_V_PATH, &mv) != 0)
    {
        EHSH_LOG_ERROR("UPS: failed to read battery_v at %s",
                       SFERALABS_UPS_BATTERY_V_PATH);
        return -1;
    }
    state->battery_mv = (ehs_sint32)mv;

    /* Read power source: "ext" = mains, "bat" = on battery */
    char buf[8];
    int fd = open(SFERALABS_UPS_POWER_SOURCE_PATH, O_RDONLY);
    if (fd < 0)
    {
        EHSH_LOG_ERROR("UPS: failed to open power_source at %s",
                       SFERALABS_UPS_POWER_SOURCE_PATH);
        return -1;
    }
    int n = (int)read(fd, buf, sizeof(buf) - 1);
    close(fd);
    if (n <= 0)
    {
        EHSH_LOG_ERROR("UPS: failed to read power_source");
        return -1;
    }
    buf[n] = '\0';
    state->on_battery = (strncmp(buf, "bat", 3) == 0) ? EHS_TRUE : EHS_FALSE;
    return 0;
}

EHS_GLOBAL int EhsTUpsRegisterCallbacks(ehs_ups_state_type *state)
{
    /* Register the power_source sysfs node with the shared poll thread.
     * sferalabs_poll_register is idempotent on duplicate paths, so it is
     * safe to call even if a second UPS instance is created (which the eRT
     * kernel prevents, but be defensive). */
    int rc = sferalabs_poll_register(SFERALABS_UPS_POWER_SOURCE_PATH,
                                     ups_power_source_changed,
                                     state);
    if (rc != 0)
    {
        /* Either duplicate (warn) or table full (error already logged) */
        EHSH_LOG_WARNING("UPS: sferalabs_poll_register returned %d", rc);
    }

    return sferalabs_poll_start();
}

#else /* board does not have UPS */

EHS_GLOBAL int EhsTUpsReadStatus(ehs_ups_state_type *state)
{
    if (state)
    {
        state->on_battery = EHS_FALSE;
        state->battery_mv = 0;
    }
    return 0;
}

EHS_GLOBAL int EhsTUpsRegisterCallbacks(ehs_ups_state_type *state)
{
    (void)state;
    EHSH_LOG_ERROR("UPS: not supported on this Sfera Labs board");
    return -1;
}

#endif /* board selection */
