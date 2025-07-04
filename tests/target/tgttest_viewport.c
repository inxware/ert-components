/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/



/** @file tgttest_viewport.c
 * Target tests for viewport
 *
 * @author: inx limited
 */

#include "hal_viewport.h"
#include "hal_console.h"
#include "hal_time.h"
#include "target.h"
#include "widget.h"
#include "graphics.h"
#include "tgttest.h"

const ehs_char* TgtTestViewport_testRgb888(void);
const ehs_char* TgtTestViewport_testArgb8888(void);
const ehs_char* TgtTestViewport_testA1(void);
const ehs_char* TgtTestViewport_testFill(void);


/**
 * Test widget for specific viewport drawing functions
 */
EHS_LOCAL EhsWidgetClass EhsL_xTestWidget;

/**
 * Set up our test widget for use in the viewport test function
 */
EHS_LOCAL void EhsL_setTestWidget(void (*pfDrawFunc)(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass *pClipRect),
                                  ehs_uint16 nLeft, ehs_uint16 nTop, ehs_uint16 nWidth, ehs_uint16 nHeight);

EHS_LOCAL void EhsL_drawRgb888(EhsWidgetClass* pWidget, EhsTVClass *pViewport, EhsGraphicsRectangleClass* pClipRect);
EHS_LOCAL void EhsL_drawArgb8888(EhsWidgetClass* pWidget, EhsTVClass *pViewport, EhsGraphicsRectangleClass* pClipRect);
EHS_LOCAL void EhsL_fill(EhsWidgetClass* pWidget, EhsTVClass *pViewport, EhsGraphicsRectangleClass* pClipRect);
EHS_LOCAL void EhsL_drawA1(EhsWidgetClass* pWidget, EhsTVClass *pViewport, EhsGraphicsRectangleClass* pClipRect);


struct TgtTestSuiteStruct TgtTestSetViewport = {"Viewport", 4,
    {
        { "Rgb888", TgtTestViewport_testRgb888},
        { "Argb8888", TgtTestViewport_testArgb8888},
        { "Colour A1", TgtTestViewport_testA1},
        { "Fill", TgtTestViewport_testFill}
    }
};

EhsTickType TgtTestViewport_elapsedTime; /* represents the amount of time that each test spends in blitter code */
EhsTickType TgtTestViewport_elapsedTimeTestCode; /* represents the amount of time each test spends in user-defined redraw code + time spent in blitter code */
static enum { TestRgb888, TestArgb1888, TestArgb8888, TestFill, TestA1 } TgtTestViewport_testcase = TestRgb888;

/**
 * Display the test screen for rgb888
 */
const ehs_char* TgtTestViewport_testRgb888(void)
{
    EhsTickType start, end;
    ehs_uint32 elapsed;
    ehs_char buff[10];
    TgtTestViewport_testcase = TestRgb888;
    EhsL_setTestWidget(EhsL_drawRgb888,0,0,1000,1000);
    TgtTestViewport_elapsedTime = 0u; /* reset elapsed time measurement */

    EhsTV_clear(&EhsTV);
    start = EhsTgtTimer_now();
    EhsTV_update(&EhsTV);
    end = EhsTgtTimer_now();

    EhsConsolePrintf("\tYou should see 16x16 display of coloured squares.\r\n");
    EhsConsolePrintf("\tBackground squares should be red (top right), blue (bottom left), green (bottom right)\r\n");
    EhsConsolePrintf("\tForeground squares should be the opposite way round with increasing opacity starting from top left (total transparency)\r\n");
    elapsed = EhsTgtTimer_tickTous(EHS_TGT_TIME_DIFFERENCE(start,end) + TgtTestViewport_elapsedTime - TgtTestViewport_elapsedTimeTestCode);
    EhsConsolePrintf("\tRendering took %d.%06d uS\n\r",elapsed/1000000u, elapsed%1000000u);
    while (!EhsConsoleLineReady())
        ;
    EhsConsoleGetLine(buff,10);

    return NULL;
}

/**
 * Display the test screen for argb8888
 */
