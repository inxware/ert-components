/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/



/** @file tgttest_graphics.c
 * Target tests for generic graphics stuff
 *
 * @author: inx limited
 */

#include "font.h"
#include "hal_string.h"
#include "html.h"
#include "tgttest.h"
#include "messages.h"

const ehs_char* TgtTestGraphicsFontLoad(void);
const ehs_char* TgtTestGraphicsFont_findGlyph(void);
const ehs_char* TgtTestGraphicsFont_drawWord(void);
const ehs_char* TgtTestGraphicsFontGlyph_draw(void);
const ehs_char* TgtTestGraphicsHtml_parse(void);

struct TgtTestSuiteStruct TgtTestSetGraphics = {"Graphics", 5,
    {
        { "Font_load", TgtTestGraphicsFontLoad},
        { "Font_findGlyph", TgtTestGraphicsFont_findGlyph},
        { "Font_drawWord", TgtTestGraphicsFont_drawWord},
        { "FontGlyph_draw", TgtTestGraphicsFontGlyph_draw},
        { "HTML_parse", TgtTestGraphicsHtml_parse}
    }
};

/**
 * Test the EhsGraphicsFont_load function.
 */
const ehs_char* TgtTestGraphicsFontLoad(void)
{
    ehs_char *szRet = NULL;

    EhsGraphicsFontTable_init();
    EhsConsolePrintf("Ignore this error:\n");
    if (EhsGraphicsFont_load(""))
    {
        szRet = "EhsGraphicsFont_load succeeded where it should have failed";
    }
    else
    {
        EhsGraphicsFontClass* pFont1 = EhsGraphicsFont_load("lsr.bdf");
        EhsGraphicsFontClass* pFont2 = EhsGraphicsFont_load("lsr.bdf");

        if (!pFont1)
            szRet = "Couldn't load lsr.bdf";
        else if (pFont1 != pFont2)
            szRet = "Loading testfont twice caused two seperate loads";
        else if (pFont1->nNumGlyphs != 2388)
            szRet = "Number of glyphs read is incorrect";
        else if (0 != EhsStrcmp(pFont1->szName,"-FreeType-Lucida Sans-Medium-R-Normal--17-120-100-100-P-93-ISO10646-1"))
            szRet = "Incorrect font name";
    }

    return szRet;
}

/**
 * Find certain glyphs: 32 (lowest value in lsr.bdf), 65279 (highest value),
 * 2320 (median value). Ensure the following aren't found: 22, 144, 65999
 */
const ehs_char* TgtTestGraphicsFont_findGlyph(void)
{
    ehs_char *szRet = NULL;
    EhsGraphicsFontClass* pFont;
    const EhsGraphicsFontGlyphClass* pGlyph;
    ehs_uint32 nFindChars[] = {32,65279, 2320,0};
    ehs_uint32 nCantFindChars[] = {22, 144, 65999,0};
    ehs_uint32* pFind;

    EhsGraphicsFontTable_init();
    pFont = EhsGraphicsFont_load("lsr.bdf");

    /* look for characters we should be able to find */
    for (pFind = nFindChars; *pFind; ++pFind)
    {
        pGlyph = EhsGraphicsFont_findGlyph(pFont, *pFind);
        if (!pGlyph)
        {
            EhsSprintf(szErrorMessage, "Glyph for character %d can't be found", *pFind);
            szRet = szErrorMessage;
            break;
        }
        else if (pGlyph->nId != *pFind)
        {
            EhsSprintf(szErrorMessage, "Wrong glyph found: %d found, instead of %d", pGlyph->nId, *pFind);
            szRet = szErrorMessage;
            break;
        }
    }
    /* look for characters we shouldn't be able to find */
    for (pFind = nCantFindChars; *pFind; ++pFind)
    {
        pGlyph = EhsGraphicsFont_findGlyph(pFont, *pFind);
        if (pGlyph != pFont->pDefaultGlyph)
        {
            EhsSprintf(szErrorMessage, "Glyph for character %d found, Id reported is %d", *pFind, pGlyph->nId);
            szRet = szErrorMessage;
            break;
        }
    }
    return szRet;
}

