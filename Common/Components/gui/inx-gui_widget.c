/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved.
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
 * with this file. If not, please visit:
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#define EHSL_MODULE_ID (EHSH_LOG_MODULE_GRAPHICS)

#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-gui_widget.h"

#include "widget.h"
#include "guiparams.h"

typedef struct inx_gui_widget_state
{
	//ehs_uint16 id;
	EhsWidgetUi gui;
	EhsWidgetClass* pUiWidgetClass;
} inx_gui_widget_state_type;

EHS_FB_FUNCTIONS_START(gui_text_string2)
EHS_FB_FUNCTION_ENTRY("create", 0x01, gui_widget_create)
EHS_FB_FUNCTION_ENTRY("destroy", 0x02, gui_widget_destroy)
EHS_FB_FUNCTION_ENTRY("show", 0x03, gui_widget_show)
EHS_FB_FUNCTION_ENTRY("hide", 0x04, gui_widget_hide)
EHS_FB_FUNCTION_ENTRY("update", 0x05, gui_widget_update)
EHS_FB_FUNCTION_ENTRY("data", 0x06, gui_text_string2_data)
EHS_FB_FUNCTIONS_END

EHS_FB_FUNCTIONS_START(gui_text_float2)
EHS_FB_FUNCTION_ENTRY("create", 0x01, gui_widget_create)
EHS_FB_FUNCTION_ENTRY("destroy", 0x02, gui_widget_destroy)
EHS_FB_FUNCTION_ENTRY("show", 0x03, gui_widget_show)
EHS_FB_FUNCTION_ENTRY("hide", 0x04, gui_widget_hide)
EHS_FB_FUNCTION_ENTRY("update", 0x05, gui_widget_update)
EHS_FB_FUNCTION_ENTRY("data", 0x06, gui_text_float2_data)
EHS_FB_FUNCTIONS_END

EHS_FB_FUNCTIONS_START(gui_text_int2)
EHS_FB_FUNCTION_ENTRY("create", 0x01, gui_widget_create)
EHS_FB_FUNCTION_ENTRY("destroy", 0x02, gui_widget_destroy)
EHS_FB_FUNCTION_ENTRY("show", 0x03, gui_widget_show)
EHS_FB_FUNCTION_ENTRY("hide", 0x04, gui_widget_hide)
EHS_FB_FUNCTION_ENTRY("update", 0x05, gui_widget_update)
EHS_FB_FUNCTION_ENTRY("data", 0x06, gui_text_int2_data)
EHS_FB_FUNCTIONS_END

EHS_FB_FUNCTIONS_START(gui_text_bool2)
EHS_FB_FUNCTION_ENTRY("create", 0x01, gui_widget_create)
EHS_FB_FUNCTION_ENTRY("destroy", 0x02, gui_widget_destroy)
EHS_FB_FUNCTION_ENTRY("show", 0x03, gui_widget_show)
EHS_FB_FUNCTION_ENTRY("hide", 0x04, gui_widget_hide)
EHS_FB_FUNCTION_ENTRY("update", 0x05, gui_widget_update)
EHS_FB_FUNCTION_ENTRY("data", 0x06, gui_text_bool2_data)
EHS_FB_FUNCTIONS_END


#define INX_gui_widget_ARG_create_abs_x 1
#define INX_gui_widget_ARG_create_abs_y 2
#define INX_gui_widget_ARG_create_wid 3
#define INX_gui_widget_ARG_create_ht 4
#define INX_gui_widget_ARG_create_data_out 5
#define INX_gui_widget_ARG_create_label_out 6
#define INX_gui_widget_ARG_create___ 1
#define INX_gui_widget_ARG_create_click 2
#define INX_gui_widget_ARG_create_mouse_down 3
#define INX_gui_widget_ARG_create_data___ 4
#define INX_gui_widget_ARG_create_change 5
#define INX_gui_widget_ARG_destroy___ 1
#define INX_gui_widget_ARG_show___ 1
#define INX_gui_widget_ARG_hide___ 1
#define INX_gui_widget_ARG_update_off_x 1
#define INX_gui_widget_ARG_update_off_y 2
#define INX_gui_widget_ARG_update_off_wid 3
#define INX_gui_widget_ARG_update_off_ht 4
#define INX_gui_widget_ARG_update_alpha 5
#define INX_gui_widget_ARG_update_abs_x 1
#define INX_gui_widget_ARG_update_abs_y 2
#define INX_gui_widget_ARG_update_wid 3
#define INX_gui_widget_ARG_update_ht 4
#define INX_gui_widget_ARG_update___ 1

#define INX_gui_text_string2_ARG_data_data 1
#define INX_gui_text_string2_ARG_data_label 2
#define INX_gui_text_string2_ARG_data_data_out 1
#define INX_gui_text_string2_ARG_data_label_out 2
#define INX_gui_text_string2_ARG_data____ 1
#define INX_gui_text_string2_ARG_data_change 2

#define INX_gui_text_float2_ARG_data_data 1
#define INX_gui_text_float2_ARG_data_label 2
#define INX_gui_text_float2_ARG_data_data_out 1
#define INX_gui_text_float2_ARG_data_label_out 2
#define INX_gui_text_float2_ARG_data____ 1
#define INX_gui_text_float2_ARG_data_change 2