const ehs_char* TgtTestViewport_testArgb8888(void)
{
    EhsTickType start, end;
    ehs_uint32 elapsed;
    ehs_char buff[10];
    TgtTestViewport_testcase = TestArgb8888;
    EhsL_setTestWidget(EhsL_drawArgb8888,0,0,1000,1000);
    TgtTestViewport_elapsedTime = 0u; /* reset elapsed time measurement */

    EhsTV_clear(&EhsTV);
    start = EhsTgtTimer_now();
    EhsTV_update(&EhsTV);
    end = EhsTgtTimer_now();

    EhsConsolePrintf("\tYou should see a single multi-coloured square.\r\n");
    EhsConsolePrintf("\tsub-divided into multiple squares of different colours\r\n");
    EhsConsolePrintf("\tTop left is predominantly red, bottom left is green, top right is blue\r\n");
    elapsed = EhsTgtTimer_tickTous(EHS_TGT_TIME_DIFFERENCE(start,end) + TgtTestViewport_elapsedTime - TgtTestViewport_elapsedTimeTestCode);
    EhsConsolePrintf("\tRendering took %d.%06d uS\n\r",elapsed/1000000u, elapsed%1000000u);
    while (!EhsConsoleLineReady())
        ;
    EhsConsoleGetLine(buff,10);

    return NULL;
}

/**
 * Display the test screen for fill
 */
const ehs_char* TgtTestViewport_testFill(void)
{
    EhsTickType start, end;
    ehs_uint32 elapsed;
    ehs_char buff[10];
    TgtTestViewport_testcase = TestFill;
    EhsL_setTestWidget(EhsL_fill,0,0,1000,1000);
    TgtTestViewport_elapsedTime = 0u; /* reset elapsed time measurement */

    EhsTV_clear(&EhsTV);
    start = EhsTgtTimer_now();
    EhsTV_update(&EhsTV);
    end = EhsTgtTimer_now();

    EhsConsolePrintf("\tYou should see 16x16 display of coloured squares.\r\n");
    EhsConsolePrintf("\tBackground squares should be red (top right), blue (bottom left), green (bottom right)\r\n");
    EhsConsolePrintf("\tForeground squares should be the opposite way round with increasing opacity starting from top left (total transparency)\r\n");
    elapsed = EhsTgtTimer_tickTous(EHS_TGT_TIME_DIFFERENCE(start,end) + TgtTestViewport_elapsedTime - TgtTestViewport_elapsedTimeTestCode);
    EhsConsolePrintf("\tRendering took %d.%06d uS\n\r",elapsed/1000000u, elapsed%1000000u);
    while (!EhsConsoleLineReady())
        ;
    EhsConsoleGetLine(buff,10);

    return NULL;
}

/**
 * Display the test screen for A1
 */
const ehs_char* TgtTestViewport_testA1(void)
{
    EhsTickType start, end;
    ehs_uint32 elapsed;
    ehs_char buff[10];
    TgtTestViewport_testcase = TestA1;
    EhsL_setTestWidget(EhsL_drawA1,0,0,1000,1000);
    TgtTestViewport_elapsedTime = 0u; /* reset elapsed time measurement */

    EhsTV_clear(&EhsTV);
    start = EhsTgtTimer_now();
    EhsTV_update(&EhsTV);
    end = EhsTgtTimer_now();

    EhsConsolePrintf("\tYou should see top-left triangle in blue, with a checkerboard in dark\r\n");
    EhsConsolePrintf("\tyellow (50%% red, 50%% green). Should result in grey-blue squares in top left\r\n");
    EhsConsolePrintf("\tyellow-(background) squares in bottom right\r\n");
    elapsed = EhsTgtTimer_tickTous(EHS_TGT_TIME_DIFFERENCE(start,end) + TgtTestViewport_elapsedTime - TgtTestViewport_elapsedTimeTestCode);
    EhsConsolePrintf("\tRendering took %d.%06d uS\n\r",elapsed/1000000u, elapsed%1000000u);
    while (!EhsConsoleLineReady())
        ;
    EhsConsoleGetLine(buff,10);

    return NULL;
}

