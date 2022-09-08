/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * @file dtv_pvr_list.h
 * This declares the PVR functions for the DTV toolkit. See
 * target-specific components directory for the implementation of this.
 *
 * @author: inx limited
 *
 */

#ifndef _EHS_DTV_PVR_LIST_H
#define _EHS_DTV_PVR_LIST_H

#include "ehs_fb_types.h"

#define EHS_FB_NAME_FILESYSTEM_DIRLIST_NAME "DtvPvrList"
#define EHS_FB_ID_FILESYSTEM_DIRLIST_NAME 0xCE13


/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DtvPvrList);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DtvPvrList);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvPvrList);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DtvPvrList)

#endif /* _EHS_DTV_PVR_LIST_H */

