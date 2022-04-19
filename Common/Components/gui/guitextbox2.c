/**
 * @file guitextbox.c
 *
 * generic processing for guitextbox (text) functions.
 * target specific handling is defined elsewhere
 *
 * @todo The handling of completions of GUI operations needs to be
 * handled better in this function. Currently it is assumed that returning
 * from a GUI operation is sufficient to complete the operation. A better approach
 * would be for EHS_FB_RUN_CONTEXT and the completion port to be passed to the GUI
 * operation, and for this to be handled upon actual completion of the GUI operation.
 * This does mean that the EhsFunctionInstanceData_triggerEvent operation needs to be
 * threadsafe, as it might be called by an OS operation.
 *
 * @author: inx limited
 * @version: $Revision: 3974 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
 *
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

/**
 * @page FunctionReference Lucid Function Block Reference Guide
 * @section guitextbox
 * @anchor guitextbox
 * @subsection appliesTo Applies To:
 * - GUI &gt; text &gt; bool
 * - GUI &gt; text &gt; int
 * - GUI &gt; text &gt; real
 * - GUI &gt; text &gt; string
 * @subsection properties Properties:
 * <b>tag.</b> This is the name of the image. Lucid GUI Builder uses this name to refer to the image
 * when it is building the display including this image.
 *
 * <b>Widget-group</b>. This refers to the type of screen layout that will contain this image.
 *
 * @subsection description Description:
 * The <em>create</em>/<em>destroy</em> events are used to allow the text boxes (and other visual widgets)
 * that appear in the same widget group to be "turned on" (or off) all at the same time. At this time
 * it is necessary to manually connect together the create and destroy events for all widgets in the same widget group.
 *
 * The <em>show</em> and <em>hide</em> events are intended to allow individual images to be displayed or
 * removed from the display.
 *
 * The <em>update</em> allows the textbox to be modified via the <em>x</em>, <em>y</em>, <em>wid</em>,
 * <em>ht</em> and <em>alpha</em> inputs.
 * The x and y inputs provide an offset to the initial screen position (which is defined in Lucid GUI Builder).
 * The width and height inputs provide an offset to the initial widget height and width.
 * The alpha input changes the opacity of the image. It ranges between 0 (totally transparent) and 255 (maximum
 * opacity). If Lucid GUI Builder defines a text box as partially transparent, the alpha value of the text box
 * will not increase its opacity beyond that point.
 * The x and y inputs provide an offset to the initial screen position (which is defined in Lucid GUI Builder).
 *
 * The <em>x</em>, <em>y</em>, <em>wid</em> and <em>ht</em> outputs return the absolute position and size of the
 * patch. These are updated by the create event and by the update event.
 *
 */

#include "target.h"
//#ifdef EHS_GUI_SUPPORT

#include "guitextbox2.h"
#include "guiparams.h"
#include "globals.h"
#include "widget.h"
#include "font.h"
/** @todo check which includes are required here */
#include <string.h>
#include <ctype.h>
#include "setCompletes.h"
#include "app_data.h"
#include "hal-api.h"

EHS_FB_FUNCTIONS_START(gui_text_string2)
EHS_FB_FUNCTION_ENTRY("create", gui_text2_create)
EHS_FB_FUNCTION_ENTRY("destroy", gui_text2_destroy)
EHS_FB_FUNCTION_ENTRY("show", gui_text2_show)
EHS_FB_FUNCTION_ENTRY("hide", gui_text2_hide)
EHS_FB_FUNCTION_ENTRY("update", gui_text_string2_update)
EHS_FB_FUNCTIONS_END

#ifdef EHS_TARGET_FP_SUPPORT
EHS_FB_FUNCTIONS_START(gui_text_float2)
EHS_FB_FUNCTION_ENTRY("create", gui_text2_create)
EHS_FB_FUNCTION_ENTRY("destroy", gui_text2_destroy)
EHS_FB_FUNCTION_ENTRY("show", gui_text2_show)
EHS_FB_FUNCTION_ENTRY("hide", gui_text2_hide)
EHS_FB_FUNCTION_ENTRY("update", gui_text_float2_update)
EHS_FB_FUNCTIONS_END
#endif

