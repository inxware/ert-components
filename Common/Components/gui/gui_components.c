/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/
/** @file core_components.c
 * Contains the static definition of the function block reference table for EHS
 *
 * @author: inx limited
 *
 */

#include "blockref_table.h"
#ifdef EHS_GUI_SUPPORT
#include "guiviewport.h"
//#include "guitextdisplay.h"
#include "guileds.h"

#include "inx-gui_widget.h"
#include "inx-gui_image_file.h"
#include "inx-gui_patch.h"

/* extended ui widgets support */
#include "ui_widgets/inx-ui_spinner.h"
#include "ui_widgets/inx-ui_chart.h"
#include "ui_widgets/inx-ui_list.h"


EhsBlockRefType EhsBlockRefTable_coreGui[] =
{
	/* inx-gui_patch.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_gui_patch,EHS_FB_ID_gui_patch, gui_patch),
	/* inx-gui_image_file.h */
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_GUI_Image_File, EHS_FB_ID_GUI_Image_File, GUI_Image_File, EHS_FB_INIT_NAME(GUI_Image_File),EHS_FB_IDENTIFY_NAME(GUI_Image_File)),
	/* inx-gui_widget.h */
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_string2,EHS_FB_ID_gui_text_string2, gui_text_string2,EHS_FB_INIT_NAME(gui_widget),EHS_FB_IDENTIFY_NAME(gui_widget)),
#ifdef EHS_TARGET_FP_SUPPORT
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_real2,EHS_FB_ID_gui_text_real2, gui_text_float2,EHS_FB_INIT_NAME(gui_widget),EHS_FB_IDENTIFY_NAME(gui_widget)),
#endif /* EHS_TARGET_FP_SUPPORT */
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_int2,EHS_FB_ID_gui_text_int2, gui_text_int2,EHS_FB_INIT_NAME(gui_widget),EHS_FB_IDENTIFY_NAME(gui_widget)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_bool2,EHS_FB_ID_gui_text_bool2, gui_text_bool2,EHS_FB_INIT_NAME(gui_widget),EHS_FB_IDENTIFY_NAME(gui_widget)),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(EHS_FB_NAME_gui_viewport,EHS_FB_ID_gui_viewport, gui_viewport),
#ifdef USB_SUPPORT /*we will put this in anyway and make it dumb for non-supporting targets*/
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_textdisplay,EHS_FB_ID_gui_textdisplay, gui_textdisplay,EHS_FB_INIT_NAME(gui_textdisplay),EHS_FB_IDENTIFY_NAME(gui_textdisplay)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_leds,EHS_FB_ID_gui_leds, gui_leds,EHS_FB_INIT_NAME(gui_leds),EHS_FB_IDENTIFY_NAME(gui_leds)),
#endif
	/* extended ui widgets support */
	/* ui_widgets/inx-ui_spinner.h */
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_ui_spinner, INXWARE_FB_ID_ui_spinner, ui_spinner),
	/* ui_widgets/inx-ui_chart.h */
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_ui_chart, INXWARE_FB_ID_ui_chart, ui_chart),
	/* ui_widgets/inx-ui_list.h */
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_ui_list, INXWARE_FB_ID_ui_list, ui_list),

    {0}
};

#endif /* EHS_GUI_SUPPORT */
