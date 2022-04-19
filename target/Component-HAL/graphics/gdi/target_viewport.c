/** @file target_viewport.c
 * This file provides the definitions necessary for simulating the target_viewport code
 *
 *
 * @author: inx limited
 * @version: $Revision: 1256 $
 * @date: $Date$
 *
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

#include "globals.h"
/*
_WIN32_WINNT should be define in target_config.h, but just in case
*/
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include "hal_viewport.h"
#include "hal_process.h"
#include "widget.h"
#include "hal_mem.h"
#include "messages.h"
#include "hal_string.h"

/**
 * Convert EHS specific colour type to windows specific colour type
 */
EHS_LOCAL COLORREF EhsTV_getColour(EhsGraphicsColourClass);

/**
 * Reference to the OSD layer window handle. This is necessary to be able
 * to refresh the window when we change the bitmaps we wish to display
 */
HWND hEhsOsdLayerWnd;

/**
 * Reference to the video layer window handle.
 */
// @todo is this needed ? HWND EhsTgtMainWindowHandle;

/**
 * Define the target viewport type
 */
struct EhsTVStruct
{
	/**
	 * Reference to current device context. This is set at the start of the
	 * painting process, and reset to null when painting has finished.
	 */
	HDC hdc;
	/**
	 * Bitmap info needed every time blit is called. Some properties remain the
	 * same every call, therefore initialised once, rather than every blit
	 */
	BITMAPINFO bmi;
	/**
	 * Structure needed for alpha blending of bitmaps. Initialised once.
	 */
	BLENDFUNCTION bf;

	ehs_uint32 init; /**< set to a special pattern to prove that we've been
					 * initialised - avoids a race condition where paint occurs before
					 * EHS is initialized */

};

/**
 * Define the target viewport. Only one viewport is defined at this
 * stage.
 */
EhsTVClass EhsTV = {0};

/**
 * Initialise the target viewport
 */
ehs_bool EhsTV_init(EhsTVClass* pViewport)
{
	/* initialise fixed bitmap header parameters */
    ZeroMemory(&(pViewport->bmi), sizeof(BITMAPINFO));
	pViewport->bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pViewport->bmi.bmiHeader.biPlanes = 1;
    pViewport->bmi.bmiHeader.biBitCount = 32;         /* four 8-bit components */
    pViewport->bmi.bmiHeader.biCompression = BI_RGB;

	/* intialise our blend function */
	pViewport->bf.BlendOp = AC_SRC_OVER;
    pViewport->bf.BlendFlags = 0;
    pViewport->bf.SourceConstantAlpha = 0xff;  // half of 0xff = 50% transparency
    pViewport->bf.AlphaFormat = AC_SRC_ALPHA;             // ignore source alpha channel
	pViewport->init = EHS_TRUE;

	return EHS_TRUE;
}

/**
 *  Reset the use of the target viewport. This function
 * releases resources occupied by the viewport.
 */
 void EhsTV_reset(EhsTVClass* pViewport)
 {
 }

/**
 *  Terminate the use of the target viewport. This function
 * releases resources occupied by the viewport.
 */
 void EhsTV_term(EhsTVClass* pViewport)
 {
 	/* not currently used, therefore not implemented */
 }

/**
 * Indicate to the target that the viewport needs updating.
 * This will eventually result in EhsTV_paint being called
 */
void EhsTV_update(EhsTVClass* pViewport)
{
	InvalidateRect(hEhsOsdLayerWnd, NULL, FALSE);
}

/**
 * Indicate to the target that a rectangle on the viewport needs updating.
 * This will eventually result in EhsTV_paint being called
 * @todo document params
 */
void EhsTV_updateRect(EhsTVClass* pViewport, ehs_uint16 nX, ehs_uint16 nY, ehs_uint16 nWidth, ehs_uint16 nHeight)
{
	/* @todo what is this function and where did its arguments come from. Why is the patch not referenced anywhere */
	InvalidateRect(hEhsOsdLayerWnd, NULL, FALSE);
	return;
	/*
	RECT r;

	r.left = nX;
	r.top = nY;
	r.right = nX + nWidth;
	r.bottom = nY + nHeight;

	InvalidateRect(hEhsOsdLayerWnd, &r, FALSE);
	*/
}

