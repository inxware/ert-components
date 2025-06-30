/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

#ifndef EHS_GRAPHICS_TYPES_H
#define EHS_GRAPHICS_TYPES_H

#include "ehs_types.h"
#include "target_config.h"

/**
 * Colours
 */
/*lint  -e960 acceptable use for a union - packing and unpacking data */
typedef union
{
    unsigned int nUint32 ;//__attribute__ ((aligned (4)));
    struct
    {
#ifdef EHS_GFXTARGET_RGBA
        ehs_uint8 nRed;
        ehs_uint8 nGreen;
        ehs_uint8 nBlue;
        ehs_uint8 nAlpha;
#else
#ifdef EHS_TARGET_BIGENDIAN
        ehs_uint8 nAlpha;
        ehs_uint8 nRed;
        ehs_uint8 nGreen;
        ehs_uint8 nBlue;

#else
        unsigned char nBlue ;//__attribute__ ((aligned (1)));
        unsigned char nGreen ;//__attribute__ ((aligned (1)));;
        unsigned char nRed ;//__attribute__ ((aligned (1)));;
        unsigned char nAlpha ;//__attribute__ ((aligned (1)));;
#endif /* EHS_TARGET_BIGENDIAN */
#endif
    } __attribute__((packed)) sComp;	/**< Component view of the alpha RGB value of colour */

} __attribute__((packed)) EhsGraphicsColourClass ;
/*lint +e960*/


/**
 * Permitted representations of colour pixels
 */
typedef enum
{
    EHS_GRAPHICS_COLOUR_ARGB8888,
    EHS_GRAPHICS_COLOUR_A8,
    EHS_GRAPHICS_COLOUR_A1,
    TARGET_SPECIFIC
} EhsGraphicsColourFormatEnum;

#define EHS_GRAPHICS_COLOUR_ARGB8888_BYTES 4
#define 	EHS_GRAPHICS_COLOUR_A8_BYTES 1
//#define 	EHS_GRAPHICS_COLOUR_A1_BYTES (1/8) /* Is this useful/correct? */


/**
 * Regions
 */
#ifdef EHS_COORD_16_ENABLED
typedef ehs_sint16 ehs_coord;
#else
typedef ehs_sint32 ehs_coord;
#endif

typedef struct
{
    ehs_coord nWidth;		/**< number of pixels across this rect */
    ehs_coord nHeight;		/**< Number of pixels vertically for this rect */
} EhsGraphicsSizeClass;

typedef struct
{
    ehs_coord nLeft;		/**< Pixels from left of screen to this rect. */
    ehs_coord nWidth;		/**< number of pixels across this rect */
    ehs_coord nTop;		/**< Number of pixels from top of this screen to top of this rect */
    ehs_coord nHeight;		/**< Number of pixels vertically for this rect */
} EhsGraphicsRectangleClass;

/*
 * Surfaces
 */
/* This Taget view surface is internally target specific. */
typedef struct EhsTVSurfaceStruct EhsTVSurfaceClass;

#endif
