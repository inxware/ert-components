/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * @file guiimage.h
 * This declares the guiimage (bitmap) function block
 *
 * @author: inx limited
 *
 */

#ifndef _EHS_GUIIMAGE_H
#define _EHS_GUIIMAGE_H

#include "ehs_fb_types.h"

#define EHS_FB_NAME_GUIIMAGE_NAME "GUI_Image"
#define EHS_FB_ID_GUIIMAGE_NAME 0x3C7C


/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(GUI_Image);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(GUI_Image);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_create);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_destroy);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_show);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_hide);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(GUI_Image)
#endif /* _EHS_GUIIMAGE_H */

