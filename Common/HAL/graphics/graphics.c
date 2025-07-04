/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file graphics.c
 * Definitions for the general-purpose graphics functions and types.
 *
 * @author: inx limited
 *
 */

#include "graphics.h"
#include "hal-api.h"

/*  @todo move file stuff somewhere else */
#ifndef EHS_GUI_SUPPORT_MODE_B
#include "png.h"
#endif
/**
 * Create a colour structure from individual red, green, blue components
 */
EhsGraphicsColourClass EhsGraphicsColour_rgb(ehs_uint8 r, ehs_uint8 g, ehs_uint8 b)
{
    EhsGraphicsColourClass c;
    c.sComp.nAlpha = (ehs_uint8)0xff;
    c.sComp.nRed = r;
    c.sComp.nGreen = g;
    c.sComp.nBlue = b;

    return c;
}

/**
 * Create a colour structure from individual red, green, blue, alpha components
 */
EhsGraphicsColourClass EhsGraphicsColour_argb(ehs_uint8 r, ehs_uint8 g, ehs_uint8 b,ehs_uint8 alpha)
{
    EhsGraphicsColourClass c;
    c.sComp.nAlpha = alpha;
    c.sComp.nRed = r;
    c.sComp.nGreen = g;
    c.sComp.nBlue = b;

    return c;
}

/**
 * True if two rectangles overlap
 */
ehs_bool EhsGraphicsRectangle_overlap(const EhsGraphicsRectangleClass* r1, const EhsGraphicsRectangleClass* r2)
{
    ehs_bool bRet;

    if (((r1->nLeft + r1->nWidth) < r2->nLeft) ||	/* r1 is completely to the left of r2 */
            ((r2->nLeft + r2->nWidth) < r1->nLeft) ||	/* r2 is completely to the left of r1 */
            ((r1->nTop + r1->nHeight) < r2->nTop) ||	/* r1 is completely above r2 */
            ((r2->nTop + r2->nHeight) < r1->nTop))		/* r2 is completely above r1 */
    {
        bRet = EHS_FALSE;
    }
    else
    {
        bRet = EHS_TRUE;
    }

    return bRet;
}

/**
 * Returns the intersection of two rectangles
 *
 * @param[out] ret Contains the intersection of the two rectangles
 * @param[in] r1 First rectangle
 * @param[in] r2 Second rectangle
 * @return false if rectangles don't intersect
 */
ehs_bool EhsGraphicsRectangle_intersect(EhsGraphicsRectangleClass* ret, const EhsGraphicsRectangleClass* r1, const EhsGraphicsRectangleClass* r2)
{
    ehs_sint32 r1Rt, r2Rt, r1Btm, r2Btm; /* calculated RHS and bottom edges of rectangle r1, r2 */
    ehs_sint32 maxLeft, maxTop; /* calculated edge of intersected rectangle */
    ehs_bool bRet = EHS_FALSE;

    if (EhsGraphicsRectangle_overlap(r1,r2))
    {
        /* you could save 4 bytes and a few cycles using ret->nLeft and ret->nTop instead of maxLeft, maxTop,
         * but in the case where r1 == ret or r2 == ret, this has side effects
         */
        maxLeft = (r1->nLeft > r2->nLeft)?r1->nLeft:r2->nLeft;
        maxTop  = (r1->nTop  > r2->nTop )?r1->nTop :r2->nTop ;
        r1Rt = (r1->nLeft + r1->nWidth) - maxLeft;
        r2Rt = (r2->nLeft + r2->nWidth) - maxLeft;
        r1Btm  = (r1->nTop  + r1->nHeight) - maxTop;
        r2Btm  = (r2->nTop  + r2->nHeight) - maxTop;

        ret->nWidth = (r1Rt < r2Rt)?r1Rt:r2Rt;
        ret->nHeight = (r1Btm < r2Btm)?r1Btm:r2Btm;
        ret->nLeft = maxLeft;
        ret->nTop = maxTop;
        bRet = EHS_TRUE;
    }

    return bRet;
}

