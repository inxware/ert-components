/** 
 * @file dtv_remote.h
 * This declares the remote control functions for the DTV toolkit.See
 * target-specific components directory for the implementation of this.
 *
 * @author: inx limited
 * @version: $Revision: 1248 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _EHS_DTV_REMOTE1_H
#define _EHS_DTV_REMOTE1_H

#include "ehs_fb_types.h"

#define EHS_FB_DTV_REMOTE1_NAME "DtvRemoteKey1"
#define EHS_FB_DTV_REMOTE1_RUN_NAME "run"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DtvRemote1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DtvRemote1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvRemote1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DtvRemote1)

#endif /* _EHS_DTV_REMOTE1_H */

