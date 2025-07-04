/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/


/** @file widget_image_png.c
 * Declarations for the image file class specifically for handling the PNG file type.
 *
 * @author: inx limited
 *
 */

//#define EHSL_MODULE_ID (EHSH_LOG_MODULE_GRAPHICS)

#include "ehs_types.h"
#include "widget.h"
#include "target.h"
#include "png/png.h"
#include "graphics.h"
#include "targetgfx_init.h"
#include "messages.h"
#include "hal-api.h"

/*Used for precalculating Alpha values for cairo.*/
#ifdef CAIRO_TARGET
#define MULT(d,c,a,t) G_STMT_START { t = c * a + 0x7f; d = ((t >> 8) + t) >> 8; } G_STMT_END
#endif /*CAIRO_TARGET*/

/**
 * Gets the data from an 8-bit rgb image.
 *
 * @par Memory
 * - The returned pointer is created by using the hal_viewport surface create method.
 * - You have to free the allocated memory yourself.
 *
 * @par Structure3295.712960:Enter EhsTV_updateRect
 *
 * - The color-sequence is Blue-Green-Red-Alpha (8 bit each).
 * - The first 4 values (RGBA) are located in the top-left corner of the image.
 * - The last 4 values (RGBA) are located in the bottom-right corner of the image.
 */
EhsTVSurfaceClass* doConvertRGB8(png_structp PngPtr, png_infop InfoPtr)
{
    EhsTVSurfaceClass* pRet = EhsTVSurface_create(&EhsTV,InfoPtr->width,InfoPtr->height,EHS_GRAPHICS_COLOUR_ARGB8888,NULL,0);//, EHS_FALSE);
    if (pRet)
    {
        ehs_uint16 i,j;
        //LOGI("Getting pixels for doConverRGB8");
        EhsGraphicsColourClass* pPixels = EhsTVSurface_pixels(pRet); /* allow pixels to be accessed */
        if (pPixels)
        {
            png_bytep* row_pointers = png_get_rows(PngPtr, InfoPtr);	/* get raw image */
            //LOGI("Getting pixels for doConverRGB8 - 1");
            ehs_uint32 pos = 0u; /* index into the array of pixels */

#ifdef EHS_ANDROID
            /* offset required to end of line if pitch > width */
            ehs_uint32 nLineOffset = (EhsTVSurface_pitch(pRet) - InfoPtr->width);
#else
            /* offset required to end of line if pitch > width */
            ehs_uint32 nLineOffset = ((EhsTVSurface_pitch(pRet) - ((InfoPtr->width) * 4))) / 4; /*Need to check this works at some point*/
#endif
            //EHSH_LOG_INFO("XXX pointer is at %x offset is %d from width=%d and stride=%d", pPixels,nLineOffset, (InfoPtr->width),EhsTVSurface_pitch(pRet) );
            /* get color values */
            EHSH_LOG_INFO("DOING RGB Convert %d by %d RGB convert -with offset %d",(ehs_uint32) InfoPtr->width,(ehs_uint32)InfoPtr->height,nLineOffset);
            for( i = 0u; i < InfoPtr->height; i++)
            {
                //EHSH_LOG_INFO("XXX pointer position b4 %d-%d-%d", i,j,pos);
                for( j = 0u; j < (3u * InfoPtr->width); j += 3u)
                {
                    pPixels[pos].sComp.nAlpha	= 0xff;
                    pPixels[pos].sComp.nRed		= row_pointers[i][j];
                    pPixels[pos].sComp.nGreen 	= row_pointers[i][j+1];
                    pPixels[pos].sComp.nBlue	= row_pointers[i][j+2];

                    pos++;
                }
                pos += nLineOffset;
            }
        }
        else EHSH_LOG_ERROR("Couldn't get pixel data");
    }
    else EHSH_LOG_ERROR("Couldn't get surface for RGB convert");
    return pRet;
}


// --------------
// doConvertRGBA8
// --------------
/**
 * Gets the data from an 8-bit rgb image with alpha values.
 *
 * @par Memory
 * - The returned pointer is created by using the new[] operator.
 * - You have to free the allocated memory yourself.
 *
 * @par Structure
 * - The color-sequence is Blue-Green-Red-Alpha (8 bit each).
 * - The first 4 values (RGBA) are located in the top-left corner of the image.
 * - The last 4 values (RGBA) are located in the bottom-right corner of the image.
 */