/**
 * Clear the target viewport
 */
void EhsTV_clear(EhsTVClass* pViewport)
{
	InvalidateRect(hEhsOsdLayerWnd, NULL, FALSE);
}


/**
 * Convert EhsColourType to colour format used by Windows.
 * EhsColourType appears as 0xbbggrraa, Windows requires 0xaarrbbgg.
 * ASM is used to avoid 320% instruction increase (or 800% depending
 * upon code used). Specifically, I can't get VC to generate the BSWAP
 * instruction, which does just what I need.
 */
EHS_LOCAL void EhsTV_convertColour(void* pD, void* pS, int size)
{
	int i;
	register char* pDst = pD;
	register char* pSrc = pS;
	for (i = 0; i < size; i+=4, pDst+=4, pSrc+=4)
	{
		__asm {
			mov ecx, dword ptr [pSrc]
			mov edx, dword ptr [pDst]
			mov eax, dword ptr [ecx]
			bswap eax
			mov dword ptr [edx], eax
		}
	}
}

/**
 * Create a device independent bitmap. This uses the BITMAPV5 header, which should, in theory
 * make a bitmap that can be used for alpha blending in layered windows
 */
HBITMAP EhsL_makeDIBitmap(HDC hdcBm, BITMAPINFO* pBmi, ehs_uint16 nWid, ehs_uint16 nHt, void** ppPixels) {
	HBITMAP hBmp;
	BITMAPV5HEADER bmihdr;

	bmihdr.bV5Size = sizeof(BITMAPV5HEADER);
	bmihdr.bV5Width = nWid;
	bmihdr.bV5Height = -nHt;
	bmihdr.bV5Planes = 1;
	bmihdr.bV5BitCount = 32;
	bmihdr.bV5Compression = BI_RGB; /* or or BI_RGB? BI_BITFIELDS? */
	bmihdr.bV5SizeImage = nWid * nHt * 4; /* not required by BI_RGB */
	bmihdr.bV5XPelsPerMeter = 4000; /* used by application only? */
	bmihdr.bV5YPelsPerMeter = 4000;
	bmihdr.bV5ClrUsed = 0; /* Number of colour indices in the colour table that are used in the image. Not relevant? */
	bmihdr.bV5ClrImportant = 0;
	bmihdr.bV5RedMask = 255; /* valid only if bv5Compression is set to BI_BITFIELDS */
	bmihdr.bV5GreenMask = 255; /* " */
	bmihdr.bV5BlueMask = 255; /* " */
	bmihdr.bV5AlphaMask = 255; /* specifies the alpha component of each pixel */
	bmihdr.bV5CSType = LCS_sRGB; /* bitmap is in sRGB colour space */
	bmihdr.bV5Intent = LCS_GM_IMAGES; /* maintains contrast. Used for photos and images */
	bmihdr.bV5ProfileSize = 0;
	bmihdr.bV5Reserved = 0;
	hBmp = CreateDIBSection(hdcBm, (BITMAPINFO*)&bmihdr, DIB_RGB_COLORS, ppPixels, NULL, (DWORD)0);

	return hBmp;
}


/**
 * Display a single line of an image to the specified X,Y position in the
 * specified viewport. The image is represented in ARGB8888 format.
 * Called indirectly by _paint, hence EhsTPMutex_viewport is locked
 * for the duration of this function.
 *
 * @param[in] pViewport Viewport to blit into
 * @param[in] pSurface The image to blit.
 * @param[in] prDst The rectangle on the viewport where the image is drawn
 * @param[in] prSrc The rectangle on the surface where the image comes from
 * @param[in] nAlpha The global alpha level
 *
 */
