/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file graphics.h
 * Declarations for the general-purpose graphics functions and types.
 *
 * @author: inx limited
 *
 */
#ifndef EHS_GRAPHICS_H
#define EHS_GRAPHICS_H

#include "ehs_types.h"
#include "target_config.h"
#include "target_file.h"
#include "graphics_types.h"
//#include "font.h"
#include "targetgfx_init.h"


/**
 * Create a colour structure from individual red, green, blue components
 */
EHS_GLOBAL EhsGraphicsColourClass EhsGraphicsColour_rgb(ehs_uint8 r, ehs_uint8 g, ehs_uint8 b);

/**
 * Create a colour structure from individual red, green, blue, alpha components
 */
EHS_GLOBAL EhsGraphicsColourClass EhsGraphicsColour_argb(ehs_uint8 r, ehs_uint8 g, ehs_uint8 b, ehs_uint8 alpha);


/**
 * True if two rectangles overlap
 */
EHS_GLOBAL ehs_bool EhsGraphicsRectangle_overlap(const EhsGraphicsRectangleClass* r1, const EhsGraphicsRectangleClass* r2);

/**
 * Returns the intersection of two rectangles
 *
 * @param[out] ret Contains the intersection of the two rectangles
 * @param[in] r1 First rectangle
 * @param[in] r2 Second rectangle
 * @return false if no intersection exists
 */
EHS_GLOBAL ehs_bool EhsGraphicsRectangle_intersect(EhsGraphicsRectangleClass* ret, const EhsGraphicsRectangleClass* r1, const EhsGraphicsRectangleClass* r2);

/**
 * Returns the union of two rectangles
 *
 * @param[out] ret Contains the union of the two rectangles
 * @param[in] r1 First rectangle
 * @param[in] r2 Second rectangle
 */
EHS_GLOBAL void EhsGraphicsRectangle_union(EhsGraphicsRectangleClass* ret, const EhsGraphicsRectangleClass* r1, const EhsGraphicsRectangleClass* r2);

/**
 * Finds the rectangle that allows one rectangle to fit within another while keeping its original proportions
 *
 * @param[out] prRet Contains the largest rectangle that fits within prBounds, but has the proportions of prSrc
 * @param[in] prBounds Specifies the rectangle that prRet must fit into.
 * @param[in] prSrc Specifies the proportions that we wish to keep
 */
EHS_GLOBAL void EhsGraphicsRectangle_proportionalScale(EhsGraphicsRectangleClass* prRet, const EhsGraphicsRectangleClass* prBounds, const EhsGraphicsRectangleClass* prSrc);

/**
 * Perform setup before loading in a new application
 */
EHS_GLOBAL void EhsTGfxSys_init();
/**
 * Perform necessary target setup per application execution
 */
EHS_GLOBAL void EhsTGfxApp_reset(void);
/**
 * Called to clean up after the execution of an application in order to make
 * sure everything has been stopped.
 */
EHS_GLOBAL void EhsTGfxApp_term(void);


/* Call to initialise the global graphics system */
EHS_GLOBAL void EhsTGfxApp_init(void);


/* File format handlers */
typedef enum
{
    EHS_GFXFILETYPE_UNKNOWN = 0,
    EHS_GFXFILETYPE_PNG     = 1,
    EHS_GFXFILETYPE_JPG	    = 2,
    EHS_GFXFILETYPE_GIF		= 3
} EhsGfxFileType ;

typedef enum
{
    EHS_GFXFILEORIENTATION_UNKNOWN     = 0,
    EHS_GFXFILEORIENTATION_TOPLEFT    = 1,
    EHS_GFXFILEORIENTATION_TOPRIGHT   = 2,
    EHS_GFXFILEORIENTATION_BOTLEFT_   = 3,
    EHS_GFXFILEORIENTATION_BOTRIGHT   = 4,
    EHS_GFXFILEORIENTATION_LEFTTOP    = 5,
    EHS_GFXFILEORIENTATION_RIGHTTOP    =6,
    EHS_GFXFILEORIENTATION_LEFTBOT    = 7,
    EHS_GFXFILEORIENTATION_TIGHTBOT    = 8
} EhsGfxFileOrientation;


ehs_bool readExif( ehs_FILE * file, EhsGfxFileOrientation * orientation);
/*** checks png and jpeg headers and may return orientation data
 * filetype check is the type of file you are expecting:
 *
 * * filetypecheck:
 * 0  -  no header check (just file existence and orintation
 * 1  - PNG header check
 * 2  - JPG header check (@todo not implemented)
 * 4  - GIF header check (@todo not implemented)
 *
 * returns *orientation (jpegs only currently):
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
 * returns success or fail & orientation defaults -1 if no information is available)
 */



ehs_bool doCheckFileHeader(const char* Path, ehs_bool bLoadImageFromAppDir, EhsGfxFileType filetypecheck, EhsGfxFileOrientation * orientation);

#endif /* EHS_GRAPHICS_H */
