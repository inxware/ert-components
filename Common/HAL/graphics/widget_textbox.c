/** @file widget_textbox.c
 * Definitions for the textbox widget handling functions
 *
 * @author: inx limited
 * @version: $Revision: 5525 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
 *
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

//#define EHSL_MODULE_ID (EHSH_LOG_MODULE_GRAPHICS)

#include "widget.h"
#include "messages.h"
#include "hal_string.h"
#include "hal_process.h"
#include "hal_viewport.h"
#include "font.h"

//#define DEFAULT_INDENT 5
/**
 * Insert a widget itno the Z-ordered list of widgets. This is called
 * after a widget has been initialized, but before the index to the widget
 * table has been updated
 */
EHS_GLOBAL void EhsWidgetTable_updateZOrder(EhsWidgetTableClass* pWidgetTable, EhsWidgetClass* pWidget);

/**
 * Create the widget. This is a necessary step prior to showing the widget.
 * NOTE: This must only be called by EhsWidget_create via the widget's
 * pfCreate function.
 */
EHS_LOCAL ehs_bool EhsWidgetTextbox_create(struct EhsWidgetStruct* pWidget);

/**
 * Destroy the widget. Required after the widget has been finished with.
 * NOTE: This must only be called by EhsWidget_destroy via the widget's
 * pfDestroy function.
 */
EHS_LOCAL void EhsWidgetTextbox_destroy(struct EhsWidgetStruct* pWidget);

/**
 * Fade the textbox according to an opacity value
 *
 * @return true if the textbox needs to be redrawn (because it's appearance has changed)
 */
EHS_LOCAL ehs_bool EhsWidgetTextbox_fade(struct EhsWidgetStruct* pWidget, ehs_uint8 nOpacity);

/**
 * Cause the widget to be displayed (assuming it's already been created). If
 * it hasn't been created, there are no ill effects.
 *
 * NOTE: This function must only be called by EhsWidget_draw via the widget's
 * pfDraw function.
 * callers listed below.
 *
 * @param pWidget. Widget to display.
 * @param pViewport. Viewport to display the widget on.
 * @param pClipRect Specifies the bounds for drawing the widget
 */
EHS_LOCAL void EhsWidgetTextbox_draw(struct EhsWidgetStruct* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect);

/**
 * Initialise the widget with the general widget and textbox specific properties including
 * position, dimensions, z-ordering and colour.
 *
 * @param nX X-coordinate for initial position of widget
 * @param nY Y-coordinate for initial position of widget
 * @param nZ Z order for widget
 * @param nWidth Widget's width for display
 * @param nHeight Widget's height for display
 * @param xFgColour Widget's foreground colour
 * @param xBgColour Widget's background colour
 * @return pointer to initialised widget, or NULL
 */
EhsWidgetClass* EhsWidgetTextbox_init(const EhsGraphicsRectangleClass* pBounds, ehs_uint16 nZ,
		ehs_uint16 nIndentL, ehs_uint16 nIndentT, ehs_uint16 nIndentR, ehs_uint16 nIndentB, ehs_uint16 nLineSep,
		EhsGraphicsColourClass xFgColour, EhsGraphicsColourClass xBgColour,
		EhsGraphicsFontClass* pFont)
{
	EhsWidgetClass* pWidget;

	EhsTPMutex_lock(EhsTPMutex_viewport);
	pWidget = EhsWidgetTable_new(&EhsWidgetTable);

	if (pWidget)
	{

		EhsWidget_init(pWidget,pBounds, nZ,  xFgColour.sComp.nAlpha);

		pWidget->eWidgetKind = EHS_WIDGET_KIND_TEXTBOX;
		pWidget->nState = EHS_WIDGET_STATE_INIT;
		pWidget->bOptimiseForSpeed = EHS_FALSE;
		pWidget->bContentChanged = EHS_TRUE;
		pWidget->pfCreateFunc = EhsWidgetTextbox_create;
		pWidget->pfDestroyFunc = EhsWidgetTextbox_destroy;
		pWidget->pfFadeFunc = EhsWidgetTextbox_fade;
		pWidget->pfDrawFunc = EhsWidgetTextbox_draw;

		EHS_WIDGET_TEXTBOX(pWidget).xFgColour = xFgColour;
		EHS_WIDGET_TEXTBOX(pWidget).nFgBaseAlpha = xFgColour.sComp.nAlpha;
		EHS_WIDGET_TEXTBOX(pWidget).xBgColour = xBgColour;
		EHS_WIDGET_TEXTBOX(pWidget).nBgBaseAlpha = xBgColour.sComp.nAlpha;
//#ifdef EHS_GUI_TEXTWIDGET_SUPPORT_HTML
		EhsHGHtmlString_init(&(EHS_WIDGET_TEXTBOX(pWidget).xText), EHS_STRING_LENGTH_MAX);
//endif
		EHS_WIDGET_TEXTBOX(pWidget).pSurface=NULL;
		EHS_WIDGET_TEXTBOX(pWidget).pFont = pFont;
#define DEFAULT_INDENT 0
		EHS_WIDGET_TEXTBOX(pWidget).nIndentBottom = nIndentB;
		EHS_WIDGET_TEXTBOX(pWidget).nIndentLeft = nIndentL;
		EHS_WIDGET_TEXTBOX(pWidget).nIndentRight = nIndentR;
		EHS_WIDGET_TEXTBOX(pWidget).nIndentTop = nIndentT;
		EHS_WIDGET_TEXTBOX(pWidget).nLineSep = nLineSep;
		EhsWidgetTable_updateZOrder(&EhsWidgetTable, pWidget);
	}
	else EHSH_LOG_ERROR("Could not initialise text box widget");
	EhsTPMutex_unlock(EhsTPMutex_viewport);
	return pWidget;
}



