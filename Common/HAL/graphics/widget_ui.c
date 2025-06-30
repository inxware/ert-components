/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/


/** @file widget_ui.c
 * Definitions for the ui widget handling functions
 *
 * @author: inx limited
 *
 */

//#define EHSL_MODULE_ID (EHSH_LOG_MODULE_GRAPHICS)

#include "widget.h"
#include "messages.h"
#include "hal_string.h"
#include "hal_process.h"
#include "hal_viewport.h"

#ifdef EHS_GUI_SUPPORT_MODE_B
#include "target_viewport_modeB.h"
#endif

//#define DEFAULT_INDENT 5
/**
 * Insert a widget itno the Z-ordered list of widgets. This is called
 * after a widget has been initialized, but before the index to the widget
 * table has been updated
 */
EHS_GLOBAL void EhsWidgetTable_updateZOrder(EhsWidgetTableClass* pWidgetTable, EhsWidgetClass* pWidget);

/**
 * Create the widget. This is a necessary step prior to showing the widget.
 * NOTE: This must only be called by EhsWidget_create via the widget's
 * pfCreate function.
 */
EHS_LOCAL ehs_bool EhsWidgetUi_create(struct EhsWidgetStruct* pWidget);

/**
 * Destroy the widget. Required after the widget has been finished with.
 * NOTE: This must only be called by EhsWidget_destroy via the widget's
 * pfDestroy function.
 */
EHS_LOCAL void EhsWidgetUi_destroy(struct EhsWidgetStruct* pWidget);

/**
 * Fade the textbox according to an opacity value
 *
 * @return true if the textbox needs to be redrawn (because it's appearance has changed)
 */
EHS_LOCAL ehs_bool EhsWidgetUi_fade(struct EhsWidgetStruct* pWidget, ehs_uint8 nOpacity);

/**
 * Cause the widget to be displayed (assuming it's already been created). If
 * it hasn't been created, there are no ill effects.
 *
 * NOTE: This function must only be called by EhsWidget_draw via the widget's
 * pfDraw function.
 * callers listed below.
 *
 * @param pWidget. Widget to display.
 * @param pViewport. Viewport to display the widget on.
 * @param pClipRect Specifies the bounds for drawing the widget
 */
EHS_LOCAL void EhsWidgetUi_draw(struct EhsWidgetStruct* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect);

/**
 * Initialise the widget with the general widget specific properties including
 * position, dimensions, z-ordering and colour.
 *
 * @param[in] id ui type ID
 * @param[in] properties any custom properties id associated with the widget
 * @param[in] curvature defines roundess of the widget
 * @param[in] parent_id id of the parent widget (@todo)
 * @param[in] xBounds Position and size of the overall widget
 * @param[in] nZ Z order for widget
 * @param[in] nIndentL indent for left hand side
 * @param[in] nIndentT indent for top side
 * @param[in] nIndentR indent for right hand side
 * @param[in] nIndentB indent for bottom
 * @param[in] nLineSep Separation between lines (distance from top of character
 * 		on one line to top of character on next line down)
 * @param[in] xFgColour Widget's foreground colour
 * @param[in] xBgColour Widget's background colour
 * @param[in] pFont Pointer to the font to use for this widget
 * @return pointer to initialised widget, or NULL
 */
