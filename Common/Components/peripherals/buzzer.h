/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file buzzer.h
 * Function block declarations for the buzzer peripheral.
 */

#ifndef _EHS_BUZZER_H
#define _EHS_BUZZER_H

#include "ehs_fb_types.h"
#include "inx-component.h"

#define EHS_FB_NAME_buzzer "buzzer"
#define EHS_FB_ID_buzzer   0x3D16

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(buzzer);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(buzzer);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(buzzer_beep);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(buzzer_stop);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(buzzer);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(buzzer)

#endif /* _EHS_BUZZER_H */
