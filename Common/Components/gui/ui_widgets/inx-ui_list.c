//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-ui_list.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "widget.h"
#include "guiparams.h"

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_ui_list_state
{
	ehs_uint16 id;
	EhsWidgetClass* pUiWidgetClass;
	EhsWidgetUiList list;
} inx_ui_list_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(ui_list)
EHS_FB_FUNCTION_ENTRY("create", 0x01, ui_list_create)
EHS_FB_FUNCTION_ENTRY("destroy", 0x02, ui_list_destroy)
EHS_FB_FUNCTION_ENTRY("show", 0x03, ui_list_show)
EHS_FB_FUNCTION_ENTRY("hide", 0x04, ui_list_hide)
EHS_FB_FUNCTION_ENTRY("update", 0x05, ui_list_update)
EHS_FB_FUNCTION_ENTRY("append", 0x07, ui_list_append)
EHS_FB_FUNCTION_ENTRY("remove", 0x08, ui_list_remove)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_ui_list_ARG_create_abs_x 1
#define INX_ui_list_ARG_create_abs_y 2
#define INX_ui_list_ARG_create_wid 3
#define INX_ui_list_ARG_create_ht 4
#define INX_ui_list_ARG_create___ 1
#define INX_ui_list_ARG_destroy___ 1
#define INX_ui_list_ARG_show___ 1
#define INX_ui_list_ARG_hide___ 1
#define INX_ui_list_ARG_update_off_x 1
#define INX_ui_list_ARG_update_off_y 2
#define INX_ui_list_ARG_update_off_wid 3
#define INX_ui_list_ARG_update_off_ht 4
#define INX_ui_list_ARG_update_alpha 5
#define INX_ui_list_ARG_update_abs_x 1
#define INX_ui_list_ARG_update_abs_y 2
#define INX_ui_list_ARG_update_wid 3
#define INX_ui_list_ARG_update_ht 4
#define INX_ui_list_ARG_update___ 1
#define INX_ui_list_ARG_append_item 1
#define INX_ui_list_ARG_append_rgb 2
#define INX_ui_list_ARG_append_appended 1
#define INX_ui_list_ARG_remove_cleared 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER

/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(ui_list)
{
	/* Uncomment the following if you need to parse the parameters to calculate memory required */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_ui_list_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(ui_list)
{
	
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_ui_list_state_type* inx_ui_list_state = (inx_ui_list_state_type*)EHS_FB_INIT_CONTEXT;
	inx_ui_list_state->pUiWidgetClass = NULL;
	EhsGuiParamsType xParams;
	char guiParams[MAX_PARAM_STR_LEN];
    const char* pParams;

	pParams = ReadParmFile(&EHS_FB_INIT_PARAMETERS[4], guiParams);
	if (guiParams) {
		EhsParseGuiParameters(guiParams,&xParams);
		
		if (xParams.eClass == EHS_WIDGET_CLASS_PATCH) // extended ui widgets are generated as patches by iGB
		{
			inx_ui_list_state->id = EHS_OTHER_UI_WIDGET_LIST;
			inx_ui_list_state->pUiWidgetClass = EhsWidgetUI_init(inx_ui_list_state->id, 0, xParams.uClass.xTextbox.nCurve, 0,
			                                                    &(xParams.xRect),xParams.nZorder,
																0, 0, 0, 0, 0,
																xParams.uClass.xPatch,
																xParams.uClass.xPatch,
																/*pFont*/NULL);
			if ((inx_ui_list_state->pUiWidgetClass == NULL) || (inx_ui_list_state->pUiWidgetClass->nState == EHS_WIDGET_STATE_EMPTY))
			{
				bRet = EHS_FALSE;
			}
			else
			{			
				inx_ui_list_state->pUiWidgetClass->bMaintainAspectRatio = EHS_FALSE;
				inx_ui_list_state->pUiWidgetClass->bCaptureClicksIgnoringZOrder = EHS_FALSE;

				inx_ui_list_state->list.state = EHS_UI_WIDGET_LIST_IDLE;
				inx_ui_list_state->list.rgb = 0xFFFFFF;
				pParams = EhsGetUint32FromString(&inx_ui_list_state->list.max_size, pParams);
				pParams = EhsGetUint8FromString(&inx_ui_list_state->list.reversed, pParams);

				bRet = EHS_TRUE;
			}
		}
	}

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(ui_list)
{
	inx_ui_list_state_type* inx_ui_list_state = (inx_ui_list_state_type*)EHS_FB_DESTROY_CONTEXT;
	if(inx_ui_list_state && inx_ui_list_state->pUiWidgetClass != NULL){
		EhsWidget_destroy(inx_ui_list_state->pUiWidgetClass);
	}
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION create MACRO START -- DO NOT ALTER
/**
 * Definition of ui_list_create.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ui_list_create)
{
	inx_ui_list_state_type* inx_ui_list_state = (inx_ui_list_state_type*)EHS_FB_RUN_CONTEXT;

	EhsWidgetClass* pWidget = (inx_ui_list_state) ? inx_ui_list_state->pUiWidgetClass : NULL;

	if(!pWidget){
		return;
	}

	inx_ui_list_state->list.state = EHS_UI_WIDGET_LIST_IDLE;
	inx_ui_list_state->list.rgb = 0xFFFFFF;
	EHS_WIDGET_UI(pWidget).data = (void*) &inx_ui_list_state->list;

	/* set up on click callback */
	//EHS_WIDGET_UI(pWidget).event_callback = ui_chart_event_callback;

	pWidget->pFIData = EHS_FB_RUN_CONTEXT_REF;

	// Your code here
	if (EHS_FB_OUT_CONNECTED_API2(INX_ui_list_ARG_create_abs_x)){
		EHS_FB_OUT_I_API2(INX_ui_list_ARG_create_abs_x) = pWidget->xCurRect.nLeft;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_ui_list_ARG_create_abs_y)){
		EHS_FB_OUT_I_API2(INX_ui_list_ARG_create_abs_y) = pWidget->xCurRect.nTop;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_ui_list_ARG_create_wid)){
		EHS_FB_OUT_I_API2(INX_ui_list_ARG_create_wid) = pWidget->xCurRect.nWidth;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_ui_list_ARG_create_ht)){
		EHS_FB_OUT_I_API2(INX_ui_list_ARG_create_ht) = pWidget->xCurRect.nHeight;
	}

	EhsWidget_create(pWidget);

	EHS_FB_FINISH(INX_ui_list_ARG_create___);
}//ICB FUNCTION create MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION destroy MACRO START -- DO NOT ALTER
/**
 * Definition of ui_list_destroy.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ui_list_destroy)
{
	inx_ui_list_state_type* inx_ui_list_state = (inx_ui_list_state_type*)EHS_FB_RUN_CONTEXT;
	if(inx_ui_list_state && inx_ui_list_state->pUiWidgetClass != NULL){
		EhsWidget_destroy(inx_ui_list_state->pUiWidgetClass);
	}

	EHS_FB_FINISH(INX_ui_list_ARG_destroy___);
}//ICB FUNCTION destroy MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION show MACRO START -- DO NOT ALTER
/**
 * Definition of ui_list_show.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ui_list_show)
{
	inx_ui_list_state_type* inx_ui_list_state = (inx_ui_list_state_type*)EHS_FB_RUN_CONTEXT;

	if(inx_ui_list_state && inx_ui_list_state->pUiWidgetClass){
		EhsWidget_show(inx_ui_list_state->pUiWidgetClass);
		EHS_FB_FINISH(INX_ui_list_ARG_show___);
	}
}//ICB FUNCTION show MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION hide MACRO START -- DO NOT ALTER
/**
 * Definition of ui_list_hide.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ui_list_hide)
{
	inx_ui_list_state_type* inx_ui_list_state = (inx_ui_list_state_type*)EHS_FB_RUN_CONTEXT;
	if(inx_ui_list_state && inx_ui_list_state->pUiWidgetClass){
		EhsWidget_hide(inx_ui_list_state->pUiWidgetClass);
		EHS_FB_FINISH(INX_ui_list_ARG_hide___);
	}
}//ICB FUNCTION hide MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION update MACRO START -- DO NOT ALTER
/**
 * Definition of ui_list_update.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ui_list_update)
{
	inx_ui_list_state_type* inx_ui_list_state = (inx_ui_list_state_type*)EHS_FB_RUN_CONTEXT;

	EhsWidgetClass* pWidget = (inx_ui_list_state) ? inx_ui_list_state->pUiWidgetClass : NULL;

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

	if (EHS_FB_IN_CONNECTED_API2(INX_ui_list_ARG_update_off_x)){
		nXoffset = EHS_FB_IN_I_API2(INX_ui_list_ARG_update_off_x);
		bXConnected = bMoveRequired = EHS_TRUE;
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_ui_list_ARG_update_off_y)){
		nYoffset = EHS_FB_IN_I_API2(INX_ui_list_ARG_update_off_y);
		bYConnected = bMoveRequired = EHS_TRUE;
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_ui_list_ARG_update_off_wid)){
		nWOffset = EHS_FB_IN_I_API2(INX_ui_list_ARG_update_off_wid);
		bWConnected = bMoveRequired = EHS_TRUE;
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_ui_list_ARG_update_off_ht)){
		nHOffset = EHS_FB_IN_I_API2(INX_ui_list_ARG_update_off_ht);
		bHConnected = bMoveRequired = EHS_TRUE;
	}
	if (EHS_FB_IN_CONNECTED_API2(INX_ui_list_ARG_update_alpha)){
		nAlpha = EHS_FB_IN_I_API2(INX_ui_list_ARG_update_alpha);
		if (nAlpha < 0) nAlpha = 0;
        if (nAlpha > 255) nAlpha = 255;
        EhsWidget_fade(pWidget, (ehs_uint8)nAlpha);
        bAlphaConnected = EHS_TRUE;
	}

	if (bMoveRequired){
        EhsWidget_move(pWidget, nXoffset, nYoffset, nWOffset, nHOffset);
    }

	if (EHS_FB_OUT_CONNECTED_API2(INX_ui_list_ARG_update_abs_x)){
		EHS_FB_OUT_I_API2(INX_ui_list_ARG_update_abs_x) = pWidget->xCurRect.nLeft;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_ui_list_ARG_update_abs_y)){
		EHS_FB_OUT_I_API2(INX_ui_list_ARG_update_abs_y) = pWidget->xCurRect.nTop;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_ui_list_ARG_update_wid)){
		EHS_FB_OUT_I_API2(INX_ui_list_ARG_update_wid) = pWidget->xCurRect.nWidth;
	}
	if (EHS_FB_OUT_CONNECTED_API2(INX_ui_list_ARG_update_ht)){
		EHS_FB_OUT_I_API2(INX_ui_list_ARG_update_ht) = pWidget->xCurRect.nHeight;
	}

	if(bAlphaConnected || bMoveRequired){
		Ehs_widget_position_update(pWidget, bAlphaConnected, nAlpha, bXConnected, nXoffset, bYConnected, nYoffset, bWConnected, nWOffset, bHConnected, nHOffset);
		Ehs_widget_commit(pWidget);
	}

	EHS_FB_FINISH(INX_ui_list_ARG_update___);
	
}//ICB FUNCTION update MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION append MACRO START -- DO NOT ALTER
/**
 * Definition of ui_list_append.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ui_list_append)
{
	inx_ui_list_state_type* inx_ui_list_state = (inx_ui_list_state_type*)EHS_FB_RUN_CONTEXT;

	EhsWidgetClass* pWidget = (inx_ui_list_state) ? inx_ui_list_state->pUiWidgetClass : NULL;

	if(!pWidget){
		return;
	}

	ehs_bool bDataUpdated = EHS_FALSE;

	if (EHS_FB_IN_CONNECTED_API2(INX_ui_list_ARG_append_item)){
		/* this should be locked while copied */
		const ehs_char* item = EHS_FB_IN_S_API2(INX_ui_list_ARG_append_item);
		if(item && item[0] != '\0'){
			ehs_uint32 rgb = inx_ui_list_state->list.rgb;
			if (EHS_FB_IN_CONNECTED_API2(INX_ui_list_ARG_append_rgb)){
				rgb = EHS_FB_IN_I_API2(INX_ui_list_ARG_append_rgb);
			}
			EhsTPMutex_lock(EhsTPMutex_viewport);
			EhsStrcpy(inx_ui_list_state->list.item, item);
			inx_ui_list_state->list.rgb = rgb;
			inx_ui_list_state->list.state = EHS_UI_WIDGET_LIST_APPEND;
			EhsTPMutex_unlock(EhsTPMutex_viewport);

			bDataUpdated = EHS_TRUE;
		}
	}
	if(bDataUpdated == EHS_TRUE){
		EhsWidgetUI_update(pWidget);
		Ehs_widget_commit(pWidget);
		EHS_FB_FINISH(INX_ui_list_ARG_append_appended);
	}	
}//ICB FUNCTION append MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION remove MACRO START -- DO NOT ALTER
/**
 * Definition of ui_list_remove.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(ui_list_remove)
{
	inx_ui_list_state_type* inx_ui_list_state = (inx_ui_list_state_type*)EHS_FB_RUN_CONTEXT;

	EhsWidgetClass* pWidget = (inx_ui_list_state) ? inx_ui_list_state->pUiWidgetClass : NULL;

	if(!pWidget){
		return;
	}

	EhsTPMutex_lock(EhsTPMutex_viewport);
	inx_ui_list_state->list.state = EHS_UI_WIDGET_LIST_CLEAR;
	EhsTPMutex_unlock(EhsTPMutex_viewport);
	EhsWidgetUI_update(pWidget);
	Ehs_widget_commit(pWidget);
	// Your code here
	EHS_FB_FINISH(INX_ui_list_ARG_remove_cleared);
}//ICB FUNCTION remove MACRO END -- DO NOT ALTER THIS LINE