EhsTVSurfaceClass* doConvertRGBA8(png_structp PngPtr, png_infop InfoPtr)
{
    int rowbytes = 0;
    EhsTVSurfaceClass* pRet = EhsTVSurface_create(&EhsTV,InfoPtr->width,InfoPtr->height,EHS_GRAPHICS_COLOUR_ARGB8888,NULL,0);//,EHS_FALSE);

    if (pRet)
    {
        ehs_uint16 i,j;
        ehs_uint32 t=0;
        //LOGI("Getting pixels RGBA");
        EhsGraphicsColourClass* pPixels = EhsTVSurface_pixels(pRet); /* allow pixels to be accessed */
        png_bytep* row_pointers = png_get_rows(PngPtr, InfoPtr);	/* get raw image */
        rowbytes= png_get_rowbytes(PngPtr, InfoPtr);

        ehs_uint32 pos = 0u; /* index into the array of pixels */
        /* offset required to end of line if pitch > width */
#ifdef EHS_ANDROID
        ehs_uint32 nLineOffset = (EhsTVSurface_pitch(pRet) - InfoPtr->width); /* EhsTVSurface pitch must return bytes not pixels - we want pixel offset here*/
#else
        //ehs_uint32 nLineOffset = (EhsTVSurface_pitch(pRet) - InfoPtr->width); /* EhsTVSurface pitch must return bytes not pixels - we want pixel offset here*/
        ehs_uint32 nLineOffset = ((EhsTVSurface_pitch(pRet) - (InfoPtr->width*4)))/4; /* EhsTVSurface pitch must return bytes not pixels - we want pixel offset here*/
#endif
        /* get color values */
        for( i = 0u; i < InfoPtr->height; i++)
        {
            for( j = 0u; j < rowbytes; j += 4)
            {
#ifdef CAIRO_TARGET
                MULT(pPixels[pos].sComp.nRed, row_pointers[i][j], row_pointers[i][j + 3], t);
                MULT(pPixels[pos].sComp.nGreen, row_pointers[i][j+1], row_pointers[i][j + 3], t);
                MULT(pPixels[pos].sComp.nBlue, row_pointers[i][j+2], row_pointers[i][j + 3], t);
                pPixels[pos++].sComp.nAlpha	= row_pointers[i][j + 3];

#else
                pPixels[pos].sComp.nAlpha	= EHS_TV_ALPHA8_SCALE(row_pointers[i][j + 3]);
                pPixels[pos].sComp.nRed		= row_pointers[i][j];
                pPixels[pos].sComp.nGreen 	= row_pointers[i][j+1];
                pPixels[pos++].sComp.nBlue	= row_pointers[i][j+2];
#endif /*CAIRO_TARGET*/
            }
            pos += nLineOffset;
        }
    }
    return pRet;
}


// --------------
// doConvertGrey8
// --------------
/**
 * Gets the data from an 8-bit monochrome image.
 *
 * @par Memory
 * - The returned pointer is created by using the new[] operator.
 * - You have to free the allocated memory yourself.
 *
 * @par Structure
 * - The color-sequence is Blue-Green-Red-Alpha (8 bit each).
 * - The first 4 values (RGBA) are located in the top-left corner of the image.
 * - The last 4 values (RGBA) are located in the bottom-right corner of the image.
 * @todo - get rid of the reference to &EhsTV r
 */
EhsTVSurfaceClass* doConvertGrey8(png_structp PngPtr, png_infop InfoPtr)
{
    EhsTVSurfaceClass* pRet = EhsTVSurface_create(&EhsTV,InfoPtr->width,InfoPtr->height,EHS_GRAPHICS_COLOUR_ARGB8888,NULL,0);//,EHS_FALSE);
    if (pRet)
    {
        ehs_uint16 i,j;
        //LOGI("Getting pixels for doConvertGrey8");
        EhsGraphicsColourClass* pPixels = EhsTVSurface_pixels(pRet); /* allow pixels to be accessed */
        png_bytep* row_pointers = png_get_rows(PngPtr, InfoPtr);	/* get raw image */

        ehs_uint32 pos = 0u; /* index into the array of pixels */
        /* offset required to end of line if pitch > width */
        ehs_uint16 nLineOffset = (EhsTVSurface_pitch(pRet) - (4*InfoPtr->width))/4;

        /* get color values */
        for( i = 0u; i < InfoPtr->height; i++)
        {
            for( j = 0u; j < (InfoPtr->width); j ++)
            {
                pPixels[pos].sComp.nAlpha	= 0x80;
                pPixels[pos].sComp.nRed		= row_pointers[i][j];
                pPixels[pos].sComp.nGreen 	= row_pointers[i][j];
                pPixels[pos++].sComp.nBlue	= row_pointers[i][j];
            }

            pos += nLineOffset;
        }
    }

    return pRet;
}


