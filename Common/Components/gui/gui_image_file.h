/** 
 * @file guiimage.h
 * This declares the guiimage (bitmap) function block
 *
 * @author: inx limited
 * @version: $Revision: 4412 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _EHS_GUIIMAGEFILE_H
#define _EHS_GUIIMAGEFILE_H

#include "ehs_fb_types.h"

#define EHS_FB_GUIIMAGEFILE_NAME "GUI_Image_File"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(GUI_ImageFile);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(GUI_ImageFile);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_ImageFile_create);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_ImageFile_destroy);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_ImageFile_load);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_ImageFile_show);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_ImageFile_hide);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(GUI_ImageFile_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(GUI_ImageFile)
#endif /* _EHS_GUIIMAGEFILE_H */

