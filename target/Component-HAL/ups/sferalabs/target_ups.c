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
 * Other boards: ReadStatus returns 0 with on_battery=false; callbacks return -1.
 *
 * Sysfs paths (Strato Pi):
 *   /sys/class/stratopi/ups/battery_v     — read millivolts
 *   /sys/class/stratopi/ups/power_source  — read "ext" (mains) or "bat" (battery)
 *
 * Note: Power-fail/restore callbacks require a background polling thread for a
 * production implementation.  This implementation registers the callbacks but
 * does NOT start a thread — the callbacks will therefore never fire unless a
 * platform-specific polling mechanism is added separately.
 */

#include "globals.h"
#include "hal_ups.h"
#include "sferalabs_hal.h"
#include "hal_logger.h"
#include <string.h>

#if defined(EHS_SFERALABS_BOARD_STRATOPI)

#define SFERALABS_UPS_BATTERY_V_PATH     SFERALABS_CLASS_PATH "/ups/battery_v"
#define SFERALABS_UPS_POWER_SOURCE_PATH  SFERALABS_CLASS_PATH "/ups/power_source"

EHS_GLOBAL int EhsTUpsReadStatus(ehs_ups_state_type *state)
{
    /* Read battery voltage in mV */
    long mv = 0;
    if (sferalabs_sysfs_read_int(SFERALABS_UPS_BATTERY_V_PATH, &mv) != 0)
    {
        EHSH_LOG_ERROR("UPS: failed to read battery_v at %s", SFERALABS_UPS_BATTERY_V_PATH);
        return -1;
    }
    state->battery_mv = (ehs_sint32)mv;

    /* Read power source: "ext" = mains, "bat" = on battery */
    char buf[8];
    int fd = open(SFERALABS_UPS_POWER_SOURCE_PATH, O_RDONLY);
    if (fd < 0)
    {
        EHSH_LOG_ERROR("UPS: failed to open power_source at %s", SFERALABS_UPS_POWER_SOURCE_PATH);
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
    /*
     * A real implementation would start a background polling thread here
     * that calls state->on_fail_cb(state->cb_ctx) when the power source
     * transitions to "bat", and state->on_restore_cb(state->cb_ctx) when it
     * transitions back to "ext".
     *
     * This sysfs-only implementation stores the callbacks but does not start
     * a thread, so the callbacks will not fire automatically.
     */
    EHSH_LOG_INFO("UPS: callbacks registered (no background thread started — polling not implemented)");
    (void)state;
    return 0;
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
