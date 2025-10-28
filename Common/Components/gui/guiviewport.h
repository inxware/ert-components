/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * @file guitextbox.h
 * This declares the guitextbox function blocks
 *
 * @author: inx limited
 *
 */

#ifndef _EHS_GUIVIEWPORT_H
#define _EHS_GUIVIEWPORT_H

#include "ehs_fb_types.h"

/*****************************************************************************/
/* patch */


#define EHS_FB_NAME_gui_viewport "gui_viewport"
#define EHS_FB_ID_gui_viewport 0x882D

/* declare functions provided by this function block */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(gui_viewport);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(gui_viewport);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(gui_viewport);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_viewport_create);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_viewport_destroy);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_viewport_show);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_viewport_hide);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(gui_viewport_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_viewport)

#endif /* _EHS_GUIVIEWPORT_H */