/**
 * Returns the union of two rectangles
 *
 * @param[out] ret Contains the union of the two rectangles
 * @param[in] r1 First rectangle
 * @param[in] r2 Second rectangle
 */
void EhsGraphicsRectangle_union(EhsGraphicsRectangleClass* ret, const EhsGraphicsRectangleClass* r1, const EhsGraphicsRectangleClass* r2)
{
    ehs_sint32 r1Rt, r2Rt, r1Btm, r2Btm; /* calculated RHS and bottom edges of rectangle r1, r2 */
    ehs_sint32 minLeft, minTop; /* minimum values for top and left */

    /* you could use ret->nLeft and ret->nTop instead of minLeft, minTop, but in the
     * case where r1 == ret or r2 == ret, this has side effects
     */
    minLeft = (r1->nLeft < r2->nLeft)?r1->nLeft:r2->nLeft;
    minTop  = (r1->nTop  < r2->nTop )?r1->nTop :r2->nTop ;
    r1Rt = (r1->nLeft + r1->nWidth) - minLeft;
    r2Rt = (r2->nLeft + r2->nWidth) - minLeft;
    r1Btm  = (r1->nTop  + r1->nHeight) - minTop;
    r2Btm  = (r2->nTop  + r2->nHeight) - minTop;

    ret->nWidth = (r1Rt > r2Rt)?r1Rt:r2Rt;
    ret->nHeight = (r1Btm > r2Btm)?r1Btm:r2Btm;
    ret->nLeft = minLeft;
    ret->nTop = minTop;
}

/**
 * Finds the rectangle that allows one rectangle to fit within another while keeping its original proportions
 *
 * @param[out] prRet Contains the largest rectangle that fits within prBounds, but has the proportions of prSrc
 * @param[in] prBounds Specifies the rectangle that prRet must fit into.
 * @param[in] prSrc Specifies the proportions that we wish to keep
 */
void EhsGraphicsRectangle_proportionalScale(EhsGraphicsRectangleClass* prRet, const EhsGraphicsRectangleClass* prBounds, const EhsGraphicsSizeClass* prSrc)
{
    ehs_float dWHbounds, dWHsrc; /* ratio of wid:ht for the two input rectangles */

    dWHbounds = (ehs_float)prBounds->nWidth/(ehs_float)prBounds->nHeight;
    dWHsrc = (ehs_float)prSrc->nWidth/(ehs_float)prSrc->nHeight;

    if (dWHbounds > dWHsrc)
    {
        /* source rectangle is taller/thinner than bounds.
         * Reduce bounds width and adjust left offset to fit it */
        prRet->nHeight = prBounds->nHeight;
        prRet->nTop = prBounds->nTop;
        /* the resulting width is height scaled by the ratio of source width:height,
         * e.g scaling a source box 100:300 into a 75:75 box, gives an
         * output box 75 high, 25 wide [=75 * 100/300], with an offset
         * of 25 [=(75-25)/2] */
        prRet->nWidth = (ehs_uint16)(prBounds->nHeight * dWHsrc);
        prRet->nLeft = prBounds->nLeft + (prBounds->nWidth - prRet->nWidth)/2;

    }
    else
    {
        /* source rectangle is wider than bounds.
         * Reduce bounds height and adjust top offset to fit it */
        prRet->nLeft = prBounds->nLeft;
        prRet->nWidth = prBounds->nWidth;
        /* the resulting height is width scaled by the ratio of source width:height
         * e.g. scaling a source box 200:50 into a 80:80 box, gives an output
         * box 80 wide, 20 high [=80 / (200/50)], with an offset of 30 [=(80-20)/2] */
        prRet->nHeight = (ehs_uint16)(prBounds->nWidth / dWHsrc);
        prRet->nTop = prBounds->nTop + (prBounds->nHeight - prRet->nHeight)/2;

    }
}

/************ Grahocs file format functions ******************************/
/* @todo REFACTOR: consider putting these in a separate file */

/* Exif meta data reader */

static ehs_bool ExifRead_word ( FILE * file, unsigned int * word)
{
    int c1, c2;
    if ((c1 = getc(file)) == EOF) return EHS_FALSE;
    if ((c2 = getc(file)) == EOF) return EHS_FALSE;
    *word = (((ehs_uint16) c1) << 8) + ((ehs_uint16) c2);
    return EHS_TRUE;
}


