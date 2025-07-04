/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file widget_image_jpg.h
 * Declarations for the image file class specifically for handling the JPG file type.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_GRAPHICS_IMAGEJPG_H
#define EHS_GRAPHICS_IMAGEJPG_H
#include "hal_viewport.h"

/**
 * Obtain the jpg specific properties of EhsWidgetImage
 */
#define EHS_IMAGE_JPG(pImage) ((EHS_WIDGET_IMAGE(pImage)).specificImageSrcType.jpg)

/**
 * EHS representation of a jpg image
 */
typedef struct
{
    EhsTVSurfaceClass* pSurface; /* image data is stored as a surface */
} EhsWidgetImageJpgSubclass;

/**
 * Load an image from a file
 *
 * @param pImage image to load jpg into
 * @param szFilename File to load image from
 * @return true if load was successful
 */
EHS_GLOBAL ehs_bool EhsWidgetImageJpg_load(EhsWidgetClass* pImage, const ehs_char* szFilename);

/**
 * Display the jpg onto the viewport (if it's ready to be shown)
 *
 * @param pWidget Widget to display
 * @param pViewport Viewport to display image in
 * @param pClipRect Specifies the bounds for drawing the widget
 */
EHS_GLOBAL void EhsWidgetImageJpg_draw(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect);

/**
 * Destroy the image data for the current image file
 *
 * @param pWidget Widget to destroy
 */
EHS_GLOBAL void EhsWidgetImageJpg_destroy(EhsWidgetClass* pWidget);

#endif /* EHS_GRAPHICS_IMAGEJPG_H */

