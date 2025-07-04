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

#ifndef _EHS_GUIPATCH_H
#define _EHS_GUIPATCH_H

#include "ehs_fb_types.h"

/*****************************************************************************/
/* patch */

#define EHS_FB_NAME_gui_patch "gui_patch"
#define EHS_FB_ID_gui_patch 0xB38B

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(gui_patch);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(gui_patch);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_patch_create);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_patch_destroy);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_patch_show);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_patch_hide);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_patch_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_patch)

#endif /* _EHS_GUIPATCH_H */