/*
 * Read orientation from Exif data in jpeg (or PNG file?) .
 * returns orientation in orientation_flag  parameter:
 * Value | 0th Row     | 0th Column
 * ------+-------------+-----------
 *   1   | top         | left
 *   2   | top         | right
 *   3   | bottom      | right
 *   4   | bottom      | left
 *   5   | left        | top
 *   6   | right       | top
 *   7   | right       | bottom
 *   8   | left        | bottom
 *
 * returns success or fail (orientation is also set -1 for an error)
 *
 */

ehs_bool readExif( ehs_FILE * file, EhsGfxFileOrientation * orientation)
{

    unsigned int length, i;
    int is_motorola;
    unsigned int offset, number_of_tags, tagnum;
    unsigned char *exif_data = NULL;
    int tempgetchar;
    int orientation_flag = 0;
    *orientation = 0;

    if (file == NULL ) return EHS_FALSE;
    exif_data = EhsHMem_tempAlloc(0xFFFF); // Assume all the data is this near the header
    if (exif_data == NULL ) return EHS_FALSE;

    /* Read File head, check for JPEG SOI + Exif APP1 */
    for (i = 0; i < 4; i++)
    {
        tempgetchar = EhsFgetc(file);
        if ( tempgetchar== EOF) goto error;
        else exif_data[i] = (unsigned char)tempgetchar;
    }
    if (exif_data[0] != 0xFF ||
            exif_data[1] != 0xD8 ||
            exif_data[2] != 0xFF ||
            ( exif_data[3] != 0xE1  && exif_data[3] != 0xE0))
    {
        goto error;
    }

    /* Get the marker parameter length count */
    if (ExifRead_word(file,&length) == EHS_FALSE)
    {
        goto error;
    }
    /* Length includes itself, so must be at least 2 */
    /* Following Exif data length must be at least 6 */
    if (length < 8 || length >= 0xFFFF)
        goto error;
    length -= 8;
    /* Read Exif head, check for "Exif" */
    for (i = 0; i < 6; i++)
    {
        if ( (tempgetchar = EhsFgetc(file)) == EOF ) goto error;
        exif_data[i] = (unsigned char)  tempgetchar;

        if (exif_data[0] != 0x45 ||
                exif_data[1] != 0x78 ||
                exif_data[2] != 0x69 ||
                exif_data[3] != 0x66 ||
                exif_data[4] != 0 ||
                exif_data[5] != 0)
            return 0;
        /* Read Exif body */
        for (i = 0; i < length; i++)
            if ( ( tempgetchar = EhsFgetc(file) ) == EOF) goto error;
            else  exif_data[i] = (unsigned char)  tempgetchar;

        if (length < 12) goto error; /* Length of an IFD entry */

        /* Discover byte order */
        if (exif_data[0] == 0x49 && exif_data[1] == 0x49)
            is_motorola = 0;
        else if (exif_data[0] == 0x4D && exif_data[1] == 0x4D)
            is_motorola = 1;
        else
            goto error;

        /* Check Tag Mark */
        if (is_motorola)
        {
            if (exif_data[2] != 0) goto error;
            if (exif_data[3] != 0x2A) goto error;
        }
        else
        {
            if (exif_data[3] != 0) goto error;
            if (exif_data[2] != 0x2A) goto error;
        }

        /* Get first IFD offset (offset to IFD0) */
        if (is_motorola)
        {
            if (exif_data[4] != 0) goto error;
            if (exif_data[5] != 0) goto error;
            offset = exif_data[6];
            offset <<= 8;
            offset += exif_data[7];
        }
        else
        {
            if (exif_data[7] != 0) goto error;
            if (exif_data[6] != 0) goto error;
            offset = exif_data[5];
            offset <<= 8;
            offset += exif_data[4];
        }
        if (offset > length - 2) goto error; /* check end of data segment */

        /* Get the number of directory entries contained in this IFD */
        if (is_motorola)
        {
            number_of_tags = exif_data[offset];
            number_of_tags <<= 8;
            number_of_tags += exif_data[offset+1];
        }
        else
        {
            number_of_tags = exif_data[offset+1];
            number_of_tags <<= 8;
            number_of_tags += exif_data[offset];
        }
        if (number_of_tags == 0) goto error;
        offset += 2;

        /* Search for Orientation Tag in IFD0 */
        for (;;)
        {
            if (offset > length - 12) goto error; /* check end of data segment */
            /* Get Tag number */
            if (is_motorola)
            {
                tagnum = exif_data[offset];
                tagnum <<= 8;
                tagnum += exif_data[offset+1];
            }
            else
            {
                tagnum = exif_data[offset+1];
                tagnum <<= 8;
                tagnum += exif_data[offset];
            }
            if (tagnum == 0x0112) break; /* found Orientation Tag */
            if (--number_of_tags == 0) goto error;
            offset += 12;
        }


        /* Get the Orientation value */
        if (is_motorola)
        {
            if (exif_data[offset+8] != 0) goto error;
            orientation_flag = exif_data[offset+9];
        }
        else
        {
            if (exif_data[offset+9] != 0) goto error;
            orientation_flag = exif_data[offset+8];
        }
        if (orientation_flag > 8) goto error;
    }

    *orientation = orientation_flag;

    /* All done. */
    EhsHMem_tempFree(exif_data);
    return EHS_TRUE;
error:
    EhsHMem_tempFree(exif_data);
    return EHS_FALSE;
}


