/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file widget_patch.h
 * Declarations for the patch widget functions. Intended to be included from widget.h. Don't include
 * this file directly.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_WIDGETPATCH_H
#define EHS_WIDGETPATCH_H

#include "ehs_types.h"
#include "globals.h"
#include "graphics.h"

/**
 * Obtain the textbox specific properties of EhsWidgetTextbox
 */
#define EHS_WIDGET_PATCH(pWidget) ((pWidget)->specificWidgetType.patch)

/**
 * Widget textbox subclass. This contains textbox-specific properties.
 * This is always accessed from a widget
 * class using the EHS_WIDGET_TEXTBOX(pWidget)
 * macro
 */
typedef struct
{
    EhsGraphicsColourClass xColour; /**< Patch colour */
    ehs_uint8 nBaseAlpha; /**< The starting alpha value for the patch - this can be modified by the fade function */
} EhsWidgetPatchSubclass;

/**
 * Initialise the widget with the general widget and textbox specific properties including
 * position, dimensions, z-ordering and colour.
 *
 * @param[in] pBounds Bounding rectangle for the textbox
 * @param[in] nZ Z order for widget
 * @param[in] xColour Widget's colour
 * @return pointer to initialised widget, or NULL
 */
EHS_GLOBAL EhsWidgetClass* EhsWidgetPatch_init(const EhsGraphicsRectangleClass* pBounds, ehs_uint16 nZ, EhsGraphicsColourClass xColour);

#endif /* EHS_WIDGETPATCH_H */