#define INX_gui_text_int2_ARG_data_data 1
#define INX_gui_text_int2_ARG_data_label 2
#define INX_gui_text_int2_ARG_data_data_out 1
#define INX_gui_text_int2_ARG_data_label_out 2
#define INX_gui_text_int2_ARG_data____ 1
#define INX_gui_text_int2_ARG_data_change 2

#define INX_gui_text_bool2_ARG_data_data 1
#define INX_gui_text_bool2_ARG_data_label 2
#define INX_gui_text_bool2_ARG_data_data_out 1
#define INX_gui_text_bool2_ARG_data_label_out 2
#define INX_gui_text_bool2_ARG_data____ 1
#define INX_gui_text_bool2_ARG_data_change 2

/* Forward declaration for auto-create in Mode B Qt */
static void gui_widget_event_callback(struct EhsWidgetStruct* pWidget, ehs_uint16 event_id, const char* label, void* data);

EHS_FB_IDENTIFY_FUNCTION(gui_widget)
{
	printf("*** QT DEBUG: gui_widget IDENTIFY called, size=%zu ***\n", sizeof(inx_gui_widget_state_type));
	fflush(stdout);
	EHSH_LOG_INFO("=== gui_widget IDENTIFY FUNCTION CALLED (memory size=%zu) ===", sizeof(inx_gui_widget_state_type));
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_gui_widget_state_type);
}