/**
 * Create the widget. This is a necessary step prior to showing the widget.
 * NOTE: This must only be called by EhsWidget_create. All widget creation should be
 * handled by EhsWidget_create.
 */
ehs_bool EhsWidgetTextbox_create(EhsWidgetClass* pWidget)
{
	/* No specific textbox creation activities are required */
	/* todo this may in fact be where the glyph blitter should be called from for initial text - for efficiency */

	return EHS_TRUE;
}

/**
 * Destroy the widget. Required after the widget has been finished with.
 * NOTE: This must only be called by EhsWidget_destroy. All widget destruction should be
 * handled by EhsWidget_destroy.
 */
void EhsWidgetTextbox_destroy(EhsWidgetClass* pWidget)
{
	EhsTVSurfaceClass* pSurface = EHS_WIDGET_TEXTBOX(pWidget).pSurface;
	if (pSurface) {
		EhsTVSurface_destroy(&EhsTV, pSurface);/* todo the use of the globaL eHStv is not good or consistent with other functions, where it is passed in by the framework */
	}
	EHS_WIDGET_TEXTBOX(pWidget).pSurface = NULL;
	/* no specific textbox destruction activities are required */
}

/**
 * Cause the widget to be displayed (assuming it's already been created). If
 * it hasn't been created, there are no ill effects.
 * This is called on each blit.
 *
 *
 * NOTE: This function must only be called by EhsWidget_draw.
 *
 * @param pWidget. Widget to display.
 * @param pViewport. Viewport to display the widget on.
 * @param pClipRect Specifies the bounds for drawing the widget
 */