EhsWidgetClass* EhsWidgetUI_init(ehs_uint16 id, ehs_uint16 properties, ehs_uint16 curvature, ehs_uint16 parent_id,
                                 const EhsGraphicsRectangleClass* xBounds, ehs_uint16 nZ,
                                 ehs_uint16 nIndentL, ehs_uint16 nIndentT, ehs_uint16 nIndentR, ehs_uint16 nIndentB, 
                                 ehs_uint16 nLineSep,EhsGraphicsColourClass xFgColour, EhsGraphicsColourClass xBgColour, 
                                 EhsGraphicsFontClass* pFont)
{
    EhsWidgetClass* pWidget;

    EhsTPMutex_lock(EhsTPMutex_viewport);
    pWidget = EhsWidgetTable_new(&EhsWidgetTable);

    if (pWidget)
    {
        EhsWidget_init(pWidget, xBounds, nZ, xFgColour.sComp.nAlpha);

        //pWidget->eWidgetKind = EHS_WIDGET_KIND_UI; // mode B (lvgl), widget kind is always ui. #def eWidgetKind in EhsWidgetClass for memory optimisation
        pWidget->nState = EHS_WIDGET_STATE_INIT;
        pWidget->bContentUpdated = EHS_FALSE;
        pWidget->bContentChanged = EHS_TRUE;
        pWidget->pfCreateFunc = EhsWidgetUi_create;
        pWidget->pfDestroyFunc = EhsWidgetUi_destroy;
        pWidget->pfFadeFunc = EhsWidgetUi_fade;
        pWidget->pfDrawFunc = EhsWidgetUi_draw;

        EHS_WIDGET_UI(pWidget).id = id;
        EHS_WIDGET_UI(pWidget).properties = properties;
        EHS_WIDGET_UI(pWidget).curvature = curvature;
        EHS_WIDGET_UI(pWidget).parent_id = parent_id;
        EHS_WIDGET_UI(pWidget).data = NULL;
        EHS_WIDGET_UI(pWidget).event_callback = NULL;
        EHS_WIDGET_UI(pWidget).xFgColour = xFgColour;
        EHS_WIDGET_UI(pWidget).nFgBaseAlpha = xFgColour.sComp.nAlpha;
        EHS_WIDGET_UI(pWidget).xBgColour = xBgColour;
        EHS_WIDGET_UI(pWidget).nBgBaseAlpha = xBgColour.sComp.nAlpha;
        //EHS_WIDGET_UI(pWidget).pFont = pFont; // not used at the moment, so disable to save memory
        EHS_WIDGET_UI(pWidget).nIndentBottom = nIndentB;
        EHS_WIDGET_UI(pWidget).nIndentLeft = nIndentL;
        EHS_WIDGET_UI(pWidget).nIndentRight = nIndentR;
        EHS_WIDGET_UI(pWidget).nIndentTop = nIndentT;
        EHS_WIDGET_UI(pWidget).nLineSep = nLineSep;
        EHS_WIDGET_UI(pWidget).pUiObject = NULL;
        EHS_WIDGET_UI(pWidget).nUiState = EHS_WIDGET_UI_STATE_EMPTY;
        EHS_WIDGET_UI(pWidget).nNoOfDecPlaces = 2;

        EhsWidgetTable_updateZOrder(&EhsWidgetTable, pWidget);
    }
    else
    { 
        EHSH_LOG_ERROR("Could not initialise UI widget");
    }
    EhsTPMutex_unlock(EhsTPMutex_viewport);
    return pWidget;
}
    

/**
 * Create the widget. This is a necessary step prior to showing the widget.
 * NOTE: This must only be called by EhsWidget_create. All widget creation should be
 * handled by EhsWidget_create.
 */
ehs_bool EhsWidgetUi_create(EhsWidgetClass* pWidget)
{
#ifdef EHS_GUI_SUPPORT_MODE_B
    EhsTargetWidgetUi_create(pWidget, &EhsTV);
#endif
    return EHS_TRUE;
}

/**
 * Destroy the widget. Required after the widget has been finished with.
 * NOTE: This must only be called by EhsWidget_destroy. All widget destruction should be
 * handled by EhsWidget_destroy.
 */
void EhsWidgetUi_destroy(EhsWidgetClass* pWidget)
{
#ifdef EHS_GUI_SUPPORT_MODE_B
    EhsTargetWidgetUi_destroy(pWidget);
#endif
    EHS_WIDGET_UI(pWidget).event_callback = NULL;
    EHS_WIDGET_UI(pWidget).data = NULL;
}


/**
 * Cause the widget to be displayed (assuming it's already been created). If
 * it hasn't been created, there are no ill effects.
 * This is called on each blit.
 *
 *
 * NOTE: This function must only be called by EhsWidget_draw.
 *
 * @param pWidget. Widget to display.
 * @param pViewport. Viewport to display the widget on.
 * @param pClipRect Specifies the bounds for drawing the widget
 */

void EhsWidgetUi_draw(struct EhsWidgetStruct* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect)
{
#ifdef EHS_GUI_SUPPORT_MODE_B
    EhsTargetWidgetUi_draw(pWidget);
#endif
}

/**
 * Set the data to be displayed onto a specific ui. This function can be called
 * directly by any functions
 *
 * @param pWidget UI to be updated
 */