void EhsL_blitArgb8888(EhsTVClass* pViewport,
					const EhsTVSurfaceClass* pSurface,
					const EhsGraphicsRectangleClass* prDst,
					const EhsGraphicsRectangleClass* prSrc,
					ehs_uint8 nImageAlpha)
{
	HBITMAP hbmOld;
	HDC hdcBm;
	HBITMAP hbitmap;
	ehs_uint8* pPixels;
	int i;
	EhsGraphicsColourClass* pLine = EhsTVSurface_pixels(pSurface);

	/* setup image-specific bitmap info. Common parameters have been set by EhsTV_init */
	pViewport->bmi.bmiHeader.biWidth = prSrc->nWidth;
	pViewport->bmi.bmiHeader.biHeight = -prSrc->nHeight;
	pViewport->bmi.bmiHeader.biSizeImage = (prSrc->nWidth * prSrc->nHeight) << 2; /* width * height (=1) * 4 (sizeof(ARGB) */

	/* create our DIB section and select the bitmap into the dc */
	if (hdcBm = CreateCompatibleDC(pViewport->hdc)) {
		if (hbitmap = EhsL_makeDIBitmap(hdcBm, &(pViewport->bmi), prSrc->nWidth, prSrc->nHeight, &pPixels)) {
			hbmOld = SelectObject(hdcBm, hbitmap);
			for (i=0;i<(prSrc->nWidth*prSrc->nHeight-1);i++) {
				/* all alpha pixels below a certain threshold are completely hidden
				 * this avoids the problem caused by a very low alpha pixel appearing
				 * as an opaque black pixel. This problem happens when we attempt
				 * to alphablend onto the directX surface that VLC currently uses. */
				if (0) { //pLine[i].sComp.nAlpha*nImageAlpha/0xFF < 0x3f /*3f*/) {
					((EhsGraphicsColourClass*)(pPixels))[i].sComp.nRed  = 0;
					((EhsGraphicsColourClass*)(pPixels))[i].sComp.nGreen= 0;
					((EhsGraphicsColourClass*)(pPixels))[i].sComp.nBlue = 0;
					((EhsGraphicsColourClass*)(pPixels))[i].sComp.nAlpha = 0;
				} else {
					((EhsGraphicsColourClass*)(pPixels))[i].sComp.nRed  =(pLine[i].sComp.nRed * pLine[i].sComp.nAlpha * nImageAlpha)/0xFFFF; // do all in one step as 32 bit processing
					((EhsGraphicsColourClass*)(pPixels))[i].sComp.nGreen=(pLine[i].sComp.nGreen * pLine[i].sComp.nAlpha* nImageAlpha)/0xFFFF;
					((EhsGraphicsColourClass*)(pPixels))[i].sComp.nBlue =(pLine[i].sComp.nBlue* pLine[i].sComp.nAlpha* nImageAlpha)/0xFFFF;
					((EhsGraphicsColourClass*)(pPixels))[i].sComp.nAlpha = ((int)(pLine[i].sComp.nAlpha*nImageAlpha))/0xFF; /* !!! this value is correct - this is scaling different to the above. */
				}
			}

			/* blit in the bitmap */
			if (!AlphaBlend(pViewport->hdc,
					prDst->nLeft, prDst->nTop, prDst->nWidth, prDst->nHeight,
					hdcBm,
					prSrc->nLeft, prSrc->nTop, prSrc->nWidth, prSrc->nHeight,
					pViewport->bf))
			{
				;/* @todo an error occurred - should log/record this */
			}

			/* Now, clean up. A memory DC always has a drawing
			* surface in it. It is created with a 1X1 monochrome
			* bitmap that we saved earlier, and need to put back
			* before we destroy it.
			*/
			if (hbmOld) SelectObject(hdcBm,hbmOld); // only swap back if the bitmap was successful
			DeleteObject(hbitmap);

		}
		DeleteDC(hdcBm);
	}
}


/**
 *  * Called indirectly by _paint, hence EhsTPMutex_viewport is locked
 * for the duration of this function.
 */
