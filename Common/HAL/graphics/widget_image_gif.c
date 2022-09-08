/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/


/** @file widget_image_gif.c
 * Definitions for the image file class specifically for handling the GIF file type.
 *
 * @author: inx limited
 *
 */
/*
 *  Cross platform GIF source code.
 *
 *  Platform: Neutral
 *
 *  Version: 2.30  1997/07/07  Original version by Lachlan Patrick.
 *  Version: 2.35  1998/09/09  Minor upgrade to list functions.
 *  Version: 2.50  2000/01/01  Added the ability to load an animated gif.
 *  Version: 3.00  2001/03/03  Fixed a few bugs and updated the interface.
 *  Version: 3.34  2002/12/18  Debugging code is now better encapsulated.
 *  Version: 3.56  2005/08/09  Silenced a size_t conversion warning.
 *  Version: 3.60  2007/06/06  Fixed a memory leak in del_gif.
 */

/* Copyright (c) L. Patrick

   This file is part of the App cross-platform programming package.
   You may redistribute it and/or modify it under the terms of the
   App Software License. See the file LICENSE.TXT for details.
*/

/* LICENSE.TXT
App Software Licence
--------------------
This package includes software which is copyright (c) L. Patrick.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. You may not sell this software package.
4. You may include this software in a distribution of other software,
   and you may charge a nominal fee for the media used.
5. You may sell derivative programs, providing that such programs
   simply use this software in a compiled form.
6. You may sell derivative programs which use a compiled, modified
   version of this software, provided that you have attempted as
   best as you can to propagate all modifications made to the source
   code files of this software package back to the original author(s)
   of this package.

THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS AS IS, AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
*/
/*
 *  Gif.c - Cross-platform code for loading and saving GIFs
 *
 *  The LZW encoder and decoder used in this file were
 *  written by Gershon Elber and Eric S. Raymond as part of
 *  the GifLib package.
 *
 *  The remainder of the code was written by Lachlan Patrick
 *  as part of the GraphApp cross-platform graphics library.
 *
 *  GIF(sm) is a service mark property of CompuServe Inc.
 *  For better compression and more features than GIF,
 *  use PNG: the Portable Network Graphics format.
 */

/*
 *  Copyright and patent information:
 *
 *  Because the LZW algorithm has been patented by
 *  CompuServe Inc, you probably can't use this file
 *  in a commercial application without first paying
 *  CompuServe the appropriate licensing fee.
 *  Contact CompuServe for more information about that.
 */

/*
 *  Known problems with this code:
 *
 *  There is really only one thing to watch out for:
 *  on a PC running a 16-bit operating system, such
 *  as Windows 95 or Windows 3.1, there is a 64K limit
 *  to the size of memory blocks. This may limit the
 *  size of GIF files you can load, perhaps to less
 *  than 256 pixels x 256 pixels. The new row pointer
 *  technique used in this version of this file should
 *  remove that limitation, but you should test this
 *  on your system before believing me.
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 *  Gif structures:
 */
//#define EHSL_MODULE_ID (EHSH_LOG_MODULE_GRAPHICS)

#include "ehs_types.h"
#include "widget.h"
#include "hal_mem.h"
#include "messages.h"
#include "hal_string.h"
#include "hal_viewport.h"

typedef struct
{
    ehs_uint32      length;
    EhsGraphicsColourClass * colours;
} GifPalette;

typedef struct EhsWidgetImageGifScreenStruct
{
    int          width, height;
    int          has_cmap, color_res, sorted, cmap_depth;
    int          bgcolour, aspect;
    GifPalette * cmap;
} EhsWidgetImageGifScreenType;

typedef struct
{
    ehs_uint32             byte_count;
    unsigned char * bytes;
} GifData;

typedef struct
{
    int        marker;
    ehs_uint32        data_count;
    GifData ** data;
} GifExtension;

typedef struct
{
    ehs_uint16              left, top, width, height;
    int              has_cmap, interlace, sorted, reserved, cmap_depth;
    GifPalette *     cmap;
    unsigned char ** data;
} GifPicture;

typedef struct EhsWidgetImageGifBlockStruct
{
    int            intro;
    GifPicture *   pic;
    GifExtension * ext;
} EhsWidgetImageGifBlockType;


#include "target.h"

/*
 *  Gif internal definitions:
 */

#define LZ_MAX_CODE     4095    /* Largest 12 bit code */
#define LZ_BITS         12

#define FLUSH_OUTPUT    4096    /* Impossible code = flush */
#define FIRST_CODE      4097    /* Impossible code = first */
#define NO_SUCH_CODE    4098    /* Impossible code = empty */

#define HT_SIZE         8192    /* 13 bit hash table size */
#define HT_KEY_MASK     0x1FFF  /* 13 bit key mask */

#define IMAGE_LOADING   0       /* file_state = processing */
#define IMAGE_SAVING    0       /* file_state = processing */
#define IMAGE_COMPLETE  1       /* finished reading or writing */

typedef struct
{
    FILE *file;
    int depth,
        clear_code, eof_code,
        running_code, running_bits,
        max_code_plus_one,
        prev_code, current_code,
        stack_ptr,
        shift_state;
    unsigned long shift_data;
    unsigned long pixel_count;
    int           file_state, position, bufsize;
    unsigned char buf[256];
    unsigned long hash_table[HT_SIZE];
} GifEncoder;

typedef struct
{
    FILE *file;
    int depth,
        clear_code, eof_code,
        running_code, running_bits,
        max_code_plus_one,
        prev_code, current_code,
        stack_ptr,
        shift_state;
    unsigned long shift_data;
    unsigned long pixel_count;
    int           file_state, position, bufsize;
    unsigned char buf[256];
    unsigned char stack[LZ_MAX_CODE+1];
    unsigned char suffix[LZ_MAX_CODE+1];
    unsigned int  prefix[LZ_MAX_CODE+1];
} GifDecoder;

