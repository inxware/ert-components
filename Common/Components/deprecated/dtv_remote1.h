/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * @file dtv_remote.h
 * This declares the remote control functions for the DTV toolkit.See
 * target-specific components directory for the implementation of this.
 *
 * @author: inx limited
 *
 */

#ifndef _EHS_DTV_REMOTE1_H
#define _EHS_DTV_REMOTE1_H

#include "ehs_fb_types.h"

#define EHS_FB_NAME_DTV_REMOTE1_NAME "DtvRemoteKey1"
#define EHS_FB_ID_DTV_REMOTE1_NAME 0xD03A

#define EHS_FB_NAME_DTV_REMOTE1_RUN_NAME "run"
#define EHS_FB_ID_DTV_REMOTE1_RUN_NAME 0x6707

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DtvRemote1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DtvRemote1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DtvRemote1);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DtvRemote1)

#endif /* _EHS_DTV_REMOTE1_H */

