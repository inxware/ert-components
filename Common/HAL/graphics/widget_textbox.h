/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file widget_textbox.h
 * Declarations for the textbox widget functions. Intended to be included from widget.h. Don't include
 * this file directly.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_WIDGETTEXTBOX_H
#define EHS_WIDGETTEXTBOX_H




#include "ehs_types.h"
#include "globals.h"
#include "font.h"
#include "html.h"


/**
 * Obtain the textbox specific properties of EhsWidgetTextbox
 */
#define EHS_WIDGET_TEXTBOX(pWidget) ((pWidget)->specificWidgetType.textbox)

/**
 * Widget textbox subclass. This contains textbox-specific properties.
 * This is always accessed from a widget
 * class using the EHS_WIDGET_TEXTBOX(pWidget)
 * macro
 */
typedef struct
{
    EhsGraphicsFontClass* pFont; /**< The font to display the string in */
    EhsGraphicsColourClass xFgColour; /**< Foreground colour */
    EhsGraphicsColourClass xBgColour; /**< Background colour */
    ehs_uint16 nIndentLeft, nIndentRight, nIndentTop, nIndentBottom;
    /**< Records the indents for the text box */
    ehs_uint16 nLineSep;		/**< Distance between the tops of two lines */
    ehs_uint8 nFgBaseAlpha; /**< The starting alpha value for the foreground colour - this can be modified by the fade function */
    ehs_uint8 nBgBaseAlpha; /**< The starting alpha value for the background colour - this can be modified by the fade function */
    EhsHGHtmlStringClass xText; /**< String to display */
    EhsTVSurfaceClass* pSurface; /* rendered text data is stored as a surface - we need to cache this */
} EhsWidgetTextboxSubclass;

/**
 * Initialise the widget with the general widget and textbox specific properties including
 * position, dimensions, z-ordering and colour.
 *
 * @param[in] xBounds Position and size of the overall textbox
 * @param[in] nZ Z order for widget
 * @param[in] nIndentL indent for left hand side
 * @param[in] nIndentT indent for top side
 * @param[in] nIndentR indent for right hand side
 * @param[in] nIndentB indent for bottom
 * @param[in] nLineSep Separation between lines (distance from top of character
 * 		on one line to top of character on next line down)
 * @param[in] xFgColour Widget's foreground colour
 * @param[in] xBgColour Widget's background colour
 * @param[in] pFont Pointer to the font to use for this textbox
 * @return pointer to initialised widget, or NULL
 */
EHS_GLOBAL EhsWidgetClass* EhsWidgetTextbox_init(const EhsGraphicsRectangleClass* xBounds, ehs_uint16 nZ,ehs_uint16 nIndentL, ehs_uint16 nIndentT, ehs_uint16 nIndentR, ehs_uint16 nIndentB, ehs_uint16 nLineSep,EhsGraphicsColourClass xFgColour, EhsGraphicsColourClass xBgColour, EhsGraphicsFontClass* pFont);

/**
 * Set the text to be displayed onto a specific textbox. This function can be called
 * directly by any functions
 *
 * @param pWidget Textbox to display text
 * @param szText String to be displayed
 */
EHS_GLOBAL void EhsWidgetTextbox_write(struct EhsWidgetStruct* pWidget, const ehs_char* szText);

#endif /* EHS_WIDGETTEXTBOX_H */