/**
 * Colour our default image block with specified argb values
 */
void EhsL_colourRect(EhsTVSurfaceClass* pSurface, ehs_uint16 len, ehs_uint8 r, ehs_uint8 g, ehs_uint8 b, ehs_uint8 a)
{
    ehs_uint16 x;
    EhsGraphicsColourClass colour = EhsGraphicsColour_argb(r,g,b,a);
    for (x = 0; x < len; x++)
    {
        EhsTVSurface_pixels(pSurface)[x] = colour;
    }
}

/**
 * Test the drawRgb888 by drawing a 16 x 16 matrix of coloured blocks
 */
void EhsL_drawRgb888(EhsWidgetClass* pWidget, EhsTVClass *pViewport, EhsGraphicsRectangleClass* pClipRect)
{
    ehs_uint16 x,y;
    ehs_uint8 r,g,b,a;
    ehs_uint16 nHt = pClipRect->nHeight/20;
    ehs_uint16 nWd = pClipRect->nWidth/20;
    ehs_uint16 nSize = nHt * nWd;
    EhsTickType start, end;
    EhsGraphicsRectangleClass xRect;
    EhsTVSurfaceClass* pSurface;

    xRect.nWidth = nWd;
    xRect.nHeight = nHt;

    pSurface = EhsTVSurface_create(pViewport,nHt,nWd,EHS_GRAPHICS_COLOUR_ARGB8888,NULL,0,EHS_TRUE);

    for (x = 0; x < 16; x++)
    {
        for (y = 0; y < 16; y++)
        {
            r = (15-y)*16;
            g = (x*y);
            b = (15-x)*16;
            a = 255;
            EhsL_colourRect(pSurface, nSize,r,g,b,a);
            start = EhsTgtTimer_now();
            xRect.nLeft = x*pClipRect->nWidth/16;
            xRect.nTop = y*pClipRect->nHeight/16;
            EhsTV_blit(pViewport, pSurface, &xRect, 255u);
            end = EhsTgtTimer_now();
            TgtTestViewport_elapsedTime += EHS_TGT_TIME_DIFFERENCE(start,end);
        }
    }

    /* repeat with an offset and an increasing value for the alpha */
    for (x = 0; x < 16; x++)
    {
        for (y = 0; y < 16; y++)
        {
            r = y*16;
            g = (15-x)*(15-y);
            b = x*16;
            a = 255;
            EhsL_colourRect(pSurface,nSize,r,g,b,a);
            start = EhsTgtTimer_now();
            xRect.nLeft = x*pClipRect->nWidth/16 + 10;
            xRect.nTop = y*pClipRect->nHeight/16 + 10;
            EhsTV_blit(pViewport, pSurface, &xRect, x+y*16);
            end = EhsTgtTimer_now();
            TgtTestViewport_elapsedTime += EHS_TGT_TIME_DIFFERENCE(start,end);
        }
    }

}

/**
 * Test the Argb8888 with a single (256x26) block at 256,256. Layer different colours on top of it
 * each with different opacity levels.
 */