EHS_LOCAL EhsL_blitA1(EhsGraphicsColourClass* pPixels,
		EhsTVSurfaceClass* pSurface,
		const EhsGraphicsRectangleClass* prDst,
		const EhsGraphicsRectangleClass* prSrc,
		ehs_uint8 nImageAlpha)
{
	ehs_uint32 x,y;
	EhsGraphicsColourClass* pLine = EhsTVSurface_pixels(pSurface);
	EhsGraphicsColourClass xBg = pSurface->pPalette[0];
	EhsGraphicsColourClass xFg = pSurface->pPalette[1];
	ehs_uint32 nPix;
	ehs_uint8 nAlpha, nVal;

	if (xFg.sComp.nAlpha == 0x0) {// FG alpha =  0;
		for (y = 0; y < prDst->nHeight; y++) {
			ehs_uint32 nPixIdx = y * prDst->nWidth;
			ehs_uint32 nBmpIdx = y * ((pSurface->nWid+7)/8);
			for (x = 0; x < prDst->nWidth; x++) {
				ehs_uint8 nBit = 1 << (7u - (x % 8u));
				if ((nBit & EhsTVSurface_pixels(pSurface)[nBmpIdx + x/8]) == nBit) {
					pPixels[nPixIdx+x].sComp.nRed = 0x0;
					pPixels[nPixIdx+x].sComp.nGreen = 0x0;
					pPixels[nPixIdx+x].sComp.nBlue = 0x0;
					pPixels[nPixIdx+x].sComp.nAlpha = 0x0;
				} else {
					pPixels[nPixIdx+x] = xBg;
				}
			}
		}
	}
	else if (xBg.sComp.nAlpha == 0x0) {// BG alpha =  0;
		for (y = 0; y < prDst->nHeight; y++) {
			ehs_uint32 nPixIdx = y * prDst->nWidth;
			ehs_uint32 nBmpIdx = y * ((pSurface->nWid+7)/8);
			for (x = 0; x < prDst->nWidth; x++) {
				ehs_uint8 nBit = 1 << (7u - (x % 8u));
				if ((nBit & EhsTVSurface_bitmap(pSurface)[nBmpIdx + x/8]) == nBit) {
					pPixels[nPixIdx+x] = xFg;
				} else {
					pPixels[nPixIdx+x].sComp.nRed = 0x0;
					pPixels[nPixIdx+x].sComp.nGreen = 0x0;
					pPixels[nPixIdx+x].sComp.nBlue = 0x0;
					pPixels[nPixIdx+x].sComp.nAlpha = 0x0;
				}
			}
		}
	}
	else {
		for (y = 0; y < prDst->nHeight; y++) {
			ehs_uint32 nPixIdx = y * prDst->nWidth;
			ehs_uint32 nBmpIdx = y * ((pSurface->nWid+7)/8);
			for (x = 0; x < prDst->nWidth; x++) {
				ehs_uint8 nBit = 1 << (7u - (x % 8u));
				if ((nBit & EhsTVSurface_bitmap(pSurface)[nBmpIdx + x/8]) == nBit) {
					pPixels[nPixIdx+x] = xFg;
				} else {
					pPixels[nPixIdx+x] = xBg;
				}
			}
		}
	}

	// Premultiply alphas etc prior to per-pixel alphablend
	for( nPix = 0; nPix < prDst->nHeight*prDst->nWidth; nPix++)
	{
		nAlpha = pPixels[nPix].sComp.nAlpha;

		nVal = pPixels[nPix].sComp.nRed;
		nVal = (nVal * nAlpha) / 255;
		pPixels[nPix].sComp.nRed = nVal;

		nVal = pPixels[nPix].sComp.nGreen;
		nVal = (nVal * nAlpha) / 255;
		pPixels[nPix].sComp.nGreen = nVal;

		nVal = pPixels[nPix].sComp.nBlue;
		nVal = (nVal * nAlpha) / 255;
		pPixels[nPix].sComp.nBlue = nVal;
	}

}
/**
 * Called indirectly by _paint, hence EhsTPMutex_viewport is locked
 * for the duration of this function.
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
	HBITMAP hbmOld;
	HDC hdcBm;
	HBITMAP hbitmap;
	UINT* pPixels;

	/* setup image-specific bitmap info. Common parameters have been set by EhsTV_init */
	pViewport->bmi.bmiHeader.biWidth = prDst->nWidth;
	pViewport->bmi.bmiHeader.biHeight = -prDst->nHeight;
	/* imagesize = width * height * sizeof(ARGB) (=4) */
	pViewport->bmi.bmiHeader.biSizeImage = (prDst->nWidth * prDst->nHeight) << 2;

	/* create our DIB section and select the bitmap into the dc */
	if (hdcBm = CreateCompatibleDC(pViewport->hdc)) {
		if (hbitmap = CreateDIBSection(hdcBm, &(pViewport->bmi), DIB_RGB_COLORS, (void**)&pPixels, NULL, 0x0)) {
			hbmOld = SelectObject(hdcBm, hbitmap);
			if (hbmOld) {

				switch (pSurface->eFormat) {
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
					EhsError(EHS_MSG_TGT_GRAPHICS_UNSUPPORTED_FORMAT("EhsTV_blit",pSurface->eFormat));
					break;
				}

				/* blit in the bitmap */
				if (!AlphaBlend(pViewport->hdc, prDst->nLeft, prDst->nTop,
					prDst->nWidth, prDst->nHeight,
					hdcBm, prSrc->nLeft, prSrc->nTop,
					prSrc->nWidth, prSrc->nHeight, pViewport->bf))
				{
					/*EhsError(EHS_MSG_TGT_WIN32("EhsTV_blit", "AlphaBlend() failed"));*/
				}

				/* Now, clean up. A memory DC always has a drawing
				* surface in it. It is created with a 1X1 monochrome
				* bitmap that we saved earlier, and need to put back
				* before we destroy it.
				*/
				SelectObject(hdcBm,hbmOld);
			}
			DeleteObject(hbitmap);

		}
		DeleteDC(hdcBm);
	}

}

