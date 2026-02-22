/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/


/** @file widget_patch.c
 * Definitions for the patch widget handling functions
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
EHS_LOCAL ehs_bool EhsWidgetPatch_create(struct EhsWidgetStruct* pWidget);

/**
 * Destroy the widget. Required after the widget has been finished with.
 * NOTE: This must only be called by EhsWidget_destroy via the widget's
 * pfDestroy function.
 */
EHS_LOCAL void EhsWidgetPatch_destroy(struct EhsWidgetStruct* pWidget);

/**
 * Fade the patch according to an opacity value
 *
 * @return true if the textbox needs to be redrawn (because it's appearance has changed)
 */
EHS_LOCAL ehs_bool EhsWidgetPatch_fade(struct EhsWidgetStruct* pWidget, ehs_uint8 nOpacity);

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
EHS_LOCAL void EhsWidgetPatch_draw(struct EhsWidgetStruct* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect);

/**
 * Initialise the widget with the general widget and textbox specific properties including
 * position, dimensions, z-ordering and colour.
 *
 * @param[in] pBounds Bounding rectangle for the textbox
 * @param[in] nZ Z order for widget
 * @param[in] xColour Widget's colour
 * @return pointer to initialised widget, or NULL
 */
EhsWidgetClass* EhsWidgetPatch_init(const EhsGraphicsRectangleClass* pBounds, ehs_uint16 nZ, EhsGraphicsColourClass xColour
    #ifdef EHS_STORE_WIDGET_NAMES
    ,ehs_char * szWidgetName
    #endif
)
{
    EhsWidgetClass* pWidget = EhsWidgetTable_new(&EhsWidgetTable);


    if (pWidget)
    {
        EhsWidget_init(pWidget,pBounds, nZ, xColour.sComp.nAlpha
            #ifdef EHS_STORE_WIDGET_NAMES
            ,szWidgetName
            #endif
            );
        /* DO the specific stuff now */
        pWidget->eWidgetClass = EHS_WIDGET_PURPOSE_PATCH;
        pWidget->eWidgetPurposeClass = EHS_WIDGET_PURPOSE_PATCH;
        pWidget->nState = EHS_WIDGET_STATE_INIT;
        pWidget->pfCreateFunc = EhsWidgetPatch_create;
        pWidget->pfDestroyFunc = EhsWidgetPatch_destroy;
        pWidget->pfFadeFunc = EhsWidgetPatch_fade;
        pWidget->pfDrawFunc = EhsWidgetPatch_draw;
        EHS_WIDGET_PATCH(pWidget).xColour = xColour;
        EHS_WIDGET_PATCH(pWidget).nBaseAlpha = xColour.sComp.nAlpha;
        EhsWidgetTable_updateZOrder(&EhsWidgetTable, pWidget);
    }
    return pWidget;
}

/**
 * Create the widget. This is a necessary step prior to showing the widget.
 * NOTE: This must only be called by EhsWidget_create. All widget creation should be
 * handled by EhsWidget_create.
 */
ehs_bool EhsWidgetPatch_create(EhsWidgetClass* pWidget)
{
    /* No specific widget creation activities are required */
    return EHS_TRUE;
}

/**
 * Destroy the widget. Required after the widget has been finished with.
 * NOTE: This must only be called by EhsWidget_destroy. All widget destruction should be
 * handled by EhsWidget_destroy.
 */
void EhsWidgetPatch_destroy(EhsWidgetClass* pWidget)
{
    /* no specific widget destruction activities are required */
}

/**
 * Cause the widget to be displayed (assuming it's already been created). If
 * it hasn't been created, there are no ill effects.
 *
 * NOTE: This function must only be called by EhsWidget_draw.
 *
 * @param pWidget. Widget to display.
 * @param pViewport. Viewport to display the widget on.
 * @param pClipRect Specifies the bounds for drawing the widget
 */
void EhsWidgetPatch_draw(struct EhsWidgetStruct* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect)
{
//	LOGI("EHS:Patch_draw-----------");
    EhsTV_fillRect(pViewport, &(pWidget->xCurRect), &(EHS_WIDGET_PATCH(pWidget).xColour));
}

/**
 * Fade the textbox according to an opacity value
 *
 * @return true if the textbox needs to be redrawn (because it's appearance has changed)
 */
ehs_bool EhsWidgetPatch_fade(struct EhsWidgetStruct* pWidget, ehs_uint8 nOpacity)
{
    ehs_bool bChanged = EHS_FALSE;
    /* determine the new foreground and background opacity required by this widget */
    ehs_uint16 nCalcOpacity = (EHS_WIDGET_PATCH(pWidget).nBaseAlpha * nOpacity) / 255;
    /* scale down to 8-bit */
    ehs_uint8 nOpacityByte = (ehs_uint8)nCalcOpacity;

    /* is it any different to the opacity we have already? */
    if (nOpacityByte != EHS_WIDGET_PATCH(pWidget).xColour.sComp.nAlpha)
    {
        EHS_WIDGET_PATCH(pWidget).xColour.sComp.nAlpha = nOpacityByte;
        bChanged = EHS_TRUE;
    }

    return bChanged;
}