void EhsL_drawArgb8888(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect)
{
    EhsTickType start, end;
    ehs_uint16 x,y;
    EhsGraphicsRectangleClass xRect;
    EhsTVSurfaceClass* pSurface = EhsTVSurface_create(pViewport,256,256,EHS_GRAPHICS_COLOUR_ARGB8888,NULL,0,EHS_TRUE);

    xRect.nLeft = 256;
    xRect.nTop = 256;
    xRect.nWidth = 256;
    xRect.nHeight = 256;

    /* completely opaque white square */
    for (x = 0; x < 255; x++)
        for (y = 0; y < 255; y++)
        {
            EhsTVSurface_pixels(pSurface)[x+y*256] = EhsGraphicsColour_argb(255,255,255,255);
        }
    start = EhsTgtTimer_now();
    EhsTV_blit(pViewport, pSurface, &xRect, 255);
    end = EhsTgtTimer_now();
    TgtTestViewport_elapsedTime += EHS_TGT_TIME_DIFFERENCE(start,end);

    /* red square with full opacity in top left */
    for (x = 0; x < 255; x++)
        for (y = 0; y < 255; y++)
        {
            ehs_uint8 a = 255-(x|y);
            EhsTVSurface_pixels(pSurface)[x+y*256] = EhsGraphicsColour_argb(255,0,0,a);
        }
    start = EhsTgtTimer_now();
    EhsTV_blit(pViewport, pSurface, &xRect, 255);
    end = EhsTgtTimer_now();
    TgtTestViewport_elapsedTime += EHS_TGT_TIME_DIFFERENCE(start,end);

    /* green square with full opacity in bottom left */
    for (x = 0; x < 255; x++)
        for (y = 0; y < 255; y++)
        {
            ehs_uint8 a = 255-((x|(~y)));
            EhsTVSurface_pixels(pSurface)[x+y*256] = EhsGraphicsColour_argb(0,255,0,a);
        }
    start = EhsTgtTimer_now();
    EhsTV_blit(pViewport, pSurface, &xRect, 255);
    end = EhsTgtTimer_now();
    TgtTestViewport_elapsedTime += EHS_TGT_TIME_DIFFERENCE(start,end);

    /* blue square with full opacity in top right */
    for (x = 0; x < 255; x++)
        for (y = 0; y < 255; y++)
        {
            ehs_uint8 a = 255-((y|(~x)));
            EhsTVSurface_pixels(pSurface)[x+y*256] = EhsGraphicsColour_argb(0,0,255,a);
        }
    start = EhsTgtTimer_now();
    EhsTV_blit(pViewport, pSurface, &xRect, 255);
    end = EhsTgtTimer_now();
    TgtTestViewport_elapsedTime += EHS_TGT_TIME_DIFFERENCE(start,end);

}

/**
 * Test the A1 with a top-left triangle in blue, with a checkerboard in dark
 * yellow (50% red, 50% green). Should result in grey-blue squares in top left
 * yellow-(background) squares in bottom right
 */
void EhsL_drawA1(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect)
{
    EhsTickType start, end;
    ehs_uint16 x,y;
    EhsGraphicsRectangleClass xRect;
    EhsGraphicsColourClass xFgPalette = EhsGraphicsColour_argb(127,127,0,127);
    EhsGraphicsColourClass xBgPalette = EhsGraphicsColour_argb(0,0,255,255);
    EhsTVSurfaceClass* pSurfaceFG;
    EhsTVSurfaceClass* pSurfaceBG;
    pSurfaceFG = EhsTVSurface_create(pViewport,256,256,EHS_GRAPHICS_COLOUR_A1,&xFgPalette,1,EHS_TRUE);
//	printf("done create1\n");getchar();
    pSurfaceBG = EhsTVSurface_create(pViewport,256,256,EHS_GRAPHICS_COLOUR_A1,&xBgPalette,1,EHS_TRUE);
//	printf("done create2\n");getchar();

    xRect.nLeft = 256;
    xRect.nTop = 256;
    xRect.nWidth = 256;
    xRect.nHeight = 256;

    /* top-left triangle */
    for (x = 0; x < xRect.nWidth; x++)
        for (y = 0; y < xRect.nHeight; y++)
        {
            ehs_uint8 nBit = 1 << (7u - (x % 8u));
            if (y < (xRect.nWidth-x))
            {
                EhsTVSurface_pixels(pSurfaceBG)[x/8+y*32] |= nBit;
            }
            else
            {
                EhsTVSurface_pixels(pSurfaceBG)[x/8+y*32] &= ~nBit;
            }
        }
//printf("done triangle\n");getchar();
    /* 8x8 checkerboard */
    for (x = 0; x < xRect.nWidth; x++)
        for (y = 0; y < xRect.nHeight; y++)
        {
            ehs_bool b1 = ((x & 8u) == 8u);
            ehs_bool b2 = ((y & 8u) == 8u);
            ehs_uint8 nBit = 1 << (7u - (x % 8u));
            if ((b1 && !b2) || (!b1 && b2))
            {
                EhsTVSurface_bitmap(pSurfaceFG)[x/8+y*32] |= nBit;
            }
            else
            {
                EhsTVSurface_bitmap(pSurfaceFG)[x/8+y*32] &= ~nBit;
            }
        }
//printf("done checkerboard\n");getchar();
    start = EhsTgtTimer_now();
    EhsTV_blit(pViewport, pSurfaceBG, &xRect, 255);
//printf("done blit 1\n");getchar();
    EhsTV_blit(pViewport, pSurfaceFG, &xRect, 255);
//	printf("done blit 2\n");getchar();
    end = EhsTgtTimer_now();
    TgtTestViewport_elapsedTime += EHS_TGT_TIME_DIFFERENCE(start,end);

}


