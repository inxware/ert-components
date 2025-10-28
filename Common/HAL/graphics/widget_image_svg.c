/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/


/* @file widget_image_svg.c
 *
 * @author: inx limited
 *
 */
//#define EHSL_MODULE_ID (EHSH_LOG_MODULE_GRAPHICS)

#include "globals.h"
#include "widget.h"

#include "graphics.h"
#include "messages.h"
#include "hal_string.h"
#include "hal_viewport.h"

/*Includes for BFPlayer*/
#include "svg/BFBasicTypes.h"
#include "svg/BFSVGT.h"

/*These need passing in from an external source*/
#define EHS_SVG_WIDTH 640
#define EHS_SVG_HEIGHT 480
/**
 * Load an PNG from a file into the EhsWidgetImagePngSubclass structure
 *
 * @param pImage image to load png into
 * @param szFilename File to load image from
 * @return true if load was successful
 */

EHS_GLOBAL ehs_bool EhsWidgetImageSvg_load(EhsWidgetClass* pImage, const ehs_char* szFilename)
{
    ehs_FILE *file; /*The SVG file we will be loading.*/
    ehs_bool bReturn = EHS_FALSE; /*Return this to indicate success or not*/
    char* result; /*Contents of SVG file after reading*/
    unsigned char* renderedPixels; /*Pointer to pixels indexed by colour sample.*/
    int i;
    int size = 0; /*Size of SVG file*/

    EHS_IMAGE_SVG(pImage).pSurface = EhsTVSurface_create(&EhsTV,EHS_SVG_WIDTH,EHS_SVG_HEIGHT,EHS_GRAPHICS_COLOUR_ARGB8888, NULL, 0);//, EHS_TRUE); /*The image surface*/
    /*Load file into memory*/
    file = Ehs_AppFopen(szFilename, "rb");
    if(file != NULL)
    {
        EhsGraphicsColourClass* pPixels; /*Pointer to pixels*/
        EhsFseek(file, 0, SEEK_END);
        size = EhsFtell(file);
        EhsFseek(file, 0, SEEK_SET);
        result = (char *)malloc(size+1);
        if (size != EhsFread(result, sizeof(char), size, file))
        {
            free(result);
            bReturn = EHS_FALSE; // -2 means file reading fail
        }
        EhsFclose(file);
//		(result)[size] = 0;

        /*Initialise BFPlayer*/
        BFDocumentRedrawStatus redrawStatus; /*Status of redraw*/
        BFStatus status; /*records status of player after a command*/
        BFPlayer * pPlayer; /*Declare as pointer here to solve initialiser error PD 03/07/2008 09:39*/
        BFPlayerAdvanceTime m_advanceTime; /*Used for advancing times in SVG animations*/
        BFPlayerRegion m_ClippingRegion; /*Needed for redrawing SVG*/
        BF_UINT32 *m_pBuffer; /*Pixel buffer that BFPlayer will render into*/

        BFNewDocument newDoc;
        BF_UINT8 *pContent; /*Memory for loading file into, this must be generated dynamically from file size at some point*/
        BFPreferences preferences;
        BFHeapContext* pMemCtx;
        BF_INT_VOID pUserData;
        BFColorMapping mapping[] =
        {
            {BF_SYSTEM_COLOR_ActiveBorder,        0x008000},
            {BF_SYSTEM_COLOR_ActiveCaption,        0x0000FF},
            {BF_SYSTEM_COLOR_AppWorkspace,         0xFFFFFF},
            {BF_SYSTEM_COLOR_Background,           0xC0C0C0},
            {BF_SYSTEM_COLOR_ButtonFace,           0x800000},
            {BF_SYSTEM_COLOR_ButtonHighlight,      0xFF0000},
            {BF_SYSTEM_COLOR_ButtonShadow,         0x800080},
            {BF_SYSTEM_COLOR_ButtonText,           0xFF00FF},
            {BF_SYSTEM_COLOR_CaptionText,          0x00FF00},
            {BF_SYSTEM_COLOR_GrayText,             0x808000},
            {BF_SYSTEM_COLOR_Highlight,            0xFFFF00},
            {BF_SYSTEM_COLOR_HighlightText,        0x000080},
            {BF_SYSTEM_COLOR_InactiveBorder,       0x008080},
            {BF_SYSTEM_COLOR_InactiveCaption,      0x00FFFF},
            {BF_SYSTEM_COLOR_InactiveCaptionText,  0x808080},
            {BF_SYSTEM_COLOR_InfoBackground,       0x008000},
            {BF_SYSTEM_COLOR_InfoText,             0x0000FF},
            {BF_SYSTEM_COLOR_Menu,                 0xFFFFFF},
            {BF_SYSTEM_COLOR_MenuText,             0xC0C0C0},
            {BF_SYSTEM_COLOR_Scrollbar,            0x800000},
            {BF_SYSTEM_COLOR_ThreeDDarkShadow,     0xFF0000},
            {BF_SYSTEM_COLOR_ThreeDFace,           0x800080},
            {BF_SYSTEM_COLOR_ThreeDHighlight,      0xFF00FF},
            {BF_SYSTEM_COLOR_ThreeDLightShadow,    0x00FF00},
            {BF_SYSTEM_COLOR_ThreeDShadow,         0x808000},
            {BF_SYSTEM_COLOR_Window,               0xFFFF00},
            {BF_SYSTEM_COLOR_WindowFrame,          0x000080},
            {BF_SYSTEM_COLOR_WindowText,           0x008080},
        };
        BFColorMappings mappings ;
        BFFrameBuffer frameBuffer;
        BFDocumentVerify verifyDoc;
        BFCacheControl cacheControl;
        BF_BOOL bMoved;
        BFCurrentFocus m_curFocus;

        /*Initialise memory*/
        void *pMemory = malloc(EHS_SVG_MEMORY_INIT_SIZE);
        pMemCtx = BFmemCreateContext( pMemory, EHS_SVG_MEMORY_INIT_SIZE);
        pUserData.p = 0; /*This is important. Remove this and it all stops working.*/

        /*Instantiate player*/
        pPlayer = BFcreatePlayer(0, pMemCtx, NULL);

        /*Create player preferences*/
        memset(&preferences, 0, sizeof(preferences));
        preferences.szLanguage = "en";
        BFprocessCommand( pPlayer, BF_COMMAND_PREFERENCES, &preferences);

        /*Set colour mappings*/
        mappings.pColorMapping = mapping;
        mappings.uLength = (sizeof (mapping) / sizeof (mapping[0]));
        BFprocessCommand(pPlayer, BF_COMMAND_DOCUMENT_SET_SYSTEM_COLOR, &mappings );

        /*Create framebuffer for rendering*/
        memset( &frameBuffer, 0, sizeof( frameBuffer ) ); /*not sure about this line, copied from windows example app*/
        m_pBuffer = malloc(EHS_SVG_HEIGHT*EHS_SVG_WIDTH*4); /*Allocate memory for view buffer. This needs calculating properly from size of requested widget*/
        frameBuffer.uWidth = EHS_SVG_WIDTH; /*These need to be generated from size of screen interface*/
        frameBuffer.uHeight = EHS_SVG_HEIGHT;
        frameBuffer.pBuffer = m_pBuffer;
        frameBuffer.uDpi = 72;
        status = BFprocessCommand( pPlayer, BF_COMMAND_VIEW_BUFFER, &frameBuffer );
        if(status != BF_NO_ERROR)
        {
            bReturn = EHS_FALSE;
        }

        pContent = (BF_UINT8*)result;

        /*Pass document to BFPlayer for parsing*/
        newDoc.uBufferSize = size;
        newDoc.szFileName = szFilename;
        newDoc.pBuffer = pContent;
        newDoc.pViewData = 0x00000; /*Possible bug fix, copied from sample app 08/07/2008 PB*/
        status = BFprocessCommand( pPlayer, BF_COMMAND_DOCUMENT_NEW, &newDoc);
        if(status != BF_NO_ERROR)
        {
            bReturn = EHS_FALSE;
        }

        /*These are set after parsing in sample app so have done the same here. 08/07/2008 PB*/
        newDoc.uMimeType = BF_SVG_MOBILE;
        newDoc.bTextOnlyAllowed = FALSE;
        newDoc.bZoomPanAllowed = TRUE;

        /*Verify document*/
        status = BFprocessCommand( pPlayer, BF_COMMAND_DOCUMENT_VERIFY, &verifyDoc );
        if(status != BF_NO_ERROR)
        {
            bReturn = EHS_FALSE;
        }

        /*Set cache control parameters*/
        memset(&cacheControl, 0, sizeof(cacheControl));
        cacheControl.bDisable = BF_FALSE;
        cacheControl.bDisableImplicit = BF_FALSE;
        cacheControl.uMaxCaches = 50;
        cacheControl.uMaxBytes = 2048 * 1024;
        BFprocessCommand(pPlayer, BF_COMMAND_CACHE_CONTROL, &cacheControl );

        /*Locate focus*/
        status = BFprocessCommand( pPlayer, BF_COMMAND_FOCUS_LOCATE, (void*)&bMoved );

        /*Create clipping region. BFPlayer needs this in order to render document. Even though it says in the docs it doesn't. 08/07/2008 PB*/
        /*These need to be calculated from some sort of passed parameter, not sure what yet though*/
        m_ClippingRegion.x1 = 0;
        m_ClippingRegion.x2 = EHS_SVG_WIDTH - 1;
        m_ClippingRegion.y1 = 0;
        m_ClippingRegion.y2 = EHS_SVG_HEIGHT - 1;
        status = BFprocessCommand( pPlayer, BF_COMMAND_VIEW_REDRAW, &m_ClippingRegion);

        if(status != BF_NO_ERROR)
        {
            bReturn = EHS_FALSE;
        }
        else
        {
            bReturn = EHS_TRUE;
        }

        /*Get current focus*/
        BFprocessCommand( pPlayer, BF_COMMAND_DOM_GET_FOCUS, &m_curFocus);

        /*Get current focus*/
        BFprocessCommand( pPlayer, BF_COMMAND_DOM_GET_FOCUS, &m_curFocus);

        /*Set pixels properly*/
        pPixels = EhsTVSurface_pixels(EHS_IMAGE_SVG(pImage).pSurface);

        /* get color values */
        renderedPixels = (unsigned char*)m_pBuffer;
        for( i = 0u; i < EHS_SVG_HEIGHT*EHS_SVG_WIDTH; i++)
        {
            pPixels[i].sComp.nAlpha	= renderedPixels[(i*4)+3];
            pPixels[i].sComp.nRed	= renderedPixels[(i*4)+2];
            pPixels[i].sComp.nGreen = renderedPixels[(i*4)+1];
            pPixels[i].sComp.nBlue	= renderedPixels[(i*4)];
        }
    }
    return (bReturn);
}