#define WID 110
#define INDENT 5
#define FONT_HT 25
#define LINE_HT FONT_HT
#define HT ((FONT_HT*5) + (INDENT*2)) /* 5 lines of text in this textbox */
/**
 * Test the draw word routine. We're just checking that the positioning of the
 * words is correct and that the word wrapping works as expected.
 */
const ehs_char* TgtTestGraphicsFont_drawWord(void)
{
    const ehs_char* pRet = NULL;
    EhsGraphicsColourClass xPalette[2]; /* it doesn't matter what colours we pick */
    printf("TgtTestGraphicsFont_drawWord\n");
    EhsTVSurfaceClass* pSurface = EhsTVSurface_create(&EhsTV,HT,WID,EHS_GRAPHICS_COLOUR_A1,xPalette,2,EHS_TRUE);
    EhsGraphicsFontClass* pFont;
    EhsGraphicsRectangleClass xTextBoxRect;
    EhsGraphicsRectangleClass xSurfaceRect;
    ehs_uint32 nX, nY; /* position of the word being written */
    int i = 0;

    xTextBoxRect.nWidth = WID - (INDENT*2);
    xTextBoxRect.nHeight = HT - (INDENT*2);
    xSurfaceRect.nWidth = WID;
    xSurfaceRect.nHeight = HT;

    EhsGraphicsFontTable_init();
    pFont = EhsGraphicsFont_load("lsr.bdf");
    EhsHLogger_setLogLevel("Graphics", 	EHSH_LOG_LEVEL_ERROR|EHSH_LOG_LEVEL_WARNING|EHSH_LOG_LEVEL_INFO|EHSH_LOG_LEVEL_ENTER|EHSH_LOG_LEVEL_EXIT);

    nX = 0;
    nY = 0;
    /* widths of the characters in "Hello" are: 13,9,5,5,10 = 42, plus space (5) = 47 */

    /* draw first word */
//	printf("drawWord: %d\r\n",i++); getchar();
    if (!EhsGraphicsFont_drawWord(pFont, pSurface, &xSurfaceRect, &xTextBoxRect, &nX, &nY, LINE_HT, "Hello"))
    {
        pRet = "Attempt to draw first 'Hello' failed";
    }
    else if ((nX != 47)||(nY != 0))
    {
        EhsSprintf(szErrorMessage,"Expected 'Hello' to draw to (47,0), not (%-d,%-d)",nX,nY);
        pRet = szErrorMessage;
        /* draw second word (on first line) */
//}	printf("drawWord: %d\r\n",i++); getchar(); if (0) {
    }
    else if (!EhsGraphicsFont_drawWord(pFont, pSurface, &xSurfaceRect, &xTextBoxRect, &nX, &nY, LINE_HT, "Hello"))
    {
        pRet = "Attempt to draw second 'Hello' failed";
    }
    else if ((nX != 94)||(nY != 0))
    {
        EhsSprintf(szErrorMessage,"Expected 'Hello' to draw to (94,0), not (%-d,%-d)",nX,nY);
        pRet = szErrorMessage;
        /* draw third word - wrap to second line */
//}	printf("drawWord: %d\r\n",i++); getchar(); if (0) {
    }
    else if (!EhsGraphicsFont_drawWord(pFont, pSurface, &xSurfaceRect, &xTextBoxRect, &nX, &nY, LINE_HT, "Hello"))
    {
        pRet = "Attempt to draw third 'Hello' failed";
    }
    else if ((nX != 47)||(nY != LINE_HT))
    {
        EhsSprintf(szErrorMessage,"Expected 'Hello' to draw to (47,%d), not (%-d,%-d)",LINE_HT,nX,nY);
        pRet = szErrorMessage;
        /* draw fourth word without enough space to add trailing space */
//}	printf("drawWord: %d\r\n",i++); getchar(); if (0) {
    }
    else if (!EhsGraphicsFont_drawWord(pFont, pSurface, &xSurfaceRect, &xTextBoxRect, &nX, &nY, LINE_HT, "Helloo"))
    {
        pRet = "Attempt to draw fourth 'Helloo' failed";
    }
    else if ((nX != 99)||(nY != LINE_HT))
    {
        EhsSprintf(szErrorMessage,"Expected 'Helloo' to draw to (99,%d), not (%-d,%-d)",LINE_HT,nX,nY);
        pRet = szErrorMessage;
        /* draw fifth word (on third line) */
    }
    else if (!EhsGraphicsFont_drawWord(pFont, pSurface, &xSurfaceRect, &xTextBoxRect, &nX, &nY, LINE_HT, "Hello"))
    {
//}	printf("drawWord: %d\r\n",i++); getchar(); if (0) {

        pRet = "Attempt to draw fifth 'Hello' failed";
    }
    else if ((nX != 47)||(nY != (LINE_HT*2)))
    {
        EhsSprintf(szErrorMessage,"Expected 'Hello' to draw to (47,%d), not (%-d,%-d)",LINE_HT*2,nX,nY);
        pRet = szErrorMessage;
//}	printf("drawWord: %d\r\n",i++); getchar(); if (0) {
        /* draw sixth word - too long for a single line - start on fourth line, end on fifth line */
    }
    else if (!EhsGraphicsFont_drawWord(pFont, pSurface, &xSurfaceRect, &xTextBoxRect, &nX, &nY, LINE_HT, "HHHHHHHHH"))
    {
        pRet = "Attempt to draw sixth 'HHHHHHHHHHHHH' failed";
    }
    else if ((nX != 31)||(nY != (LINE_HT*4)))
    {
        EhsSprintf(szErrorMessage,"Expected 'Hello' to draw to (31,%d), not (%-d,%-d)",LINE_HT*4,nX,nY);
        pRet = szErrorMessage;
//}	printf("drawWord: %d\r\n",i++); getchar(); if (0) {
        /* draw seventh word - last successful one (on fifth line) */
    }
    else if (!EhsGraphicsFont_drawWord(pFont, pSurface, &xSurfaceRect, &xTextBoxRect, &nX, &nY, LINE_HT, "Hello"))
    {
        pRet = "Attempt to draw seventh 'Hello' failed";
    }
    else if ((nX != 78)||(nY != (LINE_HT*4)))
    {
        EhsSprintf(szErrorMessage,"Expected 'Hello' to draw to (86,%d), not (%-d,%-d)",LINE_HT*4,nX,nY);
        pRet = szErrorMessage;
//	 }	printf("drawWord: %d\r\n",i++); getchar(); if (0) {
        /* draw eighth word - unsuccessful because it doesn't fit */
    }
    else if (EhsGraphicsFont_drawWord(pFont, pSurface, &xSurfaceRect, &xTextBoxRect, &nX, &nY, LINE_HT, "Hello"))
    {
        pRet = "Attempt to draw eighth 'Hello' suceeded unexpectedly";
    }

    return pRet;
}