/**
 * Rewrite the GIF using EhsHMem_readonlyAlloc. Initial parsing
 * of the GIF (may have) required use of reallocs. This will lead to
 * "holes" in memory. Rewriting at this time allows the memory to be
 * re-compressed, eliminating holes.
 *
 * @return true if successful.
 */
EHS_LOCAL ehs_bool EhsWidgetImageGif_commit(EhsWidgetClass* pImage);

/**
 * Copy a GifPalette into "readonly" memory
 */
EHS_LOCAL GifPalette* EhsWidgetImageGif_commitPalette(GifPalette* pOldPalette);

/**
 * Copy a GifPicture into "readonly" memory
 */
EHS_LOCAL GifPicture* EhsWidgetImageGif_commitPicture(GifPicture* pOldPic);

/**
 * Copy a GifExtension into "readonly" memory
 */
EHS_LOCAL GifExtension* EhsWidgetImageGif_commitExtension(GifExtension* pOldExt);


void * gif_alloc(long bytes);
void *	app_zero_alloc(long size);
void *	app_alloc(long size);
void *	app_realloc(void *ptr, long newsize);


int 	read_gif_int(FILE *file);

GifData * new_gif_data(int size);
GifData * read_gif_data(FILE *file);

GifPalette * new_gif_palette(void);
ehs_bool	read_gif_palette(FILE *file, GifPalette *cmap);

EhsWidgetImageGifScreenType * new_gif_screen(void);
ehs_bool	read_gif_screen(FILE *file, EhsWidgetImageGifScreenType *screen);

GifExtension *new_gif_extension(void);
ehs_bool	read_gif_extension(FILE *file, GifExtension *ext);

GifDecoder * new_gif_decoder(void);
void	init_gif_decoder(FILE *file, GifDecoder *decoder);

int	read_gif_code(FILE *file, GifDecoder *decoder);
ehs_bool	read_gif_line(FILE *file, GifDecoder *decoder, unsigned char *line, int length);

GifEncoder * new_gif_encoder(void);
void	write_gif_code(FILE *file, GifEncoder *encoder, int code);
void	init_gif_encoder(FILE *file, GifEncoder *encoder, int depth);
void	flush_gif_encoder(FILE *file, GifEncoder *encoder);

GifPicture * new_gif_picture(void);
ehs_bool	read_gif_picture(FILE *file, GifPicture *pic);

EhsWidgetImageGifBlockType *new_gif_block(void);
ehs_bool	read_gif_block(FILE *file, EhsWidgetImageGifBlockType *block);

ehs_bool	read_gif(FILE *file, EhsWidgetImageGifSubclass *gif);
ehs_bool	read_one_gif_picture(FILE *file, EhsWidgetImageGifSubclass *gif);

EhsWidgetImageGifSubclass *	read_gif_file(const char *filename);

/**
 * Load an gif from a file
 *
 * @todo Need to release temporary memory, otherwise this represents a "leaky" function
 * memory is allocated, but not released. (the memory will be released when the app is restarted)
 * @param pImage image to load gif into
 * @param szFilename File to load image from
 * @return true if load was successful
 */
EHS_GLOBAL ehs_bool EhsWidgetImageGif_load(EhsWidgetClass* pImage, const ehs_char* szFilename)
{
    EhsWidgetImageGifSubclass *gif = &(EHS_IMAGE_GIF(pImage));
    FILE *file;
    ehs_bool bReturn = EHS_FALSE;
    ehs_bool bLoadImageFromAppDir;

    bLoadImageFromAppDir = EhsWidgetImage_loadFileFromAppDir(pImage);

    // try to open file
    if (bLoadImageFromAppDir)
    {
        file = Ehs_AppFopen(szFilename, "rb");
    }
    else
    {
        file = Ehs_UserFopen(szFilename, "rb");
    }

    if (file != NULL)
    {
        strcpy(gif->header, "GIF87a");
        gif->screen = new_gif_screen();
        if (gif->screen)
        {
            gif->blocks = NULL;
            gif->block_count = 0;
            if (read_gif(file, gif))
            {
                if (strncmp(gif->header, "GIF", 3) != 0)
                {
                    EhsHMem_tempFree(gif->header);
                    gif = NULL;
                }
                else
                {
                    bReturn = EhsWidgetImageGif_commit(pImage);
                    /** @todo here we assume that we always have a transparent bit in the gif */
                }
            }
            EhsFclose(file);
        } /* else fail */
    }
    pImage->MediaRect.nWidth = 0;
    pImage->MediaRect.nHeight = 0;
    if (!bReturn)
    {
        /* failed - reset gif structure */
        memset(gif, sizeof(EhsWidgetImageGifSubclass), 0);

        EhsError(EHS_MSG_ERROR_LOAD_GIF_FAILED(szFilename));
    }
    else     /* Read file OK */
    {
        /* Remember the the sizes in the top level generic structyre too. */
#ifdef GIFSARE_RENDERED_ON_THE_FLY
        pImage->MediaRect.nWidth =  EhsTVSurface_width(pImage->specificWidgetType.image.specificImageSrcType.gif XXX  .pSurface);
        pImage->MediaRect.nWidth =  EhsTVSurface_height(pImage->specificWidgetType.image.specificImageSrcType.gif XXX .pSurface);
#endif
#warning "to do gif apect ratio maintain"
        //@todo - this is done for all image types, needs to be done for GIFs as well
//		if (pImage->bMaintainAspectRatio) {
//			EhsWidget_resizeWidgetToMaintainAspectRatio(pImage, &xSrcRect);
//		} else {

        EhsWidget_resetWidgetSizeToDesignTime(pImage);
//		}
    }

    /* @todo release all memory allocated on a temporary basis */
    return (bReturn);
}

