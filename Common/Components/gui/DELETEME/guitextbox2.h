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

#ifndef _EHS_GUITEXTBOX2_H
#define _EHS_GUITEXTBOX2_H

#include "ehs_fb_types.h"

/*****************************************************************************/
/* String textbox */

#define EHS_FB_NAME_gui_text_string2 "gui_text_string2"
#define EHS_FB_ID_gui_text_string2 0x150B

/* declare functions provided by this function block */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(gui_widget);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(gui_widget);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_widget_create);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_widget_destroy);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_widget_show);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_widget_hide);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_widget_update);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_string2_data);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_string2)

/*****************************************************************************/
/* "Real" textbox */

//#ifdef EHS_TARGET_FP_SUPPORT
#define EHS_FB_NAME_gui_text_real2 "gui_text_real2"
#define EHS_FB_ID_gui_text_real2 0x5149

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_float2_data);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_float2)
//#endif

/*****************************************************************************/
/* Integer textbox */

#define EHS_FB_NAME_gui_text_int2 "gui_text_int2"
#define EHS_FB_ID_gui_text_int2 0x4709

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_int2_data);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_int2)

/*****************************************************************************/
/* Boolean textbox */

#define EHS_FB_NAME_gui_text_bool2 "gui_text_bool2"
#define EHS_FB_ID_gui_text_bool2 0x89EA

/* declare functions provided by this function block */
/* identify function, create function and destroy functions are common
   to all textbox functions */
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gui_text_bool2_data);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gui_text_bool2)

#endif /* _EHS_GUITEXTBOX2_H */

