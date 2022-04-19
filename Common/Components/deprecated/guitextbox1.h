/** 
 * @file guitextbox.h
 * This declares the guitextbox function blocks
 *
 * @author: inx limited
 * @version: $Revision: 1248 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#ifndef _EHS_GUITEXTBOX1_H
#define _EHS_GUITEXTBOX1_H

#include "ehs_fb_types.h"

/*****************************************************************************/
/* String textbox */

#define EHS_FB_GUI_TEXT1_STRING_NAME "gui_text_string1"

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(gui_text1);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(gui_text1);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text1_create);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text1_destroy);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text1_show);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text1_hide);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_string1_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_string1)

/*****************************************************************************/
/* "Real" textbox */

#ifdef EHS_TARGET_FP_SUPPORT
#define EHS_FB_GUI_TEXT1_FLOAT_NAME "gui_text_real1"

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_float1_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_float1)
#endif

/*****************************************************************************/
/* Integer textbox */

#define EHS_FB_GUI_TEXT1_INT_NAME "gui_text_int1"

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_int1_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_int1)

/*****************************************************************************/
/* Boolean textbox */

#define EHS_FB_GUI_TEXT1_BOOL_NAME "gui_text_bool1"

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_bool1_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_bool1)

#endif /* _EHS_GUITEXTBOX1_H */

