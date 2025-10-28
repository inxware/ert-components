/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/
/** @file target_viewport.c
 * This file provides the definitions for EhsTargetViewportClass, which
 * drives the target's graphic device
 *
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section target_viewport
 * @anchor target_viewport
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on 2007-10-12
 * Last modified on $Date$
 *
 * This file contained no derogations to the MISRA standard.
 *
 * ??Note it is necessary to replace <sys/types.h> with <types.h> to lint this file successfully.
 */


/**
 * Provides access to the target-specific declarations of header files
 */
#define EHS_TARGET_CODE

/*****************************************************************************/
/* Included files */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#include "globals.h"
#include "hal_string.h"
#include "hal_viewport.h"
#include "hal_mem.h"
#include "hal_process.h"
#include "widget.h"
#include "typedefs.h"
#include "messages.h"


/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/**
 * Define the target viewport type. Contains target specific data required
 * by the viewport functions.
 */
struct EhsTVStruct
{
    ehs_uint8 *pDeviceMem;				/**< Points to the framebuffer used to access the viewport */
    ehs_uint8 bytesperpixel;			/**< Bytes per pixel */
    STGFX_Font_t *pFonts;				/**< Font to use with this viewport */
    EhsGraphicsRectangleClass clipRect;	/**< Rectangle currently used to clip what is being displayed */
    EhsGraphicsRectangleClass viewRect; /**< Specifies the size of the viewport */
    EhsTVSurfaceClass* pAllocSurface; /**< List of allocated surfaces - used for deallocation purposes */

};



/**
 * This macro allows the selection of an algorithm that performs alpha blending much
 * faster, but with slightly lower accuracy.
 */
/* #define EHS_TV_QUICKBLEND */
/*****************************************************************************/
/* Declare prototypes of local functions */

EHS_LOCAL inline EhsGraphicsColourClass EhsTV_alphaBlendPixel(EhsGraphicsColourClass xFg, EhsGraphicsColourClass xBg, ehs_uint8 nImageAlpha);

/**
 *  Called to display an array of single pixels (using the palette)
 */
EHS_LOCAL void EhsL_blitA1(EhsGraphicsColourClass* pPixels,
                           EhsTVSurfaceClass* pSurface,
                           const EhsGraphicsRectangleClass* prDst,
                           const EhsGraphicsRectangleClass* prSrc,
                           ehs_uint8 nImageAlpha);

EHS_LOCAL void EhsL_blitArgb8888(EhsTVClass* pViewport,
                                 const EhsTVSurfaceClass* pSurface,
                                 const EhsGraphicsRectangleClass* prDst,
                                 const EhsGraphicsRectangleClass* prSrc,
                                 ehs_uint8 nImageAlpha);


/*****************************************************************************/
/* Variables defined with file-scope */

EHS_LOCAL ehs_uint8 EhsTgtDoubleBuff[EHS_CONFIG_DISPLAY_HEIGHT*EHS_CONFIG_DISPLAY_WIDTH*EHS_TV_MAX_COLOUR_DEPTH];

/*****************************************************************************/
/* Variables defined with global-scope */

/**
 * Contains a table that contains the results of (A*B)/255 for
 * two unsigned 8-bit integers (A and B). This table is used primarily
 * in the viewport blitting code for blending alpha values
 *
 */
EHS_GLOBAL ehs_uint8 EhsTgtMultData [256*256];


/**
 * Define the target viewport. Only one viewport is defined at this
 * time.
 * ASSUMPTIONS:
 * This variable should not be accessed directly in functions.
 * A pointer reference to it (passed in the arguement) shouild be used in stead
 * The init function could dymanicall create this to make this code module re-rentrant - i.e. for devices with more than one view port, or display.
 * If this is malloced this global variable could remove this.
 */
EhsTVClass EhsTV;


/*****************************************************************************/
/* Function definitions */




/**
 * Initialise the target viewport. This function is called
 * once in the life of the EHS application at system initialization time.
 * ASSUMPTION - pViewport is initialised here - it could .
 */
