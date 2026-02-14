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
#include "inx-gui_image_file.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "widget.h"
#include "guiparams.h"

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_GUI_Image_File_state
{
	//ehs_uint16 id;
	EhsWidgetUiImage image;
	EhsWidgetClass* pUiWidgetClass;
} inx_GUI_Image_File_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(GUI_Image_File)
EHS_FB_FUNCTION_ENTRY("create", 0x01, GUI_Image_File_create)
EHS_FB_FUNCTION_ENTRY("destroy", 0x02, GUI_Image_File_destroy)
EHS_FB_FUNCTION_ENTRY("load", 0x03, GUI_Image_File_load)
EHS_FB_FUNCTION_ENTRY("show", 0x04, GUI_Image_File_show)
EHS_FB_FUNCTION_ENTRY("hide", 0x05, GUI_Image_File_hide)
EHS_FB_FUNCTION_ENTRY("update", 0x06, GUI_Image_File_update)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_GUI_Image_File_ARG_create_abs_x 1
#define INX_GUI_Image_File_ARG_create_abs_y 2
#define INX_GUI_Image_File_ARG_create_wid 3
#define INX_GUI_Image_File_ARG_create_ht 4
#define INX_GUI_Image_File_ARG_create___ 1
#define INX_GUI_Image_File_ARG_create_click 2
#define INX_GUI_Image_File_ARG_create_mouse_down 3
#define INX_GUI_Image_File_ARG_destroy___ 1
#define INX_GUI_Image_File_ARG_load_file 1
#define INX_GUI_Image_File_ARG_load___ 1
#define INX_GUI_Image_File_ARG_show___ 1
#define INX_GUI_Image_File_ARG_hide___ 1
#define INX_GUI_Image_File_ARG_update_off_x 1
#define INX_GUI_Image_File_ARG_update_off_y 2
#define INX_GUI_Image_File_ARG_update_off_wid 3
#define INX_GUI_Image_File_ARG_update_off_ht 4
#define INX_GUI_Image_File_ARG_update_alpha 5
#define INX_GUI_Image_File_ARG_update_abs_x 1
#define INX_GUI_Image_File_ARG_update_abs_y 2
#define INX_GUI_Image_File_ARG_update_wid 3
#define INX_GUI_Image_File_ARG_update_ht 4
#define INX_GUI_Image_File_ARG_update___ 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(GUI_Image_File)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_GUI_Image_File_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(GUI_Image_File)
{
	EhsGuiParamsType xParams;
    ehs_bool bRet = EHS_TRUE;
    char guiParams[MAX_PARAM_STR_LEN];
    const char* pParams;
    ehs_uint8 nByte;
    ehs_bool bMaintainAspect;
    ehs_bool bCaptureClicks;
    ehs_bool bLoadImageFromAppDir;

	inx_GUI_Image_File_state_type* inx_GUI_Image_File_state = (inx_GUI_Image_File_state_type*)EHS_FB_INIT_CONTEXT;
	inx_GUI_Image_File_state->pUiWidgetClass = NULL;
    pParams = ReadParmFile(&EHS_FB_INIT_PARAMETERS[4], guiParams);
	if (guiParams) {
		EhsParseGuiParameters(guiParams,&xParams);

		/* parse parameters */
		pParams = EhsGetUint8FromString(&nByte, pParams);
		bMaintainAspect = (ehs_bool)nByte;
		pParams = EhsGetUint8FromString(&nByte, pParams);
		bCaptureClicks = (ehs_bool)nByte;
		pParams = EhsGetUint8FromString(&nByte, pParams);
		bLoadImageFromAppDir = (ehs_bool)nByte;
#if defined(EHS_GUI_SUPPORT_MODE_A)
       /* Create and Initialise the image surfaces for rendering for MODE A rendering */
       inx_GUI_Image_File_state->pUiWidgetClass = EhsWidgetImage_init(&xParams.xRect, xParams.nZorder, xParams.uClass.xBitmap.nImageAlpha,xParams.uClass.xBitmap.szBitmapName, bLoadImageFromAppDir);
#else
		/* Set the image widget specific parameters for a widget library to render */
		inx_GUI_Image_File_state->image.bLoadImageFromAppDir = bLoadImageFromAppDir;
		inx_GUI_Image_File_state->image.bDynamicFilename = EHS_FALSE;
		EhsStrcpy(inx_GUI_Image_File_state->image.szFilename, xParams.uClass.xBitmap.szBitmapName);
		xParams.uClass.xPatch.sComp.nAlpha = xParams.uClass.xBitmap.nImageAlpha;
		inx_GUI_Image_File_state->pUiWidgetClass = EhsWidgetUI_init(EHS_OTHER_UI_WIDGET_IMAGE, 0, 0, 0,
															&(xParams.xRect), xParams.nZorder,
															0, 0, 0, 0, 0,
															xParams.uClass.xPatch,
															xParams.uClass.xPatch,
															/*pFont*/NULL);
#endif
		if ((inx_GUI_Image_File_state->pUiWidgetClass == NULL) || (inx_GUI_Image_File_state->pUiWidgetClass->nState == EHS_WIDGET_STATE_EMPTY))
		{
			bRet = EHS_FALSE;
		}
		else
		{
			/* set widget parameters */
			inx_GUI_Image_File_state->pUiWidgetClass->bMaintainAspectRatio = bMaintainAspect;
			inx_GUI_Image_File_state->pUiWidgetClass->bCaptureClicksIgnoringZOrder = bCaptureClicks;
			bRet = EHS_TRUE;
		}
	}
    return bRet;
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(GUI_Image_File)
{
	inx_GUI_Image_File_state_type *inx_GUI_Image_File_state = (inx_GUI_Image_File_state_type*)EHS_FB_DESTROY_CONTEXT;
	if (inx_GUI_Image_File_state) {
		if(inx_GUI_Image_File_state->pUiWidgetClass != NULL){
			EhsWidget_destroy(inx_GUI_Image_File_state->pUiWidgetClass);
		}
	}
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE



static void GUI_Image_File_event_callback(struct EhsWidgetStruct* pWidget, ehs_uint16 event_id, const char* label, void* data)
{
	if(pWidget){
		EhsFunctionInstanceDataType* pFIdata = pWidget->pFIData;
		if(pFIdata == NULL){
			return;
		}

		if(event_id & EHS_WIDGET_UI_EVENT_MOUSE_CLICKED){
			EHS_FB_FINISH(INX_GUI_Image_File_ARG_create_click);
		}

		if(event_id & EHS_WIDGET_UI_EVENT_MOUSE_DOWN){
			EHS_FB_FINISH(INX_GUI_Image_File_ARG_create_mouse_down);
		}
	}
}


//ICB FUNCTION create MACRO START -- DO NOT ALTER
/**
 * Definition of GUI_Image_File_create.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(GUI_Image_File_create)
{
	inx_GUI_Image_File_state_type* inx_GUI_Image_File_state = (inx_GUI_Image_File_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_GUI_Image_File_state) {
		EhsWidgetClass* pWidget = inx_GUI_Image_File_state->pUiWidgetClass;

		if(!pWidget){
			return;
		}
#if defined(EHS_GUI_SUPPORT_MODE_B)
		/* set up on click callback */
		EHS_WIDGET_UI(pWidget).event_callback = GUI_Image_File_event_callback;
		/* setup widget data */
		EHS_WIDGET_UI(pWidget).data = (void*) &inx_GUI_Image_File_state->image;
#endif
		if (pWidget->bMaintainAspectRatio)
		{
			EhsWidget_resizeWidgetToMaintainAspectRatio(pWidget);
		}
		else
		{
			EhsWidget_resetWidgetSizeToDesignTime(pWidget);
		}

		pWidget->pFIData = EHS_FB_RUN_CONTEXT_REF;

		// Your code here
		if (EHS_FB_OUT_CONNECTED_API2(INX_GUI_Image_File_ARG_create_abs_x)){
			EHS_FB_OUT_I_API2(INX_GUI_Image_File_ARG_create_abs_x) = pWidget->xCurRect.nLeft;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_GUI_Image_File_ARG_create_abs_y)){
			EHS_FB_OUT_I_API2(INX_GUI_Image_File_ARG_create_abs_y) = pWidget->xCurRect.nTop;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_GUI_Image_File_ARG_create_wid)){
			EHS_FB_OUT_I_API2(INX_GUI_Image_File_ARG_create_wid) = pWidget->xCurRect.nWidth;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_GUI_Image_File_ARG_create_ht)){
			EHS_FB_OUT_I_API2(INX_GUI_Image_File_ARG_create_ht) = pWidget->xCurRect.nHeight;
		}

		EhsWidget_create(pWidget);
