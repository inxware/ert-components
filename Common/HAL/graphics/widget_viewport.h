/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file widget_viewport.h
 * Declarations for the viewport widget functions. Intended to be included from widget.h. Don't include
 * this file directly.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_WIDGETVIEWPORT_H
#define EHS_WIDGETVIEWPORT_H

#include "globals.h"
#include "graphics.h"



/**
 * Widget textbox subclass. This contains textbox-specific properties.
 * This is always accessed from a widget
 * class using the EHS_WIDGET_TEXTBOX(pWidget)
 * macro
 */

/**
 * Initialise the widget with the general widget and textbox specific properties including
 * position, dimensions, z-ordering and colour.
 *
 * @param[in] pBounds Bounding rectangle for the textbox
 * @param[in] nZ Z order for widget
 * @param[in] xColour Widget's colour
 * @return pointer to initialised widget, or NULL
 */
EHS_GLOBAL EhsWidgetClass* EhsWidgetViewport_init(const EhsGraphicsRectangleClass* pBounds, ehs_uint16 nZ, EhsGraphicsColourClass xColour);
EHS_GLOBAL ehs_bool EhsWidgetViewport_fade(struct EhsWidgetStruct* pWidget, ehs_uint8 nOpacity, ehs_uint8 nRed, ehs_uint8 nGreen, ehs_uint8 nBlue);
//EHS_GLOBAL void EhsWidgetViewport_destroy(EhsWidgetClass* pWidget);
//EHS_GLOBAL void EhsWidgetViewport_draw(struct EhsWidgetStruct* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect);
//EHS_GLOBAL ehs_bool EhsWidgetViewport_create(EhsWidgetClass* pWidget);
EHS_GLOBAL ehs_bool EhsWidgetViewport_update(EhsWidgetClass* pWidget, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nDeltaWid, EhsDataflowIntType nDeltaHt);

EHS_GLOBAL ehs_bool EhsWidgetViewport_setwindow(EhsWidgetClass* pWidget, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nWid, EhsDataflowIntType nHt);

EHS_GLOBAL ehs_bool EhsWidgetViewport_hide(struct EhsWidgetStruct* pWidget);

EHS_GLOBAL ehs_bool EhsWidgetViewport_show(struct EhsWidgetStruct* pWidget);

EHS_GLOBAL ehs_bool EhsWidgetsetToScreenSize(ehs_sint32 *nScreenWidth, ehs_sint32 *nScreenHeight);

EHS_GLOBAL ehs_bool EhsWidgetViewport_cleanup(struct EhsWidgetStruct* pWidget);


/**
 * Structure for primary viewport information
 *
 * @todo - this should be part of a larger struct in graphics.h which defines a handle to many surfaces which in turn have handles to many viewports
 * at the moment we are assuming that there is only one viewport and it is the primary surface
 */
typedef struct
{
    ehs_bool bDefaultsChanged;
    ehs_uint16 nX;
    ehs_uint16 nY;
    ehs_uint16 nWidth;
    ehs_uint16 nHeight;
    EhsGraphicsColourClass nColour;
    ehs_bool bHasFrame;
    ehs_uint16 nZOrder;
} EhsViewportInformation;

/**
 *
 */
void EhsPrimaryViewportInfo_new();



#endif /* EHS_WIDGETPATCH_H */