void EhsWidgetTextbox_draw(struct EhsWidgetStruct* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect)
{
	EhsTVSurfaceClass* pSurface;
	ehs_bool bOk = EHS_TRUE; /* true so long as we can continue drawing to the surface */
    if (EhsStrlen(EHS_WIDGET_TEXTBOX(pWidget).xText.szHtml) > EHS_STRING_LENGTH_MAX) EHS_WIDGET_TEXTBOX(pWidget).xText.szHtml[EHS_STRING_LENGTH_MAX] = '\0'; //truncate to avoid crashes
	//LOGI("EHS:Textbox_draw--------");
	//dktodo investigate: crashes android app if tries to draw textbox
	if (pWidget->bContentChanged) {
		if (!pWidget->bOptimiseForSpeed) {
			EhsGraphicsColourClass xPalette[2];
			EhsGraphicsRectangleClass xTextRect;
			xTextRect.nHeight = pWidget->xCurRect.nHeight - (EHS_WIDGET_TEXTBOX(pWidget).nIndentTop +EHS_WIDGET_TEXTBOX(pWidget).nIndentBottom);
			xTextRect.nWidth = pWidget->xCurRect.nWidth - (EHS_WIDGET_TEXTBOX(pWidget).nIndentRight + EHS_WIDGET_TEXTBOX(pWidget).nIndentLeft);
			xTextRect.nLeft = pWidget->xCurRect.nLeft + EHS_WIDGET_TEXTBOX(pWidget).nIndentLeft;
			xTextRect.nTop = pWidget->xCurRect.nTop + EHS_WIDGET_TEXTBOX(pWidget).nIndentTop;
			xPalette[0] = EHS_WIDGET_TEXTBOX(pWidget).xBgColour;
			xPalette[1] = EHS_WIDGET_TEXTBOX(pWidget).xFgColour;
			if (pWidget->specificWidgetType.textbox.pSurface) EhsTVSurface_destroy(pViewport, pWidget->specificWidgetType.textbox.pSurface); /* there is no update function so remove and start again */
			pSurface = EhsTVSurface_create(pViewport,pWidget->xCurRect.nWidth,pWidget->xCurRect.nHeight,EHS_GRAPHICS_COLOUR_A1,xPalette,2);//,EHS_TRUE);
			EHS_WIDGET_TEXTBOX(pWidget).pSurface=pSurface;
			if (pSurface) {
				if (EHS_WIDGET_TEXTBOX(pWidget).pFont) {
					ehs_uint32 nXPos, nYPos;
					const void* pCurrentWord;
					const ehs_char* szWord;
					EhsHGHtmlWordType eType;

					nXPos = xTextRect.nLeft - pWidget->xCurRect.nLeft;
					nYPos = (xTextRect.nTop - pWidget->xCurRect.nTop)+EHS_WIDGET_TEXTBOX(pWidget).pFont->nMaxHt;

					pCurrentWord = NULL;
					/* iterate through the words in the string and deal word-by-word */
					while(bOk && EhsHGHtmlString_getWord(&(EHS_WIDGET_TEXTBOX(pWidget).xText), &szWord, &eType, &pCurrentWord)) {
						switch (eType) {
						case EHSHG_HTML_WORD_STYLE_TEXT:
							bOk = EhsGraphicsFont_drawWord(EHS_WIDGET_TEXTBOX(pWidget).pFont, pSurface,
									&(pWidget->xCurRect),
									&(xTextRect),
									&nXPos, &nYPos, EHS_WIDGET_TEXTBOX(pWidget).nLineSep, EHS_WIDGET_TEXTBOX(pWidget).nIndentLeft, szWord);
							break;
						case EHSHG_HTML_WORD_BREAK:
							bOk = EhsGraphicsFont_newLine(EHS_WIDGET_TEXTBOX(pWidget).pFont, &(xTextRect), EHS_WIDGET_TEXTBOX(pWidget).nLineSep, &nXPos, &nYPos,
									EHS_WIDGET_TEXTBOX(pWidget).nIndentLeft);
							break;
						case EHSHG_HTML_WORD_PARA:
							bOk = EhsGraphicsFont_newLine(EHS_WIDGET_TEXTBOX(pWidget).pFont, &(xTextRect),
									EHS_WIDGET_TEXTBOX(pWidget).nLineSep, &nXPos, &nYPos, EHS_WIDGET_TEXTBOX(pWidget).nIndentLeft);
							bOk = bOk && EhsGraphicsFont_newLine(EHS_WIDGET_TEXTBOX(pWidget).pFont, &(xTextRect),
									EHS_WIDGET_TEXTBOX(pWidget).nLineSep, &nXPos, &nYPos, EHS_WIDGET_TEXTBOX(pWidget).nIndentLeft);
							break;
						case EHSHG_HTML_WORD_STYLE_START:
							/*@todo implement support for styles */
						case EHSHG_HTML_WORD_STYLE_END:
						case EHSHG_HTML_WORD_IMG:
							/*@todo implement support for in-line images */
						default:
							break;
						}
					}
				}
			}
		} else {
			/**< @todo replace this with a real surface */
			EHSH_LOG_ERROR("Could not create surface for text box");
			pSurface = NULL;
		}

		if (pSurface)
		{
			EhsGraphicsRectangleClass xSrc;
			xSrc.nLeft = 0u;
			xSrc.nTop = 0u;
			xSrc.nWidth = pWidget->xCurRect.nWidth;
			xSrc.nHeight = pWidget->xCurRect.nHeight;
			EhsTVSurface_finalisePixels(pSurface); /* allows transfer to graphics memory e.g. for OpenGL.*/
			EhsTV_blit_withlock(pViewport, pSurface, &(pWidget->xCurRect),&xSrc, 255u);
			pWidget->bContentChanged = EHS_FALSE; /* This should be done in the common code */

			/* Review why this was once here */
#ifdef MAKE_IT_CRASH
			if (!pWidget->bOptimiseForSpeed) {
				EhsTVSurface_destroy(pViewport, pSurface);
				pWidget->specificWidgetType.textbox.pSurface = NULL;
			}

#endif
		}
		//LOGI("Blitting TEXT with new content");
	}
	else { /* just need to fire off the blitter for the pre-created surface*/
		EhsGraphicsRectangleClass xSrc;
		xSrc.nLeft = 0u;
		xSrc.nTop = 0u;
		xSrc.nWidth = pWidget->xCurRect.nWidth;
		xSrc.nHeight = pWidget->xCurRect.nHeight;
		EhsTV_blit_withlock(pViewport, EHS_WIDGET_TEXTBOX(pWidget).pSurface, &(pWidget->xCurRect),&xSrc, 255u);
	}
}

