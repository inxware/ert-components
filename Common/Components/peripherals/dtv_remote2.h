/**
 * @file dtv_remote2.h
 * This declares the remote control functions for the DTV toolkit.See
 * target-specific components directory for the implementation of this.
 *
 * @author: inx limited
 * @version: $Revision: 1248 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _EHS_DTV_REMOTE2_H
#define _EHS_DTV_REMOTE2_H

#include "ehs_fb_types.h"

#define EHS_FB_DTV_REMOTE2_NAME "DtvRemoteKey2"
#define EHS_FB_DTV_REMOTE2_RUN_NAME "run"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DtvRemote2);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DtvRemote2);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvRemote); // FOR SOME REAson IT'S LIKE THIS
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvRemote_Enable);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvRemote_Disable);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DtvRemote2)

#endif /* _EHS_DTV_REMOTE2_H */

