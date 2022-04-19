/** @file tgttest_viewport.c
 * Target tests for viewport
 * 
 * @author: inx limited
 * @version: $Revision: 1248 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */


#include "widget.h"
#include "tgttest.h"

/* test cases */
const ehs_char* TgtTestWidget_textbox(void);

/**
 * Test widget for specific widget drawing functions
 */
EHS_LOCAL EhsWidgetClass EhsL_xTestWidget;

struct TgtTestSuiteStruct TgtTestSetWidget = {"Widget", 1, {
		"Textbox", TgtTestWidget_textbox}
};

const ehs_char* TgtTestWidget_textbox(void)
{
	ehs_char* szRet = NULL;
	EhsWidgetClass* pWidget1;
	EhsWidgetClass* pWidget2;
	EhsGraphicsColourClass xFgColour, xBgColour;
	EhsGraphicsFontClass* pFontLsr = EhsGraphicsFont_load("lsr.bdf");
	EhsGraphicsFontClass* pFontCour = EhsGraphicsFont_load("courB18.bdf");
	EhsGraphicsRectangleClass xRect;
	
	xRect.nLeft = 100u; xRect.nTop = 100u; xRect.nWidth = 100u; xRect.nHeight = 80u;
	xFgColour = EhsGraphicsColour_argb(0,0,128,255);
	xBgColour = EhsGraphicsColour_argb(255,255,255,255);
	EhsWidgetTable_init(&EhsWidgetTable);
	pWidget1 = EhsWidgetTextbox_init(&xRect, 5, 10,10,10,10,10,xFgColour, xBgColour, pFontLsr);
	EhsWidget_create(pWidget1);
	EhsWidget_show(pWidget1);
	EhsWidgetTextbox_write(pWidget1,"Hello World");
	
	xRect.nLeft = 200u; xRect.nTop = 200u; xRect.nWidth = 100u; xRect.nHeight = 80u;
	pWidget2 = EhsWidgetTextbox_init(&xRect, 5, 10,10,10,10,10,xFgColour, xBgColour, pFontCour);
	EhsWidget_create(pWidget2);
	EhsWidget_show(pWidget2);
	EhsWidgetTextbox_write(pWidget2,"CourB18 text");
	EhsTV_update(&EhsTV);
	
	return szRet;
}