// ---------------
// doConvertGreyA8
// ---------------
/**
 * Gets the data from an 8-bit monochrome image with alpha values.
 * @todo get rif of EhsTV reference
 */
EhsTVSurfaceClass* doConvertGreyA8(png_structp PngPtr, png_infop InfoPtr)
{
    EhsTVSurfaceClass* pRet = EhsTVSurface_create(&EhsTV,InfoPtr->width,InfoPtr->height,EHS_GRAPHICS_COLOUR_ARGB8888,NULL,0);//,EHS_FALSE);
    if (pRet)
    {
        ehs_uint16 i,j;
        //LOGI("Getting pixels for doConvertGreyA8");
        EhsGraphicsColourClass* pPixels = EhsTVSurface_pixels(pRet); /* allow pixels to be accessed */
        png_bytep* row_pointers = png_get_rows(PngPtr, InfoPtr);	/* get raw image */

        ehs_uint32 pos = 0u; /* index into the array of pixels */
        /* offset required to end of line if pitch > width */
        ehs_uint16 nLineOffset = EhsTVSurface_pitch(pRet) - 2u * InfoPtr->width; /* 2u cos has alhpa values too */

        /* get color values */
        for( i = 0u; i < InfoPtr->height; i++)
        {
            for( j = 0u; j < (2u * InfoPtr->width); j += 2u)
            {
                pPixels[pos].sComp.nAlpha	= row_pointers[i][j];
                pPixels[pos].sComp.nRed		= row_pointers[i][j];
                pPixels[pos].sComp.nGreen 	= row_pointers[i][j];
                pPixels[pos++].sComp.nBlue	= row_pointers[i][j];
            }

            pos += nLineOffset;
        }
    }
    return pRet;
}

/* Converts image formats to our canonical ones */
ehs_bool doExtractCanonicData(png_structp  PngPtr, png_infop InfoPtr, EhsTVSurfaceClass** ppSurface)
{

    int color_type;
//@todo  Is there a memory leek here if we are not freeing previous images - need to initiaise widget table widgets->pSurface to null at begining
    if (*ppSurface) /* if we already had a PNG then delete its memory */
    {
        //EhsHMem_tempFree(*ppSurface);
        //*ppSurface = 0;
    }

    // get color information
    color_type = png_get_color_type(PngPtr, InfoPtr);
    // rgb
    if (color_type == PNG_COLOR_TYPE_RGB)
    {
        *ppSurface = doConvertRGB8(PngPtr, InfoPtr); //allocates memory for m_bgra
    }
    // rgb with opacity
    else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
    {
        *ppSurface = doConvertRGBA8(PngPtr, InfoPtr);//allocates memory for m_bgra
    }
    // 256 grey values
    else if (color_type == PNG_COLOR_TYPE_GRAY)
    {
        *ppSurface = doConvertGrey8(PngPtr, InfoPtr);//allocates memory for m_bgra
    }
    // 256 grey values with opacity
    else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
        *ppSurface = doConvertGreyA8(PngPtr, InfoPtr); //allocates memory for m_bgra
    }
    // check pointer
    return (*ppSurface != 0);
}








/**
 * Load an PNG from a file into the EhsWidgetImagePngSubclass structure
 *
 * @param pImage image to load png into
 * @param szFilename File to load image from
 * @return true if load was successful
 */