/**
 * Fill a rectangle with a specified colour in the specified viewport.
 * Called indirectly by _paint, hence EhsTPMutex_viewport is locked
 * for the duration of this function.
 *
 * @param[in] pViewport Viewport to fill rectangle into
 * @param[in] pRect position and size of rectangle to fill
 * @param[in] pColour Colour to use for filling rectangle (includes global alpha value)
 */
void EhsTV_fillRect(EhsTVClass* pViewport,
				const EhsGraphicsRectangleClass* pRect, const EhsGraphicsColourClass* pColour)
{
	HBITMAP hbmOld;
	HDC hdcBm;
	HBITMAP hbitmap;
	UINT* pPixels;
	int i;
	ehs_uint16 nX;
	ehs_uint16 nY;
	ehs_uint16 nWidth;
	ehs_uint16 nHeight;

	/* fill only the intersection of the clipping rectangle and the filling rectangle */
	/*if (EhsGraphicsRectangle_intersect(&intersection,&pRect,&(pViewport->viewRect))) { */
	/**@todo use the intersection rectangle here */
	nX = pRect->nLeft;
	nY = pRect->nTop;
	nWidth = pRect->nWidth;
	nHeight = pRect->nHeight;

	/* setup image-specific bitmap info. Common parameters have been set by EhsTV_init */
	pViewport->bmi.bmiHeader.biWidth = nWidth;
	pViewport->bmi.bmiHeader.biHeight = -nHeight;
	pViewport->bmi.bmiHeader.biSizeImage = (nWidth * nHeight) << 2; /* width * height (=1) * 4 (sizeof(ARGB) */

	/* create our DIB section and select the bitmap into the dc */
	if (hdcBm = CreateCompatibleDC(pViewport->hdc)) {
		if (hbitmap = CreateDIBSection(hdcBm, &(pViewport->bmi), DIB_RGB_COLORS, (void**)&pPixels, NULL, 0x0)) {
			EhsGraphicsColourClass xFillColour;
			hbmOld = SelectObject(hdcBm, hbitmap);

			xFillColour.sComp.nRed   = (pColour->sComp.nRed   * pColour->sComp.nAlpha) / 0xff;
			xFillColour.sComp.nGreen = (pColour->sComp.nGreen * pColour->sComp.nAlpha) / 0xff;
			xFillColour.sComp.nBlue  = (pColour->sComp.nBlue  * pColour->sComp.nAlpha) / 0xff;
			xFillColour.sComp.nAlpha = pColour->sComp.nAlpha;

			for (i=0; i < (nWidth*nHeight-1); i++) {
				((EhsGraphicsColourClass*)(pPixels))[i].nUint32  = xFillColour.nUint32;
			}

			/* blit in the bitmap */
			if (!AlphaBlend(pViewport->hdc, nX, nY,
				nWidth, nHeight,
				hdcBm, 0, 0, nWidth, nHeight, pViewport->bf))
			{
				nX=nX;/* @todo an error occurred - should log/record this */
			}

			/* Now, clean up. A memory DC always has a drawing
			* surface in it. It is created with a 1X1 monochrome
			* bitmap that we saved earlier, and need to put back
			* before we destroy it.
			*/
			if (hbmOld) SelectObject(hdcBm,hbmOld); // only swap back if the bitmap was successful
			DeleteObject(hbitmap);

		}
		DeleteDC(hdcBm);
	}

}

