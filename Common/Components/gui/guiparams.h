/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/**
 * @file guiparams.h
 * This declares a type for dealing with GUI parameters and functions
 * to deal with it.
 *
 * @author: inx limited
 *
 */

#ifndef _EHS_GUIPARAMS_H
#define _EHS_GUIPARAMS_H

#include "ehs_fb_types.h"
#include "graphics.h"
#include "globals.h"

/**
 * Defines the type of widget that we're displaying
 */
typedef enum {EHS_WIDGET_CLASS_INVALID, EHS_WIDGET_CLASS_BITMAP, EHS_WIDGET_CLASS_TEXTBOX, EHS_WIDGET_CLASS_PATCH, EHS_WIDGET_CLASS_VIEWPORT, EHS_WIDGET_CLASS_VIDEO_PORT} EhsWidgetClassType;

/**
 * Hold the parameters for a GUI object. Derived from the GUI
 * parameters block. Readonly during the life of the application.
 */
typedef struct
{
    EhsGraphicsRectangleClass xRect; /**< Bounding box of widget */
    ehs_uint32 nZorder; /**< Z-order. Lower values go behind higher ones  */
    EhsWidgetClassType eClass; /**< Indicates what kind of widget are we dealing */
    ehs_sint16 nTextBoxType;
    union
    {
        struct
        {
            ehs_char szBitmapName[EHS_STRING_LENGTH_MAX]; /**< Filename holding the bitmap */ //TODO:STRINGLENGTH!
            ehs_uint8 nImageAlpha ; /** This is a value for the whole image's alpha value */
        } xBitmap;
        struct
        {
            EhsGraphicsColourClass xFgColour; /**< Colour of foreground */
            EhsGraphicsColourClass xBgColour; /**< Colour of background */
            ehs_uint16 nIndentL;	/**< Left indent */
            ehs_uint16 nIndentR;	/**< Right indent */
            ehs_uint16 nIndentT;	/**< Top indent */
            ehs_uint16 nIndentB;	/**< Bottom indent */
            ehs_uint16 nLineSep;	/**< Separation between lines */
            ehs_bool bRelativeCoords; /**< used to identify of coords. should be interpreted as percentage of screen */
            ehs_char szFontName[EHS_FILESTRING_SIZE];	/**< Font name (if any) */
            ehs_uint16 nType;  /**< Extended text box type */
            ehs_uint16 nProp;  /**< Extended text box properties */
            ehs_uint16 nCurve; /**< Extended text box curveture property, used for  */
            ehs_uint16 nParent;/**< Extended text box parent id */
        } xTextbox;
        EhsGraphicsColourClass xPatch; /**< Colour of patch */
    } uClass;

} EhsGuiParamsType;

/**
 * Convert parameters text into an instance of EhsGuiParamsType.
 *
 * @param szParamsText[in] "raw" parameter text
 * @param pParams[out] Pointer to EhsGuiParamsType
 */
EHS_GLOBAL void EhsParseGuiParameters(const char* szParamsText, EhsGuiParamsType* pParams);

#endif /* _EHS_GUIPARAMS_H */