#undef HT
#undef WID
#define HT 10
#define WID 40 /* 5 * 8 = 40 pixels wide */
EHS_LOCAL ehs_uint8 xBitmap[HT*WID];
/* bitmap of 'a' drawn 5 times (clipping on 5th time. Bounding box is 7 x 9, horizontal offset
 * between characters is 9
 */
EHS_LOCAL ehs_uint8 xResultBM[HT*WID] =
{
    0u,   0u,   0u,   0u,   0u,
    0x1Cu,0x0Eu,0x07u,0x03u,0x80u,  /* glyph=0x38u */
    0x22u,0x11u,0x08u,0x84u,0x40u, /* glyph=0x44u */
    0x02u,0x01u,0x00u,0x80u,0x40u, /* glyph=0x04u */
    0x02u,0x01u,0x00u,0x80u,0x40u, /* glyph=0x04u */
    0x3Eu,0x1Fu,0x0Fu,0x87u,0xC0u, /* glyph=0x7cu */
    0x42u,0x21u,0x10u,0x88u,0x40u, /* glyph=0x84u */
    0x42u,0x21u,0x10u,0x88u,0x40u, /* glyph=0x84u */
    0x46u,0x23u,0x11u,0x88u,0xC0u, /* glyph=0x8cu */
    0x3Bu,0x1Du,0x8Eu,0xC7u,0x60u  /* glyph=0x76u */
};


