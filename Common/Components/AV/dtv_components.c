/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file dtv_components.c
 * Contains the static definition of the Digital TV block reference
 * table for EHS
 * @author: inx limited
 *
 * Copyright (c), inx limited, 2007. All rights reserved.
 */

#include "blockref_table.h"
#include "dtv_diag.h"
#include "dtv_pvr_play2.h"

/* @todo remove this - moved to media toolkit */
EhsBlockRefType EhsBlockRefTable_Dtv[] =
{
    /* dtv_pvr_play.h */
//#error Checking!
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_DtvPvrPlay2,EHS_FB_ID_DtvPvrPlay2, DtvPvrPlay2),
	/* dtv_pvr_list.h */
	/* dtv_remote2.h */
	/* dtv_diag.h */
//	EHS_BLOCKREF_ENTRY(EHS_FB_DTV_DIAGS_NAME, DtvDiagStr),
    {0}
};

