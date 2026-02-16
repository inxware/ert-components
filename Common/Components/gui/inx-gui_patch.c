/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved.
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
 * with this file. If not, please visit:
 *  <https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-gui_patch.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "widget.h"
#include "guiparams.h"

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_gui_patch_state
{
	//ehs_uint16 id;
	EhsWidgetClass* pUiWidgetClass;
} inx_gui_patch_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(gui_patch)
EHS_FB_FUNCTION_ENTRY("create", 0x01, gui_patch_create)
EHS_FB_FUNCTION_ENTRY("destroy", 0x02, gui_patch_destroy)
EHS_FB_FUNCTION_ENTRY("show", 0x03, gui_patch_show)
EHS_FB_FUNCTION_ENTRY("hide", 0x04, gui_patch_hide)
EHS_FB_FUNCTION_ENTRY("update", 0x05, gui_patch_update)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_gui_patch_ARG_create_abs_x 1
#define INX_gui_patch_ARG_create_abs_y 2
#define INX_gui_patch_ARG_create_wid 3
#define INX_gui_patch_ARG_create_ht 4
#define INX_gui_patch_ARG_create___ 1
#define INX_gui_patch_ARG_create_click 2
#define INX_gui_patch_ARG_create_mouse_down 3
#define INX_gui_patch_ARG_destroy___ 1
#define INX_gui_patch_ARG_show___ 1
#define INX_gui_patch_ARG_hide___ 1
#define INX_gui_patch_ARG_update_off_x 1
#define INX_gui_patch_ARG_update_off_y 2
#define INX_gui_patch_ARG_update_off_wid 3
#define INX_gui_patch_ARG_update_off_ht 4
#define INX_gui_patch_ARG_update_alpha 5
#define INX_gui_patch_ARG_update_abs_x 1
#define INX_gui_patch_ARG_update_abs_y 2
#define INX_gui_patch_ARG_update_wid 3
#define INX_gui_patch_ARG_update_ht 4
#define INX_gui_patch_ARG_update___ 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(gui_patch)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_gui_patch_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(gui_patch)
{
	EhsGuiParamsType xParams;
	xParams.uClass.xTextbox.nCurve = 0;
    ehs_bool bRet = EHS_TRUE; /* we would prefer to assume initialisation passes */
    char guiParams[MAX_PARAM_STR_LEN];
    const char* pParams;
    ehs_uint8 nByte;
	// this is the reference to the object data for this instance of the function block
	inx_gui_patch_state_type* inx_gui_patch_state = (inx_gui_patch_state_type*)EHS_FB_INIT_CONTEXT;
	inx_gui_patch_state->pUiWidgetClass = NULL;
    pParams = ReadParmFile(&EHS_FB_INIT_PARAMETERS[4], guiParams);
	if (guiParams) {
		EhsParseGuiParameters(guiParams,&xParams);
		if (xParams.eClass == EHS_WIDGET_CLASS_PATCH)
		{
#if defined(EHS_GUI_SUPPORT_MODE_B)
			inx_gui_patch_state->pUiWidgetClass = EhsWidgetUI_init(EHS_OTHER_UI_WIDGET_PANEL, 0, xParams.uClass.xTextbox.nCurve, 0,
			                                                    &(xParams.xRect),xParams.nZorder,
																0, 0, 0, 0, 0,
																xParams.uClass.xPatch,
																xParams.uClass.xPatch,
																/*pFont*/NULL
#ifdef EHS_STORE_WIDGET_NAMES
																,xParams.widgetName
#endif
																);
#else
			inx_gui_patch_state->pUiWidgetClass = EhsWidgetPatch_init(&xParams.xRect, xParams.nZorder,xParams.uClass.xPatch);
#endif
			if ((inx_gui_patch_state->pUiWidgetClass == NULL) || (inx_gui_patch_state->pUiWidgetClass->nState == EHS_WIDGET_STATE_EMPTY))
			{
				bRet = EHS_FALSE;
			}
			else
			{
				/* parse LAB parameters */
				ehs_uint8 nByte = 0;
				pParams = EhsGetUint8FromString(&nByte, pParams);
				inx_gui_patch_state->pUiWidgetClass->bMaintainAspectRatio = (ehs_bool)nByte; //@todo Aspect ratio parameters for text and patches should be removed - only applies to images
				pParams = EhsGetUint8FromString(&nByte, pParams);
				inx_gui_patch_state->pUiWidgetClass->bCaptureClicksIgnoringZOrder = (ehs_bool)nByte;

				bRet = EHS_TRUE;
			}
		}
	}
    return bRet; /* initialisation always succeeds -- why? [NR] */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(gui_patch)
{
	inx_gui_patch_state_type *inx_gui_patch_state = (inx_gui_patch_state_type*)EHS_FB_DESTROY_CONTEXT;
	
	if(inx_gui_patch_state && inx_gui_patch_state->pUiWidgetClass != NULL){
		EhsWidget_destroy(inx_gui_patch_state->pUiWidgetClass);
	}
	return EHS_TRUE;
}

static void gui_patch_event_callback(struct EhsWidgetStruct* pWidget, ehs_uint16 event_id, const char* label, void* data)
{
	if(pWidget){
		EhsFunctionInstanceDataType* pFIdata = pWidget->pFIData;
		if(pFIdata == NULL){
			return;
		}

		if(event_id & EHS_WIDGET_UI_EVENT_MOUSE_CLICKED){	
			EHS_FB_FINISH(INX_gui_patch_ARG_create_click);
		}

		if(event_id & EHS_WIDGET_UI_EVENT_MOUSE_DOWN){	
			EHS_FB_FINISH(INX_gui_patch_ARG_create_mouse_down);
		}
	}
}

//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION create MACRO START -- DO NOT ALTER
/**
 * Definition of gui_patch_create.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(gui_patch_create)
{
	inx_gui_patch_state_type* inx_gui_patch_state = (inx_gui_patch_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_patch_state) {
		EhsWidgetClass* pWidget = inx_gui_patch_state->pUiWidgetClass;

		if(!pWidget){
			return;
		}
#if defined(EHS_GUI_SUPPORT_MODE_B) 
		/* set up on click callback */
		EHS_WIDGET_UI(pWidget).event_callback = gui_patch_event_callback;
		/* setup widget data */
		EHS_WIDGET_UI(pWidget).data = NULL;
#endif
		pWidget->pFIData = EHS_FB_RUN_CONTEXT_REF;

		// Your code here
		if (EHS_FB_OUT_CONNECTED_API2(INX_gui_patch_ARG_create_abs_x)){
			EHS_FB_OUT_I_API2(INX_gui_patch_ARG_create_abs_x) = pWidget->xCurRect.nLeft;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_gui_patch_ARG_create_abs_y)){
			EHS_FB_OUT_I_API2(INX_gui_patch_ARG_create_abs_y) = pWidget->xCurRect.nTop;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_gui_patch_ARG_create_wid)){
			EHS_FB_OUT_I_API2(INX_gui_patch_ARG_create_wid) = pWidget->xCurRect.nWidth;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_gui_patch_ARG_create_ht)){
			EHS_FB_OUT_I_API2(INX_gui_patch_ARG_create_ht) = pWidget->xCurRect.nHeight;
		}

		EhsWidget_create(pWidget);