/**
 * Test the draw routine, using 'a' from the lsr.bdf font file. 'a' has no symmetry
 * so we'll know if it's upside down or back to front.
 */
const ehs_char* TgtTestGraphicsFontGlyph_draw(void)
{
    ehs_char *szRet = NULL;
    EhsGraphicsFontClass* pFont;
    const EhsGraphicsFontGlyphClass* pGlyph;
    ehs_uint32 nIdx;
    ehs_uint32 nX, nY;

    EhsGraphicsFontTable_init();
    pFont = EhsGraphicsFont_load("lsr.bdf");
    pGlyph = EhsGraphicsFont_findGlyph(pFont,(ehs_uint32)'a');
    /* clear bitmap */
    for (nIdx = 0u; nIdx < (HT*WID); ++nIdx)
    {
        xBitmap[nIdx] = 0u;
    }

    nX = 0;
    nY = 9;
    /* draw 'a' five times */
    if ((!EhsGraphicsFontGlyph_draw(pGlyph, xBitmap, WID, HT, WID, HT, &nX, &nY))
            || (!EhsGraphicsFontGlyph_draw(pGlyph, xBitmap, WID, HT, WID, HT, &nX, &nY))
            || (!EhsGraphicsFontGlyph_draw(pGlyph, xBitmap, WID, HT, WID, HT, &nX, &nY))
            || (!EhsGraphicsFontGlyph_draw(pGlyph, xBitmap, WID, HT, WID, HT, &nX, &nY)))
    {
        szRet = "Unexpected draw glyph out of range";
    }
    else if (EhsGraphicsFontGlyph_draw(pGlyph, xBitmap, WID, HT, WID, HT, &nX, &nY))
    {
        szRet = "Attempt to draw glyph out of range succeeded";
    }
    else if (nX != 45)
    {
        EhsSprintf(szErrorMessage,"Expected x = 45, actual value = %d",nX);
    }
    else if (nY != 9)
    {
        EhsSprintf(szErrorMessage,"Expected y = 9, actual value = %d",nY);
    }
    else
    {
        /* check bitmap */
        for (nIdx = 0; nIdx < (HT*WID); nIdx++)
        {
            if (xBitmap[nIdx] != xResultBM[nIdx])
            {
                EhsSprintf(szErrorMessage,"result differs from expected at row %d.\r\nExpected\tReceived\r\n%02x%02x%02x%02x%02x\t%02x%02x%02x%02x%02x",
                           nIdx/WID,
                           xResultBM[(nIdx/WID)], xResultBM[(nIdx/WID)+1], xResultBM[(nIdx/WID)+2], xResultBM[(nIdx/WID)+3], xResultBM[(nIdx/WID)+4],
                           xBitmap[(nIdx/WID)], xBitmap[(nIdx/WID)+1], xBitmap[(nIdx/WID)+2], xBitmap[(nIdx/WID)+3], xBitmap[(nIdx/WID)+4]);
                szRet = szErrorMessage;
            }
        }
    }
    return szRet;
}

EHS_LOCAL const ehs_char* TgtTestGraphicsHtml_parseTestWord(EhsHGHtmlWordType eTestType, const ehs_char* szTestWord, ehs_uint16 i, void** ppCurrent, EhsHGHtmlStringClass* pHtml)
{
    const ehs_char* szRet = NULL;
    EhsHGHtmlWordType eType;
    const ehs_char* szWord;

    if (!EhsHGHtmlString_getWord(pHtml,&szWord,&eType,ppCurrent))
    {
        EhsSprintf(szErrorMessage,"Get %dth word failed",i);
        szRet = szErrorMessage;
    }
    else if (eTestType != eType)
    {
        EhsSprintf(szErrorMessage,"Expected type == %d, not %d for %dth word",eTestType,eType,i);
        szRet = szErrorMessage;
    }
    else if (!szTestWord && (szTestWord != szWord))
    {
        EhsSprintf(szErrorMessage,"Expected word == NULL, not \"%s\" for %dth word", szWord, i);
    }
    else if (szTestWord && EhsStrcmp(szTestWord,szWord))
    {
        EhsSprintf(szErrorMessage,"Expected word == \"%s\", not \"%s\" for %dth word",szTestWord,szWord,i);
        szRet = szErrorMessage;
    }
    return szRet;
}

