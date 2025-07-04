/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file deprecated_components.c
 * Contains the static definition of the deprecated block reference table for EHS
 *
 * @author: inx limited
 *
 */

#include "blockref_table.h"
#include "hal_utils.h"

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
#if defined(EHS_GUI_SUPPORT) && !defined(EHS_GUI_SUPPORT_MODE_B)
	/* guiimage.h */
	/* EHS_BLOCKREF_ENTRY(EHS_FB_NAME_GUIIMAGE_NAME, GUI_Image), // compile error */
	/* guiimage1.h */
	/* EHS_BLOCKREF_ENTRY(EHS_FB_NAME_GUIIMAGE1_NAME, GUI_Image1), // compile error */
	/* guitextbox.h, guitextbox1.h */
#ifdef EHS_TARGET_FP_SUPPORT
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_real,EHS_FB_ID_gui_text_real, gui_text_float,EHS_FB_INIT_NAME(gui_text),EHS_FB_IDENTIFY_NAME(gui_text)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_real1,EHS_FB_ID_gui_text_real1, gui_text_float1,EHS_FB_INIT_NAME(gui_text1),EHS_FB_IDENTIFY_NAME(gui_text1)),
#endif /* EHS_TARGET_FP_SUPPORT */
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_string,EHS_FB_ID_gui_text_string, gui_text_string,EHS_FB_INIT_NAME(gui_text),EHS_FB_IDENTIFY_NAME(gui_text)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_int, EHS_FB_ID_gui_text_int, gui_text_int,EHS_FB_INIT_NAME(gui_text),EHS_FB_IDENTIFY_NAME(gui_text)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_bool, EHS_FB_ID_gui_text_bool, gui_text_bool,EHS_FB_INIT_NAME(gui_text),EHS_FB_IDENTIFY_NAME(gui_text)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_string1, EHS_FB_ID_gui_text_string1, gui_text_string1,EHS_FB_INIT_NAME(gui_text1),EHS_FB_IDENTIFY_NAME(gui_text1)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_int1, EHS_FB_ID_gui_text_int1, gui_text_int1,EHS_FB_INIT_NAME(gui_text1),EHS_FB_IDENTIFY_NAME(gui_text1)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_gui_text_bool1, EHS_FB_ID_gui_text_bool1, gui_text_bool1,EHS_FB_INIT_NAME(gui_text1),EHS_FB_IDENTIFY_NAME(gui_text1)),
	/* guitextbox_orig.h */
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_GUI_OutputTextBoxString, EHS_FB_ID_GUI_OutputTextBoxString, GUI_TextboxString,EHS_FB_INIT_NAME(GUI_Textbox),EHS_FB_IDENTIFY_NAME(GUI_Textbox)),
#ifdef EHS_TARGET_FP_SUPPORT
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_GUI_OutputTextBoxReal, EHS_FB_ID_GUI_OutputTextBoxReal, GUI_TextboxFloat,EHS_FB_INIT_NAME(GUI_Textbox),EHS_FB_IDENTIFY_NAME(GUI_Textbox)),
#endif /* EHS_TARGET_FP_SUPPORT */
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_GUI_OutputTextBoxInt, EHS_FB_ID_GUI_OutputTextBoxInt, GUI_TextboxInt,EHS_FB_INIT_NAME(GUI_Textbox),EHS_FB_IDENTIFY_NAME(GUI_Textbox)),
	EHS_BLOCKREF_ENTRY_EX(EHS_FB_NAME_GUI_OutputTextBoxBool, EHS_FB_ID_GUI_OutputTextBoxBool, GUI_TextboxBool,EHS_FB_INIT_NAME(GUI_Textbox),EHS_FB_IDENTIFY_NAME(GUI_Textbox)),
	/* dtv_remote.h */
//	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DTV_REMOTE_NAME, DtvRemote),
//	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DTV_REMOTE1_NAME, DtvRemote1),
#endif /* EHS_GUI_SUPPORT */
	/* eventCounter1.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_EventCounterVersatile,EHS_FB_ID_EventCounterVersatile,EventCounterVersatile),
	/* operator.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ComparatorEqualInt,EHS_FB_ID_ComparatorEqualInt,ComparatorEqualInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ComparatorGreaterInt,EHS_FB_ID_ComparatorGreaterInt,ComparatorGreaterInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ComparatorGreaterEqualInt,EHS_FB_ID_ComparatorGreaterEqualInt,ComparatorGreaterEqualInt),
#ifdef EHS_TARGET_FP_SUPPORT
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ComparatorEqualReal,EHS_FB_ID_ComparatorEqualReal,ComparatorEqualReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_CComparatorGreaterReal,EHS_FB_ID_ComparatorGreaterReal,ComparatorGreaterReal),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ComparatorGreaterEqualReal,EHS_FB_ID_ComparatorGreaterEqualReal,ComparatorGreaterEqualReal),
#endif /* EHS_TARGET_FP_SUPPORT */
	/* from arraystring.h */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ArrayString,EHS_FB_ID_ArrayString,ArrayString),
	/* const.h */
#ifdef EHS_TARGET_FP_SUPPORT
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_Constant_Real, EHS_FB_ID_Constant_Real, ConstantFloat),
#endif /* EHS_TARGET_FP_SUPPORT */
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_Constant_Int,EHS_FB_ID_Constant_Int, ConstantInt),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_Constant_String,EHS_FB_ID_Constant_String, ConstantString),
	EHS_BLOCKREF_ENTRY(EHS_FB_NAME_Constant_Bool,EHS_FB_ID_Constant_Bool, ConstantBool),
	/* EHS_BLOCKREF_ENTRY(EHS_FB_NAME_DTV_PVR_PLAY_NAME, DtvPvrPlay1), // compile error */
	/* EHS_BLOCKREF_ENTRY(EHS_FB_NAME_FILESYSTEM_DIRLIST_NAME, DtvPvrList), // compile error */
	/* calc_i.h */
	//EHS_BLOCKREF_ENTRY(EHS_FB_NAME_ICalc,EHS_FB_ID_ICalc, ICalc),
	{0}
};

