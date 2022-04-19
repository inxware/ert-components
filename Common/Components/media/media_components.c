/** @file media_components.c
 * Contains the static definition of the PlayManager reference table for EHS
 *
 * @author: inx limited
 * @version: $Revision: 1650 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006), $
 *
 * Copyright (c), inx limited, 2007. All rights reserved.
 */

#include "blockref_table.h"
#ifdef EHS_VIDEO_SUPPORT
#include "dtv_pvr_play2.h"
#endif
#include "playManager.h"

EhsBlockRefType EhsBlockRefTable_Media[] =
{
#ifdef EHS_VIDEO_SUPPORT
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_DTV_PVR_PLAY_NAME, DtvPvrPlay2),
#endif
	/* playManager.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_MEDIA_PLAYMANAGER_NAME, PlayManager),
	{0}
};

