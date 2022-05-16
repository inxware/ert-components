/** @file core_components.c
 * Contains the static definition of the function block reference table for EHS
 * 
 * @author: inx limited
 * @version: $Revision: 1250 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006), $
 * 
 * Copyright (c), inx limited, 2007. All rights reserved.
 */

#include "blockref_table.h"
#include "keypress.h"

EhsBlockRefType EhsBlockRefTable_windows[] =
{
	/* keypress.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_KEYPRESS_NAME, windows_keypress),
	{0}
};

