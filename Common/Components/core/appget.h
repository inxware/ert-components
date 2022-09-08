/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/
/**
 * fileFunctions.h
 *
 * function prototypes for rtinfo.c - deals with all types
 *
 * @author: inx limited
 *
 */

#ifndef EHS_APPGET_FUNCTIONS_H
#define EHS_APPGET_FUNCTIONS_H
#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare the block entries */

#define EHS_FB_NAME_AppGet "AppGet"
#define EHS_FB_ID_AppGet 0x78CF


EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(appget);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(appget);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(appget_getinfo);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(appget_getapp);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(appget_list_remote);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(appget)

#endif /* EHS_RTINFO_FUNCTIONS_H */