/**
 * Test the drawFill by drawing a 16 x 16 matrix of coloured blocks. Uses the same basic test
 * as rgb888
 */
void EhsL_fill(EhsWidgetClass* pWidget, EhsTVClass *pViewport, EhsGraphicsRectangleClass* pClipRect)
{
    ehs_uint16 x,y;
    ehs_uint16 nSize = pClipRect->nWidth/20 * pClipRect->nHeight/20;
    EhsTickType start, end;
    EhsGraphicsRectangleClass imgRect;
    EhsGraphicsColourClass fillColour;
    imgRect.nWidth = pClipRect->nWidth/20;
    imgRect.nHeight = pClipRect->nHeight/20;

    for (x = 0; x < 16; x++)
    {
        for (y = 0; y < 16; y++)
        {
            fillColour.sComp.nRed = (15-y)*16;
            fillColour.sComp.nGreen = (x*y);
            fillColour.sComp.nBlue = (15-x)*16;
            fillColour.sComp.nAlpha = 255;
            imgRect.nLeft = x*pClipRect->nWidth/16;
            imgRect.nTop = y*pClipRect->nHeight/16;
            start = EhsTgtTimer_now();
            EhsTV_fillRect(pViewport, &imgRect, &fillColour);
            end = EhsTgtTimer_now();
            TgtTestViewport_elapsedTime += EHS_TGT_TIME_DIFFERENCE(start,end);
        }
    }

    /* repeat with an offset and an increasing value for the alpha */
    for (x = 0; x < 16; x++)
    {
        for (y = 0; y < 16; y++)
        {
            fillColour.sComp.nRed = y*16;
            fillColour.sComp.nGreen = (15-x)*(15-y);
            fillColour.sComp.nBlue = x*16;
            fillColour.sComp.nAlpha = x+y*16;
            imgRect.nLeft = 10+x*pClipRect->nWidth/16;
            imgRect.nTop = 10+y*pClipRect->nHeight/16;
            start = EhsTgtTimer_now();
            EhsTV_fillRect(pViewport, &imgRect, &fillColour);
            end = EhsTgtTimer_now();
            TgtTestViewport_elapsedTime += EHS_TGT_TIME_DIFFERENCE(start,end);
        }
    }

}

/**
 * Set up our test widget for use in the viewport test function
 */
void EhsL_setTestWidget(void (*pfDrawFunc)(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass *pClipRect),
                        ehs_uint16 nLeft, ehs_uint16 nTop, ehs_uint16 nWidth, ehs_uint16 nHeight)
{
    EhsL_xTestWidget.nState = EHS_WIDGET_STATE_INIT | EHS_WIDGET_STATE_CREATED | EHS_WIDGET_STATE_SHOW;
    EhsL_xTestWidget.xCurRect.nLeft = nLeft;
    EhsL_xTestWidget.xCurRect.nTop = nTop;
    EhsL_xTestWidget.xCurRect.nWidth = nWidth;
    EhsL_xTestWidget.xCurRect.nHeight = nHeight;
    EhsL_xTestWidget.pfDrawFunc = pfDrawFunc;
    EhsWidgetTable.nSize = 1;
    EhsWidgetTable.pZOrderedWidget[0] = & EhsL_xTestWidget;

}
