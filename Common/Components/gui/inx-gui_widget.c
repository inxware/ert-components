
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


EHS_FB_IDENTIFY_FUNCTION(gui_widget)
{

	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_gui_widget_state_type);
}

EHS_FB_INIT_FUNCTION(gui_widget)
{
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
	
	pParams = ReadParmFile(&EHS_FB_INIT_PARAMETERS[4], guiParams);
	if (guiParams[0]) {
		EhsParseGuiParameters(guiParams, &xParams);
		
		if (xParams.eClass == EHS_WIDGET_CLASS_TEXTBOX)
		{
#ifdef EHS_GUI_SUPPORT_MODE_B
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
#else
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

#endif // EHS_GUI_SUPPORT_MODE_B
			if ((inx_gui_widget_state->pUiWidgetClass == NULL) || (inx_gui_widget_state->pUiWidgetClass->nState == EHS_WIDGET_STATE_EMPTY))
			{
				bRet = EHS_FALSE;
			}
			else
			{
				/* parse parameters */
				ehs_uint8 nByte = 0;
				pParams = EhsGetUint8FromString(&nByte, pParams);
				inx_gui_widget_state->pUiWidgetClass->bMaintainAspectRatio = (ehs_bool)nByte;
				pParams = EhsGetUint8FromString(&nByte, pParams);
				inx_gui_widget_state->pUiWidgetClass->bCaptureClicksIgnoringZOrder = (ehs_bool)nByte;
#ifdef EHS_GUI_SUPPORT_MODE_B
				if(EhsWidgetUI_is_float_type(inx_gui_widget_state->pUiWidgetClass)){
					ehs_sint32 nNoOfDecPlaces = 0;
					pParams = EhsGetSint32FromString(&nNoOfDecPlaces, pParams);
					if(nNoOfDecPlaces >= 0){
						EHS_WIDGET_UI(inx_gui_widget_state->pUiWidgetClass).nNoOfDecPlaces = (ehs_uint16)nNoOfDecPlaces;
					}
				}
#endif // EHS_GUI_SUPPORT_MODE_B
				inx_gui_widget_state->pUiWidgetClass->bContentChanged = EHS_TRUE; /* This should be done in the common code */
				bRet = EHS_TRUE;
			}
		}
    }

	/* Add any further intialisation code here */
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
			return;
		}

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
				const char* str = (const char*)data;
				static const ehs_uint32 UI_OUTPUT_STR_LEN = EHS_STRING_LENGTH_MAX-1;
				(EHS_FB_OUT_S_API2(INX_gui_widget_ARG_create_data_out))[UI_OUTPUT_STR_LEN] = '\0';
				EhsStrncpy(EHS_FB_OUT_S_API2(INX_gui_widget_ARG_create_data_out), str, UI_OUTPUT_STR_LEN);
				bDataChanged = (event_id & EHS_WIDGET_UI_EVENT_DATA_CHANGED) ? EHS_TRUE : EHS_FALSE;
			}else if(EhsWidgetUI_is_bool_type(pWidget)){
				const ehs_bool* bool_val = (const ehs_bool*)data;
				EHS_FB_OUT_B_API2(INX_gui_widget_ARG_create_data_out) = *bool_val;
				bDataChanged = (event_id & EHS_WIDGET_UI_EVENT_DATA_CHANGED) ? EHS_TRUE : EHS_FALSE;
			}else if(EhsWidgetUI_is_int_type(pWidget)){
				const ehs_sint32* int_val = (const ehs_sint32*)data;
				EHS_FB_OUT_I_API2(INX_gui_widget_ARG_create_data_out) = *int_val;
				bDataChanged = (event_id & EHS_WIDGET_UI_EVENT_DATA_CHANGED) ? EHS_TRUE : EHS_FALSE;
			}else if(EhsWidgetUI_is_float_type(pWidget)){
				const float float_val = *(const float*)data;
				EHS_FB_OUT_F_API2(INX_gui_widget_ARG_create_data_out) = float_val;
				bDataChanged = (event_id & EHS_WIDGET_UI_EVENT_DATA_CHANGED) ? EHS_TRUE : EHS_FALSE;
			}
		}
		// updated by the input event port
		if((event_id & EHS_WIDGET_UI_EVENT_LABEL_UPDATED) || (event_id & EHS_WIDGET_UI_EVENT_DATA_UPDATED)){
			EHS_FB_FINISH(INX_gui_widget_ARG_create_data___);
		}
		// changed by the user via ui
		if(bDataChanged){
			EHS_FB_FINISH(INX_gui_widget_ARG_create_change);
		}
		// output mouse click event
		if(event_id & EHS_WIDGET_UI_EVENT_MOUSE_CLICKED){	
			EHS_FB_FINISH(INX_gui_widget_ARG_create_click);
		}

		if(event_id & EHS_WIDGET_UI_EVENT_MOUSE_DOWN){	
			EHS_FB_FINISH(INX_gui_widget_ARG_create_mouse_down);
		}
	}
}