/**
 * Display the gif onto the viewport.
 *
 * @param pWidget Widget to display
 * @param pViewport Viewport to display it on
 */
void EhsWidgetImageGif_draw(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect)
{
    ehs_uint16 iBlock; /* used to iterate over gif blocks */
    ehs_uint16 nRow, nCol; /* used to iterate over coordinates within a gif block */
    GifPalette* pPalette; /* palette used to display the current image */
    EhsWidgetImageGifSubclass *pGif = &(EHS_IMAGE_GIF(pWidget));
    EhsTVSurfaceClass *pSurface;
    EhsGraphicsColourClass *pCurrent; /* points to the current item within the line */

    for (iBlock = 0; iBlock < pGif->block_count; iBlock++)
    {
        GifPicture* pPicture = pGif->blocks[iBlock]->pic;

        if (!pPicture)
        {
            continue;
        }
        else if (pPicture->interlace)
        {
            /** @todo provide support for interlaced images */
            /* Need to perform 4 passes on the images: */
            //for (iCount = 0; iCount < 4; iCount++)
            //{
            //	for (jCount = nRow + InterlacedOffset[iCount]; jCount < pPicture->Height; jCount += InterlacedJumps[iCount])
            //	{
            //		memcpy(&(PixMap[(jCount)*(pPicture->Width-1)]),ScreenBuffer,pPicture->Width);
            //	}
            //}
        }
        else
        {
            EhsGraphicsRectangleClass xSrc;
            EhsGraphicsRectangleClass xDstRect; /* defines the bounds of the rectangle on the target */
            /* I've taken the path of least resistance here - eventually, we need to implement
             * EHS_GRAPHICS_COLOUR_A8, which mean that we simply copy the points into the bitmap
             */
            pSurface = EhsTVSurface_create(pViewport,
                                           pPicture->width, pPicture->height, EHS_GRAPHICS_COLOUR_ARGB8888,
                                           NULL, 0);//, EHS_TRUE);

            /* use local or global colour map? */
            if (pPicture->has_cmap)
            {
                pPalette = pPicture->cmap;
            }
            else
            {
                pPalette = pGif->screen->cmap;
            }

            /* for each row of the image */
            pCurrent = EhsTVSurface_pixels(pSurface);
            for (nRow = 0; nRow < pPicture->height; nRow++)
            {
                /* fill a single line with colours */
                for (nCol = 0; nCol < pPicture->width; nCol++, pCurrent++)
                {
                    *pCurrent = pPalette->colours[pPicture->data[nRow][nCol]];
                }

            }
            /* @todo Check that pWidget->xRect nWidth and nHeight are set */
#ifdef NOTREFACTORINGMEDIASRCINFO
            xSrc.nLeft = 0;
            xSrc.nTop = 0;
            // source is always the size of the image NOT the widget
//			xSrc.nWidth = pWidget->xCurRect.nWidth;
//			xSrc.nHeight = pWidget->xCurRect.nHeight;
            xSrc.nWidth = EhsTVSurface_width(pSurface);
            xSrc.nHeight = EhsTVSurface_height(pSurface);
#endif
            // removed as widget is now sized to proportions of image if flag set for maintain aspect ratio
//			EhsGraphicsRectangle_proportionalScale(&xDstRect,&(pWidget->xCurRect),&xSrc);
//			EhsTV_blit_withlock(pViewport,pSurface, &xDstRect, &xSrc, EHS_WIDGET_IMAGE(pWidget).nCurrentImageAlpha);

            EhsTV_blit_withlock(pViewport, pSurface, &(pWidget->xCurRect), &(pWidget->MediaRect)/*&xSrc*/, EHS_WIDGET_IMAGE(pWidget).nCurrentImageAlpha);

            EhsTVSurface_destroy(pViewport,pSurface);
        }
    }
}

/**
 * Rewrite the GIF using EhsMemReadonly_alloc. Initial parsing
 * of the GIF (may have) required use of reallocs. This will lead to
 * "holes" in memory. Rewriting at this time allows the memory to be
 * re-compressed, eliminating holes.
 *
 * @return true if successful.
 */
