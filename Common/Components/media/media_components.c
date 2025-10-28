/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file media_components.c
 * Contains the static definition of the PlayManager reference table for EHS
 *
 * @author: inx limited
 *
 */

#include "blockref_table.h"
#ifdef EHS_VIDEO_SUPPORT
#include "dtv_pvr_play2.h"
#endif
#include "playManager.h"

EHS_C_CPP_EXPORT const EhsBlockRefType EhsBlockRefTable_Media[] =
{
#ifdef EHS_VIDEO_SUPPORT
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_DtvPvrPlay2, EHS_FB_ID_DtvPvrPlay2,DtvPvrPlay2),
#endif
	/* playManager.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_PlayManager,EHS_FB_ID_PlayManager, PlayManager),
	{0}
};