#ifdef BLIT
void EhsWidgetTextbox_draw(struct EhsWidgetStruct* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect)
{
	EhsTVSurfaceClass* pSurface;
	EhsGraphicsColourClass xPalette[2];
	xPalette[0] = EHS_WIDGET_TEXTBOX(pWidget).xBgColour;
	xPalette[1] = EHS_WIDGET_TEXTBOX(pWidget).xFgColour;
	pSurface = EhsTVSurface_create(pViewport,pWidget->xCurRect.nWidth,pWidget->xCurRect.nHeight,EHS_GRAPHICS_COLOUR_A1,xPalette,2);//,EHS_TRUE);
	EhsTVSurface_destroy(pViewport, pSurface);
}
#endif /*BLIT*/
/**
 * Set the text to be displayed onto a specific textbox. This function can be called
 * directly by any functions
 *
 * @param pWidget Textbox to display text
 * @param szText String to be displayed
 */
void EhsWidgetTextbox_write(struct EhsWidgetStruct* pWidget, const ehs_char* szText)
{
	EhsTPMutex_lock(EhsTPMutex_viewport);
	/* @todo check for changes to HTML - don't update viewport if no change has occcured */
//#define	EHS_GUI_TEXTWIDGET_SUPPORT_HTML
#ifdef EHS_GUI_TEXTWIDGET_SUPPORT_HTML
	if (EhsHGHtmlString_parse(&(EHS_WIDGET_TEXTBOX(pWidget).xText),szText,EHS_TRUE)) {
#else
		if (EhsHGHtmlString_parse(&(EHS_WIDGET_TEXTBOX(pWidget).xText),szText,EHS_FALSE)) {
#endif
			/* if pWidget is visible, update viewport */
			if (EHS_WIDGET_STATE_SHOWN(pWidget->nState))
			{
				EhsTV_updateRect(&EhsTV, pWidget->xCurRect.nLeft, pWidget->xCurRect.nTop, pWidget->xCurRect.nWidth, pWidget->xCurRect.nHeight);
			}
		}
		EhsTPMutex_unlock(EhsTPMutex_viewport);
	}

/**
 * Fade the textbox according to an opacity value. Called from widget_fade, hence EhsTPMutex_viewport
 * is already locked.
 *
 * @return true if the textbox needs to be redrawn (because it's appearance has changed)
 */
ehs_bool EhsWidgetTextbox_fade(struct EhsWidgetStruct* pWidget, ehs_uint8 nOpacity)
{
	ehs_bool bChanged = EHS_FALSE;

	/* determine the new foreground and background opacity required by this widget */
	ehs_uint16 nCalcFgOpacity = (EHS_WIDGET_TEXTBOX(pWidget).nFgBaseAlpha * nOpacity) / 255;
	ehs_uint16 nCalcBgOpacity = (EHS_WIDGET_TEXTBOX(pWidget).nBgBaseAlpha * nOpacity) / 255;
	/* scale down to 8-bit */
	ehs_uint8 nFgOpacity = (ehs_uint8)nCalcFgOpacity;
	ehs_uint8 nBgOpacity = (ehs_uint8)nCalcBgOpacity;

	/* is it any different to the opacity we have already? */
	if ((nFgOpacity != EHS_WIDGET_TEXTBOX(pWidget).xFgColour.sComp.nAlpha) ||
		(nBgOpacity != EHS_WIDGET_TEXTBOX(pWidget).xBgColour.sComp.nAlpha))
	{
		EHS_WIDGET_TEXTBOX(pWidget).xFgColour.sComp.nAlpha = nFgOpacity;
		EHS_WIDGET_TEXTBOX(pWidget).xBgColour.sComp.nAlpha = nBgOpacity;
		bChanged = EHS_TRUE;
	}

	return bChanged;
}
