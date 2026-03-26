/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
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

#include "globals.h"
#include "ehs_fb_types.h"
#include "graphics.h"
#include "widget.h" // Needed for purpose enum


/**
 * Hold the parameters for a GUI object. Derived from the GUI
 * parameters block. Readonly during the life of the application.
 */
typedef struct
{
    EhsWidgetClassType eClass; /**< Indicates what base type of widget are we dealing with (data, button, image, viewport etc.)*/
    EhsWidgetPurposeClassType ePurposeClass; /* Which purpose  class (e.g. what function block type including data format ) */
    EhsGraphicsRectangleClass xRect; /**< Bounding box of widget */
    ehs_uint32 nZorder; /**< Z-order. Lower values go behind higher ones  */
#if defined(EHS_STORE_WIDGET_NAMES)
    ehs_char widgetName[EHS_WIDGET_NAME_LENGTH_MAX];
#endif
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
            ehs_uint16 nExtType;  /**< Extended text box type */
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

/**
 * Parse GUI parameters for a textbox widget with a known data type.
 *
 * Identical to EhsParseGuiParameters() but skips the internal type-detection
 * step. The caller supplies the purpose class directly (e.g.
 * EHS_WIDGET_PURPOSE_INT for a gui_text_int2 block), so geometry and colour
 * data are always parsed regardless of what the GUI parameter type token says.
 *
 * @param szParamsText[in]   "raw" parameter text (same as EhsParseGuiParameters)
 * @param pParams[out]       Pointer to EhsGuiParamsType
 * @param purposeClass[in]   The known data type (must not be EHS_WIDGET_PURPOSE_INVALID)
 * @return EHS_TRUE if geometry was parsed successfully
 */
EHS_GLOBAL ehs_bool EhsParseGuiParametersTextbox(const char* szParamsText, EhsGuiParamsType* pParams,
                                                  EhsWidgetPurposeClassType purposeClass);

#endif /* _EHS_GUIPARAMS_H */