EHS_FB_INIT_FUNCTION(gui_widget)
{
	printf("*** QT DEBUG: gui_widget INIT called ***\n");
	fflush(stdout);
	EHSH_LOG_INFO("=== gui_widget INIT FUNCTION CALLED ===");
	ehs_bool bRet = EHS_TRUE;

	EhsGuiParamsType xParams;
	xParams.uClass.xTextbox.nType = 0;  // type is comming from iGB and is mainly used for extended ui e.g. button, slider etc.
	xParams.uClass.xTextbox.nProp = 0;
	xParams.uClass.xTextbox.nCurve = 0;
	xParams.uClass.xTextbox.nParent = 0;
	char guiParams[MAX_PARAM_STR_LEN];
    const char* pParams;

	//this is the reference to the object data for this instance of the function block
	inx_gui_widget_state_type* inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_INIT_CONTEXT;
	inx_gui_widget_state->pUiWidgetClass = NULL;

	printf("*** QT DEBUG: About to call ReadParmFile, EHS_FB_INIT_PARAMETERS=%p\n", (void*)EHS_FB_INIT_PARAMETERS);
	fflush(stdout);
	EHSH_LOG_INFO("  About to call ReadParmFile, EHS_FB_INIT_PARAMETERS=%p", (void*)EHS_FB_INIT_PARAMETERS);
	pParams = ReadParmFile(&EHS_FB_INIT_PARAMETERS[4], guiParams);
	printf("*** QT DEBUG: ReadParmFile returned, guiParams[0]=0x%02x ('%c'), strlen=%zu\n",
	       (unsigned char)guiParams[0], guiParams[0] >= 32 && guiParams[0] < 127 ? guiParams[0] : '.', strlen(guiParams));
	fflush(stdout);
	printf("*** QT DEBUG: First 80 chars of guiParams: '%.80s'\n", guiParams);
	fflush(stdout);
	EHSH_LOG_INFO("  ReadParmFile returned, guiParams[0]='%c' (0x%02x), first 50 chars: '%.50s'",
	              guiParams[0] ? guiParams[0] : '?', (unsigned char)guiParams[0], guiParams);
	if (guiParams[0]) {
		printf("*** QT DEBUG: guiParams[0] is NOT empty, proceeding to parse\n");
		fflush(stdout);
		EHSH_LOG_INFO("gui_widget INIT: Read GUI parameters, calling EhsParseGuiParameters");
		EhsParseGuiParameters(guiParams, &xParams);

		EHSH_LOG_INFO("gui_widget INIT: After parsing, eClass=%d (TEXTBOX=%d, BITMAP=%d, PATCH=%d, VIEWPORT=%d)",
		              xParams.eClass, EHS_WIDGET_CLASS_TEXTBOX, EHS_WIDGET_CLASS_BITMAP,
		              EHS_WIDGET_CLASS_PATCH, EHS_WIDGET_CLASS_VIEWPORT);

		if (xParams.eClass == EHS_WIDGET_CLASS_TEXTBOX)
		{
			EHSH_LOG_INFO("gui_widget INIT: Creating TEXTBOX widget (nTextBoxType=%d)", xParams.nTextBoxType);

#if defined(EHS_GUI_SUPPORT_MODE_B) || defined(EHS_GUI_SUPPORT_MODE_B_QT)

			EHSH_LOG_INFO("  Using Mode B/Qt widget creation path");
			inx_gui_widget_state->gui.data = NULL;
			inx_gui_widget_state->gui.label = NULL;
			ehs_uint16 nId = EHS_STRING_UI_WIDGET;
			switch(xParams.nTextBoxType){ // text box type is comming from iGB and can either be string (0), bool (0), int (0), float (0)
				case 0: // string widget type
				{
					nId = EHS_STRING_UI_WIDGET + xParams.uClass.xTextbox.nType;
					break;
				}
				case 1: // bool widget type
				{
					nId = EHS_BOOL_UI_WIDGET + xParams.uClass.xTextbox.nType;
					break;
				}
				case 2: // int widget type
				{
					nId = EHS_INT_UI_WIDGET + xParams.uClass.xTextbox.nType;
					break;
				}
				case 3: // float widget type
				{
					nId = EHS_FLOAT_UI_WIDGET + xParams.uClass.xTextbox.nType;
					break;
				}
			}
			EHSH_LOG_INFO("  Calling EhsWidgetUI_init with nId=%u", nId);
			inx_gui_widget_state->pUiWidgetClass = EhsWidgetUI_init(nId, xParams.uClass.xTextbox.nProp,
																	xParams.uClass.xTextbox.nCurve,
																	xParams.uClass.xTextbox.nParent,
																	&(xParams.xRect),xParams.nZorder,
																	xParams.uClass.xTextbox.nIndentL,
																	xParams.uClass.xTextbox.nIndentT,
																	xParams.uClass.xTextbox.nIndentR,
																	xParams.uClass.xTextbox.nIndentB,
																	xParams.uClass.xTextbox.nLineSep,
																	xParams.uClass.xTextbox.xFgColour,
																	xParams.uClass.xTextbox.xBgColour,
																	/*pFont*/NULL);
			EHSH_LOG_INFO("  EhsWidgetUI_init returned: %p", (void*)inx_gui_widget_state->pUiWidgetClass);

#else // EHS_GUI_SUPPORT_MODE_B || EHS_GUI_SUPPORT_MODE_B_QT

			EhsGraphicsFontClass* pFont = NULL;

#ifndef EHS_DONT_USE_BASIC_FONTS
			pFont = EhsGraphicsFont_load(xParams.uClass.xTextbox.szFontName);
#endif // EHS_DONT_USE_BASIC_FONTS

			inx_gui_widget_state->pUiWidgetClass =
													EhsWidgetTextbox_init(&(xParams.xRect),xParams.nZorder,
													xParams.uClass.xTextbox.nIndentL,
													xParams.uClass.xTextbox.nIndentT,
													xParams.uClass.xTextbox.nIndentR,
													xParams.uClass.xTextbox.nIndentB,
													xParams.uClass.xTextbox.nLineSep,
													xParams.uClass.xTextbox.xFgColour,
													xParams.uClass.xTextbox.xBgColour,
													pFont);

#endif // EHS_GUI_SUPPORT_MODE_B || EHS_GUI_SUPPORT_MODE_B_QT

			if ((inx_gui_widget_state->pUiWidgetClass == NULL) || (inx_gui_widget_state->pUiWidgetClass->nState == EHS_WIDGET_STATE_EMPTY))
			{
				EHSH_LOG_ERROR("gui_widget INIT: Widget creation FAILED (pWidget=%p, nState=%d)",
				               (void*)inx_gui_widget_state->pUiWidgetClass,
				               inx_gui_widget_state->pUiWidgetClass ? inx_gui_widget_state->pUiWidgetClass->nState : -1);
				bRet = EHS_FALSE;
			}
			else
			{
				EHSH_LOG_INFO("gui_widget INIT: Widget created successfully, nState=%d",
				              inx_gui_widget_state->pUiWidgetClass->nState);
				/* parse parameters */
				ehs_uint8 nByte = 0;
				pParams = EhsGetUint8FromString(&nByte, pParams);
				inx_gui_widget_state->pUiWidgetClass->bMaintainAspectRatio = (ehs_bool)nByte;
				pParams = EhsGetUint8FromString(&nByte, pParams);
				inx_gui_widget_state->pUiWidgetClass->bCaptureClicksIgnoringZOrder = (ehs_bool)nByte;

#if defined(EHS_GUI_SUPPORT_MODE_B) || defined(EHS_GUI_SUPPORT_MODE_B_QT)

				if(EhsWidgetUI_is_float_type(inx_gui_widget_state->pUiWidgetClass)){
					ehs_sint32 nNoOfDecPlaces = 0;
					pParams = EhsGetSint32FromString(&nNoOfDecPlaces, pParams);
					if(nNoOfDecPlaces >= 0){
						EHS_WIDGET_UI(inx_gui_widget_state->pUiWidgetClass).nNoOfDecPlaces = (ehs_uint16)nNoOfDecPlaces;
					}
				}

#endif // EHS_GUI_SUPPORT_MODE_B || EHS_GUI_SUPPORT_MODE_B_QT

				inx_gui_widget_state->pUiWidgetClass->bContentChanged = EHS_TRUE; /* This should be done in the common code */
				bRet = EHS_TRUE;
			}
		}
		else
		{
			printf("*** QT DEBUG: Widget class is NOT TEXTBOX (eClass=%d)\n", xParams.eClass);
			fflush(stdout);
			EHSH_LOG_WARNING("gui_widget INIT: Parsed widget class is NOT TEXTBOX (eClass=%d), widget not created", xParams.eClass);
		}
    }
	else
	{
		printf("*** QT DEBUG: guiParams[0] IS EMPTY - no parameters to parse!\n");
		fflush(stdout);
		EHSH_LOG_WARNING("gui_widget INIT: No GUI parameters to parse (guiParams is empty)");
	}

	/* Add any further intialisation code here */

#ifdef EHS_GUI_SUPPORT_MODE_B_QT
	// For Mode B Qt auto-create: set up event callback and data pointers now
	// so they're ready when pfCreateFunc is called from the tick callback
	if (bRet && inx_gui_widget_state->pUiWidgetClass != NULL) {
		EhsWidgetClass* pWidget = inx_gui_widget_state->pUiWidgetClass;

		// Set up event callback (same as gui_widget_create RUN function does)
		EHS_WIDGET_UI(pWidget).event_callback = gui_widget_event_callback;

		// Setup widget data pointer (same as gui_widget_create RUN function does)
		EHS_WIDGET_UI(pWidget).data = (void*) &inx_gui_widget_state->gui;

		EHSH_LOG_INFO("gui_widget INIT: Pre-registered event callback for auto-create (callback=%p, data=%p)",
		              (void*)gui_widget_event_callback, (void*)&inx_gui_widget_state->gui);

		// NOTE: We cannot set pFIData here because we don't have the function instance pointer
		// in INIT context. The auto-create code will need to set it from inx_gui_widget_state.
		// We'll store the state pointer in a way that auto-create can find it.
		// Store inx_gui_widget_state in the widget so auto-create can find it
		// We'll use a field that's not otherwise used... Actually, let's use pFIData temporarily
		// and have auto-create fix it up properly.
		// HACK: Store inx_gui_widget_state in pFIData temporarily - auto-create will fix this
		pWidget->pFIData = (EhsFunctionInstanceDataType*)inx_gui_widget_state;
		EHSH_LOG_INFO("gui_widget INIT: Stored state pointer in pFIData for auto-create to fix");
	}
#endif

	printf("*** QT DEBUG: gui_widget INIT: EXIT - bRet=%d\n", bRet);
	fflush(stdout);
	EHSH_LOG_INFO("gui_widget INIT: EXIT - returning bRet=%d", bRet);
	return bRet; /* initialisation always succeeds */
}

