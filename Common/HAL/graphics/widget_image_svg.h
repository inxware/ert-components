/*
 * @file widget_image_gif.h
 * Declarations for the image file class specifically for handling the PNG file type.
 *
 * @author: inx limited
 * @version: $Revision: 1248 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
 *
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_GRAPHICS_IMAGESVG_H
#define EHS_GRAPHICS_IMAGESVG_H
#include "hal_viewport.h"

/**
 * Obtain the svg specific properties of EhsWidgetImage
 */
#define EHS_IMAGE_SVG(pImage) ((EHS_WIDGET_IMAGE(pImage)).specificImageSrcType.svg)

/**
 * EHS representation of a svg image
 */
typedef struct {
	EhsTVSurfaceClass* pSurface; /* image data is stored as a surface */
} EhsWidgetImageSvgSubclass;

/**
 * Load an image from a file
 *
 * @param pImage image to load svg into
 * @param szFilename File to load image from
 * @return true if load was successful
 */
EHS_GLOBAL ehs_bool EhsWidgetImageSvg_load(EhsWidgetClass* pImage, const ehs_char* szFilename);

/**
 * Display the svg onto the viewport (if it's ready to be shown)
 *
 * @param pWidget Widget to display
 * @param pViewport Viewport to display image in
 * @param pClipRect Specifies the bounds for drawing the widget
 */
EHS_GLOBAL void EhsWidgetImageSvg_draw(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect);

/**
 * Destroy the image data for the current image file
 *
 * @param pWidget Widget to destroy
 */
EHS_GLOBAL void EhsWidgetImageSvg_destroy(EhsWidgetClass* pWidget);


#endif /* EHS_GRAPHICS_IMAGESVG_H */