EHS_FB_FUNCTIONS_START(gui_text_int2)
EHS_FB_FUNCTION_ENTRY("create", gui_text2_create)
EHS_FB_FUNCTION_ENTRY("destroy", gui_text2_destroy)
EHS_FB_FUNCTION_ENTRY("show", gui_text2_show)
EHS_FB_FUNCTION_ENTRY("hide", gui_text2_hide)
EHS_FB_FUNCTION_ENTRY("update", gui_text_int2_update)
EHS_FB_FUNCTIONS_END

EHS_FB_FUNCTIONS_START(gui_text_bool2)
EHS_FB_FUNCTION_ENTRY("create", gui_text2_create)
EHS_FB_FUNCTION_ENTRY("destroy", gui_text2_destroy)
EHS_FB_FUNCTION_ENTRY("show", gui_text2_show)
EHS_FB_FUNCTION_ENTRY("hide", gui_text2_hide)
EHS_FB_FUNCTION_ENTRY("update", gui_text_bool2_update)
EHS_FB_FUNCTIONS_END

EHS_LOCAL void Ehs_gui_text2_update(EhsWidgetClass* pWidget, const ehs_char* szString, ehs_bool bAlphaConnected, EhsDataflowIntType nAlpha,
		ehs_bool bXConnected, EhsDataflowIntType nXoffset,
		ehs_bool bYConnected, EhsDataflowIntType nYoffset,
		ehs_bool bWConnected, EhsDataflowIntType nWoffset,
		ehs_bool bHConnected, EhsDataflowIntType nHoffset);

#define EHS_FB_GUI_TEXT_IN_X 0		/**< Function block input for X offset */
#define EHS_FB_GUI_TEXT_IN_Y 1		/**< Function block input for Y offset */
#define EHS_FB_GUI_TEXT_IN_WID 2	/**< Function block input for width offset */
#define EHS_FB_GUI_TEXT_IN_HT 3		/**< Function block input for height offset */
#define EHS_FB_GUI_TEXT_IN_ALPHA 4	/**< Function block input for alpha shading */
#define EHS_FB_GUI_TEXT_IN_VALUE 5  /**< Function block input for value to display */
#define EHS_FB_GUI_TEXT_OUT_X 0		/**< Function block output for X offset */
#define EHS_FB_GUI_TEXT_OUT_Y 1		/**< Function block output for Y offset */
#define EHS_FB_GUI_TEXT_OUT_WID 2	/**< Function block output for width offset */
#define EHS_FB_GUI_TEXT_OUT_HT 3		/**< Function block output for height offset */

/**
 * Define the identify function
 */
EHS_FB_IDENTIFY_FUNCTION(gui_text2)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(EhsWidgetClass**);
}

/**
 * Load the GUI parameters into the params structure
 */
EHS_FB_INIT_FUNCTION(gui_text2)
{
	EhsGuiParamsType xParams;
	EhsGraphicsFontClass* pFont;
	ehs_bool bRet = EHS_FALSE; /* assume initialisation fails */
	char guiParams[MAX_PARAM_STR_LEN];
	const char* pParams;
	ehs_uint8 nByte;

	EHS_TRACE_FUNCTION(EHS_FB_INIT_NAME(GUI_ImageFile));
	pParams = ReadParmFile(&EHS_FB_INIT_PARAMETERS[4], guiParams);
	EhsParseGuiParameters(guiParams,&xParams);

	if (xParams.eClass == EHS_WIDGET_CLASS_TEXTBOX)
	{
		pFont = EhsGraphicsFont_load(xParams.uClass.xTextbox.szFontName);
		*(EhsWidgetClass**)EHS_FB_INIT_CONTEXT =
			EhsWidgetTextbox_init(&(xParams.xRect),xParams.nZorder,
					xParams.uClass.xTextbox.nIndentL,
					xParams.uClass.xTextbox.nIndentT,
					xParams.uClass.xTextbox.nIndentR,
					xParams.uClass.xTextbox.nIndentB,
					xParams.uClass.xTextbox.nLineSep,
					xParams.uClass.xTextbox.xFgColour,
					xParams.uClass.xTextbox.xBgColour,
					pFont);

		if ((*(EhsWidgetClass**)EHS_FB_INIT_CONTEXT == NULL) ||
			((*(EhsWidgetClass**)(EHS_FB_INIT_CONTEXT))->nState == EHS_WIDGET_STATE_EMPTY)) {
			bRet = EHS_FALSE;
		} else {

			/* parse parameters */
			pParams = EhsGetUint8FromString(&nByte, pParams);
			(*(EhsWidgetClass**)EHS_FB_RUN_CONTEXT)->bMaintainAspectRatio = (ehs_bool)nByte;
			pParams = EhsGetUint8FromString(&nByte, pParams);
			(*(EhsWidgetClass**)EHS_FB_RUN_CONTEXT)->bCaptureClicksIgnoringZOrder = (ehs_bool)nByte;

			bRet = EHS_TRUE;
		}
		(*(EhsWidgetClass**)EHS_FB_RUN_CONTEXT)->bContentChanged = EHS_TRUE; /* This should be done in the common code */
			}
	else {
		EHSH_LOG_WARNING("xParams.eClass != EHS_WIDGET_CLASS_TEXTBOX");
	}
	return bRet;
}