/**
 * Paint all of the current EHS objects that need painting. Called by
 * main "Windows" thread.
 *
 * @param hWnd Target window hook
 * @param lpPS long pointer to PAINTSTRUCT containing display device context and other data (win_GDI)
 */
void EhsTV_paint(HWND hWnd, LPPAINTSTRUCT lpPS)
{
	RECT rc; /* client rectangle */
	HBITMAP hbmMem; /* bitmap containing the client rectangle */
	HBITMAP hbmOld; /* ummm... Windows magic - need to keep the old device context to restore later, apparently */
	EhsGraphicsRectangleClass clip;

	GetClientRect(hWnd, &rc); /* get target paint area coordinates from window hook */

	/* this section restructured July 08 by N Rutherford to comply with MISRA 2004 rule 13.1 */

	/* create memory device context (i.e. a virtual copy; double buffering?) compatible with application's current screen */
	EhsTV.hdc = CreateCompatibleDC(lpPS->hdc);

	if (EhsTV.hdc != NULL) {
		/* create a bitmap that can hold the client rectangle */
		hbmMem = CreateCompatibleBitmap(lpPS->hdc, rc.right-rc.left,rc.bottom-rc.top);
		if (hbmMem != NULL) {
			/* select the bitmap into the off-screen DC (which replaces the previous bitmap) and store the old one (which is its return value)*/
			/* must select something into EhsTV.hdc before being able to use it, as prior to this it consists of a 1x1 monochromatic bitmap (WinGDI:CreateCompatibleDC(HDC)) */
			hbmOld = SelectObject(EhsTV.hdc, hbmMem);
			if (hbmOld != NULL) { /* N.B. this checks that things are going smoothly, NOT that the previous bitmap was NULL (it is 1x1 monochrome as default) */
				if (EhsTV.init)
				{
					clip.nLeft = rc.left;
					clip.nTop = rc.top;
					clip.nWidth = rc.right - rc.left;
					clip.nHeight = rc.bottom - rc.top;
					/* resource locking is handled within EhsWidgetTable_draw */
					EhsWidgetTable_draw(&EhsWidgetTable,&EhsTV, &clip);

					BitBlt(lpPS->hdc,
						rc.left, rc.top,
						rc.right-rc.left, rc.bottom-rc.top,
						EhsTV.hdc,
						0, 0,
						SRCCOPY);

				}
				SelectObject(EhsTV.hdc, hbmOld); /* do we need this we're going to delete .hdc next?? It also isn't a drawing object which is warned against deletion during use by a dc in msdn */

			} /* hbmem context OK */
			DeleteObject(hbmMem); /*clear client rectangle bitmap*/
		}/* compatible bitmap OK */
		DeleteDC(EhsTV.hdc); /* delete buffer device context */
		EhsTV.hdc = NULL;
	} /* compatible context OK */
}

/**
 * Convert EHS specific colour type to windows specific colour type
 */
