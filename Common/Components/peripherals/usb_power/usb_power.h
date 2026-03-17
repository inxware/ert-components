/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file usb_power.h
 * Function block declarations for USB port power control.
 */

#ifndef _EHS_USB_POWER_H
#define _EHS_USB_POWER_H

#include "ehs_fb_types.h"
#include "inx-component.h"

#define EHS_FB_NAME_usb_power "usb_power"
#define EHS_FB_ID_usb_power   0x9A05

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(usb_power);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(usb_power);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(usb_power_enable);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(usb_power_disable);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(usb_power_read_status);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(usb_power);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(usb_power)

#endif /* _EHS_USB_POWER_H */
