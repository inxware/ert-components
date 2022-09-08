/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file widget_image_gif.h
 * Declarations for the image file class specifically for handling the PNG file type.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_GRAPHICS_IMAGEPNG_H
#define EHS_GRAPHICS_IMAGEPNG_H
#include "hal_viewport.h"

/**
 * Obtain the png specific properties of EhsWidgetImage
 */
#define EHS_IMAGE_PNG(pImage) ((EHS_WIDGET_IMAGE(pImage)).specificImageSrcType.png)

/**
 * EHS representation of a png image
 */
typedef struct
{
    EhsTVSurfaceClass* pSurface; /* image data is stored as a surface */
} EhsWidgetImagePngSubclass;

/**
 * Load an image from a file
 *
 * @param pImage image to load png into
 * @param szFilename File to load image from
 * @return true if load was successful
 */
EHS_GLOBAL ehs_bool EhsWidgetImagePng_load(EhsWidgetClass* pImage, const ehs_char* szFilename);

/**
 * Display the gif onto the viewport (if it's ready to be shown)
 *
 * @param pWidget Widget to display
 * @param pViewport Viewport to display image in
 * @param pClipRect Specifies the bounds for drawing the widget
 */
EHS_GLOBAL void EhsWidgetImagePng_draw(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect);

/**
 * Destroy the image data for the current image file
 *
 * @param pWidget Widget to destroy
 */
EHS_GLOBAL void EhsWidgetImagePng_destroy(EhsWidgetClass* pWidget);


#endif /* EHS_GRAPHICS_IMAGEPNG_H */