EHS_FB_RUN_FUNCTION(gui_widget_create)
{
	inx_gui_widget_state_type* inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_widget_state && inx_gui_widget_state->pUiWidgetClass) {
		EhsWidgetClass* pWidget = inx_gui_widget_state->pUiWidgetClass;
		if (pWidget) {
#ifdef EHS_GUI_SUPPORT_MODE_B
			/* set up event callback */
			EHS_WIDGET_UI(pWidget).event_callback = gui_widget_event_callback;
			/* setup widget data */
			EHS_WIDGET_UI(pWidget).data = (void*) &inx_gui_widget_state->gui;
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

			EhsWidget_create(pWidget);
#ifndef EHS_GUI_SUPPORT_MODE_B
			/*Set number of mouseClick port*/
			pWidget->mouseClickPortNumber = INX_gui_widget_ARG_create_click;
			pWidget->mouseDownPortNumber = INX_gui_widget_ARG_create_mouse_down;
#endif
			pWidget->bContentChanged = EHS_TRUE; /* This should be done in the common code */

			EHS_FB_FINISH(INX_gui_widget_ARG_create___);		
		}
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

#ifndef EHS_GUI_SUPPORT_MODE_B
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
#endif // EHS_GUI_SUPPORT_MODE_B

EHS_FB_RUN_FUNCTION(gui_text_string2_data)
{
	inx_gui_widget_state_type* inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_widget_state && inx_gui_widget_state->pUiWidgetClass) {
		EhsWidgetClass* pWidget = inx_gui_widget_state->pUiWidgetClass;
#ifdef EHS_GUI_SUPPORT_MODE_B
		ehs_bool bCommit = EHS_FALSE;

		if (EHS_FB_IN_CONNECTED_API2(INX_gui_text_string2_ARG_data_data)){
			inx_gui_widget_state->gui.data = (void*)EHS_FB_IN_S_API2(INX_gui_text_string2_ARG_data_data);
			bCommit = EHS_TRUE;
		}

		if (EHS_FB_IN_CONNECTED_API2(INX_gui_text_string2_ARG_data_label)){
			inx_gui_widget_state->gui.label = (void*)EHS_FB_IN_S_API2(INX_gui_text_string2_ARG_data_label);
			bCommit = EHS_TRUE;
		}

		if(bCommit){
			EhsWidgetUI_update(pWidget);
			Ehs_widget_commit(pWidget);
			// finish signal fired in the event callback
		}else{
			EHS_FB_FINISH(INX_gui_text_string2_ARG_data____);
		}
#else
		if(EHS_FB_IN_CONNECTED_API2(INX_gui_text_string2_ARG_data_data)){
			char *str = EHS_FB_IN_S_API2(INX_gui_text_string2_ARG_data_data);
			Ehs_gui_text2_write(pWidget, str);
		}
		EHS_FB_FINISH(INX_gui_text_string2_ARG_data____);
#endif // EHS_GUI_SUPPORT_MODE_B
	}
}

EHS_FB_RUN_FUNCTION(gui_text_float2_data)
{
	inx_gui_widget_state_type* inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_widget_state && inx_gui_widget_state->pUiWidgetClass) {
		EhsWidgetClass* pWidget = inx_gui_widget_state->pUiWidgetClass;
#ifdef EHS_GUI_SUPPORT_MODE_B
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
#endif // EHS_GUI_SUPPORT_MODE_B
	}
}

EHS_FB_RUN_FUNCTION(gui_text_int2_data)
{
	inx_gui_widget_state_type* inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_widget_state && inx_gui_widget_state->pUiWidgetClass) {
		EhsWidgetClass* pWidget = inx_gui_widget_state->pUiWidgetClass;
#ifdef EHS_GUI_SUPPORT_MODE_B
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
#else
		if(EHS_FB_IN_CONNECTED_API2(INX_gui_text_int2_ARG_data_data)){
			char str[EHS_STRING_LENGTH_MAX];
			EhsSprintf(str,"%d",EHS_FB_IN_I_API2(INX_gui_text_int2_ARG_data_data));
			Ehs_gui_text2_write(pWidget, str);
		}
		EHS_FB_FINISH(INX_gui_text_int2_ARG_data____);
#endif // EHS_GUI_SUPPORT_MODE_B
	}
}

EHS_FB_RUN_FUNCTION(gui_text_bool2_data)
{
	inx_gui_widget_state_type* inx_gui_widget_state = (inx_gui_widget_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_widget_state && inx_gui_widget_state->pUiWidgetClass) {
		EhsWidgetClass* pWidget = inx_gui_widget_state->pUiWidgetClass;
#ifdef EHS_GUI_SUPPORT_MODE_B
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
#else
		if(EHS_FB_IN_CONNECTED_API2(INX_gui_text_bool2_ARG_data_data)){
			char str[EHS_STRING_LENGTH_MAX];
			str[0] = (EHS_FB_IN_B_API2(INX_gui_text_bool2_ARG_data_data))?'T':'F';
			str[1] = '\0';
			Ehs_gui_text2_write(pWidget, str);
		}
		EHS_FB_FINISH(INX_gui_text_bool2_ARG_data____);
#endif // EHS_GUI_SUPPORT_MODE_B
	}
}
