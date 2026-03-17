/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_usb_power.h
 * Hardware abstraction layer for USB port power control.
 *
 * @author: inx limited
 */

#ifndef _HAL_USB_POWER_H
#define _HAL_USB_POWER_H

#include "globals.h"

typedef struct
{
    ehs_sint32 port_num;
    ehs_bool   powered;
} ehs_usb_power_state_type;

/** Enable power on USB port state->port_num.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTUsbPowerEnable(ehs_usb_power_state_type *state);

/** Disable power on USB port state->port_num.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTUsbPowerDisable(ehs_usb_power_state_type *state);

/** Read power status of USB port state->port_num into state->powered.
 *  Returns 0 on success, -1 on error. */
EHS_GLOBAL int EhsTUsbPowerReadStatus(ehs_usb_power_state_type *state);

#endif /* _HAL_USB_POWER_H */