#if defined(EHS_GUI_SUPPORT_MODE_A)
		/*Set number of mouseClick port*/
		pWidget->mouseClickPortNumber = INX_GUI_Image_File_ARG_create_click;
		pWidget->mouseDownPortNumber = INX_GUI_Image_File_ARG_create_mouse_down;
#endif
		EHS_FB_FINISH(INX_GUI_Image_File_ARG_create___);
	}
}//ICB FUNCTION create MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION destroy MACRO START -- DO NOT ALTER
/**
 * Definition of GUI_Image_File_destroy.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(GUI_Image_File_destroy)
{
	inx_GUI_Image_File_state_type* inx_GUI_Image_File_state = (inx_GUI_Image_File_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_GUI_Image_File_state) {
		if(inx_GUI_Image_File_state->pUiWidgetClass != NULL){
			EhsWidget_destroy(inx_GUI_Image_File_state->pUiWidgetClass);
		}
		EHS_FB_FINISH(INX_GUI_Image_File_ARG_destroy___);
	}
}//ICB FUNCTION destroy MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION load MACRO START -- DO NOT ALTER
/**
 * Definition of GUI_Image_File_load.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(GUI_Image_File_load)
{
	inx_GUI_Image_File_state_type* inx_GUI_Image_File_state = (inx_GUI_Image_File_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_GUI_Image_File_state) {
		EhsFunctionInstanceDataType* iBeforeVal = NULL;
		EhsWidgetClass *pWidget = inx_GUI_Image_File_state->pUiWidgetClass;

		if (pWidget && pWidget->nState && EHS_FB_IN_CONNECTED_API2(INX_GUI_Image_File_ARG_load_file))   /* if a string is connected then get a dynamic variable for the filename. */
		{
#if defined(EHS_GUI_SUPPORT_MODE_B)
			EhsStrcpy(inx_GUI_Image_File_state->image.szFilename, EHS_FB_IN_S_API2(INX_GUI_Image_File_ARG_load_file) );
#else
			EhsWidgetImage_setFilename(pWidget, EHS_FB_IN_S_API2(INX_GUI_Image_File_ARG_load_file), EHS_TRUE);
#endif
			/* check that:
			* 1. the widget has been initialised by the subclass
			* 2. the widget has previously been created */
			if (EHS_WIDGET_STATE_INITIALIZED(pWidget->nState) && ((pWidget->nState & EHS_WIDGET_STATE_CREATED) == EHS_WIDGET_STATE_CREATED))
			{
				// implicit destroy and recreate
				iBeforeVal = pWidget->pFIData;

				EhsWidget_destroy(pWidget);
#if defined(EHS_GUI_SUPPORT_MODE_B)
				inx_GUI_Image_File_state->image.bDynamicFilename = EHS_TRUE;
				/* set up on click callback */
				EHS_WIDGET_UI(pWidget).event_callback = GUI_Image_File_event_callback;
				/* setup widget data */
				EHS_WIDGET_UI(pWidget).data = (void*) &inx_GUI_Image_File_state->image;
#endif
				EhsWidget_create(pWidget);
#if defined(EHS_GUI_SUPPORT_MODE_A)
				/*Set number of mouseClick port ~todo - this needs making nicer!*/
				pWidget->mouseClickPortNumber = INX_GUI_Image_File_ARG_create_click;
				pWidget->mouseDownPortNumber = INX_GUI_Image_File_ARG_create_mouse_down;
#endif
				//NOTE - as pfiData is a static associated with each function have to reset pfiData to value set by create function
				pWidget->pFIData = iBeforeVal;
				/* Use any dynamic offsets for the widget to position the new image to where eer it may have moved to */
				//pWidget->UpdatedOffsettRect;

				EhsWidget_resetWidgetSizeToDesignTime(pWidget);

				Ehs_widget_position_update(pWidget, EHS_TRUE, pWidget->nAlpha, EHS_TRUE, pWidget->UpdatedOffsettRect.nLeft, EHS_TRUE, pWidget->UpdatedOffsettRect.nTop, EHS_TRUE, pWidget->UpdatedOffsettRect.nWidth, EHS_TRUE, pWidget->UpdatedOffsettRect.nHeight);

				if (pWidget->bMaintainAspectRatio){
					EhsWidget_resizeWidgetToMaintainAspectRatio(pWidget);
				}

				Ehs_widget_commit(pWidget);
			}
		}

		EHS_FB_FINISH(INX_GUI_Image_File_ARG_load___);
	}
}//ICB FUNCTION load MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION show MACRO START -- DO NOT ALTER
/**
 * Definition of GUI_Image_File_show.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(GUI_Image_File_show)
{
	inx_GUI_Image_File_state_type* inx_GUI_Image_File_state = (inx_GUI_Image_File_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_GUI_Image_File_state && inx_GUI_Image_File_state->pUiWidgetClass) {
		EhsWidget_show(inx_GUI_Image_File_state->pUiWidgetClass);
		EHS_FB_FINISH(INX_GUI_Image_File_ARG_show___);
	}
}//ICB FUNCTION show MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION hide MACRO START -- DO NOT ALTER
/**
 * Definition of GUI_Image_File_hide.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(GUI_Image_File_hide)
{
	inx_GUI_Image_File_state_type* inx_GUI_Image_File_state = (inx_GUI_Image_File_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_GUI_Image_File_state && inx_GUI_Image_File_state->pUiWidgetClass) {
		EhsWidget_hide(inx_GUI_Image_File_state->pUiWidgetClass);
		EHS_FB_FINISH(INX_GUI_Image_File_ARG_hide___);
	}
}//ICB FUNCTION hide MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION update MACRO START -- DO NOT ALTER
/**
 * Definition of GUI_Image_File_update.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(GUI_Image_File_update)
{
	inx_GUI_Image_File_state_type* inx_GUI_Image_File_state = (inx_GUI_Image_File_state_type*)EHS_FB_RUN_CONTEXT;
	if (inx_GUI_Image_File_state) {
		EhsWidgetClass* pWidget = inx_GUI_Image_File_state->pUiWidgetClass;

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

		if (EHS_FB_IN_CONNECTED_API2(INX_GUI_Image_File_ARG_update_off_x)){
			nXoffset = EHS_FB_IN_I_API2(INX_GUI_Image_File_ARG_update_off_x);
			bXConnected = bMoveRequired = EHS_TRUE;
		}
		if (EHS_FB_IN_CONNECTED_API2(INX_GUI_Image_File_ARG_update_off_y)){
			nYoffset = EHS_FB_IN_I_API2(INX_GUI_Image_File_ARG_update_off_y);
			bYConnected = bMoveRequired = EHS_TRUE;
		}
		if (EHS_FB_IN_CONNECTED_API2(INX_GUI_Image_File_ARG_update_off_wid)){
			nWOffset = EHS_FB_IN_I_API2(INX_GUI_Image_File_ARG_update_off_wid);
			bWConnected = bMoveRequired = EHS_TRUE;
		}
		if (EHS_FB_IN_CONNECTED_API2(INX_GUI_Image_File_ARG_update_off_ht)){
			nHOffset = EHS_FB_IN_I_API2(INX_GUI_Image_File_ARG_update_off_ht);
			bHConnected = bMoveRequired = EHS_TRUE;
		}
		if (EHS_FB_IN_CONNECTED_API2(INX_GUI_Image_File_ARG_update_alpha)){
			nAlpha = EHS_FB_IN_I_API2(INX_GUI_Image_File_ARG_update_alpha);
			if (nAlpha < 0) nAlpha = 0;
			if (nAlpha > 255) nAlpha = 255;
			EhsWidget_fade(pWidget, (ehs_uint8)nAlpha);
			bAlphaConnected = EHS_TRUE;
		}

		if (bMoveRequired){
			EhsWidget_move(pWidget, nXoffset, nYoffset, nWOffset, nHOffset);
		}

		if (EHS_FB_OUT_CONNECTED_API2(INX_GUI_Image_File_ARG_update_abs_x)){
			EHS_FB_OUT_I_API2(INX_GUI_Image_File_ARG_update_abs_x) = pWidget->xCurRect.nLeft;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_GUI_Image_File_ARG_update_abs_y)){
			EHS_FB_OUT_I_API2(INX_GUI_Image_File_ARG_update_abs_y) = pWidget->xCurRect.nTop;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_GUI_Image_File_ARG_update_wid)){
			EHS_FB_OUT_I_API2(INX_GUI_Image_File_ARG_update_wid) = pWidget->xCurRect.nWidth;
		}
		if (EHS_FB_OUT_CONNECTED_API2(INX_GUI_Image_File_ARG_update_ht)){
			EHS_FB_OUT_I_API2(INX_GUI_Image_File_ARG_update_ht) = pWidget->xCurRect.nHeight;
		}

		if(bAlphaConnected || bMoveRequired){
			EhsWidget_resetWidgetSizeToDesignTime(pWidget); // do everything relative to the design position and not the last AR maintained position and size
			Ehs_widget_position_update(pWidget, bAlphaConnected, nAlpha, bXConnected, nXoffset, bYConnected, nYoffset, bWConnected, nWOffset, bHConnected, nHOffset);
			if (pWidget->bMaintainAspectRatio){
				EhsWidget_resizeWidgetToMaintainAspectRatio(pWidget);
			}else{
				//EhsWidget_resetWidgetSizeToDesignTime(pWidget);
			}
			Ehs_widget_commit(pWidget);
		}

		EHS_FB_FINISH(INX_GUI_Image_File_ARG_update___);
	}
}//ICB FUNCTION update MACRO END -- DO NOT ALTER THIS LINE