ehs_bool EhsWidgetImageGif_commit(EhsWidgetClass* pImage)
{
    EhsWidgetImageGifSubclass *gif = &(EHS_IMAGE_GIF(pImage));
    EhsWidgetImageGifScreenType *pNewScreen = EhsHMem_readonlyAlloc(sizeof(EhsWidgetImageGifScreenType));
    EhsWidgetImageGifBlockType** ppNewBlocks = EhsHMem_readonlyAlloc(sizeof(EhsWidgetImageGifBlockType*)*gif->block_count);
    ehs_uint32 iBlock;
    ehs_bool bRet = EHS_TRUE;

    /** @todo Calculate whether we have enough memory to perform this operation
     * if not, skip before we start. This makes for better startup.
     */
    if (pNewScreen && ppNewBlocks)
    {
        /* copy everything (including pointers, which we will shortly update) */
        memcpy(pNewScreen,gif->screen,sizeof(EhsWidgetImageGifScreenType));
        /* rewrite allocated components */
        pNewScreen->cmap = EhsWidgetImageGif_commitPalette(gif->screen->cmap);

        /* get ready to find largest height/width for the current item */
        /* - we don't want to do this as it changes the widget size to fit the image
        		pImage->xCurRect.nWidth = 0;
        		pImage->xCurRect.nHeight = 0;
        */
        for (iBlock = 0; iBlock < gif->block_count; iBlock++)
        {
            ppNewBlocks[iBlock] = EhsHMem_readonlyAlloc(sizeof(EhsWidgetImageGifBlockType));
            if (ppNewBlocks[iBlock])
            {
                ppNewBlocks[iBlock]->intro = gif->blocks[iBlock]->intro;
                ppNewBlocks[iBlock]->pic = EhsWidgetImageGif_commitPicture(gif->blocks[iBlock]->pic);
                ppNewBlocks[iBlock]->ext = EhsWidgetImageGif_commitExtension(gif->blocks[iBlock]->ext);

                if (gif->blocks[iBlock]->pic)
                {
                    if (!(ppNewBlocks[iBlock]->pic))
                    {
                        bRet = EHS_FALSE;
                        break;
                    }
                    else
                    {
                        /* - we don't want to do this as it changes the widget size to fit the image
                        						if (ppNewBlocks[iBlock]->pic->height > pImage->xCurRect.nHeight)
                        						{
                        							pImage->xCurRect.nHeight = ppNewBlocks[iBlock]->pic->height;
                        						}

                        						if (ppNewBlocks[iBlock]->pic->width > pImage->xCurRect.nWidth)
                        						{
                        							pImage->xCurRect.nWidth = ppNewBlocks[iBlock]->pic->width;
                        						}
                        */
                    }
                }

                if (gif->blocks[iBlock]->ext)
                {
                    if (!ppNewBlocks[iBlock]->ext)
                    {
                        bRet = EHS_FALSE;
                        break;
                    }
                }

            }
            else
            {
                bRet = EHS_FALSE;
                break;
            }

        }
        gif->screen = pNewScreen;
        gif->blocks = ppNewBlocks;
    }
    else
    {
        bRet = EHS_FALSE;
    }

    return bRet;
}

/**
 * Copy a GifPalette into "readonly" memory
 */
GifPalette* EhsWidgetImageGif_commitPalette(GifPalette* pOldPalette)
{
    GifPalette* pNewPalette = NULL;

    if (pOldPalette)
    {
        pNewPalette = EhsHMem_readonlyAlloc(sizeof(GifPalette));

        if (pNewPalette)
        {
            pNewPalette->length = pOldPalette->length;
            pNewPalette->colours = EhsHMem_readonlyAlloc(sizeof(EhsGraphicsColourClass)*pNewPalette->length);
            if (pNewPalette->colours)
            {
                memcpy(pNewPalette->colours, pOldPalette->colours, sizeof(EhsGraphicsColourClass)*pNewPalette->length);
            }
            else
            {
                /** @todo this represents a memory leak that lasts until the next time a SODL file is loaded. */
                pNewPalette = NULL;
            }
        }
    }
    return pNewPalette;
}

/**
 * Copy a GifPicture into "readonly" memory
 */
GifPicture* EhsWidgetImageGif_commitPicture(GifPicture* pOldPic)
{
    ehs_uint32 iRow;
    GifPicture* pNewPic = NULL;

    if (pOldPic)
    {
        pNewPic = EhsHMem_readonlyAlloc(sizeof(GifPicture));

        if (pNewPic)
        {
            /* copy non-pointer attributes (actually easier to copy everything,
             * then update pointers */
            memcpy(pNewPic,pOldPic,sizeof(GifPicture));
            pNewPic->cmap = EhsWidgetImageGif_commitPalette(pOldPic->cmap);
            if (pNewPic->cmap)
            {
                pNewPic->data = EhsHMem_readonlyAlloc(pNewPic->height*sizeof(unsigned char*));
                if (pNewPic->data)
                {
                    for (iRow = 0; iRow < pNewPic->height; iRow++)
                    {
                        pNewPic->data[iRow] = EhsHMem_readonlyAlloc(pNewPic->width*sizeof(unsigned char));
                        if (pNewPic->data[iRow])
                        {
                            memcpy(pNewPic->data[iRow],pOldPic->data[iRow],pNewPic->width*sizeof(unsigned char));
                        }
                        else
                        {
                            pNewPic = NULL; /** @todo Fix this memory leak (which only occurs on gif load failed) */
                            break;
                        }
                    }
                }
                else
                {
                    pNewPic = NULL; /** @todo Fix this memory leak (which only occurs on gif load failed) */
                }
            }
            else
            {
                pNewPic = NULL; /** @todo Fix this memory leak (which only occurs on gif load failed) */
            }
        }
    }
    return pNewPic;
}

/**
 * Copy a GifExtension into "readonly" memory
 */
GifExtension* EhsWidgetImageGif_commitExtension(GifExtension* pOldExt)
{
    ehs_uint32 iData;
    GifExtension* pNewExt = NULL;

    if (pOldExt)
    {
        pNewExt = EhsHMem_readonlyAlloc(sizeof(GifExtension));

        if (pNewExt)
        {
            /* copy non-pointer attributes */
            pNewExt->marker = pOldExt->marker;
            pNewExt->data_count = pOldExt->data_count;

            pNewExt->data = EhsHMem_readonlyAlloc(pNewExt->data_count*sizeof(GifData*));
            if (pNewExt->data)
            {
                for (iData = 0; iData < pNewExt->data_count; iData++)
                {
                    pNewExt->data[iData] = EhsHMem_readonlyAlloc(sizeof(GifData));
                    if (pNewExt->data[iData])
                    {
                        pNewExt->data[iData]->bytes = EhsHMem_readonlyAlloc(pOldExt->data[iData]->byte_count*sizeof(unsigned char));
                        if (pNewExt->data[iData]->bytes)
                        {
                            pNewExt->data[iData]->byte_count = pOldExt->data[iData]->byte_count;
                            memcpy(pNewExt->data[iData]->bytes,pOldExt->data[iData]->bytes,pNewExt->data[iData]->byte_count*sizeof(unsigned char));
                        }
                        else
                        {
                            pNewExt = NULL;  /** @todo Fix this memory leak (which only occurs on gif load failed) */
                        }
                    }
                    else
                    {
                        pNewExt = NULL;  /** @todo Fix this memory leak (which only occurs on gif load failed) */
                    }
                }
            }
            else
            {
                pNewExt = NULL;  /** @todo Fix this memory leak (which only occurs on gif load failed) */
            }
        }
    }
    return pNewExt;
}