void EhsWidgetUI_update(struct EhsWidgetStruct* pWidget)
{
#ifdef EHS_GUI_SUPPORT_MODE_B
    EhsTPMutex_lock(EhsTPMutex_viewport);
    pWidget->bContentUpdated = EHS_TRUE;
    EhsTPMutex_unlock(EhsTPMutex_viewport);
#endif
}

/**
 * @param pWidget UI to be checked
 * @return returns true if expected widget type
 */
ehs_bool EhsWidgetUI_is_string_type(struct EhsWidgetStruct* pWidget)
{
    ehs_uint16 id = EHS_WIDGET_UI(pWidget).id;
    return (id >= EHS_STRING_UI_WIDGET && id < EHS_STRING_UI_WIDGET_COUNT) ? EHS_TRUE : EHS_FALSE;
}

/**
 * @param pWidget UI to be checked
 * @return returns true if expected widget type
 */
ehs_bool EhsWidgetUI_is_bool_type(struct EhsWidgetStruct* pWidget)
{
    ehs_uint16 id = EHS_WIDGET_UI(pWidget).id;
    return (id >= EHS_BOOL_UI_WIDGET && id < EHS_BOOL_UI_WIDGET_COUNT) ? EHS_TRUE : EHS_FALSE;
}

/**
 * @param pWidget UI to be checked
 * @return returns true if expected widget type
 */
ehs_bool EhsWidgetUI_is_int_type(struct EhsWidgetStruct* pWidget)
{
    ehs_uint16 id = EHS_WIDGET_UI(pWidget).id;
    return (id >= EHS_INT_UI_WIDGET && id < EHS_INT_UI_WIDGET_COUNT) ? EHS_TRUE : EHS_FALSE;
}

/**
 * @param pWidget UI to be checked
 * @return returns true if expected widget type
 */
ehs_bool EhsWidgetUI_is_float_type(struct EhsWidgetStruct* pWidget)
{
    ehs_uint16 id = EHS_WIDGET_UI(pWidget).id;
    return (id >= EHS_FLOAT_UI_WIDGET && id < EHS_FLOAT_UI_WIDGET_COUNT) ? EHS_TRUE : EHS_FALSE;
}

/**
 * @param pWidget UI to be checked
 * @return returns true if expected widget type
 */
ehs_bool EhsWidgetUI_is_other_type(struct EhsWidgetStruct* pWidget)
{
    ehs_uint16 id = EHS_WIDGET_UI(pWidget).id;
    return (id >= EHS_OTHER_UI_WIDGET && id < EHS_OTHER_UI_WIDGET_COUNT) ? EHS_TRUE : EHS_FALSE;
}


/**
 * Fade the textbox according to an opacity value. Called from widget_fade, hence EhsTPMutex_viewport
 * is already locked.
 *
 * @return true if the textbox needs to be redrawn (because it's appearance has changed)
 */
ehs_bool EhsWidgetUi_fade(struct EhsWidgetStruct* pWidget, ehs_uint8 nOpacity)
{
    ehs_bool bChanged = EHS_FALSE;

    /* determine the new foreground and background opacity required by this widget */
    ehs_uint16 nCalcFgOpacity = (EHS_WIDGET_UI(pWidget).nFgBaseAlpha * nOpacity) / 255;
    ehs_uint16 nCalcBgOpacity = (EHS_WIDGET_UI(pWidget).nBgBaseAlpha * nOpacity) / 255;
    /* scale down to 8-bit */
    ehs_uint8 nFgOpacity = (ehs_uint8)nCalcFgOpacity;
    ehs_uint8 nBgOpacity = (ehs_uint8)nCalcBgOpacity;

    /* is it any different to the opacity we have already? */
    if ((nFgOpacity != EHS_WIDGET_UI(pWidget).xFgColour.sComp.nAlpha) ||
        (nBgOpacity != EHS_WIDGET_UI(pWidget).xBgColour.sComp.nAlpha))
    {
        EHS_WIDGET_UI(pWidget).xFgColour.sComp.nAlpha = nFgOpacity;
        EHS_WIDGET_UI(pWidget).xBgColour.sComp.nAlpha = nBgOpacity;
        bChanged = EHS_TRUE;
    }

    return bChanged;
}
