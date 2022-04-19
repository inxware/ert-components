/**
 * fileFunctions.h
 *
 * function prototypes for rtinfo.c - deals with all types
 *
 * @author: MDD
 * @version: $Revision: 971 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef EHS_APPGET_FUNCTIONS_H
#define EHS_APPGET_FUNCTIONS_H
#include "ehs_fb_types.h"

/******************************************************************************/
/* Declare the block entries */

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(appget);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(appget);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(appget_getinfo);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(appget_getapp);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(appget_list_remote);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(appget)

#endif /* EHS_RTINFO_FUNCTIONS_H */