ehs_bool EhsTV_init(EhsTVClass* pViewport)
{
    int width;
    int height;
    unsigned char *data;
    int nMemSize;
    int nBytes;
    unsigned long lVal;
    unsigned short uVal;
    char *FontFileName = "courB18.bdf";
    EhsGraphicsRectangleClass clip;

    // open framebuffer device and read out info
    int fd = open("/dev/fb0", O_RDWR);
    if (fd < 0)
    {
        exit(1);
    }
    struct fb_var_screeninfo screeninfo;
    struct fb_fix_screeninfo fixed;
    ioctl(fd,FBIOGET_FSCREENINFO,&fixed);
    ioctl(fd, FBIOGET_VSCREENINFO, &screeninfo);
    //determine size
    pViewport->viewRect.nLeft = 0u;
    pViewport->viewRect.nTop = 0u;
    pViewport->viewRect.nWidth = screeninfo.xres;
    pViewport->viewRect.nHeight = screeninfo.yres;
    pViewport->bytesperpixel = screeninfo.bits_per_pixel / 8;
    nMemSize = pViewport->viewRect.nWidth*pViewport->viewRect.nHeight*pViewport->bytesperpixel; // 4 bytes for ARGB

    // embed framebuffer into memory
    pViewport->pDeviceMem = (unsigned char *)mmap(0, nMemSize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    // load font for text
    //GFX_LoadFont(NULL, FontFileName, &(pViewport->pFonts), 0); //@todo should this be removed
    // Initialise clipping rectangle
    clip.nLeft = 0u;
    clip.nTop = 0u;
    clip.nWidth = pViewport->viewRect.nWidth;
    clip.nHeight = pViewport->viewRect.nHeight;
    pViewport->clipRect = clip;
    pViewport->pAllocSurface = NULL; /* set chain of allocated memory to null - this doesn't include the double buffer */


    //EhsTV_update(pViewport);
    return EHS_TRUE;
}


/* This should do any target specific resetting */

void EhsTV_reset(EhsTVClass* pViewport)
{
    ENTER(EhsTV_reset);
    EhsTPMutex_lock(EhsTPMutex_widgetTable);

    EhsTPMutex_unlock(EhsTPMutex_widgetTable);

    LEAVE(EhsTV_reset);
}

/**
 *  Shutdown the target viewport. This function
 * releases resources occupied by the viewport - this is not currently used.
 */
void EhsTV_term(EhsTVClass* pViewport)
{
    ENTER(EhsTV_term);

    //EhsTV_reset(pViewport);
    //gdk_threads_enter();  //PPP:  out these back
    /* release GTK objects */
    //g_object_unref(EhsTV.pMainWindow);
    //gdk_threads_leave();

    LEAVE(EhsTV_term);
}

/*Change size and position of viewport*/
void EhsTV_move(EhsTVClass* pViewport, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nDeltaWid, EhsDataflowIntType nDeltaHt)
{
    /*
    ENTER(EhsTV_move);
    if((newX != nX) || (newY != nY))
    {
    	newX = nX;
    	newY = nY;
    	windowMoved = 1;
    }

    newWidth = nDeltaWid;
    newHeight = nDeltaHt;

    LEAVE(EhsTV_move);
    */
}

/*Change alpha value of viewport*/
void EhsTV_fade(EhsTVClass* pViewport, EhsGraphicsColourClass nColour)
{

    /*
    ENTER(EhsTV_fade);
    viewColour = nColour;
    EhsTV_update(pViewport);
    LEAVE(EhsTV_fade);
    */
}

/**
* Indicate to the target that the entire viewport needs updating.
*
*/







/**
 *  Terminate the use of the target viewport. This function
 * releases resources occupied by the viewport.
 */
void EhsTV_term(EhsTVClass* pViewport)
{
    /* not currently used, therefore not implemented */
}

/**
 * Indicate to the target that the entire viewport needs updating.
 */
void EhsTV_update(EhsTVClass* pViewport)
{
    EhsTV_updateRect(pViewport, 0u, 0u, pViewport->viewRect.nWidth, pViewport->viewRect.nHeight);
}

/**
 * Redraw a specified part of the viewport.
 *
 */
// @todo delete me when tested: void EhsTV_updateRect(EhsTVClass* pViewport, ehs_uint16 nX, ehs_uint16 nY, ehs_uint16 nWidth, ehs_uint16 nHeight)
void EhsTV_updateRect(EhsTVClass* pViewport, ehs_sint32 nX, ehs_sint32 nY, ehs_sint32 nWidth, ehs_sint32 nHeight)
{
    ehs_uint16 nRow;
    ehs_uint16 nCol;
    EhsGraphicsRectangleClass clip;
    EhsGraphicsRectangleClass viewportRect;
    ehs_uint8* pDoubleBuff = &(EhsTgtDoubleBuff[((pViewport->viewRect.nWidth*nY)+nX)*pViewport->bytesperpixel]);
    ehs_uint8* pVideoMem = &(pViewport->pDeviceMem[((pViewport->viewRect.nWidth*(nY))+nX)*pViewport->bytesperpixel]);
    ehs_uint16 nViewportWidth = pViewport->viewRect.nWidth*pViewport->bytesperpixel;

    clip.nLeft = nX;
    clip.nTop = nY;
    clip.nWidth = nWidth;
    clip.nHeight = nHeight;
    /* Make sure we're not out of bounds */
    if (EhsGraphicsRectangle_intersect(&clip,&clip,&(pViewport->viewRect)))
    {
        /* clear the redraw area */
        for (nRow = 0; nRow < nHeight; nRow++)
        {
            EhsMemset(pDoubleBuff,0u,nWidth*pViewport->bytesperpixel);
            pDoubleBuff += nViewportWidth;
        }

        /* draw the widgets falling within the rectangle to the double buffer */
        clip.nLeft = nX;
        clip.nTop = nY;
        clip.nWidth = nWidth;
        clip.nHeight = nHeight;
        pViewport->clipRect = clip;
        EhsWidgetTable_draw(&EhsWidgetTable,pViewport,&clip);

        /* copy the double buffer to the video memory */
        pDoubleBuff = &(EhsTgtDoubleBuff[((pViewport->viewRect.nWidth*nY)+nX)*pViewport->bytesperpixel]);
        for (nRow = 0; nRow < nHeight; nRow++)
        {
            //for (nRow = 0; nRow < 768; nRow++) {
            EhsMemcpy(pVideoMem, pDoubleBuff, nWidth*pViewport->bytesperpixel);
            //EhsMemcpy(pVideoMem, pDoubleBuff, 100*pViewport->bytesperpixel);
            pVideoMem += nViewportWidth;
            pDoubleBuff += nViewportWidth;
        }
    }
}

/**
 * Clear the target viewport
 */
void EhsTV_clear(EhsTVClass* pViewport)
{
    /* Fill the video area with 0 */
    EhsMemset(&(pViewport->pDeviceMem[0]), 0u, pViewport->viewRect.nWidth*pViewport->viewRect.nHeight*pViewport->bytesperpixel);
}

/**
 * Alpha blend a single pixel with the background pixel at a specific address
 * Formula taken from http://en.wikipedia.org/wiki/Alpha_Blend
 *
 * @param[in] xFg Foreground colour (ARGB)
 * @param[in] xBg Background colour (ARGB)
 * @param[in] nImageAlpha Global image alpha level
 * @return blended pixel value.
 */
EHS_LOCAL inline EhsGraphicsColourClass EhsTV_alphaBlendPixel(EhsGraphicsColourClass xFg, EhsGraphicsColourClass xBg, ehs_uint8 nImageAlpha)
{

#ifdef EHS_TV_QUICKBLEND
    ehs_uint8 nNewAlpha = EhsTgtMultData[(xFg.sComp.nAlpha<<8)|nImageAlpha];
    ehs_uint32 nBlendAlpha = nNewAlpha | nNewAlpha << 8 | nNewAlpha << 16;
    ehs_uint32 nBlendInvAlpha = (~nBlendAlpha) & 0x00ffffff;
    ehs_uint32 *pDblBuff = &(EhsTgtDoubleBuff[nDataBaseIndex]);
    /* alpha = alphaA + alphaB (1 - alphaA) */
    ehs_uint32 nTgtAlpha = (nNewAlpha << 24) + (((~nNewAlpha) << 24)*(*pDblBuff & 0xff000000));

    *pDblBuff = (*pDblBuff & nBlendInvAlpha) | (xFg.nUint32 & nBlendAlpha) | nTgtAlpha;
#else
    /* requires 65 cycles */
    /* EhsTgtMultData provides data for multiplying two 8-bit numbers then dividing the product
     * by 255.
     *
     * Find the total alpha level of the pixel we are calculating - given by:
     * nNewAlpha = (xFg.sComp.nAlpha * nImageAlpha) / 255
     */
    ehs_uint8 nNewAlpha = EhsTgtMultData[(xFg.sComp.nAlpha<<8)|nImageAlpha];
    ehs_uint8 n255MinusNewAlpha = 255 - nNewAlpha;
    /* nNATab provides a table that multiplies a number by nNewAlpha */
    ehs_uint8 *nNATab = &(EhsTgtMultData[(nNewAlpha<<8)]);
    /* nInvNATab provides a table that multiplies a number by (255-nNewAlpha) */
    ehs_uint8 *nInvNATab = &(EhsTgtMultData[(n255MinusNewAlpha<<8)]);

    xBg.sComp.nBlue = nInvNATab[xBg.sComp.nBlue] + nNATab[xFg.sComp.nBlue];
    xBg.sComp.nGreen = nInvNATab[xBg.sComp.nGreen] + nNATab[xFg.sComp.nGreen];
    xBg.sComp.nRed = nInvNATab[xBg.sComp.nRed] + nNATab[xFg.sComp.nRed];
    xBg.sComp.nAlpha = xBg.sComp.nAlpha + nNATab[0xffu - xFg.sComp.nAlpha];
    return xBg;
#endif
}

/**
 * @TODO review resource locking arrangements for Linux
 *
 * @param[in] pViewport Current viewport
 * @param[in] pSurface The image to blit
 * @param[in] prDst Rectangle on the viewport to blit to
 * @param[in] prSrc Rectangle on the surface to blit from
 * @param[in] nAlpha The global alpha level
 */
void EhsTV_blit(EhsTVClass* pViewport,
                const EhsTVSurfaceClass* pSurface,
                const EhsGraphicsRectangleClass* prDst,
                const EhsGraphicsRectangleClass* prSrc,
                ehs_uint8 nImageAlpha)
{
    EhsGraphicsColourClass* pPixels = NULL;

    switch (pSurface->eFormat)
    {
    case EHS_GRAPHICS_COLOUR_ARGB8888:
        EhsL_blitArgb8888(pViewport,
                          pSurface,
                          prDst,
                          prSrc,
                          nImageAlpha);
        break;
    case EHS_GRAPHICS_COLOUR_A1:
        EhsL_blitA1(pPixels, pSurface, prDst, prSrc, nImageAlpha);
        break;
    case EHS_GRAPHICS_COLOUR_A8:
    /* @todo support blitting for this kind of image */
    default:
        EHSH_LOG_ERROR(EHS_MSG_TGT_GRAPHICS_UNSUPPORTED_FORMAT("EhsTV_blit",pSurface->eFormat));
        break;
    }

}

/**
 *  Called to display an array of single pixels (using the palette)
 */
void EhsL_blitA1(EhsGraphicsColourClass* pPixels,
                 EhsTVSurfaceClass* pSurface,
                 const EhsGraphicsRectangleClass* prDst,
                 const EhsGraphicsRectangleClass* prSrc,
                 ehs_uint8 nImageAlpha)
{
    ehs_uint32 x,y;
    EhsGraphicsColourClass* pLine = EhsTVSurface_pixels(pSurface);
    EhsGraphicsColourClass xBg = pSurface->pPalette[0];
    EhsGraphicsColourClass xFg = pSurface->pPalette[1];

    for (y = 0; y < prDst->nHeight; y++)
    {
        ehs_uint32 nPixIdx = y * prDst->nWidth;
        ehs_uint32 nBmpIdx = y * ((pSurface->nWid+7)/8);
        for (x = 0; x < prDst->nWidth; x++)
        {
            ehs_uint8 nBit = 1 << (7u - (x % 8u));
            if ((nBit & EhsTVSurface_pixels(pSurface)[nBmpIdx + x/8]) == nBit)
            {
                pPixels[nPixIdx+x] = EhsTV_alphaBlendPixel(xFg, pPixels[nPixIdx+x], nImageAlpha);
            }
            else
            {
                pPixels[nPixIdx+x] = EhsTV_alphaBlendPixel(xBg, pPixels[nPixIdx+x], nImageAlpha);
            }
        }
    }
}

/**
 * General blitting function used by the three specific blitting functions.
 *
 * @param[in] pViewport Viewport to blit into
 * @param[in] pImgData The content of the image represented as a 2d array of colours.
 * @param[in] pImgBounds The bounding box for the image
 * @param[in] nAlpha The global alpha level
 * @param[in] bAlpha Does the image require alpha blending (i.e. some pixels are partially see-through)
 * @param[in] bSprite Are some image pixels completely transparent?
 *
 */
void EhsL_blitArgb8888(EhsTVClass* pViewport,
                       const EhsTVSurfaceClass* pSurface,
                       const EhsGraphicsRectangleClass* prDst,
                       const EhsGraphicsRectangleClass* prSrc,
                       ehs_uint8 nImageAlpha)
{
    EhsGraphicsRectangleClass blitBounds;		/* This is the area that we are blitting this image into */
    const EhsGraphicsColourClass* pSrc; 		/* points to our source data */
    EhsGraphicsColourClass* pDest;				/* points to the destination data */
    ehs_uint16 nRow, nCol;						/* index into image */


    // SDG: REVISIT. Not sure what's going on here

    /* calculate the parts of the image that we need to update */
    EhsGraphicsRectangle_intersect(&blitBounds,prDst,&(pViewport->clipRect));

    /* source points to the first part of the image data contained in blitBounds */
    //pSrc = &(pSurface->uSurface.pBitmap[
    //			((blitBounds.nTop-pImgBounds->nTop) * pImgBounds->nWidth) + /* vertical offset */
    //			(blitBounds.nLeft - pImgBounds->nLeft)]);					/* horizontal offset */
    // SDG: REVISIT - Check with Pierre whether I need offset
    pSrc = &(pSurface->uSurface.pBitmap);
    /* dest points to the pixel we are writing to in the double buffer */
    pDest = (EhsGraphicsColourClass*)EhsTgtDoubleBuff;
    pDest = &(pDest[
                  (blitBounds.nTop*pViewport->viewRect.nWidth) +					/* vertical offset */
                  blitBounds.nLeft]);											/* horizontal offset */

    /* Perform the alpha blending within the blitBounds rectangle */
    for (nRow=0u; nRow<(blitBounds.nHeight); nRow++)
    {
        for (nCol=0u; nCol<(blitBounds.nWidth); nCol++)
        {
            if (pSrc->sComp.nAlpha > 0u)   /* only do the blending for visible pixels */
            {
                *pDest = EhsTV_alphaBlendPixel(*pSrc,*pDest, nImageAlpha);
            }
            pDest++;
            pSrc++;
        }
        /* Add the difference between the amount we've blitted and the image/device width */
        // SDG: REVISIT - Find out what pImgBounds is
        //pSrc += pImgBounds->nWidth - blitBounds.nWidth;
        pDest += pViewport->viewRect.nWidth - blitBounds.nWidth;
    }
}

/**
 * Fill a rectangle with a specified colour in the specified viewport.
 *
 * @param[in] pViewport Viewport to fill rectangle into
 * @param[in] pRect position and size of rectangle to fill
 * @param[in] pColour Colour to use for filling rectangle (includes global alpha value)
 */
// SDG: REVISIT - Check this function draws a patch.
void EhsTV_fillRect(EhsTVClass* pViewport,
                    const EhsGraphicsRectangleClass* pRect, const EhsGraphicsColourClass* pColour)
{
    EhsGraphicsRectangleClass blitBounds;		/* This is the area that we are blitting this image into */
    const EhsGraphicsColourClass* pSrc; 		/* points to our source data */
    EhsGraphicsColourClass* pDest;				/* points to the destination data */
    ehs_uint16 nRow, nCol;						/* index into image */


    /* calculate the parts of the image that we need to update */
    EhsGraphicsRectangle_intersect(&blitBounds, pRect, &(pViewport->clipRect));

    /* source points to the first part of the image data contained in blitBounds */
    // SDG: REVISIT
    //pSrc = &(pImgData[
    //			((blitBounds.nTop-pImgBounds->nTop) * pImgBounds->nWidth) + /* vertical offset */
    //			(blitBounds.nLeft - pImgBounds->nLeft)]);					/* horizontal offset */
    /* dest points to the pixel we are writing to in the double buffer */
    pDest = (EhsGraphicsColourClass*)EhsTgtDoubleBuff;
    pDest = &(pDest[
                  (blitBounds.nTop*pViewport->viewRect.nWidth) +					/* vertical offset */
                  blitBounds.nLeft]);											/* horizontal offset */

    /* render according to image details */
    if (pColour->sComp.nAlpha == 0xff)   /* simply copy all pixels */
    {
        for (nRow = 0u; nRow < (blitBounds.nHeight); nRow++)
        {
            for (nCol=0u; nCol < (blitBounds.nWidth); nCol++)
            {
                *pDest = *pColour;
                pDest++;
            }
            /* Add the difference between the amount we've blitted and the image/device width */
            // SDG: REVISIT
            //pSrc += pImgBounds->nWidth - blitBounds.nWidth;
            pDest += pViewport->viewRect.nWidth - blitBounds.nWidth;
        }
    }
    else if (pColour->sComp.nAlpha == 0)    /* invisible - do nothing */
    {
    }
    else     /* full alpha blending of pixels */
    {
        /* Perform the alpha blending within the blitBounds rectangle */
        for (nRow=0u; nRow < (blitBounds.nHeight); nRow++)
        {
            for (nCol=0u; nCol < (blitBounds.nWidth); nCol++)
            {
                // SDG: REVISIT - Check this code alpha blends.
                // I'm assuming the double buffer already has the source.
                //*pDest = EhsTV_alphaBlendPixel(*pSrc,*pColour, 0xff);
                *pDest = EhsTV_alphaBlendPixel(*pDest,*pColour, 0xff);
                pDest++;
            }
            /* Add the difference between the amount we've blitted and the image/device width */
            // SDG: REVISIT
            //pSrc += pImgBounds->nWidth - blitBounds.nWidth;
        }
    }
}

/**
 * Free memory used by a surface. Assumes that this function is never called for memory
 * allocated when bTemporary = FALSE in Surface_create
 */
void EhsTVSurface_destroy(EhsTVClass* pViewport, EhsTVSurfaceClass *pSurface)
{

    if(pSurface) {

        switch (pSurface->eFormat)
        {
        case EHS_GRAPHICS_COLOUR_A1:
        case EHS_GRAPHICS_COLOUR_A8:
            EhsHMem_tempFree(EhsTVSurface_pixels(pSurface));
            EhsTVSurface_bitmap(pSurface) = NULL; /* TODO this needs to ne replaced - this call will actually result in an allocation */
            EhsHMem_tempFree(pSurface->pPalette);
            pSurface->pPalette = NULL;
            break;
        case EHS_GRAPHICS_COLOUR_ARGB8888:
            EhsHMem_tempFree(EhsTVSurface_pixels(pSurface));
            EhsTVSurface_pixels(pSurface) = NULL; /* TODO this needs to ne replaced - this call will actually result in an allocation */
        }
        if (pViewport->pAllocSurface == pSurface)
        {
            pViewport->pAllocSurface = pSurface->pNext;
        }
        else
        {
            for (pPrev = pViewport->pAllocSurface; pPrev && (pPrev->pNext != pSurface); pPrev = pPrev->pNext)
                ;
            if (pPrev)
                pPrev->pNext = pSurface->pNext;
        }
        /* delete the structure */
        EhsHMem_tempFree(pSurface);
    }

}


/**
 * Allocate memory for images (to subsequently blit to screen)
 *
 * @param[in] pViewport Viewport where surface will be used
 * @param[in] nWidth Width of the viewport
 * @param[in] nHeight Height of the viewport
 * @param[in] eFormat format that is to be used for the bitmap
 * @param[in] pPalette array of ARGB8888 colours to use for the palette (NULL if not required)
 * @param[in] nPaletteSize number of entries in the palette
 * @param[in] bTemporary Create using the temporary memory (allows it to be destroyed
 * 	while the application is running).
 * @return pointer to the surface, or null if an error occured.
 */
EhsTVSurfaceClass* EhsTVSurface_create(EhsTVClass* pViewport,
                                       ehs_uint16 nWidth, ehs_uint16 nHeight, EhsGraphicsColourFormatEnum eFormat,
                                       EhsGraphicsColourClass* pPalette, ehs_uint16 nPaletteSize)//, ehs_bool bTemporary)
{
    EhsTVSurfaceClass* pSurface;
    ehs_uint8 nPixelsPerByte; /* number of pixels that appear in a single byte in the lookup table type formats */

    //if (bTemporary) {
    pSurface = EhsHMem_tempAlloc(sizeof(EhsTVSurfaceClass));
    //} else {
    //	pSurface = EhsHMem_writeableAlloc(sizeof(EhsTVSurfaceClass));
    //}

    if (pSurface)
    {
        pSurface->nWid = nWidth;
        pSurface->nHt = nHeight;
        pSurface->eFormat = eFormat;

        switch (eFormat)
        {
        case EHS_GRAPHICS_COLOUR_A1:
        case EHS_GRAPHICS_COLOUR_A8:
            nPixelsPerByte = (eFormat == EHS_GRAPHICS_COLOUR_A1)?8u:1u;
            /* calculate number of bytes required = (nWidth/(number of pixels per byte) rounded up) * nHeight */
            //if (bTemporary) {
            EhsTVSurface_bitmap(pSurface) = (EhsGraphicsColourClass*)EhsHMem_tempAlloc((nWidth+(nPixelsPerByte-1))/nPixelsPerByte*nHeight);
            //} else {
            //	EhsTVSurface_bitmap(pSurface) = (EhsGraphicsColourClass*)EhsHMem_writeableAlloc((nWidth+(nPixelsPerByte-1))/nPixelsPerByte*nHeight);
            //}
            if (!EhsTVSurface_pixels(pSurface))  //todo this looks wrong
            {
                //if (bTemporary) {
                EhsHMem_tempFree(pSurface);
                //}
                pSurface = NULL;
            }
            else
            {
                /* clear bitmap */
                EhsMemset(EhsTVSurface_bitmap(pSurface),0,(nWidth+(nPixelsPerByte-1))/nPixelsPerByte*nHeight);
                /* create space for palette */
                //if (bTemporary) {
                pSurface->pPalette = (EhsGraphicsColourClass*)EhsHMem_tempAlloc(nPaletteSize*sizeof(EhsGraphicsColourClass));
                //} else {
                //	pSurface->pPalette = (EhsGraphicsColourClass*)EhsHMem_writeableAlloc(nPaletteSize*sizeof(EhsGraphicsColourClass));
                //}
                if (!pSurface->pPalette)
                {
                    //if (bTemporary) {
                    EhsHMem_tempFree(EhsTVSurface_bitmap(pSurface));
                    EhsHMem_tempFree(pSurface);
                    //}
                    pSurface = NULL;
                }
            }
            if (pSurface)
            {
                /* copy palette */
                EhsMemcpy(pSurface->pPalette, pPalette, nPaletteSize*sizeof(EhsGraphicsColourClass));
            }
            break;
        case EHS_GRAPHICS_COLOUR_ARGB8888:
            //if (bTemporary) {
            EhsTVSurface_pixels(pSurface) = (EhsGraphicsColourClass*)EhsHMem_tempAlloc(sizeof(EhsGraphicsColourClass) * nWidth * nHeight);
            //} else {
            //	EhsTVSurface_pixels(pSurface) = (EhsGraphicsColourClass*)EhsHMem_writeableAlloc(sizeof(EhsGraphicsColourClass) * nWidth * nHeight);
            //}
            pSurface->pPalette = NULL;
            break;
        default:
            EHSH_LOG_ERROR(EHS_MSG_TGT_GRAPHICS_UNSUPPORTED_MODE("EhsTVSurface_create"));
            //if (bTemporary) {
            EhsHMem_tempFree(pSurface);
            //}
            pSurface = NULL;
        }
    }

    return pSurface;
}

