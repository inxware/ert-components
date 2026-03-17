/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file sd_select.h
 * Function block declarations for dual SD-card slot selection.
 */

#ifndef _EHS_SD_SELECT_H
#define _EHS_SD_SELECT_H

#include "ehs_fb_types.h"
#include "inx-component.h"

#define EHS_FB_NAME_sd_select "sd_select"
#define EHS_FB_ID_sd_select   0x9A04

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(sd_select);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(sd_select);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(sd_select_select);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(sd_select_read_active);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(sd_select);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(sd_select)

#endif /* _EHS_SD_SELECT_H */
