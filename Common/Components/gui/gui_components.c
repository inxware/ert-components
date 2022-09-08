/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/
/** @file core_components.c
 * Contains the static definition of the function block reference table for EHS
 *
 * @author: inx limited
 *
 */

#include "blockref_table.h"
#ifdef EHS_GUI_SUPPORT
#include "gui_image_file.h"
#include "guitextbox2.h"
#include "guipatch.h"
#include "gui_image_file.h"
#include "guiviewport.h"
//#include "guitextdisplay.h"
#include "guileds.h"

EhsBlockRefType EhsBlockRefTable_coreGui[] =
{
	/* guipatch.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_gui_patch,EHS_FB_ID_gui_patch, gui_patch),
	/* gui_image_file.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_GUI_Image_File,EHS_FB_ID_GUI_Image_File, GUI_ImageFile),
	/* guitextbox.h */
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_string2,EHS_FB_ID_gui_text_string2, gui_text_string2,EHS_FB_INIT_NAME(gui_text2),EHS_FB_IDENTIFY_NAME(gui_text2)),
#ifdef EHS_TARGET_FP_SUPPORT
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_real2,EHS_FB_ID_gui_text_real2, gui_text_float2,EHS_FB_INIT_NAME(gui_text2),EHS_FB_IDENTIFY_NAME(gui_text2)),
#endif /* EHS_TARGET_FP_SUPPORT */
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_int2,EHS_FB_ID_gui_text_int2, gui_text_int2,EHS_FB_INIT_NAME(gui_text2),EHS_FB_IDENTIFY_NAME(gui_text2)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_bool2,EHS_FB_ID_gui_text_bool2, gui_text_bool2,EHS_FB_INIT_NAME(gui_text2),EHS_FB_IDENTIFY_NAME(gui_text2)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_viewport,EHS_FB_ID_gui_viewport, gui_viewport,EHS_FB_INIT_NAME(gui_viewport),EHS_FB_IDENTIFY_NAME(gui_viewport)),
#ifdef USB_SUPPORT /*we will put this in anyway and make it dumb for non-supporting targets*/
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_textdisplay,EHS_FB_ID_gui_textdisplay, gui_textdisplay,EHS_FB_INIT_NAME(gui_textdisplay),EHS_FB_IDENTIFY_NAME(gui_textdisplay)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_leds,EHS_FB_ID_gui_leds, gui_leds,EHS_FB_INIT_NAME(gui_leds),EHS_FB_IDENTIFY_NAME(gui_leds)),
#endif
    {0}
};

#endif /* EHS_GUI_SUPPORT */
