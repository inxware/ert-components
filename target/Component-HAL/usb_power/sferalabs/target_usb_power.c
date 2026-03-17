/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file target_usb_power.c
 * @brief Sfera Labs sysfs USB power control HAL implementation.
 *
 * Supported boards: Strato Pi Max only.
 * All other Sfera Labs boards return -1.
 *
 * Sysfs path (Strato Pi Max):
 *   /sys/class/stratopimax/usb/usb<N>/enabled — write "1"/"0", read "1"/"0"
 *   where N is state->port_num.
 */

#include "globals.h"
#include "hal_usb_power.h"
#include "sferalabs_hal.h"
#include "hal_logger.h"

#if defined(EHS_SFERALABS_BOARD_STRATOPIMAX)

EHS_GLOBAL int EhsTUsbPowerEnable(ehs_usb_power_state_type *state)
{
    char path[SFERALABS_SYSFS_BUF_SIZE];
    snprintf(path, sizeof(path),
             SFERALABS_CLASS_PATH "/usb/usb%d/enabled", (int)state->port_num);
    if (sferalabs_sysfs_write(path, "1", 1) != 0)
    {
        EHSH_LOG_ERROR("USB power: failed to enable port %d at %s", state->port_num, path);
        return -1;
    }
    state->powered = EHS_TRUE;
    EHSH_LOG_INFO("USB power: enabled port %d", state->port_num);
    return 0;
}

EHS_GLOBAL int EhsTUsbPowerDisable(ehs_usb_power_state_type *state)
{
    char path[SFERALABS_SYSFS_BUF_SIZE];
    snprintf(path, sizeof(path),
             SFERALABS_CLASS_PATH "/usb/usb%d/enabled", (int)state->port_num);
    if (sferalabs_sysfs_write(path, "0", 1) != 0)
    {
        EHSH_LOG_ERROR("USB power: failed to disable port %d at %s", state->port_num, path);
        return -1;
    }
    state->powered = EHS_FALSE;
    EHSH_LOG_INFO("USB power: disabled port %d", state->port_num);
    return 0;
}

EHS_GLOBAL int EhsTUsbPowerReadStatus(ehs_usb_power_state_type *state)
{
    char path[SFERALABS_SYSFS_BUF_SIZE];
    snprintf(path, sizeof(path),
             SFERALABS_CLASS_PATH "/usb/usb%d/enabled", (int)state->port_num);
    char ch;
    if (sferalabs_sysfs_read_char(path, &ch) != 0)
    {
        EHSH_LOG_ERROR("USB power: failed to read status for port %d at %s", state->port_num, path);
        return -1;
    }
    state->powered = (ch == '1') ? EHS_TRUE : EHS_FALSE;
    return 0;
}

#else /* board does not have USB power control */

EHS_GLOBAL int EhsTUsbPowerEnable(ehs_usb_power_state_type *state)
{
    (void)state;
    EHSH_LOG_ERROR("USB power: not supported on this Sfera Labs board");
    return -1;
}

EHS_GLOBAL int EhsTUsbPowerDisable(ehs_usb_power_state_type *state)
{
    (void)state;
    EHSH_LOG_ERROR("USB power: not supported on this Sfera Labs board");
    return -1;
}

EHS_GLOBAL int EhsTUsbPowerReadStatus(ehs_usb_power_state_type *state)
{
    if (state)
        state->powered = EHS_FALSE;
    EHSH_LOG_ERROR("USB power: not supported on this Sfera Labs board");
    return -1;
}

#endif /* board selection */
