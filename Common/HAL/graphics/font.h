/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file font.h
 * Supports the use of fonts derived from BDF files
 *
 * @author: inx limited
 *
 */

#ifndef EHS_GRAPHICS_FONT_H_
#define EHS_GRAPHICS_FONT_H_

/*****************************************************************************/
/* Included files */
#include "globals.h"
#include "graphics_types.h"

//#include "hal_viewport.h"



/*****************************************************************************/
/* Define macros  */

/**
 * Default font for EHS
 */
#define EHSHG_FONT_DEFAULT "default.bdf"

/*****************************************************************************/
/* Define types */

/**
 * Represents a single item (glyph) from the font
 */
typedef struct
{
    ehs_uint32 nId; 		/**< Identifies the glyph */
    ehs_uint16 nGlyphWid; 	/**< Width of the glyph */
    ehs_uint16 nGlyphHt; 	/**< Height of the glyph */
    ehs_uint16 nPixWid;		/**< Width of the pixel data supplied in xPixels */
    ehs_uint16 nPixHt;  	/**< Height of the pixel data supplied in xPixels */
    ehs_uint16 nBytesPerRow;	/**< Number of bytes required to represent a single row of pixels */
    ehs_sint16 nXOff;		/**< X-offset from the current drawing origin (positive = left) */
    ehs_sint16 nYOff;		/**< Y-offset from the current drawing origin (positive = up) */
    ehs_uint8* xPixels;		/**< pixel by pixel data arranged as nHt rows of (nWid/8) bytes */
} EhsGraphicsFontGlyphClass;

/**
 * Represents a complete collection of glyphs
 */
typedef struct
{
    ehs_uint32 nNumGlyphs; 	/**< Number of glphys read in so far */
    ehs_uint16 nMaxWid;		/**< Largest width glyph */
    ehs_uint16 nMaxHt;		/**< Largest height glpyh */
    ehs_char* szName;		/**< Name of the font */
    ehs_char szFilename[EHS_FILESTRING_SIZE];	/**< Filename of the font */
    EhsGraphicsFontGlyphClass* pGlyphs;			/**< Points to the array of glyphs */
    const EhsGraphicsFontGlyphClass* pDefaultGlyph;	/**< Used in to substitute characters not found */
} EhsGraphicsFontClass;

/**
 * Represents a mapping of file names to fonts
 */
typedef struct
{
    ehs_uint16 nNumFonts;			/**< True if a font has been loaded into this entry */
    EhsGraphicsFontClass xFont[EHS_MAX_FONTS];	/**< Font that's been loaded */
} EhsGraphicsFontTableClass;

/*****************************************************************************/
/* Declare global variables */

/**
 * Contains all fonts defined for application
 */
EHS_EXTERN EhsGraphicsFontTableClass EhsGraphicsFontTable;

/*****************************************************************************/
/* Declare function prototypes  */

/**
 * Draw a glyph onto a 1 bit depth bitmap. Attempting to draw outside the bitmap causes
 * the function to return false (no character is drawn).
 *
 * @param[in] pGlyph Glyph to draw
 * @param[in,out] pBitmap Bitmap to draw glyph onto
 * @param[in] nWid Width of the bitmap (pixels)
 * @param[in] nHt Height of the bitmap (pixels)
 * @param[in] nTxtWid Width of the text drawing area (pixels). Must be less than or equal to nWid
 * @param[in] nTxtHt Height of the text drawing area (pixels). Must be less than or equal to nHt
 * @param[in,out] pnX Start drawing position of next character - leftmost point (advanced after draw operation)
 * @param[in,out] pnY Start drawing position of next character - bottom-most point(advanced after draw operation)
 * @return true if the character was successfully drawn in the bitmap, false if it would have overflowed.
 */
ehs_bool EhsGraphicsFontGlyph_draw(const EhsGraphicsFontGlyphClass* pGlyph, ehs_uint8* pBitmap, ehs_uint32 nWid, ehs_uint32 nHt, ehs_uint32 nTxtWid, ehs_uint32 nTxtHt, ehs_uint32* pnX, ehs_uint32* pnY);

/**
 * Finds a glyph from a character value. Characters are utf-32 (unicode) characters.
 * @param pFont Font to search for glyph
 * @param nChar Character to search for
 * @return Pointer to the glyph, or NULL if character cannot be found.
 */
const EhsGraphicsFontGlyphClass* EhsGraphicsFont_findGlyph(EhsGraphicsFontClass* pFont, ehs_uint32 nChar);

/**
 * Load a font from a BDF file.
 * @param[in] szFilename Name of the BDF font to load
 * @return pointer to the font that has been loaded. Null if loading failed
 */
EhsGraphicsFontClass* EhsGraphicsFont_load(ehs_char* szFilename);

/**
 * Update the X and Y positions to reflect a line break
 * @param[in] pFont Font used for the line break
 * @param[in] pTextBoxRect rectangle in which the bounding box appears
 * @param[in] nLineSep Line separation - distance to do the new line
 * @param[in,out] pnX Position of drawing point
 * @param[in,out] pnY Position of drawing point
 * @param[in] nIndentLeft - Position of left-most character
 * @return true if we haven't gone off the bottom of the textbox
 */
ehs_bool EhsGraphicsFont_newLine(const EhsGraphicsFontClass* pFont,
        const EhsGraphicsRectangleClass* pTextBoxRect, ehs_uint16 nLineSep,
        ehs_uint32* pnX, ehs_uint32* pnY, ehs_uint16 nIndentLeft);

/**
 * Draw a word onto a bitmap, performs word wrap if possible, otherwise prints
 * ellipsis (...)
 * @param[in] pFont Font to draw word with
 * @param[in,out] pSurface surface to draw the word onto
 * @param[in] pSurfaceRect Rectangle in which the surface appears
 * @param[in] pTextBoxRect Rectangle representing bounding box in which text
 * appears (should be smaller than pBitmapRect)
 * @param[in,out] pnX Start drawing position of the next character - leftmost
 *  point (advanced after draw operation)
 * @param[in,out] pnY Start drawing position of the next character - topmost
 *  point (advanced after draw operation)
 * @param[in] nLineSep Distance to separate lines if a word wrap is required
 * @param[in] nIndentLeft - Position of left-most character
 * @param[in] szWord Word to write - utf8 string
 * @return true if word was drawn onto the bitmap
 */
ehs_bool EhsGraphicsFont_drawWord(EhsGraphicsFontClass* pFont,
        EhsTVSurfaceClass* pSurface,
        const EhsGraphicsRectangleClass* pSurfaceRect,
        const EhsGraphicsRectangleClass* pTextBoxRect,
        ehs_uint32* pnX, ehs_uint32* pnY, ehs_uint16 nLineSep,
        ehs_uint16 nIndentLeft, const ehs_char* szWord);

/**
 * Initialise the table of fonts
 */
void EhsGraphicsFontTable_init();

#endif /*EHS_GRAPHICS_FONT_H_*/
