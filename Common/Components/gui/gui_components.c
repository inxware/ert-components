/** @file core_components.c
 * Contains the static definition of the function block reference table for EHS
 *
 * @author: inx limited
 * @version: $Revision: 5630 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006), $
 *
 * Copyright (c), inx limited, 2007. All rights reserved.
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
	EHS_BLOCKREF_ENTRY(EHS_FB_GUI_PATCH_NAME, gui_patch),
	/* gui_image_file.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_GUIIMAGEFILE_NAME, GUI_ImageFile),
	/* guitextbox.h */
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUI_TEXT_STRING_NAME, gui_text_string2,EHS_FB_INIT_NAME(gui_text2),EHS_FB_IDENTIFY_NAME(gui_text2)),
#ifdef EHS_TARGET_FP_SUPPORT
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUI_TEXT_FLOAT_NAME, gui_text_float2,EHS_FB_INIT_NAME(gui_text2),EHS_FB_IDENTIFY_NAME(gui_text2)),
#endif /* EHS_TARGET_FP_SUPPORT */
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUI_TEXT_INT_NAME, gui_text_int2,EHS_FB_INIT_NAME(gui_text2),EHS_FB_IDENTIFY_NAME(gui_text2)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUI_TEXT_BOOL_NAME, gui_text_bool2,EHS_FB_INIT_NAME(gui_text2),EHS_FB_IDENTIFY_NAME(gui_text2)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUI_VIEWPORT_NAME, gui_viewport,EHS_FB_INIT_NAME(gui_viewport),EHS_FB_IDENTIFY_NAME(gui_viewport)),
#ifdef USB_SUPPORT /*we will put this in anyway and make it dumb for non-supporting targets*/
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUI_TEXTDISPLAY_NAME, gui_textdisplay,EHS_FB_INIT_NAME(gui_textdisplay),EHS_FB_IDENTIFY_NAME(gui_textdisplay)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUI_LEDS_NAME, gui_leds,EHS_FB_INIT_NAME(gui_leds),EHS_FB_IDENTIFY_NAME(gui_leds)),
#endif
	{0}
};

#endif /* EHS_GUI_SUPPORT */
