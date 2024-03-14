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

#ifndef _EHS_GUIIMAGEFILE_H
#define _EHS_GUIIMAGEFILE_H

#include "ehs_fb_types.h"


#define EHS_FB_NAME_GUI_Image_File "GUI_Image_File"
#define EHS_FB_ID_GUI_Image_File 0xE8E1


/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(GUI_Image_File);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(GUI_Image_File);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_File_create);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_File_destroy);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_File_load);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_File_show);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_File_hide);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_Image_File_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(GUI_Image_File)
#endif /* _EHS_GUIIMAGEFILE_H */

