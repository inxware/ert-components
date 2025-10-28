/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/


/** @file widget_image_jpg.c
 * Declarations for the image file class specifically for handling the JPG file type.
 *
 * @author: inx limited
 *
 */

//#define EHSL_MODULE_ID (EHSH_LOG_MODULE_GRAPHICS)
#include <setjmp.h>

#include "globals.h"
#include "widget.h"

#include "graphics.h"
#include "messages.h"
#include "hal-api.h"

/**
 * Jump buffer to support error handling of EHS
 */
jmp_buf EhsWidgetImageJpg_err;

#include "jpeg/jpeglib.h"

/**
 * Load an JPG from a file into the EhsWidgetImagePngSubclass structure
 *
 * @param pImage image to load png into
 * @param szFilename File to load image from
 * @return true if load was successful
 */
EHS_GLOBAL ehs_bool EhsWidgetImageJpg_load(EhsWidgetClass* pImage, const ehs_char* szFilename)
{

    //@todo refactor jpg widget images to use hal functions - follow pattern of widget_image_png.c

    ehs_FILE *pFile;
    ehs_bool bReturn = EHS_TRUE;
    EhsGraphicsColourClass* pPixels;
    ehs_uint8* pLine;
    int row_stride;
    int pixWidth;
    int pos = 0;
    ehs_bool bLoadImageFromAppDir;
    EhsGfxFileOrientation orientation;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    bLoadImageFromAppDir = EhsWidgetImage_loadFileFromAppDir(pImage);
    if (doCheckFileHeader(szFilename, bLoadImageFromAppDir,EHS_GFXFILETYPE_JPG,&orientation)  == EHS_TRUE)
    {
        if (orientation != EHS_GFXFILEORIENTATION_UNKNOWN)  /* see https://github.com/cloudflare/jpegtran/blob/master/transupp.c for implementing rotations on decompress */
        {
            EHSH_LOG_ERROR("Orientation in jpeg is ignored");
        }
        if (setjmp(EhsWidgetImageJpg_err))
        {
            EHSH_LOG_ERROR(EHS_MSG_ERROR_WIDGET_LOADFAILED(szFilename));
            bReturn = EHS_FALSE;
        }
        else
        {
            /* 	Allocate and initialize a JPEG decompression object */
            cinfo.err = jpeg_std_error(&jerr);
            jpeg_create_decompress(&cinfo);

            // try to open file
            if (bLoadImageFromAppDir)
            {
                pFile = Ehs_AppFopen(szFilename, "rb");
            }
            else
            {
                pFile = Ehs_UserFopen(szFilename, "rb");
            }

            if (!pFile)
            {
                EHSH_LOG_ERROR(EHS_MSG_ERROR_WIDGET_FILEOPEN(szFilename));
                bReturn = EHS_FALSE;
            }
            else
            {
                jpeg_stdio_src(&cinfo, pFile);

                /* Call jpeg_read_header() to obtain image info */
                jpeg_read_header(&cinfo, TRUE);

                /* Set parameters for decompression */
                cinfo.out_color_space = JCS_RGB; /* use RGB colour space for output */
                cinfo.out_color_components = 4; /* ARGB */
                cinfo.output_components = 4;
                row_stride = cinfo.output_width * cinfo.output_components;

                /* jpeg_start_decompress(...); */
                jpeg_start_decompress(&cinfo);

                EHS_IMAGE_JPG(pImage).pSurface = EhsTVSurface_create(&EhsTV,
                                                 cinfo.output_width,cinfo.output_height,EHS_GRAPHICS_COLOUR_ARGB8888, NULL, 0);//, EHS_TRUE);
                pPixels = EhsTVSurface_pixels(EHS_IMAGE_JPG(pImage).pSurface);

                // resize widget to proportions of image
                EhsGraphicsRectangleClass xSrcRect;
                xSrcRect.nLeft = 0;
                xSrcRect.nTop = 0;
                xSrcRect.nWidth = EhsTVSurface_width(EHS_IMAGE_JPG(pImage).pSurface);
                xSrcRect.nHeight = EhsTVSurface_height(EHS_IMAGE_JPG(pImage).pSurface);

                pLine = (ehs_uint8*)malloc(cinfo.output_width*4); /* @todo really, I only expect 3 channels per cell */
                /* while (scan lines remain to be read)
                jpeg_read_scanlines(...); */
                while (cinfo.output_scanline < cinfo.output_height)
                {
                    /* jpeg_read_scanlines expects an array of pointers to scanlines.
                     * Here the array is only one element long, but you could ask for
                     * more than one scanline at a time if that's more convenient.
                     */
                    (void) jpeg_read_scanlines(&cinfo, &pLine, 1);
                    /* jpeg_read_scanlines only gets RGB components with no gap for alpha */
                    for (pixWidth = 0; pixWidth < cinfo.output_width; pixWidth++)
                    {
                        pPixels[pos + pixWidth].sComp.nBlue = pLine[pixWidth*3+2];
                        pPixels[pos + pixWidth].sComp.nGreen = pLine[pixWidth*3+1];
                        pPixels[pos + pixWidth].sComp.nRed = pLine[pixWidth*3+0];
                        pPixels[pos + pixWidth].sComp.nAlpha = 255;
                    }
                    /*At end of row pos becomes equal to pos+pixWidth*/
                    pos = pos + pixWidth;
                }

                free(pLine);

                /* Release the JPEG decompression object */
                jpeg_finish_decompress(&cinfo);
            }
        }
    }
    else bReturn = EHS_FALSE;

    if (bReturn == EHS_TRUE)
    {
        pImage->MediaRect.nWidth =  EhsTVSurface_width(pImage->specificWidgetType.image.specificImageSrcType.jpg.pSurface);
        pImage->MediaRect.nHeight =  EhsTVSurface_height(pImage->specificWidgetType.image.specificImageSrcType.jpg.pSurface);
    }
    return (bReturn);
}