/**
 * Create the GUI image. Cause a handle to be created in the GUI system, but don't display
 * the item.
 */
EHS_FB_RUN_FUNCTION(gui_text2_create)
{
	EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
	EhsWidget_create(pWidget);
	/*Set pointer in widget structure to point at instance data. Used for mouse click.*/
	pWidget->pFIData = EHS_FB_RUN_CONTEXT_REF;

	/* set the output values for this widget */
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_X) 	= pWidget->xCurRect.nLeft;
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_Y) 	= pWidget->xCurRect.nTop;
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_WID)	= pWidget->xCurRect.nWidth;
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_HT) 	= pWidget->xCurRect.nHeight;

	/*Set number of mouseClick port*/
	pWidget->mouseClickPortNumber = 2;
	pWidget->mouseDownPortNumber = 3;
	pWidget->bContentChanged = EHS_TRUE; /* This should be done in the common code */

	EHS_FB_FINISH(1);
	return;
}

/**
 * Destroy the textbox - remove the text from the screen
 * if it's visible.
 */
EHS_FB_RUN_FUNCTION(gui_text2_destroy)
{
	EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
	EhsWidget_destroy(pWidget);
	EHS_FB_FINISH(1);
	return;
}

/**
 * Make the GUI text visible
 */
EHS_FB_RUN_FUNCTION(gui_text2_show)
{
	EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
	EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(gui_text_show));
	EhsWidget_show(pWidget);
	EHS_FB_FINISH(1);
}






/**
 * Remove the GUI text from the screen.
 */
EHS_FB_RUN_FUNCTION(gui_text2_hide)
{
	EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
	EHS_TRACE_FUNCTION(EHS_FB_RUN_NAME(gui_text_hide));
	EhsWidget_hide(pWidget);
	EHS_FB_FINISH(1);
}

/**
 * General purpose update function, contains the common code for all of the text update functions.
 *
 * @param[in] pWidget Widget to update
 * @param[in] szString text to be displayed
 * @param[in] bAlphaConnected true if the alpha value should be used
 * @param[in] nAlpha current alpha level to use for image
 * @param[in] bXConnected true if the x coordinate should be used
 * @param[in] nXoffset current x coordinate to use (value is offset to default image position)
 * @param[in] bYConnected true if the y coordinate should be used
 * @param[in] nYoffset current y coordinate to use (value is offset to default image position)
 * @param[in] bWConnected true if the width value should be used
 * @param[in] nWoffset current width offset to use (value is offset to default image position)
 * @param[in] bHConnected true if the height offset should be used
 * @param[in] nHoffset current height offset to use (value is offset to default image position)
 *
 */
void Ehs_gui_text2_update(EhsWidgetClass* pWidget, const ehs_char* szString, ehs_bool bAlphaConnected, EhsDataflowIntType nAlpha,
		ehs_bool bXConnected, EhsDataflowIntType nXoffset,
		ehs_bool bYConnected, EhsDataflowIntType nYoffset,
		ehs_bool bWConnected, EhsDataflowIntType nWoffset,
		ehs_bool bHConnected, EhsDataflowIntType nHoffset)
{

	Ehs_widget_position_update(pWidget, bAlphaConnected, nAlpha, bXConnected, nXoffset, bYConnected, nYoffset, bWConnected, nWoffset, bHConnected, nHoffset);

	EhsWidgetTextbox_write(pWidget,szString);
}