EHS_FB_DESTROY_FUNCTION(gui_widget)
{
	inx_gui_widget_state_type *inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_DESTROY_CONTEXT;
	
	if(inx_gui_widget_state && inx_gui_widget_state->pUiWidgetClass != NULL){
		EhsWidget_destroy(inx_gui_widget_state->pUiWidgetClass);
	}
	return EHS_TRUE;
}

/* Widget update callback */
static void gui_widget_event_callback(struct EhsWidgetStruct* pWidget, ehs_uint16 event_id, const char* label, void* data)
{
	if(pWidget){
		EhsFunctionInstanceDataType* pFIdata = pWidget->pFIData;
		if(pFIdata == NULL){
			EHSH_LOG_WARNING("pFIdata is NULL - do nothing");
			return;
		}

		EHSH_LOG_INFO("gui_widget_event_callback: event_id=0x%04x (MOUSE_DOWN=%d, MOUSE_CLICKED=%d, DATA_CHANGED=%d)",
		              event_id,
		              (event_id & EHS_WIDGET_UI_EVENT_MOUSE_DOWN) ? 1 : 0,
		              (event_id & EHS_WIDGET_UI_EVENT_MOUSE_CLICKED) ? 1 : 0,
		              (event_id & EHS_WIDGET_UI_EVENT_DATA_CHANGED) ? 1 : 0);

		ehs_bool bDataChanged = EHS_FALSE;
		// update widget label data
		if ((event_id & EHS_WIDGET_UI_EVENT_LABEL_UPDATED || event_id & EHS_WIDGET_UI_EVENT_LABEL_CHANGED) && EHS_FB_OUT_CONNECTED_API2(INX_gui_widget_ARG_create_label_out)){
			if(label){
				static const ehs_uint32 UI_OUTPUT_STR_LEN = EHS_STRING_LENGTH_MAX-1;
				(EHS_FB_OUT_S_API2(INX_gui_widget_ARG_create_label_out))[UI_OUTPUT_STR_LEN] = '\0';
				EhsStrncpy(EHS_FB_OUT_S_API2(INX_gui_widget_ARG_create_label_out), label, UI_OUTPUT_STR_LEN);
			}else{
				EhsStrcpy(EHS_FB_OUT_S_API2(INX_gui_widget_ARG_create_label_out), "");
			}
			bDataChanged = (event_id & EHS_WIDGET_UI_EVENT_LABEL_CHANGED) ? EHS_TRUE : EHS_FALSE;
		}
		if(data && (event_id & EHS_WIDGET_UI_EVENT_DATA_UPDATED || event_id & EHS_WIDGET_UI_EVENT_DATA_CHANGED) && EHS_FB_OUT_CONNECTED_API2(INX_gui_widget_ARG_create_data_out)){
			if(EhsWidgetUI_is_string_type(pWidget)){
				printf("EhsWidgetUI_is_string_type\n"); fflush(stdout);
				const char* str = (const char*)data;
				static const ehs_uint32 UI_OUTPUT_STR_LEN = EHS_STRING_LENGTH_MAX-1;
				(EHS_FB_OUT_S_API2(INX_gui_widget_ARG_create_data_out))[UI_OUTPUT_STR_LEN] = '\0';
				EhsStrncpy(EHS_FB_OUT_S_API2(INX_gui_widget_ARG_create_data_out), str, UI_OUTPUT_STR_LEN);
				bDataChanged = (event_id & EHS_WIDGET_UI_EVENT_DATA_CHANGED) ? EHS_TRUE : EHS_FALSE;
			}else if(EhsWidgetUI_is_bool_type(pWidget)){
				printf("EhsWidgetUI_is_bool_type\n"); fflush(stdout);
				const ehs_bool* bool_val = (const ehs_bool*)data;
				EHS_FB_OUT_B_API2(INX_gui_widget_ARG_create_data_out) = *bool_val;
				bDataChanged = (event_id & EHS_WIDGET_UI_EVENT_DATA_CHANGED) ? EHS_TRUE : EHS_FALSE;
			}else if(EhsWidgetUI_is_int_type(pWidget)){
				printf("EhsWidgetUI_is_int_type\n"); fflush(stdout);
				const ehs_sint32* int_val = (const ehs_sint32*)data;
				EHS_FB_OUT_I_API2(INX_gui_widget_ARG_create_data_out) = *int_val;
				bDataChanged = (event_id & EHS_WIDGET_UI_EVENT_DATA_CHANGED) ? EHS_TRUE : EHS_FALSE;
			}else if(EhsWidgetUI_is_float_type(pWidget)){
				printf("EhsWidgetUI_is_float_type\n"); fflush(stdout);
				const float float_val = *(const float*)data;
				EHS_FB_OUT_F_API2(INX_gui_widget_ARG_create_data_out) = float_val;
				bDataChanged = (event_id & EHS_WIDGET_UI_EVENT_DATA_CHANGED) ? EHS_TRUE : EHS_FALSE;
			}
		}
		// updated by the input event port
		if((event_id & EHS_WIDGET_UI_EVENT_LABEL_UPDATED) || (event_id & EHS_WIDGET_UI_EVENT_DATA_UPDATED)){
			EHSH_LOG_INFO("  -> Triggering EHS event: data___ (data updated)");
			EHS_FB_FINISH(INX_gui_widget_ARG_create_data___);
		}
		// changed by the user via ui
		if(bDataChanged){
			EHSH_LOG_INFO("  -> Triggering EHS event: change (user changed data)");
			EHS_FB_FINISH(INX_gui_widget_ARG_create_change);
		}
		// output mouse click event
		if(event_id & EHS_WIDGET_UI_EVENT_MOUSE_CLICKED){
			EHSH_LOG_INFO("  -> Triggering EHS event: click (mouse released)");
			EHS_FB_FINISH(INX_gui_widget_ARG_create_click);
		}

		if(event_id & EHS_WIDGET_UI_EVENT_MOUSE_DOWN){
			EHSH_LOG_INFO("  -> Triggering EHS event: mouse_down (mouse pressed)");
			EHS_FB_FINISH(INX_gui_widget_ARG_create_mouse_down);
		}
	}
}