/**
 * Display the jpg onto the viewport.
 * Note this is a generic 2-D bgra8888 blitter and should really be in the generic graphics blitter code module
 *
 * @param pWidget Widget to display
 * @param pViewport Viewport to display it on
 * @param pClipRect Specifies the bounds for drawing the widget
 */
void EhsWidgetImageJpg_draw(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect)
{

    EhsGraphicsRectangleClass xDstRect; /* defines the bounds of the rectangle on the target */
#ifdef NOTREFACORINGMEDIASRCINFO
    EhsGraphicsRectangleClass xSrcRect; /* defines the bounds of the source image */
    /* we want to draw the entire bitmap, so set left and top to 0 */
    xSrcRect.nLeft = 0u;
    xSrcRect.nTop = 0u;
    xSrcRect.nWidth = EhsTVSurface_width(EHS_IMAGE_JPG(pWidget).pSurface);
    xSrcRect.nHeight = EhsTVSurface_height(EHS_IMAGE_JPG(pWidget).pSurface);
#endif

    // removed as widget is now sized to proportions of image if flag set for maintain aspect ratio
//	EhsGraphicsRectangle_proportionalScale(&xDstRect,&(pWidget->xCurRect),&xSrcRect);
//	EhsTV_blit_withlock(pViewport, (EHS_IMAGE_JPG(pWidget).pSurface), &xDstRect, &xSrcRect, EHS_WIDGET_IMAGE(pWidget).nCurrentImageAlpha);

    EhsTV_blit_withlock(pViewport, (EHS_IMAGE_JPG(pWidget).pSurface), &(pWidget->xCurRect), &(pWidget->MediaRect)/*&xSrcRect*/, EHS_WIDGET_IMAGE(pWidget).nCurrentImageAlpha);
    //@todo clear the viewport like for gifs??
}

/**
 * Destroy the image data for the current image file
 *
 * @param pWidget Widget to destroy
 */
void EhsWidgetImageJpg_destroy(EhsWidgetClass* pWidget)
{
    EhsTVSurface_destroy(&EhsTV, EHS_IMAGE_JPG(pWidget).pSurface);
}