/**
 * Update the textbox string using text input, alpha level, x-offset and y-offset (where these are connected)
 */
EHS_FB_RUN_FUNCTION(gui_text_string2_update)
{
	EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
	char *str = EHS_FB_IN_S(EHS_FB_GUI_TEXT_IN_VALUE);

	Ehs_gui_text2_update(pWidget,str,
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_ALPHA),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_ALPHA),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_X),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_X),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_Y),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_Y),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_WID),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_WID),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_HT),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_HT));

	/* set the output values for this widget */
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_X) 	= pWidget->xCurRect.nLeft;
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_Y) 	= pWidget->xCurRect.nTop;
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_WID)	= pWidget->xCurRect.nWidth;
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_HT) 	= pWidget->xCurRect.nHeight;

	EHS_FB_FINISH(1);
}

//#ifdef EHS_TARGET_FP_SUPPORT
/**
 * Update the textbox float using float input, alpha level, x-offset and y-offset (where these are connected)
 */
EHS_FB_RUN_FUNCTION(gui_text_float2_update)
{
	EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
	char str[EHS_STRING_LENGTH_MAX];
	EhsSprintf(str,"%f",EHS_FB_IN_F(EHS_FB_GUI_TEXT_IN_VALUE));


	Ehs_gui_text2_update(pWidget,str,
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_ALPHA),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_ALPHA),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_X),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_X),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_Y),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_Y),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_WID),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_WID),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_HT),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_HT));

	/* set the output values for this widget */
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_X) 	= pWidget->xCurRect.nLeft;
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_Y) 	= pWidget->xCurRect.nTop;
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_WID)	= pWidget->xCurRect.nWidth;
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_HT) 	= pWidget->xCurRect.nHeight;

	EHS_FB_FINISH(1);
}
//#endif

/**
 * Update the textbox Int using int input, alpha level, x-offset and y-offset (where these are connected)
 */
EHS_FB_RUN_FUNCTION(gui_text_int2_update)
{
	EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
	char str[EHS_STRING_LENGTH_MAX];
	EhsSprintf(str,"%d",EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_VALUE));

	Ehs_gui_text2_update(pWidget,str,
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_ALPHA),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_ALPHA),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_X),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_X),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_Y),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_Y),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_WID),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_WID),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_HT),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_HT));

	/* set the output values for this widget */
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_X) 	= pWidget->xCurRect.nLeft;
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_Y) 	= pWidget->xCurRect.nTop;
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_WID)	= pWidget->xCurRect.nWidth;
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_HT) 	= pWidget->xCurRect.nHeight;

	EHS_FB_FINISH(1);
}
/**
 * Update the textbox bool using bool input, alpha level, x-offset and y-offset (where these are connected)
 */
EHS_FB_RUN_FUNCTION(gui_text_bool2_update)
{
	EhsWidgetClass *pWidget = *(EhsWidgetClass**)EHS_FB_RUN_CONTEXT;
	char str[EHS_STRING_LENGTH_MAX];

	str[0] = (EHS_FB_IN_B(EHS_FB_GUI_TEXT_IN_VALUE))?'T':'F';
	str[1] = '\0';

	Ehs_gui_text2_update(pWidget,str,
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_ALPHA),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_ALPHA),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_X),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_X),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_Y),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_Y),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_WID),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_WID),
		EHS_FB_IN_CONNECTED(EHS_FB_GUI_TEXT_IN_HT),EHS_FB_IN_I(EHS_FB_GUI_TEXT_IN_HT));

	/* set the output values for this widget */
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_X) 	= pWidget->xCurRect.nLeft;
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_Y) 	= pWidget->xCurRect.nTop;
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_WID)	= pWidget->xCurRect.nWidth;
	EHS_FB_OUT_I(EHS_FB_GUI_TEXT_OUT_HT) 	= pWidget->xCurRect.nHeight;

	EHS_FB_FINISH(1);
}

//#endif /* EHS_GUI_SUPPORT */