EHS_FB_RUN_FUNCTION(gui_widget_create)
{
	EHSH_LOG_INFO("gui_widget_create: RUN function ENTRY");
	inx_gui_widget_state_type* inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_widget_state && inx_gui_widget_state->pUiWidgetClass) {
		EHSH_LOG_INFO("  Widget state and pUiWidgetClass are valid (%p)", (void*)inx_gui_widget_state->pUiWidgetClass);
		EhsWidgetClass* pWidget = inx_gui_widget_state->pUiWidgetClass;
		if (pWidget) {
#if defined(EHS_GUI_SUPPORT_MODE_B) || defined(EHS_GUI_SUPPORT_MODE_B_QT)
			/* set up event callback */
			EHS_WIDGET_UI(pWidget).event_callback = gui_widget_event_callback;
			/* setup widget data */
			EHS_WIDGET_UI(pWidget).data = (void*) &inx_gui_widget_state->gui;
			EHSH_LOG_INFO("gui_widget_create: Registered event callback for widget (id=%d)", EHS_WIDGET_UI(pWidget).id);
#endif
			pWidget->pFIData = EHS_FB_RUN_CONTEXT_REF;

			if (EHS_FB_OUT_CONNECTED_API2(INX_gui_widget_ARG_create_abs_x)){
				EHS_FB_OUT_I_API2(INX_gui_widget_ARG_create_abs_x) = pWidget->xCurRect.nLeft;
			}
			if (EHS_FB_OUT_CONNECTED_API2(INX_gui_widget_ARG_create_abs_y)){
				EHS_FB_OUT_I_API2(INX_gui_widget_ARG_create_abs_y) = pWidget->xCurRect.nTop;
			}
			if (EHS_FB_OUT_CONNECTED_API2(INX_gui_widget_ARG_create_wid)){
				EHS_FB_OUT_I_API2(INX_gui_widget_ARG_create_wid) = pWidget->xCurRect.nWidth;
			}
			if (EHS_FB_OUT_CONNECTED_API2(INX_gui_widget_ARG_create_ht)){
				EHS_FB_OUT_I_API2(INX_gui_widget_ARG_create_ht) = pWidget->xCurRect.nHeight;
			}

			EHSH_LOG_INFO("  Calling EhsWidget_create(pWidget=%p)", (void*)pWidget);
			EhsWidget_create(pWidget);
			EHSH_LOG_INFO("  EhsWidget_create completed, nState=%d", pWidget->nState);
#if !defined(EHS_GUI_SUPPORT_MODE_B) && !defined(EHS_GUI_SUPPORT_MODE_B_QT)
			/*Set number of mouseClick port*/
			pWidget->mouseClickPortNumber = INX_gui_widget_ARG_create_click;
			pWidget->mouseDownPortNumber = INX_gui_widget_ARG_create_mouse_down;
#endif
			pWidget->bContentChanged = EHS_TRUE; /* This should be done in the common code */

			EHSH_LOG_INFO("gui_widget_create: RUN function EXIT - triggering create event");
			EHS_FB_FINISH(INX_gui_widget_ARG_create___);
		}
	}
	else
	{
		EHSH_LOG_WARNING("gui_widget_create: RUN function EXIT - widget state or pUiWidgetClass is NULL");
	}
}