/**
 * Test the Ehs HTML Parser
 */
const ehs_char* TgtTestGraphicsHtml_parse(void)
{
    const ehs_char* szRet = NULL;
    ehs_char* szRawText = "test several words<br>and <p>&lt;symbols&gt;<p/> <SPAN class=\"alt\">other text</span> ";
    const ehs_char* szWord;
    EhsHGHtmlStringClass xHtml;
    EhsHGHtmlWordType eType;
    void* pCurrent = NULL;

    EhsHGHtmlString_init(&xHtml,0);

    if (!EhsHGHtmlString_parse(&xHtml,szRawText))
    {
        szRet = "Parsing failed";
    }
    if (!szRet) szRet = TgtTestGraphicsHtml_parseTestWord(EHSHG_HTML_WORD_STYLE_TEXT,"test",1,&pCurrent,&xHtml);
    if (!szRet) szRet = TgtTestGraphicsHtml_parseTestWord(EHSHG_HTML_WORD_STYLE_TEXT,"several",2,&pCurrent,&xHtml);
    if (!szRet) szRet = TgtTestGraphicsHtml_parseTestWord(EHSHG_HTML_WORD_STYLE_TEXT,"words",3,&pCurrent,&xHtml);
    if (!szRet) szRet = TgtTestGraphicsHtml_parseTestWord(EHSHG_HTML_WORD_BREAK,NULL,4,&pCurrent,&xHtml);
    if (!szRet) szRet = TgtTestGraphicsHtml_parseTestWord(EHSHG_HTML_WORD_STYLE_TEXT,"and",5,&pCurrent,&xHtml);
    if (!szRet) szRet = TgtTestGraphicsHtml_parseTestWord(EHSHG_HTML_WORD_PARA,NULL,6,&pCurrent,&xHtml);
    if (!szRet) szRet = TgtTestGraphicsHtml_parseTestWord(EHSHG_HTML_WORD_STYLE_TEXT,"<symbols>",7,&pCurrent,&xHtml);
    if (!szRet) szRet = TgtTestGraphicsHtml_parseTestWord(EHSHG_HTML_WORD_PARA,NULL,8,&pCurrent,&xHtml);
    if (!szRet) szRet = TgtTestGraphicsHtml_parseTestWord(EHSHG_HTML_WORD_STYLE_START,"alt",9,&pCurrent,&xHtml);
    if (!szRet) szRet = TgtTestGraphicsHtml_parseTestWord(EHSHG_HTML_WORD_STYLE_TEXT,"other",10,&pCurrent,&xHtml);
    if (!szRet) szRet = TgtTestGraphicsHtml_parseTestWord(EHSHG_HTML_WORD_STYLE_TEXT,"text",11,&pCurrent,&xHtml);
    if (!szRet) szRet = TgtTestGraphicsHtml_parseTestWord(EHSHG_HTML_WORD_STYLE_END,NULL,12,&pCurrent,&xHtml);
    if (EhsHGHtmlString_getWord(&xHtml,&szWord,&eType,&pCurrent))
    {
        szRet = "Get word should have failed at the end of the string";
    }

    /*@todo test broken values:
     *  "&whoops i forgot to terminate this",
     * "&notReal;"
     * "<imaginary>"
     * "<pForgot closing bracket<br/>"
     * "<\span> Close span without open not allowed"
     * "<span class="new"><span class="other">nested spans not allowed</span></span> "
     */
    return szRet;
}
