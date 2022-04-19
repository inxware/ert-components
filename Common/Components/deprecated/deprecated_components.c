/** @file deprecated_components.c
 * Contains the static definition of the deprecated block reference table for EHS
 *
 * @author: inx limited
 * @version: $Revision: 5011 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006), $
 *
 * Copyright (c), inx limited, 2007. All rights reserved.
 */

#include "blockref_table.h"
#include "parse_sodl.h"

#ifdef EHS_GUI_SUPPORT
#include "guitextbox.h"
#include "guitextbox1.h"
#include "guitextbox_orig.h"
#include "guiimage.h"
#include "guiimage1.h"
#include "dtv_remote.h"
#include "dtv_remote1.h"
#endif

#include "event_counter.h"
#include "operator.h"
#include "arraystring.h"
#include "const.h"
#include "dtv_pvr_play.h"
#include "dtv_pvr_list.h"
#include "calc_i.h"


const EhsBlockRefType EhsBlockRefTable_deprecated[] =
{
#ifdef EHS_GUI_SUPPORT
	/* guiimage.h */
	/* EHS_BLOCKREF_ENTRY(EHS_FB_GUIIMAGE_NAME, GUI_Image), // compile error */
	/* guiimage1.h */
	/* EHS_BLOCKREF_ENTRY(EHS_FB_GUIIMAGE1_NAME, GUI_Image1), // compile error */
	/* guitextbox.h, guitextbox1.h */
#ifdef EHS_TARGET_FP_SUPPORT
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUI_TEXT_FLOAT_NAME, gui_text_float,EHS_FB_INIT_NAME(gui_text),EHS_FB_IDENTIFY_NAME(gui_text)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUI_TEXT1_FLOAT_NAME, gui_text_float1,EHS_FB_INIT_NAME(gui_text1),EHS_FB_IDENTIFY_NAME(gui_text1)),
#endif /* EHS_TARGET_FP_SUPPORT */
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUI_TEXT_STRING_NAME, gui_text_string,EHS_FB_INIT_NAME(gui_text),EHS_FB_IDENTIFY_NAME(gui_text)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUI_TEXT_INT_NAME, gui_text_int,EHS_FB_INIT_NAME(gui_text),EHS_FB_IDENTIFY_NAME(gui_text)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUI_TEXT_BOOL_NAME, gui_text_bool,EHS_FB_INIT_NAME(gui_text),EHS_FB_IDENTIFY_NAME(gui_text)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUI_TEXT1_STRING_NAME, gui_text_string1,EHS_FB_INIT_NAME(gui_text1),EHS_FB_IDENTIFY_NAME(gui_text1)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUI_TEXT1_INT_NAME, gui_text_int1,EHS_FB_INIT_NAME(gui_text1),EHS_FB_IDENTIFY_NAME(gui_text1)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUI_TEXT1_BOOL_NAME, gui_text_bool1,EHS_FB_INIT_NAME(gui_text1),EHS_FB_IDENTIFY_NAME(gui_text1)),
	/* guitextbox_orig.h */
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUITEXTBOX_STRING_NAME, GUI_TextboxString,EHS_FB_INIT_NAME(GUI_Textbox),EHS_FB_IDENTIFY_NAME(GUI_Textbox)),
#ifdef EHS_TARGET_FP_SUPPORT
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUITEXTBOX_FLOAT_NAME, GUI_TextboxFloat,EHS_FB_INIT_NAME(GUI_Textbox),EHS_FB_IDENTIFY_NAME(GUI_Textbox)),
#endif /* EHS_TARGET_FP_SUPPORT */
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUITEXTBOX_INT_NAME, GUI_TextboxInt,EHS_FB_INIT_NAME(GUI_Textbox),EHS_FB_IDENTIFY_NAME(GUI_Textbox)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_GUITEXTBOX_BOOL_NAME, GUI_TextboxBool,EHS_FB_INIT_NAME(GUI_Textbox),EHS_FB_IDENTIFY_NAME(GUI_Textbox)),
	/* dtv_remote.h */
//	EHS_BLOCKREF_ENTRY(EHS_FB_DTV_REMOTE_NAME, DtvRemote),
//	EHS_BLOCKREF_ENTRY(EHS_FB_DTV_REMOTE1_NAME, DtvRemote1),
#endif /* EHS_GUI_SUPPORT */
	/* eventCounter1.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_EVENT_COUNTER_VERSATILE_NAME,EventCounterVersatile),
	/* operator.h */
	EHS_BLOCKREF_ENTRY(EHS_COMPARATOR_EQUAL_INT_NAME,ComparatorEqualInt),
	EHS_BLOCKREF_ENTRY(EHS_COMPARATOR_GREATER_INT_NAME,ComparatorGreaterInt),
	EHS_BLOCKREF_ENTRY(EHS_COMPARATOR_GREATER_EQUAL_INT_NAME,ComparatorGreaterEqualInt),
#ifdef EHS_TARGET_FP_SUPPORT
	EHS_BLOCKREF_ENTRY(EHS_COMPARATOR_EQUAL_FLOAT_NAME,ComparatorEqualReal),
	EHS_BLOCKREF_ENTRY(EHS_COMPARATOR_GREATER_FLOAT_NAME,ComparatorGreaterReal),
	EHS_BLOCKREF_ENTRY(EHS_COMPARATOR_GREATER_EQUAL_FLOAT_NAME,ComparatorGreaterEqualReal),
#endif /* EHS_TARGET_FP_SUPPORT */
	/* from arraystring.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_ARRAYSTRING_NAME,ArrayString),
	/* const.h */
#ifdef EHS_TARGET_FP_SUPPORT
	EHS_BLOCKREF_ENTRY(EHS_FB_CONSTANT_FLOAT_NAME, ConstantFloat),
#endif /* EHS_TARGET_FP_SUPPORT */
	EHS_BLOCKREF_ENTRY(EHS_FB_CONSTANT_INT_NAME, ConstantInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_CONSTANT_STRING_NAME, ConstantString),
	EHS_BLOCKREF_ENTRY(EHS_FB_CONSTANT_BOOL_NAME, ConstantBool),
	/* EHS_BLOCKREF_ENTRY(EHS_FB_DTV_PVR_PLAY_NAME, DtvPvrPlay1), // compile error */
	/* EHS_BLOCKREF_ENTRY(EHS_FB_FILESYSTEM_DIRLIST_NAME, DtvPvrList), // compile error */
	/* calc_i.h */
	//EHS_BLOCKREF_ENTRY(EHS_FB_CALC_INT_NAME, ICalc),
	{0}
};

