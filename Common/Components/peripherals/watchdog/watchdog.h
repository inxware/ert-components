/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file watchdog.h
 * Function block declarations for the hardware watchdog timer.
 */

#ifndef _EHS_WATCHDOG_H
#define _EHS_WATCHDOG_H

#include "ehs_fb_types.h"
#include "inx-component.h"

#define EHS_FB_NAME_watchdog "watchdog"
#define EHS_FB_ID_watchdog   0x9A01

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(watchdog);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(watchdog);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(watchdog_enable);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(watchdog_disable);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(watchdog_kick);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(watchdog_read_status);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(watchdog);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(watchdog)

#endif /* _EHS_WATCHDOG_H */