/*
 *  GIF memory allocation helper functions.
 */

void * gif_alloc(long bytes)
{
    return (void *)app_zero_alloc(bytes);
}

/*
 *  GIF file input/output functions.
 */

static unsigned char read_byte(FILE *file)
{
    int ch = EhsFgetc(file);
    if (ch == EOF)
        ch = 0;
    return ch;
}


static int read_stream(FILE *file, unsigned char buffer[], int length)
{
    int count = (int) EhsFread(buffer, 1, length, file);
    int i = count;
    while (i < length)
        buffer[i++] = '\0';
    return count;
}


int read_gif_int(FILE *file)
{
    int output;
    unsigned char buf[2];

    if (EhsFread(buf, 1, 2, file) != 2)
        return 0;
    output = (((unsigned int) buf[1]) << 8) | buf[0];
    return output;
}


/*
 *  Gif data blocks:
 */

GifData * new_gif_data(int size)
{
    GifData *data = gif_alloc(sizeof(GifData));
    if (data)
    {
        data->byte_count = size;
        data->bytes = app_zero_alloc(size * sizeof(unsigned char));
        if (!data->bytes)
        {
            EhsHMem_tempFree(data->bytes);
            data = NULL;
        }
    }
    return data;
}

/*
 *  Read one code block from the Gif file.
 *  This routine should be called until NULL is returned.
 */
GifData * read_gif_data(FILE *file)
{
    GifData *data;
    int size;

    size = read_byte(file);

    if (size > 0)
    {
        data = new_gif_data(size);
        read_stream(file, data->bytes, size);
    }
    else
    {
        data = NULL;
    }
    return data;
}


/*
 *  Read the next byte from a Gif file.
 *
 *  This function is aware of the block-nature of Gif files,
 *  and will automatically skip to the next block to find
 *  a new byte to read, or return 0 if there is no next block.
 */
static unsigned char read_gif_byte(FILE *file, GifDecoder *decoder)
{
    unsigned char *buf = decoder->buf;
    unsigned char next;

    if (decoder->file_state == IMAGE_COMPLETE)
        return '\0';

    if (decoder->position == decoder->bufsize)
    {
        /* internal buffer now empty! */
        /* read the block size */
        decoder->bufsize = read_byte(file);
        if (decoder->bufsize == 0)
        {
            decoder->file_state = IMAGE_COMPLETE;
            return '\0';
        }
        read_stream(file, buf, decoder->bufsize);
        next = buf[0];
        decoder->position = 1;	/* where to get chars */
    }
    else
    {
        next = buf[decoder->position++];
    }

    return next;
}

/*
 *  Read to end of an image, including the zero block.
 */
static void finish_gif_picture(FILE *file, GifDecoder *decoder)
{
    unsigned char *buf = decoder->buf;

    while (decoder->bufsize != 0)
    {
        decoder->bufsize = read_byte(file);
        if (decoder->bufsize == 0)
        {
            decoder->file_state = IMAGE_COMPLETE;
            break;
        }
        read_stream(file, buf, decoder->bufsize);
    }
}

/*
 *  Colour maps:
 */

GifPalette * new_gif_palette(void)
{
    return gif_alloc(sizeof(GifPalette));
}

ehs_bool read_gif_palette(FILE *file, GifPalette *cmap)
{
    ehs_uint32 i;
    unsigned char r, g, b;
    ehs_bool bRet = EHS_FALSE;

    cmap->colours = app_alloc(cmap->length * sizeof(EhsGraphicsColourClass));
    if (cmap->colours)
    {
        for (i=0; i<cmap->length; i++)
        {
            r = read_byte(file);
            g = read_byte(file);
            b = read_byte(file);
            cmap->colours[i] = EhsGraphicsColour_rgb(r,g,b);
        }
        bRet = EHS_TRUE;
    }

    return bRet;
}

/*
 *  EhsWidgetImageGifScreenType:
 */

EhsWidgetImageGifScreenType * new_gif_screen(void)
{
    EhsWidgetImageGifScreenType *screen = gif_alloc(sizeof(EhsWidgetImageGifScreenType));
    if (screen)
    {
        screen->cmap = new_gif_palette();
        if (!screen->cmap)
        {
            EhsHMem_tempFree(screen->cmap);
            screen = NULL;
        }
    }
    return screen;
}

ehs_bool read_gif_screen(FILE *file, EhsWidgetImageGifScreenType *screen)
{
    unsigned char info;
    ehs_bool bRet;

    screen->width       = read_gif_int(file);
    screen->height      = read_gif_int(file);

    info                = read_byte(file);
    screen->has_cmap    =  (info & 0x80) >> 7;
    screen->color_res   = ((info & 0x70) >> 4) + 1;
    screen->sorted      =  (info & 0x08) >> 3;
    screen->cmap_depth  =  (info & 0x07)       + 1;

    screen->bgcolour    = read_byte(file);
    screen->aspect      = read_byte(file);

    if (screen->has_cmap)
    {
        screen->cmap->length = 1 << screen->cmap_depth;
        bRet = read_gif_palette(file, screen->cmap);
        /* set the background colour to alpha */
        screen->cmap->colours[screen->bgcolour].sComp.nAlpha = 0x0;
    }
    else
    {
        bRet = EHS_TRUE;
    }

    return bRet;
}