EHS_FB_RUN_FUNCTION(gui_widget_destroy)
{
	inx_gui_widget_state_type* inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_widget_state) {
		if(inx_gui_widget_state->pUiWidgetClass != NULL){
			EhsWidget_destroy(inx_gui_widget_state->pUiWidgetClass);
		}
		EHS_FB_FINISH(INX_gui_widget_ARG_destroy___);
	}
}

EHS_FB_RUN_FUNCTION(gui_widget_show)
{
	inx_gui_widget_state_type* inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_widget_state && inx_gui_widget_state->pUiWidgetClass) {
		EhsWidget_show(inx_gui_widget_state->pUiWidgetClass);
		EHS_FB_FINISH(INX_gui_widget_ARG_show___);
	}
}

EHS_FB_RUN_FUNCTION(gui_widget_hide)
{
	inx_gui_widget_state_type* inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_widget_state && inx_gui_widget_state->pUiWidgetClass) {
		EhsWidget_hide(inx_gui_widget_state->pUiWidgetClass);
		EHS_FB_FINISH(INX_gui_widget_ARG_hide___);
	}
}

EHS_FB_RUN_FUNCTION(gui_widget_update)
{
	inx_gui_widget_state_type* inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_widget_state && inx_gui_widget_state->pUiWidgetClass) {
		EhsWidgetClass* pWidget = inx_gui_widget_state->pUiWidgetClass;

		ehs_bool bMoveRequired = EHS_FALSE; /* assume that we never need to move, unless inputs are connected */
		EhsDataflowIntType nXoffset = 0;
		EhsDataflowIntType nYoffset = 0;
		EhsDataflowIntType nWOffset = 0;
		EhsDataflowIntType nHOffset = 0;
		EhsDataflowIntType nAlpha = 0;

		ehs_bool bAlphaConnected = EHS_FALSE;
		ehs_bool bXConnected = EHS_FALSE;
		ehs_bool bYConnected = EHS_FALSE;
		ehs_bool bWConnected = EHS_FALSE;
		ehs_bool bHConnected = EHS_FALSE;

		if (EHS_FB_IN_CONNECTED_API2(INX_gui_widget_ARG_update_off_x)){
			nXoffset = EHS_FB_IN_I_API2(INX_gui_widget_ARG_update_off_x);
			bXConnected = bMoveRequired = EHS_TRUE;
		}
		if (EHS_FB_IN_CONNECTED_API2(INX_gui_widget_ARG_update_off_y)){
			nYoffset = EHS_FB_IN_I_API2(INX_gui_widget_ARG_update_off_y);
			bYConnected = bMoveRequired = EHS_TRUE;
		}
		if (EHS_FB_IN_CONNECTED_API2(INX_gui_widget_ARG_update_off_wid)){
			nWOffset = EHS_FB_IN_I_API2(INX_gui_widget_ARG_update_off_wid);
			bWConnected = bMoveRequired = EHS_TRUE;
		}
		if (EHS_FB_IN_CONNECTED_API2(INX_gui_widget_ARG_update_off_ht)){
			nHOffset = EHS_FB_IN_I_API2(INX_gui_widget_ARG_update_off_ht);
			bHConnected = bMoveRequired = EHS_TRUE;
		}
		if (EHS_FB_IN_CONNECTED_API2(INX_gui_widget_ARG_update_alpha)){
			nAlpha = EHS_FB_IN_I_API2(INX_gui_widget_ARG_update_alpha);
			if (nAlpha < 0) nAlpha = 0;
			if (nAlpha > 255) nAlpha = 255;
			EhsWidget_fade(pWidget, (ehs_uint8)nAlpha);
			bAlphaConnected = EHS_TRUE;
		}

		if (bMoveRequired){
			EhsWidget_move(pWidget, nXoffset, nYoffset, nWOffset, nHOffset);
		}

		if (EHS_FB_OUT_CONNECTED_API2(INX_gui_widget_ARG_update_abs_x)){
			EHS_FB_OUT_I_API2(INX_gui_widget_ARG_update_abs_x) = pWidget->xCurRect.nLeft;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_gui_widget_ARG_update_abs_y)){
			EHS_FB_OUT_I_API2(INX_gui_widget_ARG_update_abs_y) = pWidget->xCurRect.nTop;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_gui_widget_ARG_update_wid)){
			EHS_FB_OUT_I_API2(INX_gui_widget_ARG_update_wid) = pWidget->xCurRect.nWidth;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_gui_widget_ARG_update_ht)){
			EHS_FB_OUT_I_API2(INX_gui_widget_ARG_update_ht) = pWidget->xCurRect.nHeight;
		}

		if(bAlphaConnected || bMoveRequired){
			Ehs_widget_position_update(pWidget, bAlphaConnected, nAlpha, bXConnected, nXoffset, bYConnected, nYoffset, bWConnected, nWOffset, bHConnected, nHOffset);
			Ehs_widget_commit(pWidget);
		}

		EHS_FB_FINISH(INX_gui_widget_ARG_update___);
	}
}

