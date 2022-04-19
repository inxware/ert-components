/** @file dtv_components.c
 * Contains the static definition of the Digital TV block reference table for EHS
 *
 * @author: inx limited
 * @version: $Revision: 3760 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006), $
 *
 * Copyright (c), inx limited, 2007. All rights reserved.
 */

#include "blockref_table.h"
#include "dtv_remote2.h"
#include "dtv_diag.h"
#include "dtv_pvr_play2.h"

/* @todo remove this - moved to media toolkit */
EhsBlockRefType EhsBlockRefTable_Dtv[] =
{
	/* dtv_pvr_play.h */
	//EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_DTV_PVR_PLAY_NAME, DtvPvrPlay2),
	/* dtv_pvr_list.h */
	/* dtv_remote2.h */
	/* dtv_diag.h */
//	EHS_BLOCKREF_ENTRY(EHS_FB_DTV_DIAGS_NAME, DtvDiagStr),
	{0}
};