/*
 *  GifExtension:
 */

GifExtension *new_gif_extension(void)
{
    return gif_alloc(sizeof(GifExtension));
}

ehs_bool read_gif_extension(FILE *file, GifExtension *ext)
{
    GifData *data;
    ehs_bool bRet = EHS_TRUE;
    int i;
    ext->marker = read_byte(file);

    data = read_gif_data(file);
    while (data)
    {
        /* Append the data object: */
        i = ++ext->data_count;
        ext->data = app_realloc(ext->data, i * sizeof(GifData *));
        if (!ext->data)
        {
            bRet = EHS_FALSE;
            break;
        }
        ext->data[i-1] = data;
        data = read_gif_data(file);
    }
    return bRet;
}


/*
 *  GifDecoder:
 */

GifDecoder * new_gif_decoder(void)
{
    return gif_alloc(sizeof(GifDecoder));
}

void init_gif_decoder(FILE *file, GifDecoder *decoder)
{
    int i, depth;
    int lzw_min;
    unsigned int *prefix;

    lzw_min = read_byte(file);
    depth = lzw_min;

    decoder->file_state   = IMAGE_LOADING;
    decoder->position     = 0;
    decoder->bufsize      = 0;
    decoder->buf[0]       = 0;
    decoder->depth        = depth;
    decoder->clear_code   = (1 << depth);
    decoder->eof_code     = decoder->clear_code + 1;
    decoder->running_code = decoder->eof_code + 1;
    decoder->running_bits = depth + 1;
    decoder->max_code_plus_one = 1 << decoder->running_bits;
    decoder->stack_ptr    = 0;
    decoder->prev_code    = NO_SUCH_CODE;
    decoder->shift_state  = 0;
    decoder->shift_data   = 0;

    prefix = decoder->prefix;
    for (i = 0; i <= LZ_MAX_CODE; i++)
        prefix[i] = NO_SUCH_CODE;
}

/*
 *  Read the next Gif code word from the file.
 *
 *  This function looks in the decoder to find out how many
 *  bits to read, and uses a buffer in the decoder to remember
 *  bits from the last byte input.
 */
int read_gif_code(FILE *file, GifDecoder *decoder)
{
    int code;
    unsigned char next_byte;
    static int code_masks[] =
    {
        0x0000, 0x0001, 0x0003, 0x0007,
        0x000f, 0x001f, 0x003f, 0x007f,
        0x00ff, 0x01ff, 0x03ff, 0x07ff,
        0x0fff
    };

    while (decoder->shift_state < decoder->running_bits)
    {
        /* Need more bytes from input file for next code: */
        next_byte = read_gif_byte(file, decoder);
        decoder->shift_data |=
            ((unsigned long) next_byte) << decoder->shift_state;
        decoder->shift_state += 8;
    }

    code = decoder->shift_data & code_masks[decoder->running_bits];

    decoder->shift_data >>= decoder->running_bits;
    decoder->shift_state -= decoder->running_bits;

    /* If code cannot fit into running_bits bits,
     * we must raise its size.
     * Note: codes above 4095 are used for signalling. */
    if (++decoder->running_code > decoder->max_code_plus_one
            && decoder->running_bits < LZ_BITS)
    {
        decoder->max_code_plus_one <<= 1;
        decoder->running_bits++;
    }
    return code;
}

/*
 *  Routine to trace the prefix-linked-list until we get
 *  a prefix which is a pixel value (less than clear_code).
 *  Returns that pixel value.
 *
 *  If the picture is defective, we might loop here forever,
 *  so we limit the loops to the maximum possible if the
 *  picture is okay, i.e. LZ_MAX_CODE times.
 */
static int trace_prefix(unsigned int *prefix, int code, int clear_code)
{
    int i = 0;

    while (code > clear_code && i++ <= LZ_MAX_CODE)
        code = prefix[code];
    return code;
}

/*
 *  The LZ decompression routine:
 *  Call this function once per scanline to fill in a picture.
 */