#if !defined(EHS_GUI_SUPPORT_MODE_B) && !defined(EHS_GUI_SUPPORT_MODE_B_QT)
/**
 * General purpose update function, contains the common code for all of the text update functions.
 *
 * @param[in] pWidget Widget to update
 * @param[in] szString text to be displayed
 *
 */
void Ehs_gui_text2_write(EhsWidgetClass* pWidget, const ehs_char* szString)
{
    Ehs_widget_position_update(pWidget, EHS_FALSE, 0, EHS_FALSE, 0, EHS_FALSE, 0, EHS_FALSE, 0, EHS_FALSE, 0);

    EhsWidgetTextbox_write(pWidget,szString);

    Ehs_widget_commit(pWidget);
}
#endif // ! EHS_GUI_SUPPORT_MODE_B && ! EHS_GUI_SUPPORT_MODE_B_QT

EHS_FB_RUN_FUNCTION(gui_text_string2_data)
{
	EHSH_LOG_INFO("gui_text_string2_data: ENTRY");
	inx_gui_widget_state_type* inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_widget_state && inx_gui_widget_state->pUiWidgetClass) {
		EhsWidgetClass* pWidget = inx_gui_widget_state->pUiWidgetClass;
#if defined(EHS_GUI_SUPPORT_MODE_B) || defined(EHS_GUI_SUPPORT_MODE_B_QT)
		ehs_bool bCommit = EHS_FALSE;

		if (EHS_FB_IN_CONNECTED_API2(INX_gui_text_string2_ARG_data_data)){
			const char* new_text = EHS_FB_IN_S_API2(INX_gui_text_string2_ARG_data_data);
			EHSH_LOG_INFO("  Setting widget data to: '%s'", new_text);
			inx_gui_widget_state->gui.data = (void*)new_text;
			bCommit = EHS_TRUE;
		}

		if (EHS_FB_IN_CONNECTED_API2(INX_gui_text_string2_ARG_data_label)){
			const char* new_label = EHS_FB_IN_S_API2(INX_gui_text_string2_ARG_data_label);
			EHSH_LOG_INFO("  Setting widget label to: '%s'", new_label);
			inx_gui_widget_state->gui.label = (void*)new_label;
			bCommit = EHS_TRUE;
		}

		if(bCommit){
			EHSH_LOG_INFO("  Calling EhsWidgetUI_update(pWidget=%p)", (void*)pWidget);
			EhsWidgetUI_update(pWidget);
			EHSH_LOG_INFO("  Calling Ehs_widget_commit(pWidget=%p)", (void*)pWidget);
			Ehs_widget_commit(pWidget);
			// finish signal fired in the event callback
		}else{
			EHSH_LOG_INFO("  No data/label connected, firing finish event directly");
			EHS_FB_FINISH(INX_gui_text_string2_ARG_data____);
		}
#else
		if(EHS_FB_IN_CONNECTED_API2(INX_gui_text_string2_ARG_data_data)){
			char *str = EHS_FB_IN_S_API2(INX_gui_text_string2_ARG_data_data);
			Ehs_gui_text2_write(pWidget, str);
		}
		EHS_FB_FINISH(INX_gui_text_string2_ARG_data____);
#endif // EHS_GUI_SUPPORT_MODE_B || EHS_GUI_SUPPORT_MODE_B_QT
	}
}

