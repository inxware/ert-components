/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file ups.h
 * Function block declarations for the UPS / power-fail monitor.
 */

#ifndef _EHS_UPS_H
#define _EHS_UPS_H

#include "ehs_fb_types.h"
#include "inx-component.h"

#define EHS_FB_NAME_ups "ups"
#define EHS_FB_ID_ups   0x9A02

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ups);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ups);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ups_read_status);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ups_power_fail_cb);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ups_power_restore_cb);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(ups);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ups)

#endif /* _EHS_UPS_H */
