/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * @file guitextbox.h
 * This declares the guitextbox function blocks
 *
 * @author: inx limited
 *
 */

#ifndef _EHS_GUITEXTBOX1_H
#define _EHS_GUITEXTBOX1_H

#include "ehs_fb_types.h"

/*****************************************************************************/
/* String textbox */

#define EHS_FB_NAME_gui_text_string1 "gui_text_string1"
#define EHS_FB_ID_gui_text_string1 0x144B


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
#define EHS_FB_NAME_gui_text_real1 "gui_text_real1"
#define EHS_FB_ID_gui_text_real1E 0x5009

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_float1_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_float1)
#endif

/*****************************************************************************/
/* Integer textbox */

#define EHS_FB_NAME_gui_text_int1 "gui_text_int1"
#define EHS_FB_ID_gui_text_int1 0x4649

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_int1_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_int1)

/*****************************************************************************/
/* Boolean textbox */

#define EHS_FB_NAME_gui_text_bool1 "gui_text_bool1"
#define EHS_FB_ID_gui_text_bool1 0x88AA

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_bool1_update);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_bool1)

#endif /* _EHS_GUITEXTBOX1_H */