ehs_bool read_gif_line(FILE *file, GifDecoder *decoder,
                       unsigned char *line, int length)
{
    ehs_bool bRet = EHS_TRUE;
    int i = 0, j;
    int current_code, eof_code, clear_code;
    int current_prefix, prev_code, stack_ptr;
    unsigned char *stack, *suffix;
    unsigned int *prefix;

    prefix	= decoder->prefix;
    suffix	= decoder->suffix;
    stack	= decoder->stack;
    stack_ptr	= decoder->stack_ptr;
    eof_code	= decoder->eof_code;
    clear_code	= decoder->clear_code;
    prev_code	= decoder->prev_code;

    if (stack_ptr != 0)
    {
        /* Pop the stack */
        while (stack_ptr != 0 && i < length)
            line[i++] = stack[--stack_ptr];
    }

    while (i < length)
    {
        current_code = read_gif_code(file, decoder);

        if (current_code == eof_code)
        {
            /* unexpected EOF */
            if (i != length - 1 || decoder->pixel_count != 0)
            {
                bRet = EHS_FALSE;
                break;
            }
            i++;
        }
        else if (current_code == clear_code)
        {
            /* reset prefix table etc */
            for (j = 0; j <= LZ_MAX_CODE; j++)
                prefix[j] = NO_SUCH_CODE;
            decoder->running_code = decoder->eof_code + 1;
            decoder->running_bits = decoder->depth + 1;
            decoder->max_code_plus_one = 1 << decoder->running_bits;
            prev_code = decoder->prev_code = NO_SUCH_CODE;
        }
        else
        {
            /* Regular code - if in pixel range
             * simply add it to output pixel stream,
             * otherwise trace code-linked-list until
             * the prefix is in pixel range. */
            if (current_code < clear_code)
            {
                /* Simple case. */
                line[i++] = current_code;
            }
            else
            {
                /* This code needs to be traced:
                 * trace the linked list until the prefix is a
                 * pixel, while pushing the suffix pixels on
                 * to the stack. If finished, pop the stack
                 * to output the pixel values. */
                if ((current_code < 0) || (current_code > LZ_MAX_CODE))
                {
                    bRet = EHS_FALSE;
                    break; /* image defect */
                }
                if (prefix[current_code] == NO_SUCH_CODE)
                {
                    /* Only allowed if current_code is exactly
                     * the running code:
                     * In that case current_code = XXXCode,
                     * current_code or the prefix code is the
                     * last code and the suffix char is
                     * exactly the prefix of last code! */
                    if (current_code == decoder->running_code - 2)
                    {
                        current_prefix = prev_code;
                        suffix[decoder->running_code - 2]
                            = stack[stack_ptr++]
                              = trace_prefix(prefix, prev_code, clear_code);
                    }
                    else
                    {
                        bRet = EHS_FALSE;
                        break; /* image defect */
                    }
                }
                else
                    current_prefix = current_code;

                /* Now (if picture is okay) we should get
                 * no NO_SUCH_CODE during the trace.
                 * As we might loop forever (if picture defect)
                 * we count the number of loops we trace and
                 * stop if we get LZ_MAX_CODE.
                 * Obviously we cannot loop more than that. */
                j = 0;
                while (j++ <= LZ_MAX_CODE
                        && current_prefix > clear_code
                        && current_prefix <= LZ_MAX_CODE)
                {
                    stack[stack_ptr++] = suffix[current_prefix];
                    current_prefix = prefix[current_prefix];
                }
                if (j >= LZ_MAX_CODE || current_prefix > LZ_MAX_CODE)
                {
                    bRet = EHS_FALSE;
                    break; /* image defect */
                }

                /* Push the last character on stack: */
                stack[stack_ptr++] = current_prefix;

                /* Now pop the entire stack into output: */
                while (stack_ptr != 0 && i < length)
                    line[i++] = stack[--stack_ptr];
            }
            if (prev_code != NO_SUCH_CODE)
            {
                if ((decoder->running_code < 2) ||
                        (decoder->running_code > LZ_MAX_CODE+2))
                    break; /* image defect - but not apparently one that causes problems - don't need to set bRet to false */
                prefix[decoder->running_code - 2] = prev_code;

                if (current_code == decoder->running_code - 2)
                {
                    /* Only allowed if current_code is exactly
                     * the running code:
                     * In that case current_code = XXXCode,
                     * current_code or the prefix code is the
                     * last code and the suffix char is
                     * exactly the prefix of the last code! */
                    suffix[decoder->running_code - 2]
                        = trace_prefix(prefix, prev_code, clear_code);
                }
                else
                {
                    suffix[decoder->running_code - 2]
                        = trace_prefix(prefix, current_code, clear_code);
                }
            }
            prev_code = current_code;
        }
    }

    decoder->prev_code = prev_code;
    decoder->stack_ptr = stack_ptr;

    return bRet;
}

/*
 *  Hash table:
 */

/*
 *  The 32 bits contain two parts: the key & code:
 *  The code is 12 bits since the algorithm is limited to 12 bits
 *  The key is a 12 bit prefix code + 8 bit new char = 20 bits.
 */
#define HT_GET_KEY(x)	((x) >> 12)
#define HT_GET_CODE(x)	((x) & 0x0FFF)
#define HT_PUT_KEY(x)	((x) << 12)
#define HT_PUT_CODE(x)	((x) & 0x0FFF)

/*
 *  Generate a hash key from the given unique key.
 *  The given key is assumed to be 20 bits as follows:
 *    lower 8 bits are the new postfix character,
 *    the upper 12 bits are the prefix code.
 */
static int gif_hash_key(unsigned long key)
{
    return ((key >> 12) ^ key) & HT_KEY_MASK;
}

/*
 *  GifPicture:
 */

GifPicture * new_gif_picture(void)
{
    GifPicture *pic = gif_alloc(sizeof(GifPicture));
    if (pic)
    {
        pic->cmap = new_gif_palette();
        pic->data = NULL;
    }
    return pic;
}

static ehs_bool read_gif_picture_data(FILE *file, GifPicture *pic)
{
    ehs_bool bRet = EHS_TRUE;
    GifDecoder *decoder;
    long w, h;
    int interlace_start[] = {0, 4, 2, 1};
    int interlace_step[]  = {8, 8, 4, 2};
    int scan_pass, row;

    w = pic->width;
    h = pic->height;
    pic->data = app_alloc(h * sizeof(unsigned char *));
    if (pic->data)
    {
        for (row=0; row < h; row++)
        {
            pic->data[row] = app_zero_alloc(w * sizeof(unsigned char));
            if (!pic->data[row])
            {
                bRet = EHS_FALSE;
                pic->data = NULL;
                break;
            }
        }
    }

    if (pic->data)
    {
        decoder = new_gif_decoder();
        init_gif_decoder(file, decoder);

        if (pic->interlace)
        {
            for (scan_pass = 0; scan_pass < 4; scan_pass++)
            {
                row = interlace_start[scan_pass];
                while (row < h)
                {
                    bRet = bRet && read_gif_line(file, decoder, pic->data[row], w);
                    row += interlace_step[scan_pass];
                }
            }
        }
        else
        {
            row = 0;
            while (row < h)
            {
                bRet = bRet && read_gif_line(file, decoder, pic->data[row], w);
                row += 1;
            }
        }
        finish_gif_picture(file, decoder);

    }
    else
        bRet = EHS_FALSE;

    return bRet;
}

