/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * target_usb_power.c — stubbed USB power HAL.
 * All operations are no-ops.  Selected when EHS_USB_POWER_SUPPORT=stubbed.
 */

#include "globals.h"
#include "hal_usb_power.h"

EHS_GLOBAL int EhsTUsbPowerEnable(ehs_usb_power_state_type *state)
{
    if (state)
        state->powered = EHS_TRUE;
    return 0;
}

EHS_GLOBAL int EhsTUsbPowerDisable(ehs_usb_power_state_type *state)
{
    if (state)
        state->powered = EHS_FALSE;
    return 0;
}

EHS_GLOBAL int EhsTUsbPowerReadStatus(ehs_usb_power_state_type *state)
{
    if (state)
        state->powered = EHS_TRUE;
    return 0;
}
