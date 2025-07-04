/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file networking_components.c
 * Contains the static definition of the function block reference table for EHS
 *
 * @author: inx limited
 *
 */

#include "blockref_table.h"



#include "devman_player.h"
#include "url_get.h"
#include "inx-network_config.h"

#ifdef EHS_COMPONENT_NETWORKING_SUPPORT

extern const EhsBlockRefType EhsBlockRefTable_networking[] =
{
    /* devman_player.h */
#ifdef EHS_DEVMAN_SUPPORT
#ifndef EHS_COMPONENTS_NETWORK_DEVMAN_PLAYER__NONE
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DevmanPlayer,EHS_FB_ID_DevmanPlayer, DevmanPlayer),
#endif
#endif // EHS_DEVMAN_SUPPORT

#ifndef EHS_COMPONENTS_NETWORK_URL_GET__NONE
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_UrlGet,EHS_FB_ID_UrlGet,UrlGet),
#endif

#ifndef EHS_COMPONENTS_NETWORK_CONFIG_SUPPORT__NONE
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_network_config,INXWARE_FB_ID_network_config,network_config),
#endif
	{0}
};

#endif /* EHS_COMPONENT_NETWORKING_SUPPORT */