COLORREF EhsTV_getColour(EhsGraphicsColourClass xColour)
{
	return RGB(xColour.sComp.nRed, xColour.sComp.nGreen, xColour.sComp.nBlue);
}

/**
 * Free memory used by a surface. Assumes that this function is never called for memory
 * allocated when bTemporary = FALSE in Surface_create
 */
void EhsTVSurface_destroy(EhsTVClass* pViewport, EhsTVSurfaceClass *pSurface)
{
	switch (pSurface->eFormat) {
	case EHS_GRAPHICS_COLOUR_A1:
	case EHS_GRAPHICS_COLOUR_A8:
		EhsHMem_tempFree(EhsTVSurface_pixels(pSurface));
		EhsTVSurface_bitmap(pSurface) = NULL;
		EhsHMem_tempFree(pSurface->pPalette);
		pSurface->pPalette = NULL;
		break;
	case EHS_GRAPHICS_COLOUR_ARGB8888:
		EhsHMem_tempFree(EhsTVSurface_pixels(pSurface));
		EhsTVSurface_pixels(pSurface) = NULL;
	}
	EhsHMem_tempFree(pSurface);
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

	if (pSurface) {
		pSurface->nWid = nWidth;
		pSurface->nHt = nHeight;
		pSurface->eFormat = eFormat;

		switch (eFormat) {
		case EHS_GRAPHICS_COLOUR_A1:
		case EHS_GRAPHICS_COLOUR_A8:
			nPixelsPerByte = (eFormat == EHS_GRAPHICS_COLOUR_A1)?8u:1u;
			/* calculate number of bytes required = (nWidth/(number of pixels per byte) rounded up) * nHeight */
			//if (bTemporary) {
				EhsTVSurface_bitmap(pSurface) = (EhsGraphicsColourClass*)EhsHMem_tempAlloc((nWidth+(nPixelsPerByte-1))/nPixelsPerByte*nHeight);
			//} else {
				EhsTVSurface_bitmap(pSurface) = (EhsGraphicsColourClass*)EhsHMem_writeableAlloc((nWidth+(nPixelsPerByte-1))/nPixelsPerByte*nHeight);
			//}
			if (!EhsTVSurface_pixels(pSurface)) {// todo looks wrong
				//if (bTemporary) {
					EhsHMem_tempFree(pSurface);
				//}
				pSurface = NULL;
			} else {
				/* clear bitmap */
				EhsMemset(EhsTVSurface_bitmap(pSurface),0,(nWidth+(nPixelsPerByte-1))/nPixelsPerByte*nHeight);
				/* create space for palette */
				//if (bTemporary) {
					pSurface->pPalette = (EhsGraphicsColourClass*)EhsHMem_tempAlloc(nPaletteSize*sizeof(EhsGraphicsColourClass));
				//} else {
				//	pSurface->pPalette = (EhsGraphicsColourClass*)EhsHMem_writeableAlloc(nPaletteSize*sizeof(EhsGraphicsColourClass));
				//}
				if (!pSurface->pPalette) {
					//if (bTemporary) {
					EhsHMem_tempFree(EhsTVSurface_bitmap(pSurface));
					EhsHMem_tempFree(pSurface);
					//}
					pSurface = NULL;
				}
			}
			if (pSurface) {
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
			EhsError(EHS_MSG_TGT_GRAPHICS_UNSUPPORTED_FORMAT("EhsTVSurface_create", eFormat));
			//if (bTemporary) {
				EhsHMem_tempFree(pSurface);
			//}
			pSurface = NULL;
		}
	}

	return pSurface;
}

/*Change size and position of viewport*/
void EhsTV_move(EhsTVClass* pViewport, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nDeltaWid, EhsDataflowIntType nDeltaHt)
{
 /*Dummy function for the moment*/
}

/*Change alpha value of viewport*/
void EhsTV_fade(EhsTVClass* pViewport, ehs_uint32 nColour)
{
/*Dummy function*/
}

void EhsTV_hideViewport()
{
	/*Dummy function*/
}

void EhsTV_showViewport(ehs_uint16 zorder)
{
	/*Dummy function*/
}