EHS_GLOBAL ehs_bool EhsWidgetImagePng_load(EhsWidgetClass* pImage, const ehs_char* szFilename)
{
    EhsWidgetImagePngSubclass *pPng = &(EHS_IMAGE_PNG(pImage));
    ehs_FILE *file;
    png_infop info_ptr;
    ehs_bool bReturn = EHS_FALSE;
    ehs_bool bLoadImageFromAppDir;
    EhsGfxFileOrientation orientation;
    char idx='A';
    // check filetype
    bLoadImageFromAppDir = EhsWidgetImage_loadFileFromAppDir(pImage);
    //if (bLoadImageFromAppDir) //LOGI("Loading OK");//,bLoadImageFromAppDir);
    //else //LOGI("Loading failed");
    if (doCheckFileHeader(szFilename, bLoadImageFromAppDir,EHS_GFXFILETYPE_PNG,&orientation))   /* This function also supports the canonical file access protocol */
    {
        // try to open file
        if (bLoadImageFromAppDir)
        {
            file = Ehs_AppFopen(szFilename, "rb");
        }
        else
        {
            file = Ehs_UserFopen(szFilename, "rb");
        }
        if (file != NULL) // double check
        {
            EhsGraphicsColourClass* pPixels; // pointer to canonical pPixels data
            /* code to read the png from file */
            // create read struct
            png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
            // check pointer
            if (png_ptr == 0)
            {
                EhsFclose(file);
                return EHS_FALSE;
            }
            // create info struct
            info_ptr = png_create_info_struct(png_ptr);
            // check pointer
            if (info_ptr == 0)
            {
                png_destroy_read_struct(&png_ptr, 0, 0); //no  need to do this, but left just in case
                EhsFclose(file);
                EHSH_LOG_ERROR("Could not create PNG info ptr");
                return EHS_FALSE;
            }

            // set error handling
            if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, 0);//no  need to do this, but left just in case
                EhsFclose(file);
                EHSH_LOG_ERROR("Could not set error handler for PNG loader");
                return EHS_FALSE;
            }
            // I/O initialization using standard C streams
            png_init_io(png_ptr, file);
            // read entire image (high level)
            png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
            // convert the png bytes to BGRA
            if (!doExtractCanonicData(png_ptr, info_ptr,&(EHS_IMAGE_PNG(pImage).pSurface)))
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, 0);//no  need to do this, but left just in case
                EHSH_LOG_ERROR("Could not extract canonical pixel data");
                EhsFclose(file);
                return EHS_FALSE;
            }
            pPixels = EhsTVSurface_pixels(EHS_IMAGE_PNG(pImage).pSurface);
            {
                ehs_uint32 i;
                ehs_bool bInvert = EHS_TRUE;
                for (i = 0; i < info_ptr->width*info_ptr->height; i++)
                {
                    if (pPixels[i].sComp.nAlpha != 0)
                    {
                        bInvert = EHS_FALSE;
                        break;
                    }
                }
                if (bInvert)
                {
                    for (i = 0; i < info_ptr->width*info_ptr->height; i++)
                    {
                        pPixels[i].sComp.nAlpha = 0xff;
                    }
                }
            }



            //LOGI("FINALISING THE PNG SURFACE");
            EhsTVSurface_finalisePixels((EHS_IMAGE_PNG(pImage).pSurface));


            // free memory
            png_destroy_read_struct(&png_ptr, &info_ptr, 0); //no  need to do this, but left just in case
            // close file
            EhsFclose(file);
            bReturn=EHS_TRUE;
        }
    }
    else EHSH_LOG_WARNING("Couldn't Load PNG");

    if (bReturn == EHS_TRUE)
    {
        pImage->MediaRect.nWidth =  EhsTVSurface_width(pImage->specificWidgetType.image.specificImageSrcType.png.pSurface);
        pImage->MediaRect.nHeight =  EhsTVSurface_height(pImage->specificWidgetType.image.specificImageSrcType.png.pSurface);

        if (pImage->bMaintainAspectRatio)
        {
            EhsWidget_resizeWidgetToMaintainAspectRatio(pImage);
        }
        else
        {
            EhsWidget_resetWidgetSizeToDesignTime(pImage);
        }
    }
    //LOGI("Loaded PNG");
    return (bReturn);
}

/**
 * Display the png onto the viewport.
 * Note this is a generic 2-D bgra8888 blitter and should really be in the generic graphics blitter code module
 *
 * @param pWidget Widget to display
 * @param pViewport Viewport to display it on
 * @param pClipRect Specifies the bounds for drawing the widget
 */

void EhsWidgetImagePng_draw(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect)
{
    /* convert the image data into an array of EhsGraphicsColourClass, which are 32bit ints containing
     * 8 bits each of alpha, red, green and blue. Alpha 0xff is opaque, 0x00 is transparent.
     * The image could be converted once by the load routine, or every time "draw" takes place.
     * Convert once likely results in a heavier use of memory. Convert on draw is more CPU intensive.
     * To minimise memory use, it's also possible to convert a line at a time - see example code below
     */

    /* note: we use pWidget->xCurRect for destination and use its  width/height for the source.
     * This assumes that PNGs are always the same size as when defined in LGB */

    EhsTV_blit_withlock(pViewport, (EHS_IMAGE_PNG(pWidget).pSurface), &(pWidget->xCurRect), &(pWidget->MediaRect), EHS_WIDGET_IMAGE(pWidget).nCurrentImageAlpha);

}
#ifdef BLIT
void EhsWidgetImagePng_draw(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect)
{
}
#endif /*BLIT*/
/**
 * Release the memory held by the png widget
 *
 */
void EhsWidgetImagePng_destroy(EhsWidgetClass* pWidget)
{
    EhsTVSurface_destroy(&EhsTV, EHS_IMAGE_PNG(pWidget).pSurface);
}