EHS_FB_RUN_FUNCTION(gui_text_float2_data)
{
	inx_gui_widget_state_type* inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_widget_state && inx_gui_widget_state->pUiWidgetClass) {
		EhsWidgetClass* pWidget = inx_gui_widget_state->pUiWidgetClass;
#if defined(EHS_GUI_SUPPORT_MODE_B) || defined(EHS_GUI_SUPPORT_MODE_B_QT)
		ehs_bool bCommit = EHS_FALSE;

		if (EHS_FB_IN_CONNECTED_API2(INX_gui_text_float2_ARG_data_data)){
			inx_gui_widget_state->gui.data = (void*)&EHS_FB_IN_F_API2(INX_gui_text_float2_ARG_data_data);
			bCommit = EHS_TRUE;
		}

		if (EHS_FB_IN_CONNECTED_API2(INX_gui_text_float2_ARG_data_label)){
			inx_gui_widget_state->gui.label = (void*)EHS_FB_IN_S_API2(INX_gui_text_float2_ARG_data_label);
			bCommit = EHS_TRUE;
		}

		if(bCommit){
			EhsWidgetUI_update(pWidget);
			Ehs_widget_commit(pWidget);
			// finish signal fired in the event callback
		}else{
			EHS_FB_FINISH(INX_gui_text_float2_ARG_data____);
		}
#else
		if(EHS_FB_IN_CONNECTED_API2(INX_gui_text_float2_ARG_data_data)){
			char str[EHS_STRING_LENGTH_MAX];
			EhsSprintf(str,"%f",EHS_FB_IN_F_API2(INX_gui_text_float2_ARG_data_data));
			Ehs_gui_text2_write(pWidget, str);
		}
		EHS_FB_FINISH(INX_gui_text_float2_ARG_data____);
#endif // EHS_GUI_SUPPORT_MODE_B || EHS_GUI_SUPPORT_MODE_B_QT
	}
}

EHS_FB_RUN_FUNCTION(gui_text_int2_data)
{
	inx_gui_widget_state_type* inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_widget_state && inx_gui_widget_state->pUiWidgetClass) {
		EhsWidgetClass* pWidget = inx_gui_widget_state->pUiWidgetClass;
#if defined(EHS_GUI_SUPPORT_MODE_B) || defined(EHS_GUI_SUPPORT_MODE_B_QT)
		ehs_bool bCommit = EHS_FALSE;

		if (EHS_FB_IN_CONNECTED_API2(INX_gui_text_int2_ARG_data_data)){
			inx_gui_widget_state->gui.data = (void*)&EHS_FB_IN_I_API2(INX_gui_text_int2_ARG_data_data);
			bCommit = EHS_TRUE;
		}

		if (EHS_FB_IN_CONNECTED_API2(INX_gui_text_int2_ARG_data_label)){
			inx_gui_widget_state->gui.label = (void*)EHS_FB_IN_S_API2(INX_gui_text_int2_ARG_data_label);
			bCommit = EHS_TRUE;
		}

		if(bCommit){
			EhsWidgetUI_update(pWidget);
			Ehs_widget_commit(pWidget);
			// finish signal fired in the event callback
		}else{
			EHS_FB_FINISH(INX_gui_text_int2_ARG_data____);
		}
#else // EHS_GUI_SUPPORT_MODE_B || EHS_GUI_SUPPORT_MODE_B_QT
		if(EHS_FB_IN_CONNECTED_API2(INX_gui_text_int2_ARG_data_data)){
			char str[EHS_STRING_LENGTH_MAX];
			EhsSprintf(str,"%d",EHS_FB_IN_I_API2(INX_gui_text_int2_ARG_data_data));
			Ehs_gui_text2_write(pWidget, str);
		}
		EHS_FB_FINISH(INX_gui_text_int2_ARG_data____);
#endif // EHS_GUI_SUPPORT_MODE_B || EHS_GUI_SUPPORT_MODE_B_QT
	}
}

EHS_FB_RUN_FUNCTION(gui_text_bool2_data)
{
	inx_gui_widget_state_type* inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_widget_state && inx_gui_widget_state->pUiWidgetClass) {
		EhsWidgetClass* pWidget = inx_gui_widget_state->pUiWidgetClass;
#if defined(EHS_GUI_SUPPORT_MODE_B) || defined(EHS_GUI_SUPPORT_MODE_B_QT)
		ehs_bool bCommit = EHS_FALSE;

		if (EHS_FB_IN_CONNECTED_API2(INX_gui_text_bool2_ARG_data_data)){
			inx_gui_widget_state->gui.data = (void*)&EHS_FB_IN_B_API2(INX_gui_text_bool2_ARG_data_data);
			bCommit = EHS_TRUE;
		}

		if (EHS_FB_IN_CONNECTED_API2(INX_gui_text_bool2_ARG_data_label)){
			inx_gui_widget_state->gui.label = (void*)EHS_FB_IN_S_API2(INX_gui_text_bool2_ARG_data_label);
			bCommit = EHS_TRUE;
		}

		if(bCommit){
			EhsWidgetUI_update(pWidget);
			Ehs_widget_commit(pWidget);
			// finish signal fired in the event callback
		}else{
			EHS_FB_FINISH(INX_gui_text_bool2_ARG_data____);
		}
#else // EHS_GUI_SUPPORT_MODE_B || EHS_GUI_SUPPORT_MODE_B_QT
		if(EHS_FB_IN_CONNECTED_API2(INX_gui_text_bool2_ARG_data_data)){
			char str[EHS_STRING_LENGTH_MAX];
			str[0] = (EHS_FB_IN_B_API2(INX_gui_text_bool2_ARG_data_data))?'T':'F';
			str[1] = '\0';
			Ehs_gui_text2_write(pWidget, str);
		}
		EHS_FB_FINISH(INX_gui_text_bool2_ARG_data____);
#endif // EHS_GUI_SUPPORT_MODE_B || EHS_GUI_SUPPORT_MODE_B_QT
	}
}
