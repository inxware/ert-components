/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file rs485_config.h
 * Function block declarations for RS-485 port configuration.
 */

#ifndef _EHS_RS485_CONFIG_H
#define _EHS_RS485_CONFIG_H

#include "ehs_fb_types.h"
#include "inx-component.h"

#define EHS_FB_NAME_rs485_config "rs485_config"
#define EHS_FB_ID_rs485_config   0x2F4D

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(rs485_config);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(rs485_config);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(rs485_config_config);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(rs485_config_read);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(rs485_config);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(rs485_config)

#endif /* _EHS_RS485_CONFIG_H */