ehs_bool read_gif_picture(FILE *file, GifPicture *pic)
{
    unsigned char info;
    ehs_bool bRet = EHS_TRUE;

    pic->left   = read_gif_int(file);
    pic->top    = read_gif_int(file);
    pic->width  = read_gif_int(file);
    pic->height = read_gif_int(file);

    info = read_byte(file);
    pic->has_cmap    = (info & 0x80) >> 7;
    pic->interlace   = (info & 0x40) >> 6;
    pic->sorted      = (info & 0x20) >> 5;
    pic->reserved    = (info & 0x18) >> 3;

    if (pic->has_cmap)
    {
        pic->cmap_depth  = (info & 0x07) + 1;
        pic->cmap->length = 1 << pic->cmap_depth;
        bRet = bRet && read_gif_palette(file, pic->cmap);
    }

    bRet = bRet && read_gif_picture_data(file, pic);

    return bRet;
}


/*
 *  EhsWidgetImageGifBlockType:
 */

EhsWidgetImageGifBlockType *new_gif_block(void)
{
    return gif_alloc(sizeof(EhsWidgetImageGifBlockType));
}

ehs_bool read_gif_block(FILE *file, EhsWidgetImageGifBlockType *block)
{
    ehs_bool bRet = EHS_TRUE;

    block->intro = read_byte(file);
    if (block->intro == 0x2C)
    {
        block->pic = new_gif_picture();
        bRet = bRet && read_gif_picture(file, block->pic);
    }
    else if (block->intro == 0x21)
    {
        block->ext = new_gif_extension();
        bRet = bRet && read_gif_extension(file, block->ext);
    }
    return bRet;
}

/*
 *  Gif:
 */

ehs_bool read_gif(FILE *file, EhsWidgetImageGifSubclass *gif)
{
    ehs_bool bRet = EHS_TRUE;
    int i;
    EhsWidgetImageGifBlockType *block;
    for (i=0; i<6; i++)
        gif->header[i] = read_byte(file);
    if (strncmp(gif->header, "GIF", 3) != 0)
        bRet = EHS_FALSE; /* error */

    bRet = read_gif_screen(file, gif->screen);
    while (bRet)
    {
        block = new_gif_block();
        if (block)
        {
            bRet = bRet && read_gif_block(file, block);

            if (block->intro == 0x3B)  	/* terminator */
            {
                EhsHMem_tempFree(block);
                break;
            }
            else  if (block->intro == 0x2C)  	/* image */
            {
                /* Append the block: */
                i = ++gif->block_count;
                gif->blocks = app_realloc(gif->blocks, i * sizeof(EhsWidgetImageGifBlockType *));
                if (gif->blocks)
                    gif->blocks[i-1] = block;
                else
                    bRet = EHS_FALSE;
            }
            else  if (block->intro == 0x21)  	/* extension */
            {
                /* Append the block: */
                i = ++gif->block_count;
                gif->blocks = app_realloc(gif->blocks, i * sizeof(EhsWidgetImageGifBlockType *));
                if (gif->blocks)
                    gif->blocks[i-1] = block;
                else
                    bRet = EHS_FALSE;
            }
            else  	/* error */
            {
                EhsHMem_tempFree(block);
                break;
            }
        }
        else
        {
            bRet = EHS_FALSE;
        }
    }
    return bRet;
}

ehs_bool read_one_gif_picture(FILE *file, EhsWidgetImageGifSubclass *gif)
{
    ehs_bool bRet = EHS_TRUE;
    int i;
    EhsWidgetImageGifBlockType *block;

    for (i=0; i<6; i++)
        gif->header[i] = read_byte(file);
    if (strncmp(gif->header, "GIF", 3) != 0)
        bRet = EHS_FALSE; /* error */
    else
        bRet = read_gif_screen(file, gif->screen);

    while (bRet)
    {
        block = new_gif_block();
        bRet = read_gif_block(file, block);
        if (!bRet)
            break;

        if (block->intro == 0x3B)  	/* terminator */
        {
            EhsHMem_tempFree(block);
            break;
        }
        else if (block->intro == 0x2C)   /* image */
        {
            /* Append the block: */
            i = ++gif->block_count;
            gif->blocks = app_realloc(gif->blocks, i * sizeof(EhsWidgetImageGifBlockType *));
            if (gif->blocks)
                gif->blocks[i-1] = block;
            else
                bRet = EHS_FALSE;
            break;
        }
        else if (block->intro == 0x21)   /* extension */
        {
            /* Append the block: */
            i = ++gif->block_count;
            gif->blocks = app_realloc(gif->blocks, i * sizeof(EhsWidgetImageGifBlockType *));
            if (gif->blocks)
                gif->blocks[i-1] = block;
            else
                bRet = EHS_FALSE;
            continue;
        }
        else  	/* error! */
        {
            EhsHMem_tempFree(block);
            break;
        }
    }
    return bRet;
}


void *	app_zero_alloc(long size)
{
    void *ret = EhsHMem_tempAlloc(size);

    if (ret)
    {
        memset(ret,0,size);
    }

    return ret;
}

void *	app_alloc(long size)
{
    return EhsHMem_tempAlloc(size);
}

/**
 * Reallocate memory - in order to fit in with the EHS approach, we actually
 * just allocate more memory, and delete all allocated memory at a later stage
 */
void *	app_realloc(void *ptr, long newsize)
{
    void* pMem = app_alloc(newsize);

    if (ptr)
    {
        memcpy(pMem,ptr,newsize);
    }
    else
    {
        memset(pMem,0,newsize);
    }
    return pMem;
}
