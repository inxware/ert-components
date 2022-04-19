/** @file networking_components.c
 * Contains the static definition of the function block reference table for EHS
 *
 * @author: inx limited
 * @version: $Revision: 1654 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006), $
 *
 * Copyright (c), inx limited, 2007. All rights reserved.
 */

#include "blockref_table.h"



#include "devman_player.h"
#include "url_get.h"

#ifdef EHS_COMPONENT_NETWORKING_SUPPORT

EhsBlockRefType EhsBlockRefTable_networking[] =
{
	/* devman_player.h */
#ifdef EHS_DEVMAN_SUPPORT
	EHS_BLOCKREF_ENTRY(EHS_FB_DEVMAN_PLAYER_NAME, DevmanPlayer),
#endif
	EHS_BLOCKREF_ENTRY("UrlGet",UrlGet),
	{0}
};

#endif /* EHS_COMPONENT_NETWORKING_SUPPORT */