/*** @brief Checks the file header for valid characters for the type give and also checks for xif data
 *
 *
 * filetypecheck:
 * 0  -  no header check (just file existence and orintation
 * 1  - PNG header check
 * 2  - JPG header check (@todo not implemented)
 * 4  - GIF header check (@todo not implemented)
 *
 *
 *returns true of header is OK, and xif orientation as:
 *  * Value | 0th Row     | 0th Column
 * ------+-------------+-----------
 *   1   | top         | left
 *   2   | top         | right
 *   3   | bottom      | right
 *   4   | bottom      | left
 *   5   | left        | top
 *   6   | right       | top
 *   7   | right       | bottom
 *   8   | left        | bottom
 *  -1   | if no exif header was not found
 * */


ehs_bool doCheckFileHeader(const char* Path, ehs_bool bLoadImageFromAppDir, EhsGfxFileType filetypecheck, EhsGfxFileOrientation * orientation)
{
    int result = -1; // assume failure, 0 is a match
    
    #ifndef EHS_GUI_SUPPORT_MODE_B
    const int nread = 8;
    unsigned char buffer[8];
    #endif

    *orientation = 0;

    ehs_FILE* file;
    if (bLoadImageFromAppDir)
    {
        file = Ehs_AppFopen(Path, "rb");
    }
    else
    {
        file = Ehs_UserFopen(Path, "rb");
    }

    // unable to open
    if (file == 0)
    {
        EHSH_LOG_ERROR(EHS_MSG_ERROR_FILE_NOT_OPEN(Path));
    }
    else
    {
        switch (filetypecheck)
        {
        case 0:   // don't check
            result = EHS_GFXFILETYPE_UNKNOWN;
            break;
        case EHS_GFXFILETYPE_PNG:   // PNG header check
            result = 0;
#ifndef EHS_GUI_SUPPORT_MODE_B
            if (EhsFread(buffer, 1, nread, file) == nread)	result = png_sig_cmp(buffer, 0, nread);
#endif
            break;
        case EHS_GFXFILETYPE_JPG:  // JPG check
            EHSH_LOG_WARNING("Did not check PNG header validity as requested");
            result = readExif(file, orientation); // only checks fr=or orientation @todo we could get this to check validity (using the Motorola check!!)
            break;
        case EHS_GFXFILETYPE_GIF:  // GIF check
            result =0;
            EHSH_LOG_WARNING("Did not check GIF header validity as requested"); // @todo need non exif tag testing
            break;
        default:
            break;
        }

        EhsFclose(file);
    }
    return(result == 0);
}
