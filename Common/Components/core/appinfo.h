/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * fileFunctions.h
 *
 * function prototypes for rtinfo.c - deals with all types
 *
 * @author: inx limited
 *
 */

#ifndef EHS_APPINFO_FUNCTIONS_H
#define EHS_APPINFO_FUNCTIONS_H
#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare the block entries */
#define EHS_FB_NAME_AppInfo "AppInfo"
#define EHS_FB_ID_AppInfo 0xF207
#

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(appinfo);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(appinfo);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(appinfo_getinfo);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(appinfo)

#endif /* EHS_RTINFO_FUNCTIONS_H */