/**
 * Display the svg onto the viewport.
 * Note this is a generic 2-D bgra8888 blitter and should really be in the generic graphics blitter code module
 *
 * @param pWidget Widget to display
 * @param pViewport Viewport to display it on
 * @param pClipRect Specifies the bounds for drawing the widget
 */

void EhsWidgetImageSvg_draw(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect)
{
    /* convert the image data into an array of EhsGraphicsColourClass, which are 32bit ints containing
     * 8 bits each of alpha, red, green and blue. Alpha 0xff is opaque, 0x00 is transparent.
     * The image could be converted once by the load routine, or every time "draw" takes place.
     * Convert once likely results in a heavier use of memory. Convert on draw is more CPU intensive.
     * To minimise memory use, it's also possible to convert a line at a time - see example code below
     */

    /* note: we use pWidget->xCurRect for destination and use its  width/height for the source.
     * This assumes that PNGs are always the same size as when defined in LGB */
    EhsGraphicsRectangleClass xSrcRect = NULL; /* defines the bounds of the source image */
    EhsGraphicsRectangleClass xDstRect = NULL; /* defines the bounds of the rectangle on the target */
    //xSrcRect.nWidth = pWidget->xCurRect.nWidth;
    //xSrcRect.nHeight = pWidget->xCurRect.nHeight;
#ifdef NOTREFACORINGMEDIASRCINFO
    xSrcRect.nLeft = 0;
    xSrcRect.nTop = 0;
    xSrcRect.nWidth = EhsTVSurface_width(EHS_IMAGE_SVG(pWidget).pSurface);
    xSrcRect.nHeight = EhsTVSurface_height(EHS_IMAGE_SVG(pWidget).pSurface);
#endif
    EhsGraphicsRectangle_proportionalScale(&xDstRect,&(pWidget->xCurRect),&(pWidget->MediaRect));

    EhsTV_blit_withlock(pViewport, (EHS_IMAGE_SVG(pWidget).pSurface), &xDstRect, &xSrcRect, EHS_WIDGET_IMAGE(pWidget).nCurrentImageAlpha);
    //@todo clear as for gifs??

}

void EhsWidgetImageSvg_destroy(EhsWidgetClass* pWidget)
{
    EhsTVSurface_destroy(&EhsTV, EHS_IMAGE_SVG(pWidget).pSurface);
}