#if defined(EHS_GUI_SUPPORT_MODE_A) 
		/*Set number of mouseClick port*/
		pWidget->mouseClickPortNumber = INX_gui_patch_ARG_create_click;
		pWidget->mouseDownPortNumber = INX_gui_patch_ARG_create_mouse_down;
#endif
		EHS_FB_FINISH(INX_gui_patch_ARG_create___);
	}
}//ICB FUNCTION create MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION destroy MACRO START -- DO NOT ALTER
/**
 * Definition of gui_patch_destroy.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(gui_patch_destroy)
{
	inx_gui_patch_state_type* inx_gui_patch_state = (inx_gui_patch_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_patch_state) {
		if(inx_gui_patch_state->pUiWidgetClass != NULL){
			EhsWidget_destroy(inx_gui_patch_state->pUiWidgetClass);
		}
		EHS_FB_FINISH(INX_gui_patch_ARG_destroy___);
	}
}//ICB FUNCTION destroy MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION show MACRO START -- DO NOT ALTER
/**
 * Definition of gui_patch_show.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(gui_patch_show)
{
	inx_gui_patch_state_type* inx_gui_patch_state = (inx_gui_patch_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_patch_state && inx_gui_patch_state->pUiWidgetClass) {
		EhsWidget_show(inx_gui_patch_state->pUiWidgetClass);

		EHS_FB_FINISH(INX_gui_patch_ARG_show___);
	}
}//ICB FUNCTION show MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION hide MACRO START -- DO NOT ALTER
/**
 * Definition of gui_patch_hide.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(gui_patch_hide)
{
	inx_gui_patch_state_type* inx_gui_patch_state = (inx_gui_patch_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_patch_state && inx_gui_patch_state->pUiWidgetClass) {
		EhsWidget_hide(inx_gui_patch_state->pUiWidgetClass);

		EHS_FB_FINISH(INX_gui_patch_ARG_hide___);
	}
}//ICB FUNCTION hide MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION update MACRO START -- DO NOT ALTER
/**
 * Definition of gui_patch_update.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(gui_patch_update)
{
	inx_gui_patch_state_type* inx_gui_patch_state = (inx_gui_patch_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_gui_patch_state) {
		EhsWidgetClass* pWidget = inx_gui_patch_state->pUiWidgetClass;

		if(!pWidget){
			return;
		}

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

		if (EHS_FB_IN_CONNECTED_API2(INX_gui_patch_ARG_update_off_x)){
			nXoffset = EHS_FB_IN_I_API2(INX_gui_patch_ARG_update_off_x);
			bXConnected = bMoveRequired = EHS_TRUE;
		}
		if (EHS_FB_IN_CONNECTED_API2(INX_gui_patch_ARG_update_off_y)){
			nYoffset = EHS_FB_IN_I_API2(INX_gui_patch_ARG_update_off_y);
			bYConnected = bMoveRequired = EHS_TRUE;
		}
		if (EHS_FB_IN_CONNECTED_API2(INX_gui_patch_ARG_update_off_wid)){
			nWOffset = EHS_FB_IN_I_API2(INX_gui_patch_ARG_update_off_wid);
			bWConnected = bMoveRequired = EHS_TRUE;
		}
		if (EHS_FB_IN_CONNECTED_API2(INX_gui_patch_ARG_update_off_ht)){
			nHOffset = EHS_FB_IN_I_API2(INX_gui_patch_ARG_update_off_ht);
			bHConnected = bMoveRequired = EHS_TRUE;
		}
		if (EHS_FB_IN_CONNECTED_API2(INX_gui_patch_ARG_update_alpha)){
			nAlpha = EHS_FB_IN_I_API2(INX_gui_patch_ARG_update_alpha);
			if (nAlpha < 0) nAlpha = 0;
			if (nAlpha > 255) nAlpha = 255;
			EhsWidget_fade(pWidget, (ehs_uint8)nAlpha);
			bAlphaConnected = EHS_TRUE;
		}

		if (bMoveRequired){
			EhsWidget_move(pWidget, nXoffset, nYoffset, nWOffset, nHOffset);
		}

		if (EHS_FB_OUT_CONNECTED_API2(INX_gui_patch_ARG_update_abs_x)){
			EHS_FB_OUT_I_API2(INX_gui_patch_ARG_update_abs_x) = pWidget->xCurRect.nLeft;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_gui_patch_ARG_update_abs_y)){
			EHS_FB_OUT_I_API2(INX_gui_patch_ARG_update_abs_y) = pWidget->xCurRect.nTop;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_gui_patch_ARG_update_wid)){
			EHS_FB_OUT_I_API2(INX_gui_patch_ARG_update_wid) = pWidget->xCurRect.nWidth;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_gui_patch_ARG_update_ht)){
			EHS_FB_OUT_I_API2(INX_gui_patch_ARG_update_ht) = pWidget->xCurRect.nHeight;
		}

		if(bAlphaConnected || bMoveRequired){
			Ehs_widget_position_update(pWidget, bAlphaConnected, nAlpha, bXConnected, nXoffset, bYConnected, nYoffset, bWConnected, nWOffset, bHConnected, nHOffset);
			Ehs_widget_commit(pWidget);
		}

		EHS_FB_FINISH(INX_gui_patch_ARG_update___);
	}
}//ICB FUNCTION update MACRO END -- DO NOT ALTER THIS LINE
