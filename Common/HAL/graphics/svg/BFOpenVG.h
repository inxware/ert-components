/*
 * Filename : BFOpenVG.h
 *
 *  Version : 6.4.0_2271s
 *     Date : 20080508-20.49.08
 *
 *
 * (c) COPYRIGHT 2002- 2008 Open Text Corp. - BitFlash Division. ALL RIGHTS RESERVED
 *
 *
 * This software is the property and confidential information of Open Text and is furnished under
 * license by BitFlash Inc.  This software may be used only in accordance with the terms of said license.
 *
 * This software may not be copied, transmitted, provided to or otherwise made available to any other person,
 * company, corporation or other entity except as specified in the terms of said license.
 *
 * This copyright notice may not be removed or altered without the prior written permission of Open Text.
 *
 *
 */

#ifndef BF_OPENVG_H
#define BF_OPENVG_H

#ifdef __cplusplus
BF_BEGIN_EXTERN_C
#endif

/** \cond OVG_INTERNAL_SHOW_DOXYGEN */

typedef struct BF__OpenVG*  BFOpenVG;

#if ( defined( BF_VGFLOAT_AS_FLOAT ) && defined( BF_VGFLOAT_AS_FIXED_POINT ) ) || \
    ( !defined( BF_VGFLOAT_AS_FLOAT ) && !defined( BF_VGFLOAT_AS_FIXED_POINT ) )
#error must define VGFloat as either float or fixed-point
#endif

#if defined( BF_VGFLOAT_AS_FLOAT )

#define BF_FP_TO_VGF(F)  ( (float)(F) / (float)( 1<<BF_PRECISION ) )
#define BF_VGF_TO_FP(F)  ( (BFFixedPoint)( (F) * (float)( 1<<BF_PRECISION ) ) )
#define BF_INT_TO_VGF(F) ( (float)(F) )

#define VG_FLOOR(a)      ( (int)floor( (a) ) )
#define VG_MUL(A, B)     ( (A) * (B) )
#define VG_DIV(A, B)     ( (A) / (B) )
#define VG_KAPPA         0.5522f
#define VG_ONE           1.0f

#define BF_COLOR_TO_VGCOLOR( color, rgbColor, alpha ) \
	                    (color)[0] = (float)BF_GET_RED( (rgbColor) ) / 255.0f;\
	                    (color)[1] = (float)BF_GET_GREEN( (rgbColor) ) / 255.0f;\
	                    (color)[2] = (float)BF_GET_BLUE( (rgbColor) ) / 255.0f;\
	                    (color)[3] = (float)(alpha) / 255.0f;
#define BF_VGCOLOR_TO_COLOR( color, rgbColor, alpha ) \
                    alpha = (BF_UINT8)((color)[3] * 255.0f + 0.5f); \
                    rgbColor = BF_ARGB_MAKE(alpha, (BF_UINT8)((color)[0] * 255.0f + 0.5f), \
                                           (BF_UINT8)((color)[1] * 255.0f + 0.5f), \
                                           (BF_UINT8)((color)[2] * 255.0f + 0.5f) );
#define BF_VGCOLOR_TO_SOLID_COLOR( color, rgbColor, alpha ) \
                    alpha = (BF_UINT8)((color)[3] * 255.0f + 0.5f); \
                    rgbColor = BF_ARGB_MAKE(255, (BF_UINT8)((color)[0] * 255.0f + 0.5f), \
                                           (BF_UINT8)((color)[1] * 255.0f + 0.5f), \
                                           (BF_UINT8)((color)[2] * 255.0f + 0.5f) );

/* convert between VGuint color (sRGBA_8888 representation) and VGFloat[4] */
#define BF_INT_TO_FLOAT_COLOR(iColor, pFloatColor) \
                              (pFloatColor)[0] = (((iColor) >> 24) & 0xff)/255.0f; \
                              (pFloatColor)[1] = (((iColor) >> 16) & 0xff)/255.0f; \
                              (pFloatColor)[2] = (((iColor) >> 8) & 0xff)/255.0f; \
                              (pFloatColor)[3] = ( (iColor) & 0xff)/255.0f;
#define BF_FLOAT_TO_INT_COLOR(iColor, pFloatColor) \
                              iColor = (VGuint)((((VGint)((pFloatColor)[0]*255.0f + 0.5f)) << 24) | \
                                                (((VGint)((pFloatColor)[1]*255.0f + 0.5f)) << 16) | \
                                                (((VGint)((pFloatColor)[2]*255.0f + 0.5f)) <<  8) | \
                                                  (VGint)((pFloatColor)[3]*255.0f + 0.5f));

#elif defined( BF_VGFLOAT_AS_FIXED_POINT )

#define BF_FP_TO_VGF(F)  (F)
#define BF_VGF_TO_FP(F)  (F)
#define BF_INT_TO_VGF(F) ( BF_Int2Fixed( (F) ) )

#define VG_FLOOR(a)      BF_FixedFloor( (a) )
#define VG_MUL(A, B)     BF_FixedMul( (A) , (B) )
#define VG_DIV(A, B)     BF_FixedDiv( (A) , (B) )
#define VG_KAPPA         36195 /*  4(sqrt(2) - 1)/3 = 0.5522847498.... * BF_FP_ONE, used to approximate a quarter-circle using a cubic Bezier */
#define VG_ONE           BF_FP_ONE


#define BF_FP_ONE_OVER_255  257  /* 1.0/255.0 = 0.00392156863 -> * 65536 = 257.003921569 */
#define BF_COLOR_TO_VGCOLOR( color, rgbColor, alpha ) \
	                    (color)[0] = (BFFixedPoint)(BF_GET_RED( (rgbColor) ) * BF_FP_ONE_OVER_255);\
	                    (color)[1] = (BFFixedPoint)(BF_GET_GREEN( (rgbColor) ) * BF_FP_ONE_OVER_255);\
	                    (color)[2] = (BFFixedPoint)(BF_GET_BLUE( (rgbColor) ) * BF_FP_ONE_OVER_255);\
	                    (color)[3] = (BFFixedPoint)((alpha) * BF_FP_ONE_OVER_255);
#define BF_VGCOLOR_TO_COLOR( color, rgbColor, alpha ) \
                    alpha = ((BF_UINT8)BF_RoundFixed2UInt16((color)[3] * 255)); \
                    rgbColor = BF_ARGB_MAKE(alpha, BF_RoundFixed2UInt16((color)[0] * 255), \
                                           BF_RoundFixed2UInt16((color)[1] * 255), \
                                           BF_RoundFixed2UInt16((color)[2] * 255) );
#define BF_VGCOLOR_TO_SOLID_COLOR( color, rgbColor, alpha ) \
                    alpha = ((BF_UINT8)BF_RoundFixed2UInt16((color)[3] * 255)); \
                    rgbColor = BF_ARGB_MAKE(255, BF_RoundFixed2UInt16((color)[0] * 255), \
                                           BF_RoundFixed2UInt16((color)[1] * 255), \
                                           BF_RoundFixed2UInt16((color)[2] * 255) );

/* convert between VGuint color (sRGBA_8888 representation) and VGFloat[4] */
#define BF_INT_TO_FLOAT_COLOR(iColor, pFloatColor) \
                            pFloatColor[0] = (BFFixedPoint)((iColor >> 24) & 0xff) * BF_FP_ONE_OVER_255; \
                            pFloatColor[1] = (BFFixedPoint)((iColor >> 16) & 0xff) * BF_FP_ONE_OVER_255; \
                            pFloatColor[2] = (BFFixedPoint)((iColor >> 8) & 0xff) * BF_FP_ONE_OVER_255; \
                            pFloatColor[3] = (BFFixedPoint)( iColor & 0xff) * BF_FP_ONE_OVER_255;
#define BF_FLOAT_TO_INT_COLOR(iColor, pFloatColor) \
                              iColor = (VGuint)((BF_RoundFixed2UInt16((pFloatColor)[0]*255) << 24) | \
                                                (BF_RoundFixed2UInt16((pFloatColor)[1]*255) << 16) | \
                                                (BF_RoundFixed2UInt16((pFloatColor)[2]*255) <<  8) | \
                                                 BF_RoundFixed2UInt16((pFloatColor)[3]*255));

#endif

#define BF_API_CALL

#ifndef BF_CFG_USE_EXTERNAL_OPENVG

#define OPENVG_VERSION_1_0 1

typedef BF_INT8         VGbyte;
typedef BF_UINT8        VGubyte;
typedef BF_INT16        VGshort;
typedef BF_INT32        VGint;
typedef BF_UINT32       VGuint;
typedef BF_UINT32       VGbitfield;

typedef enum
{
    VG_FALSE = 0,
    VG_TRUE  = 1
} VGboolean;


#define VG_MAXSHORT ((VGshort)((~((unsigned)0)) >> 1))
#define VG_MAXINT ((VGint)((~((unsigned)0)) >> 1))

typedef VGuint VGHandle;

#define VG_INVALID_HANDLE ((VGHandle)0)

typedef enum
{
    VG_NO_ERROR                                 = 0,
    VG_BAD_HANDLE_ERROR                         = 0x1000,
    VG_ILLEGAL_ARGUMENT_ERROR                   = 0x1001,
    VG_OUT_OF_MEMORY_ERROR                      = 0x1002,
    VG_PATH_CAPABILITY_ERROR                    = 0x1003,
    VG_UNSUPPORTED_IMAGE_FORMAT_ERROR           = 0x1004,
    VG_UNSUPPORTED_PATH_FORMAT_ERROR            = 0x1005,
    VG_IMAGE_IN_USE_ERROR                       = 0x1006,
    VG_NO_CONTEXT_ERROR                         = 0x1007, /* JE NOTE: this is taken from openvg.h from version 1.0.1 */

    /* font extensions */
    VG_OUT_OF_GLYPHS_ERROR                      = 0x1008,

    /* Khronos extensions */
    VG_UNSUPPORTED_DRAWABLE_FORMAT_ERROR_KHR    = 0x1009

} VGErrorCode;

typedef enum
{
    /* Mode settings */
    VG_MATRIX_MODE                              = 0x1100,
    VG_FILL_RULE                                = 0x1101,
    VG_IMAGE_QUALITY                            = 0x1102,
    VG_RENDERING_QUALITY                        = 0x1103,
    VG_BLEND_MODE                               = 0x1104,
    VG_IMAGE_MODE                               = 0x1105,

    /* Scissoring rectangles */
    VG_SCISSOR_RECTS                            = 0x1106,

    /* Stroke parameters */
    VG_STROKE_LINE_WIDTH                        = 0x1110,
    VG_STROKE_CAP_STYLE                         = 0x1111,
    VG_STROKE_JOIN_STYLE                        = 0x1112,
    VG_STROKE_MITER_LIMIT                       = 0x1113,
    VG_STROKE_DASH_PATTERN                      = 0x1114,
    VG_STROKE_DASH_PHASE                        = 0x1115,

    /* Edge fill color for VG_TILE_FILL tiling mode */
    VG_TILE_FILL_COLOR                          = 0x1120,

    /* Color for vgClear */
    VG_CLEAR_COLOR                              = 0x1121,

    /* Enable/disable alpha masking and scissoring */
    VG_MASKING                                  = 0x1130,
    VG_SCISSORING                               = 0x1131,

    /* Pixel layout hint information */
    VG_PIXEL_LAYOUT                             = 0x1140,

    /* Source format selection for image filters */
    VG_FILTER_FORMAT_LINEAR                     = 0x1150,
    VG_FILTER_FORMAT_PREMULTIPLIED              = 0x1151,

    /* Destination write enable mask for image filters */
    VG_FILTER_CHANNEL_MASK                      = 0x1152,

    /* Implementation limits (read-only) */
    VG_MAX_SCISSOR_RECTS                        = 0x1160,
    VG_MAX_DASH_COUNT                           = 0x1161,
    VG_MAX_KERNEL_SIZE                          = 0x1162,
    VG_MAX_SEPARABLE_KERNEL_SIZE                = 0x1163,
    VG_MAX_COLOR_RAMP_STOPS                     = 0x1164,
    VG_MAX_IMAGE_WIDTH                          = 0x1165,
    VG_MAX_IMAGE_HEIGHT                         = 0x1166,
    VG_MAX_IMAGE_PIXELS                         = 0x1167,
    VG_MAX_IMAGE_BYTES                          = 0x1168,
    VG_MAX_FLOAT                                = 0x1169
} VGParamType;

typedef enum
{
    VG_RENDERING_QUALITY_NONANTIALIASED         = 0x1200,
    VG_RENDERING_QUALITY_FASTER                 = 0x1201,
    VG_RENDERING_QUALITY_BETTER                 = 0x1202 /* Default */
} VGRenderingQuality;

typedef enum
{
    VG_PIXEL_LAYOUT_UNKNOWN                     = 0x1300,
    VG_PIXEL_LAYOUT_RGB_VERTICAL                = 0x1301,
    VG_PIXEL_LAYOUT_BGR_VERTICAL                = 0x1302,
    VG_PIXEL_LAYOUT_RGB_HORIZONTAL              = 0x1303,
    VG_PIXEL_LAYOUT_BGR_HORIZONTAL              = 0x1304
} VGPixelLayout;

typedef enum
{
    VG_MATRIX_PATH_USER_TO_SURFACE              = 0x1400,
    VG_MATRIX_IMAGE_USER_TO_SURFACE             = 0x1401,
    VG_MATRIX_FILL_PAINT_TO_USER                = 0x1402,
    VG_MATRIX_STROKE_PAINT_TO_USER              = 0x1403
} VGMatrixMode;

typedef enum
{
    VG_CLEAR_MASK                               = 0x1500,
    VG_FILL_MASK                                = 0x1501,
    VG_SET_MASK                                 = 0x1502,
    VG_UNION_MASK                               = 0x1503,
    VG_INTERSECT_MASK                           = 0x1504,
    VG_SUBTRACT_MASK                            = 0x1505
} VGMaskOperation;

#define VG_PATH_FORMAT_STANDARD 0

typedef enum
{
    VG_PATH_DATATYPE_INVALID                    = -1,
    VG_PATH_DATATYPE_S_8                        =  0,
    VG_PATH_DATATYPE_S_16                       =  1,
    VG_PATH_DATATYPE_S_32                       =  2,
    VG_PATH_DATATYPE_F                          =  3
} VGPathDatatype;

typedef enum
{
    VG_ABSOLUTE                                 = 0,
    VG_RELATIVE                                 = 1
} VGPathAbsRel;

typedef enum
{
    VG_CLOSE_PATH                               = ( 0 << 1),
    VG_MOVE_TO                                  = ( 1 << 1),
    VG_LINE_TO                                  = ( 2 << 1),
    VG_HLINE_TO                                 = ( 3 << 1),
    VG_VLINE_TO                                 = ( 4 << 1),
    VG_QUAD_TO                                  = ( 5 << 1),
    VG_CUBIC_TO                                 = ( 6 << 1),
    VG_SQUAD_TO                                 = ( 7 << 1),
    VG_SCUBIC_TO                                = ( 8 << 1),
    VG_SCCWARC_TO                               = ( 9 << 1),
    VG_SCWARC_TO                                = (10 << 1),
    VG_LCCWARC_TO                               = (11 << 1),
    VG_LCWARC_TO                                = (12 << 1)
} VGPathSegment;

typedef enum
{
    VG_MOVE_TO_ABS                              = VG_MOVE_TO    | VG_ABSOLUTE,
    VG_MOVE_TO_REL                              = VG_MOVE_TO    | VG_RELATIVE,
    VG_LINE_TO_ABS                              = VG_LINE_TO    | VG_ABSOLUTE,
    VG_LINE_TO_REL                              = VG_LINE_TO    | VG_RELATIVE,
    VG_HLINE_TO_ABS                             = VG_HLINE_TO   | VG_ABSOLUTE,
    VG_HLINE_TO_REL                             = VG_HLINE_TO   | VG_RELATIVE,
    VG_VLINE_TO_ABS                             = VG_VLINE_TO   | VG_ABSOLUTE,
    VG_VLINE_TO_REL                             = VG_VLINE_TO   | VG_RELATIVE,
    VG_QUAD_TO_ABS                              = VG_QUAD_TO    | VG_ABSOLUTE,
    VG_QUAD_TO_REL                              = VG_QUAD_TO    | VG_RELATIVE,
    VG_CUBIC_TO_ABS                             = VG_CUBIC_TO   | VG_ABSOLUTE,
    VG_CUBIC_TO_REL                             = VG_CUBIC_TO   | VG_RELATIVE,
    VG_SQUAD_TO_ABS                             = VG_SQUAD_TO   | VG_ABSOLUTE,
    VG_SQUAD_TO_REL                             = VG_SQUAD_TO   | VG_RELATIVE,
    VG_SCUBIC_TO_ABS                            = VG_SCUBIC_TO  | VG_ABSOLUTE,
    VG_SCUBIC_TO_REL                            = VG_SCUBIC_TO  | VG_RELATIVE,
    VG_SCCWARC_TO_ABS                           = VG_SCCWARC_TO | VG_ABSOLUTE,
    VG_SCCWARC_TO_REL                           = VG_SCCWARC_TO | VG_RELATIVE,
    VG_SCWARC_TO_ABS                            = VG_SCWARC_TO  | VG_ABSOLUTE,
    VG_SCWARC_TO_REL                            = VG_SCWARC_TO  | VG_RELATIVE,
    VG_LCCWARC_TO_ABS                           = VG_LCCWARC_TO | VG_ABSOLUTE,
    VG_LCCWARC_TO_REL                           = VG_LCCWARC_TO | VG_RELATIVE,
    VG_LCWARC_TO_ABS                            = VG_LCWARC_TO  | VG_ABSOLUTE,
    VG_LCWARC_TO_REL                            = VG_LCWARC_TO  | VG_RELATIVE
} VGPathCommand;

typedef VGHandle VGPath;

typedef enum
{
    VG_PATH_CAPABILITY_APPEND_FROM              = (1 <<  0),
    VG_PATH_CAPABILITY_APPEND_TO                = (1 <<  1),
    VG_PATH_CAPABILITY_MODIFY                   = (1 <<  2),
    VG_PATH_CAPABILITY_TRANSFORM_FROM           = (1 <<  3),
    VG_PATH_CAPABILITY_TRANSFORM_TO             = (1 <<  4),
    VG_PATH_CAPABILITY_INTERPOLATE_FROM         = (1 <<  5),
    VG_PATH_CAPABILITY_INTERPOLATE_TO           = (1 <<  6),
    VG_PATH_CAPABILITY_PATH_LENGTH              = (1 <<  7),
    VG_PATH_CAPABILITY_POINT_ALONG_PATH         = (1 <<  8),
    VG_PATH_CAPABILITY_TANGENT_ALONG_PATH       = (1 <<  9),
    VG_PATH_CAPABILITY_PATH_BOUNDS              = (1 << 10),
    VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS  = (1 << 11),
    VG_PATH_CAPABILITY_READ                     = (1 << 12),
    VG_PATH_CAPABILITY_PICK                     = (1 << 13),
    VG_PATH_CAPABILITY_ALL                      = (1 << 14) - 1
} VGPathCapabilities;

typedef enum
{
    VG_PATH_FORMAT                              = 0x1600,
    VG_PATH_DATATYPE                            = 0x1601,
    VG_PATH_SCALE                               = 0x1602,
    VG_PATH_BIAS                                = 0x1603,
    VG_PATH_NUM_SEGMENTS                        = 0x1604,
    VG_PATH_NUM_COORDS                          = 0x1605
} VGPathParamType;

typedef enum
{
    VG_CAP_BUTT                                 = 0x1700,
    VG_CAP_ROUND                                = 0x1701,
    VG_CAP_SQUARE                               = 0x1702
} VGCapStyle;

typedef enum
{
    VG_JOIN_MITER                               = 0x1800,
    VG_JOIN_ROUND                               = 0x1801,
    VG_JOIN_BEVEL                               = 0x1802
} VGJoinStyle;

typedef enum
{
    VG_EVEN_ODD                                 = 0x1900,
    VG_NON_ZERO                                 = 0x1901
} VGFillRule;

typedef enum
{
    VG_STROKE_PATH                              = (1 << 0),
    VG_FILL_PATH                                = (1 << 1)
} VGPaintMode;

typedef VGHandle VGPaint;

typedef enum
{
    /* Color paint parameters */
    VG_PAINT_TYPE                               = 0x1A00,
    VG_PAINT_COLOR                              = 0x1A01,
    VG_PAINT_COLOR_RAMP_SPREAD_MODE             = 0x1A02,
    VG_PAINT_COLOR_RAMP_STOPS                   = 0x1A03,

    /* Linear gradient paint parameters */
    VG_PAINT_LINEAR_GRADIENT                    = 0x1A04,

    /* Radial gradient paint parameters */
    VG_PAINT_RADIAL_GRADIENT                    = 0x1A05,

    /* Pattern paint parameters */
    VG_PAINT_PATTERN_TILING_MODE                = 0x1A06
} VGPaintParamType;

typedef enum
{
    VG_PAINT_TYPE_COLOR                         = 0x1B00,
    VG_PAINT_TYPE_LINEAR_GRADIENT               = 0x1B01,
    VG_PAINT_TYPE_RADIAL_GRADIENT               = 0x1B02,
    VG_PAINT_TYPE_PATTERN                       = 0x1B03
} VGPaintType;

typedef enum
{
    VG_COLOR_RAMP_SPREAD_PAD                    = 0x1C00,
    VG_COLOR_RAMP_SPREAD_REPEAT                 = 0x1C01,
    VG_COLOR_RAMP_SPREAD_REFLECT                = 0x1C02
} VGColorRampSpreadMode;

typedef enum
{
    VG_TILE_FILL                                = 0x1D00,
    VG_TILE_PAD                                 = 0x1D01,
    VG_TILE_REPEAT                              = 0x1D02,
    VG_TILE_REFLECT                             = 0x1D03
} VGTilingMode;

/* JE NOTE: this is taken from openvg.h from version 1.0.1 */
typedef enum
{
    /* RGB{A,X} channel ordering */
    VG_sRGBX_8888                               =  0,
    VG_sRGBA_8888                               =  1,
    VG_sRGBA_8888_PRE                           =  2,
    VG_sRGB_565                                 =  3,
    VG_sRGBA_5551                               =  4,
    VG_sRGBA_4444                               =  5,
    VG_sL_8                                     =  6,
    VG_lRGBX_8888                               =  7,
    VG_lRGBA_8888                               =  8,
    VG_lRGBA_8888_PRE                           =  9,
    VG_lL_8                                     = 10,
    VG_A_8                                      = 11,
    VG_BW_1                                     = 12,

    /* {A,X}RGB channel ordering */
    VG_sXRGB_8888                               =  0 | (1 << 6),
    VG_sARGB_8888                               =  1 | (1 << 6),
    VG_sARGB_8888_PRE                           =  2 | (1 << 6),
    VG_sARGB_1555                               =  4 | (1 << 6),
    VG_sARGB_4444                               =  5 | (1 << 6),
    VG_lXRGB_8888                               =  7 | (1 << 6),
    VG_lARGB_8888                               =  8 | (1 << 6),
    VG_lARGB_8888_PRE                           =  9 | (1 << 6),

    /* BGR{A,X} channel ordering */
    VG_sBGRX_8888                               =  0 | (1 << 7),
    VG_sBGRA_8888                               =  1 | (1 << 7),
    VG_sBGRA_8888_PRE                           =  2 | (1 << 7),
    VG_sBGR_565                                 =  3 | (1 << 7),
    VG_sBGRA_5551                               =  4 | (1 << 7),
    VG_sBGRA_4444                               =  5 | (1 << 7),
    VG_lBGRX_8888                               =  7 | (1 << 7),
    VG_lBGRA_8888                               =  8 | (1 << 7),
    VG_lBGRA_8888_PRE                           =  9 | (1 << 7),

    /* {A,X}BGR channel ordering */
    VG_sXBGR_8888                               =  0 | (1 << 6) | (1 << 7),
    VG_sABGR_8888                               =  1 | (1 << 6) | (1 << 7),
    VG_sABGR_8888_PRE                           =  2 | (1 << 6) | (1 << 7),
    VG_sABGR_1555                               =  4 | (1 << 6) | (1 << 7),
    VG_sABGR_4444                               =  5 | (1 << 6) | (1 << 7),
    VG_lXBGR_8888                               =  7 | (1 << 6) | (1 << 7),
    VG_lABGR_8888                               =  8 | (1 << 6) | (1 << 7),
    VG_lABGR_8888_PRE                           =  9 | (1 << 6) | (1 << 7)
} VGImageFormat;

typedef VGHandle VGImage;

typedef enum
{
    VG_IMAGE_QUALITY_NONANTIALIASED             = (1 << 0),
    VG_IMAGE_QUALITY_FASTER                     = (1 << 1),
    VG_IMAGE_QUALITY_BETTER                     = (1 << 2)
} VGImageQuality;

typedef enum
{
    VG_IMAGE_FORMAT                             = 0x1E00,
    VG_IMAGE_WIDTH                              = 0x1E01,
    VG_IMAGE_HEIGHT                             = 0x1E02
} VGImageParamType;

typedef enum
{
    VG_DRAW_IMAGE_NORMAL                        = 0x1F00,
    VG_DRAW_IMAGE_MULTIPLY                      = 0x1F01,
    VG_DRAW_IMAGE_STENCIL                       = 0x1F02
} VGImageMode;

typedef enum
{
    VG_RED                                      = (1 << 3),
    VG_GREEN                                    = (1 << 2),
    VG_BLUE                                     = (1 << 1),
    VG_ALPHA                                    = (1 << 0)
} VGImageChannel;

typedef enum
{
    VG_BLEND_SRC                                = 0x2000,
    VG_BLEND_SRC_OVER                           = 0x2001,
    VG_BLEND_DST_OVER                           = 0x2002,
    VG_BLEND_SRC_IN                             = 0x2003,
    VG_BLEND_DST_IN                             = 0x2004,
    VG_BLEND_MULTIPLY                           = 0x2005,
    VG_BLEND_SCREEN                             = 0x2006,
    VG_BLEND_DARKEN                             = 0x2007,
    VG_BLEND_LIGHTEN                            = 0x2008,
    VG_BLEND_ADDITIVE                           = 0x2009
} VGBlendMode;

typedef enum
{
    VG_IMAGE_FORMAT_QUERY                       = 0x2100,
    VG_PATH_DATATYPE_QUERY                      = 0x2101
} VGHardwareQueryType;

typedef enum
{
    VG_HARDWARE_ACCELERATED                     = 0x2200,
    VG_HARDWARE_UNACCELERATED                   = 0x2201
} VGHardwareQueryResult;

typedef enum
{
    VG_VENDOR                                   = 0x2300,
    VG_RENDERER                                 = 0x2301,
    VG_VERSION                                  = 0x2302,
    VG_EXTENSIONS                               = 0x2303
} VGStringID;

#else
#include "openvg.h"
#endif

typedef VGHandle VGFont;

typedef enum
{
    VG_FONT_GLYPH_MAPPING                       = 0x2F00,
    VG_FONT_ID                                  = 0x2F01,
    VG_FONT_NUM_GLYPHS                          = 0x2F02
} VGFontParamType;

typedef enum
{
    VG_GLYPH_MAPPING_UNICODE                    = 1,
    VG_GLYPH_MAPPING_FONT_NATIVE                = 2,
    VG_GLYPH_MAPPING_CUSTOM                     = 3
} VGFontGlyphMapping;


/* Function Prototypes */

BF_API_CALL VGErrorCode BFvgGetError( BFOpenVG pCX );

BF_API_CALL void BFvgFlush( BFOpenVG pCX );
BF_API_CALL void BFvgFinish( BFOpenVG pCX);

/* Getters and Setters */
BF_API_CALL void BFvgSetf (BFOpenVG pCX, VGParamType type, VGfloat value);
BF_API_CALL void BFvgSeti (BFOpenVG pCX, VGParamType type, VGint value);
BF_API_CALL void BFvgSetfv(BFOpenVG pCX, VGParamType type, VGint count,
                           const VGfloat * values);
BF_API_CALL void BFvgSetiv(BFOpenVG pCX, VGParamType type, VGint count,
                           const VGint * values);

BF_API_CALL VGfloat BFvgGetf(BFOpenVG pCX, VGParamType type);
BF_API_CALL VGint   BFvgGeti(BFOpenVG pCX, VGParamType type);
BF_API_CALL VGint   BFvgGetVectorSize(BFOpenVG pCX, VGParamType type);
BF_API_CALL void    BFvgGetfv(BFOpenVG pCX, VGParamType type, VGint count, VGfloat * values);
BF_API_CALL void    BFvgGetiv(BFOpenVG pCX, VGParamType type, VGint count, VGint * values);

BF_API_CALL void BFvgSetParameterf(BFOpenVG pCX, VGHandle object,
                                   VGint paramType,
                                   VGfloat value);
BF_API_CALL void BFvgSetParameteri(BFOpenVG pCX, VGHandle object,
                                   VGint paramType,
                                   VGint value);
BF_API_CALL void BFvgSetParameterfv(BFOpenVG pCX, VGHandle object,
                                    VGint paramType,
                                    VGint count, const VGfloat * values);
BF_API_CALL void BFvgSetParameteriv(BFOpenVG pCX, VGHandle object,
                                    VGint paramType,
                                    VGint count, const VGint * values);

BF_API_CALL VGfloat BFvgGetParameterf(BFOpenVG pCX, VGHandle object,
                                      VGint paramType);
BF_API_CALL VGint BFvgGetParameteri(BFOpenVG pCX, VGHandle object,
                                    VGint paramType);
BF_API_CALL VGint BFvgGetParameterVectorSize(BFOpenVG pCX, VGHandle object,
        VGint paramType);
BF_API_CALL void BFvgGetParameterfv(BFOpenVG pCX, VGHandle object,
                                    VGint paramType,
                                    VGint count, VGfloat * values);
BF_API_CALL void BFvgGetParameteriv(BFOpenVG pCX, VGHandle object,
                                    VGint paramType,
                                    VGint count, VGint * values);

/* Matrix Manipulation */
BF_API_CALL void BFvgLoadIdentity(BFOpenVG pCX );
BF_API_CALL void BFvgLoadMatrix(BFOpenVG pCX, const VGfloat * m);
BF_API_CALL void BFvgGetMatrix(BFOpenVG pCX, VGfloat * m);
BF_API_CALL void BFvgMultMatrix(BFOpenVG pCX, const VGfloat * m);
BF_API_CALL void BFvgTranslate(BFOpenVG pCX, VGfloat tx, VGfloat ty);
BF_API_CALL void BFvgScale(BFOpenVG pCX, VGfloat sx, VGfloat sy);
BF_API_CALL void BFvgShear(BFOpenVG pCX, VGfloat shx, VGfloat shy);
BF_API_CALL void BFvgRotate(BFOpenVG pCX, VGfloat angle);

/* Masking and Clearing */
BF_API_CALL void BFvgMask(BFOpenVG pCX, VGImage mask, VGMaskOperation operation,
                          VGint x, VGint y, VGint width, VGint height);
BF_API_CALL void BFvgClear(BFOpenVG pCX, VGint x, VGint y, VGint width, VGint height);


/* Paths */
BF_API_CALL VGPath BFvgCreatePath(BFOpenVG pCX, VGint pathFormat,
                                  VGPathDatatype datatype,
                                  VGfloat scale, VGfloat bias,
                                  VGint segmentCapacityHint,
                                  VGint coordCapacityHint,
                                  VGbitfield capabilities);
BF_API_CALL void BFvgClearPath(BFOpenVG pCX, VGPath path, VGbitfield capabilities);
BF_API_CALL void BFvgDestroyPath(BFOpenVG pCX, VGPath path);
BF_API_CALL void BFvgRemovePathCapabilities(BFOpenVG pCX, VGPath path,
        VGbitfield capabilities);
BF_API_CALL VGbitfield BFvgGetPathCapabilities(BFOpenVG pCX, VGPath path);
BF_API_CALL void BFvgAppendPath(BFOpenVG pCX, VGPath dstPath, VGPath srcPath);
BF_API_CALL void BFvgAppendPathData(BFOpenVG pCX, VGPath dstPath,
                                    VGint numSegments,
                                    const VGubyte * pathSegments,
                                    const void * pathData);
BF_API_CALL void BFvgReadPathData(BFOpenVG pCX, VGPath dstPath,
                                  VGint segmentIndex,
                                  VGubyte * pathSegment,
                                  void * pathData);
BF_API_CALL void BFvgModifyPathCoords(BFOpenVG pCX, VGPath dstPath, VGint startIndex,
                                      VGint numSegments,
                                      const void * pathData);
BF_API_CALL void BFvgTransformPath(BFOpenVG pCX, VGPath dstPath, VGPath srcPath);
BF_API_CALL VGboolean BFvgInterpolatePath(BFOpenVG pCX, VGPath dstPath,
        VGPath startPath,
        VGPath endPath,
        VGfloat amount);
BF_API_CALL VGfloat BFvgPathLength(BFOpenVG pCX, VGPath path,
                                   VGint startSegment, VGint numSegments);
BF_API_CALL void BFvgPointAlongPath(BFOpenVG pCX, VGPath path,
                                    VGint startSegment, VGint numSegments,
                                    VGfloat distance,
                                    VGfloat * x, VGfloat * y,
                                    VGfloat * tangentX, VGfloat * tangentY);
BF_API_CALL void BFvgPathBounds(BFOpenVG pCX, VGPath path,
                                VGfloat * minX, VGfloat * minY,
                                VGfloat * width, VGfloat * height);
BF_API_CALL void BFvgPathTransformedBounds(BFOpenVG pCX, VGPath path,
        VGfloat * minX, VGfloat * minY,
        VGfloat * width, VGfloat * height);
BF_API_CALL void BFvgDrawPath(BFOpenVG pCX, VGPath path, VGbitfield paintModes);
BF_API_CALL VGboolean BFvgPickPath(BFOpenVG pCX, VGPath path, VGbitfield paintModes, VGint x, VGint y); /** BF extension */

/* Paint */
BF_API_CALL VGPaint BFvgCreatePaint(BFOpenVG pCX);
BF_API_CALL void BFvgDestroyPaint(BFOpenVG pCX, VGPaint paint);
BF_API_CALL void BFvgSetPaint(BFOpenVG pCX, VGPaint paint, VGbitfield paintModes);
BF_API_CALL VGPaint BFvgGetPaint(BFOpenVG pCX, VGPaintMode paintMode);
BF_API_CALL void BFvgSetColor(BFOpenVG pCX, VGPaint paint, VGuint rgba);
BF_API_CALL VGuint BFvgGetColor(BFOpenVG pCX, VGPaint paint);
BF_API_CALL void BFvgPaintPattern(BFOpenVG pCX, VGPaint paint, VGImage pattern);

/* Images */
BF_API_CALL VGImage BFvgCreateImage(BFOpenVG pCX, VGImageFormat format,
                                    VGint width, VGint height,
                                    VGbitfield allowedQuality);
BF_API_CALL VGImage BFvgCreateImageWithBuffer(BFOpenVG pCX, VGImageFormat format,
        VGint width, VGint height,
        VGbitfield allowedQuality, VGint dataStride, void *pBuffer);   /** BF extension */
BF_API_CALL void BFvgDestroyImage(BFOpenVG pCX, VGImage image);
BF_API_CALL void BFvgClearImage(BFOpenVG pCX, VGImage image,
                                VGint x, VGint y, VGint width, VGint height);
BF_API_CALL void BFvgImageSubData(BFOpenVG pCX, VGImage image,
                                  const void * data, VGint dataStride,
                                  VGImageFormat dataFormat,
                                  VGint x, VGint y, VGint width, VGint height);
BF_API_CALL void BFvgGetImageSubData(BFOpenVG pCX, VGImage image,
                                     void * data, VGint dataStride,
                                     VGImageFormat dataFormat,
                                     VGint x, VGint y,
                                     VGint width, VGint height);
BF_API_CALL VGImage BFvgChildImage(BFOpenVG pCX, VGImage parent,
                                   VGint x, VGint y, VGint width, VGint height);
BF_API_CALL VGImage BFvgGetParent(BFOpenVG pCX, VGImage image);
BF_API_CALL void BFvgCopyImage(BFOpenVG pCX, VGImage dst, VGint dx, VGint dy,
                               VGImage src, VGint sx, VGint sy,
                               VGint width, VGint height,
                               VGboolean dither);
BF_API_CALL void BFvgDrawImage(BFOpenVG pCX, VGImage image);

BF_API_CALL VGboolean BFvgPickImage(BFOpenVG pCX, VGImage image, VGint x, VGint y); /** BF extension */

BF_API_CALL void BFvgSetPixels(BFOpenVG pCX, VGint dx, VGint dy,
                               VGImage src, VGint sx, VGint sy,
                               VGint width, VGint height);
BF_API_CALL void BFvgWritePixels(BFOpenVG pCX, const void * data, VGint dataStride,
                                 VGImageFormat dataFormat,
                                 VGint dx, VGint dy,
                                 VGint width, VGint height);
BF_API_CALL void BFvgGetPixels(BFOpenVG pCX, VGImage dst, VGint dx, VGint dy,
                               VGint sx, VGint sy,
                               VGint width, VGint height);
BF_API_CALL void BFvgReadPixels(BFOpenVG pCX, void * data, VGint dataStride,
                                VGImageFormat dataFormat,
                                VGint sx, VGint sy,
                                VGint width, VGint height);
BF_API_CALL void BFvgCopyPixels(BFOpenVG pCX, VGint dx, VGint dy,
                                VGint sx, VGint sy,
                                VGint width, VGint height);

/* Image Filters */
BF_API_CALL void BFvgColorMatrix(BFOpenVG pCX, VGImage dst, VGImage src,
                                 const VGfloat * matrix);
BF_API_CALL void BFvgConvolve(BFOpenVG pCX, VGImage dst, VGImage src,
                              VGint kernelWidth, VGint kernelHeight,
                              VGint shiftX, VGint shiftY,
                              const VGshort * kernel,
                              VGfloat scale,
                              VGfloat bias,
                              VGTilingMode tilingMode);
BF_API_CALL void BFvgSeparableConvolve(BFOpenVG pCX, VGImage dst, VGImage src,
                                       VGint kernelWidth,
                                       VGint kernelHeight,
                                       VGint shiftX, VGint shiftY,
                                       const VGshort * kernelX,
                                       const VGshort * kernelY,
                                       VGfloat scale,
                                       VGfloat bias,
                                       VGTilingMode tilingMode);
BF_API_CALL void BFvgGaussianBlur(BFOpenVG pCX, VGImage dst, VGImage src,
                                  VGfloat stdDeviationX,
                                  VGfloat stdDeviationY,
                                  VGTilingMode tilingMode);
BF_API_CALL void BFvgLookup(BFOpenVG pCX, VGImage dst, VGImage src,
                            const VGubyte * redLUT,
                            const VGubyte * greenLUT,
                            const VGubyte * blueLUT,
                            const VGubyte * alphaLUT,
                            VGboolean outputLinear,
                            VGboolean outputPremultiplied);
BF_API_CALL void BFvgLookupSingle(BFOpenVG pCX, VGImage dst, VGImage src,
                                  const VGuint * lookupTable,
                                  VGImageChannel sourceChannel,
                                  VGboolean outputLinear,
                                  VGboolean outputPremultiplied);

/* Hardware Queries */
BF_API_CALL VGHardwareQueryResult BFvgHardwareQuery(BFOpenVG pCX, VGHardwareQueryType key,
        VGint setting);

/* Renderer and Extension Information */
BF_API_CALL const VGubyte * BFvgGetString(BFOpenVG pCX, VGStringID name);


/* Font extensions */
#ifdef BF_CFG_HAS_OVG_FONT_API
BF_API_CALL void BFvgDestroyFont(BFOpenVG pCX, VGFont font);
BF_API_CALL VGFont BFvgCreateFont(BFOpenVG pCX, VGuint fontID, VGint numGlyphs, VGFontGlyphMapping glyphMapping);
BF_API_CALL void BFvgSetGlyphToPath (BFOpenVG pCX, VGFont font, VGuint glyphIndex, VGPath path, VGboolean isHinted, VGfloat glyphOrigin [2], VGfloat escapement[2], VGboolean bOutline, VGubyte uThickness);
#endif


/* BFOVG status flags */
#define BFVG_RENDER_OVERFLOW            0x01      /* fixed-point overflow occured while rendering */

#define BFVG_CONTEXT                    pRCX->pVGContext

#ifdef BF_CFG_USE_BFOPENVG


#define vgGetError()                    BFvgGetError( BFVG_CONTEXT )
#define vgFlush()                       BFvgFlush( BFVG_CONTEXT )
#define vgFinish()                      BFvgFinish( BFVG_CONTEXT )
#define vgSetf(a,b)                     BFvgSetf( BFVG_CONTEXT, a, b )
#define vgSeti(a,b)                     BFvgSeti( BFVG_CONTEXT, a, b )
#define vgSetfv(a,b,c)                  BFvgSetfv( BFVG_CONTEXT, a, b, c )
#define vgSetiv(a,b,c)                  BFvgSetiv( BFVG_CONTEXT, a, b, c )
#define vgGetf(a)                       BFvgGetf( BFVG_CONTEXT, a )
#define vgGeti(a)                       BFvgGeti( BFVG_CONTEXT, a )
#define vgGetVectorSize(a)              BFvgGetVectorSize( BFVG_CONTEXT, a )
#define vgGetfv(a,b,c)                  BFvgGetfv( BFVG_CONTEXT, a, b, c )
#define vgGetiv(a,b,c)                  BFvgGetiv( BFVG_CONTEXT, a, b, c )
#define vgSetParameterf(a,b,c)          BFvgSetParameterf( BFVG_CONTEXT, a, b, c )
#define vgSetParameteri(a,b,c)          BFvgSetParameteri( BFVG_CONTEXT, a, b, c )
#define vgSetParameterfv(a,b,c,d)       BFvgSetParameterfv( BFVG_CONTEXT, a, b, c, d )
#define vgSetParameteriv(a,b,c,d)       BFvgSetParameteriv( BFVG_CONTEXT, a, b, c, d )
#define vgGetParameterf(a,b)            BFvgGetParameterf( BFVG_CONTEXT, a, b )
#define vgGetParameteri(a,b)            BFvgGetParameteri( BFVG_CONTEXT, a, b )
#define vgGetParameterVectorSize(a,b)   BFvgGetParameterVectorSize( BFVG_CONTEXT, a, b )
#define vgGetParameterfv(a,b,c,d)       BFvgGetParameterfv( BFVG_CONTEXT, a, b, c, d )
#define vgGetParameteriv(a,b,c,d)       BFvgGetParameteriv( BFVG_CONTEXT, a, b, c, d )
#define vgLoadIdentity()                BFvgLoadIdentity( BFVG_CONTEXT )
#define vgLoadMatrix(a)                 BFvgLoadMatrix( BFVG_CONTEXT, a )
#define vgGetMatrix(a)                  BFvgGetMatrix( BFVG_CONTEXT, a )
#define vgMultMatrix(a)                 BFvgMultMatrix( BFVG_CONTEXT, a )
#define vgTranslate(a,b)                BFvgTranslate( BFVG_CONTEXT, a, b )
#define vgScale(a,b)                    BFvgScale( BFVG_CONTEXT, a, b )
#define vgShear(a,b)                    BFvgShear( BFVG_CONTEXT, a, b )
#define vgRotate(a)                     BFvgRotate( BFVG_CONTEXT, a )
#define vgMask(a,b,c,d,e,f)             BFvgMask( BFVG_CONTEXT, a, b, c, d, e, f )
#define vgClear(a,b,c,d)                BFvgClear( BFVG_CONTEXT, a, b, c, d )
#define vgCreatePath(a,b,c,d,e,f,g )    BFvgCreatePath( BFVG_CONTEXT, a, b, c, d, e, f, g )
#define vgClearPath(a,b)                BFvgClearPath( BFVG_CONTEXT, a, b )
#define vgDestroyPath(a)                BFvgDestroyPath( BFVG_CONTEXT, a )
#define vgRemovePathCapabilities(a,b)   BFvgRemovePathCapabilities( BFVG_CONTEXT, a, b )
#define vgGetPathCapabilities(a)        BFvgGetPathCapabilities( BFVG_CONTEXT, a )
#define vgAppendPath(a,b)               BFvgAppendPath( BFVG_CONTEXT, a, b )
#define vgAppendPathData(a,b,c,d)       BFvgAppendPathData( BFVG_CONTEXT, a, b, c, d )
#define vgReadPathData(a,b,c,d)         BFvgReadPathData( BFVG_CONTEXT, a, b, c, d )
#define vgModifyPathCoords(a,b,c,d)     BFvgModifyPathCoords( BFVG_CONTEXT, a, b, c, d )
#define vgTransformPath(a,b)            BFvgTransformPath( BFVG_CONTEXT, a, b )
#define vgInterpolatePath(a,b,c,d)      BFvgInterpolatePath( BFVG_CONTEXT, a, b, c, d )
#define vgPathLength(a,b,c)             BFvgPathLength( BFVG_CONTEXT, a, b, c )
#define vgPointAlongPath(a,b,c,d,e,f,g,h)  BFvgPointAlongPath( BFVG_CONTEXT, a, b, c, d, e, f, g, h )
#define vgPathBounds(a,b,c,d,e)         BFvgPathBounds( BFVG_CONTEXT, a, b, c, d, e )
#define vgPathTransformedBounds(a,b,c,d,e) BFvgPathTransformedBounds( BFVG_CONTEXT, a, b, c, d, e )
#define vgDrawPath(a,b)                 BFvgDrawPath( BFVG_CONTEXT, a, b )
#define vgPickPath(a,b,c,d)             BFvgPickPath( BFVG_CONTEXT, a, b, c, d )
#define vgCreatePaint()                 BFvgCreatePaint( BFVG_CONTEXT )
#define vgDestroyPaint(a)               BFvgDestroyPaint( BFVG_CONTEXT, a )
#define vgSetPaint(a,b)                 BFvgSetPaint( BFVG_CONTEXT, a, b )
#define vgGetPaint(a)                   BFvgGetPaint( BFVG_CONTEXT, a )
#define vgSetColor(a,b)                 BFvgSetColor( BFVG_CONTEXT, a, b )
#define vgGetColor(a)                   BFvgGetColor( BFVG_CONTEXT, a )
#define vgPaintPattern(a,b)             BFvgPaintPattern( BFVG_CONTEXT, a, b )
#define vgCreateImage(a,b,c,d)          BFvgCreateImage( BFVG_CONTEXT, a, b, c, d )
#define vgDestroyImage(a)               BFvgDestroyImage( BFVG_CONTEXT, a )
#define vgClearImage(a,b,c,d,e)         BFvgClearImage( BFVG_CONTEXT, a, b, c, d, e )
#define vgImageSubData(a,b,c,d,e,f,g,h)   BFvgImageSubData( BFVG_CONTEXT, a, b, c, d, e, f, g, h )
#define vgGetImageSubData(a,b,c,d,e,f,g,h) BFvgGetImageSubData( BFVG_CONTEXT, a, b, c, d, e, f, g, h )
#define vgChildImage(a,b,c,d,e)         BFvgChildImage( BFVG_CONTEXT, a, b, c, d, e )
#define vgGetParent(a)                  BFvgGetParent( BFVG_CONTEXT, a )
#define vgCopyImage(a,b,c,d,e,f,g,h,i)  BFvgCopyImage( BFVG_CONTEXT, a, b, c, d, e, f, g, h, i )
#define vgDrawImage(a)                  BFvgDrawImage( BFVG_CONTEXT, a )
#define vgPickImage(a,b,c)              BFvgPickImage( BFVG_CONTEXT, a, b, c )
#define vgSetPixels(a,b,c,d,e,f,g)      BFvgSetPixels( BFVG_CONTEXT, a, b, c, d, e, f, g )
#define vgWritePixels(a,b,c,d,e,f,g)    BFvgWritePixels( BFVG_CONTEXT, a, b, c, d, e, f, g )
#define vgGetPixels(a,b,c,d,e,f,g)      BFvgGetPixels( BFVG_CONTEXT, a, b, c, d, e, f, g )
#define vgReadPixels(a,b,c,d,e,f,g)     BFvgReadPixels( BFVG_CONTEXT, a, b, c, d, e, f, g )
#define vgCopyPixels(a,b,c,d,e,f)       BFvgCopyPixels( BFVG_CONTEXT, a, b, c, d, e, f )
#define vgColorMatrix(a,b,c)            BFvgColorMatrix( BFVG_CONTEXT, a,b, c )
#define vgConvolve(a,b,c,d,e,f,g,h,i,j,k)   BFvgConvolve( BFVG_CONTEXT, a, b, c, d, e, f, g, h, i, j, k )
#define vgSeparableConvolve(a,b,c,d,e,f,g,h,i,j,k,l,m)     BFvgSeparableConvolve( BFVG_CONTEXT, a, b, c, d, e, f, g, h, i, j, k, l, m )
#define vgGaussianBlur(a,b,c,d,e)       BFvgGaussianBlur( BFVG_CONTEXT, a, b, c, d, e )
#define vgLookup(a,b,c,d,e,f,g,h)       BFvgLookup( BFVG_CONTEXT, a, b, c, d, e, f, g, h )
#define vgLookupSingle(a,b,c,d,e,f)     BFvgLookupSingle( BFVG_CONTEXT, a, b, c, d, e, f )
#define vgHardwareQuery(a,b)            BFvgHardwareQuery( BFVG_CONTEXT, a, b )
#define vgGetString(a)                  BFvgGetString( BFVG_CONTEXT, a )

#else

/* BF extensions to OpenVG. The interfaces defined must be implemented by the external OpenVG developer if the extensions
 * are supported by the external OpenVG implementation.
 */
#include "vgReadPathData.h" /* functionality must be implemented if BF_CFG_USE_EXTERNAL_OPENVG_READPATH_EXT is defined */
#include "vgPick.h"         /* functionality must be implemented if BF_CFG_USE_EXTERNAL_OPENVG_PICK_EXT is defined */

#endif

/* BF extensions to OpenVG. Types defined in these files are required regardless of OpenVG configuration. The interfaces defined
 * must be implemented by the external OpenVG developer if the extensions are supported by the external OpenVG implementation.
 */
#include "vgDrawableKHR.h"  /* functionality must be implemented if BF_CFG_USE_EXTERNAL_OPENVG_DRAWABLE_EXT is defined */


#if defined( BF_CFG_USE_BFOPENVG ) && !defined ( BF_CFG_OVG_COMPLIANCE )

/* Context setters */
#define ovgSetMatrixMode( aMode )                   BFvgSetMatrixMode( BFVG_CONTEXT, aMode )
#define ovgSetFillRule( aFillRule )                 BFvgSetFillRule( BFVG_CONTEXT, aFillRule )
#define ovgSetImageQuality( aImageQuality )         BFvgSetImageQuality( BFVG_CONTEXT, aImageQuality )
#define ovgSetRenderingQuality( aRenderingQuality ) BFvgSetRenderingQuality( BFVG_CONTEXT, aRenderingQuality )
#define ovgSetBlendMode( aBlendMode )               BFvgSetBlendMode( BFVG_CONTEXT, aBlendMode )
#define ovgSetImageMode( aImageMode )               BFvgSetImageMode( BFVG_CONTEXT, aImageMode )
#define ovgSetScissorRects( count, aScissorRects )  BFvgSetScissorRects( BFVG_CONTEXT, count, 0, aScissorRects )
#define ovgSetStrokeLineWidth( aStrokeLineWidth )   BFvgSetStrokeLineWidth( BFVG_CONTEXT, aStrokeLineWidth )
#define ovgSetCapStyle( aCapStyle )                 BFvgSetCapStyle( BFVG_CONTEXT, aCapStyle )
#define ovgSetJoinStyle( aJoinStyle )               BFvgSetJoinStyle( BFVG_CONTEXT, aJoinStyle )
#define ovgSetMiterLimit( aMiterLimit )             BFvgSetMiterLimit( BFVG_CONTEXT, aMiterLimit )
#define ovgSetDashPattern( count, aDashPattern )    BFvgSetDashPattern( BFVG_CONTEXT, count, aDashPattern, 0 )
#define ovgSetDashPhase( aDashPhase )               BFvgSetDashPhase( BFVG_CONTEXT, aDashPhase )
#define ovgSetTileFillColor( aTileFillColor )       BFvgSetTileFillColor( BFVG_CONTEXT, aTileFillColor, 0 )
#define ovgSetClearColor( aClearColor )             BFvgSetClearColor( BFVG_CONTEXT, aClearColor, 0 )
#define ovgSetMasking( aMasking )                   BFvgSetMasking( BFVG_CONTEXT, aMasking )
#define ovgSetScissoring( aScissoring )             BFvgSetScissoring( BFVG_CONTEXT, aScissoring )
#define ovgSetPixelLayout( aPixelLayout )           BFvgSetPixelLayout( BFVG_CONTEXT, aPixelLayout )

#define ovgSetFilterFormatLinear( aFilterFormatLinear )                 BFvgSetFilterFormatLinear( BFVG_CONTEXT, aFilterFormatLinear )
#define ovgSetFilterFormatPremultiplied( aFilterFormatPremultiplied )   BFvgSetFilterFormatPremultiplied( BFVG_CONTEXT, aFilterFormatPremultiplied )
#define ovgSetFilterChannelMask( aFilterChannelMask )                   BFvgSetFilterChannelMask( BFVG_CONTEXT, aFilterChannelMask )

/* Context getters */
#define ovgGetMatrixMode()                          BFvgGetMatrixMode( BFVG_CONTEXT )
#define ovgGetFillRule()                            BFvgGetFillRule( BFVG_CONTEXT )
#define ovgGetImageQuality()                        BFvgGetImageQuality( BFVG_CONTEXT )
#define ovgGetRenderingQuality()                    BFvgGetRenderingQuality( BFVG_CONTEXT )
#define ovgGetBlendMode()                           BFvgGetBlendMode( BFVG_CONTEXT )
#define ovgGetImageMode()                           BFvgGetImageMode( BFVG_CONTEXT )
#define ovgGetScissorRects( count, aScissorRects )  BFvgGetScissorRects( BFVG_CONTEXT, count, 0, aScissorRects )
#define ovgGetStrokeLineWidth()                     BFvgGetStrokeLineWidth( BFVG_CONTEXT )
#define ovgGetCapStyle()                            BFvgGetCapStyle( BFVG_CONTEXT )
#define ovgGetJoinStyle()                           BFvgGetJoinStyle( BFVG_CONTEXT )
#define ovgGetMiterLimit()                          BFvgGetMiterLimit( BFVG_CONTEXT )
#define ovgGetDashPattern( count, aDashPattern )    BFvgGetDashPattern( BFVG_CONTEXT, count, aDashPattern, 0 )
#define ovgGetDashPhase()                           BFvgGetDashPhase( BFVG_CONTEXT )
#define ovgGetTileFillColor( aTileFillColor )       BFvgGetTileFillColor( BFVG_CONTEXT, aTileFillColor, 0 )
#define ovgGetClearColor( aClearColor )             BFvgGetClearColor( BFVG_CONTEXT, aClearColor, 0 )
#define ovgGetMasking()                             BFvgGetMasking( BFVG_CONTEXT )
#define ovgGetScissoring()                          BFvgGetScissoring( BFVG_CONTEXT )
#define ovgGetPixelLayout()                         BFvgGetPixelLayout( BFVG_CONTEXT )

#define ovgGetFilterFormatLinear()                  BFvgGetFilterFormatLinear( BFVG_CONTEXT )
#define ovgGetFilterFormatPremultiplied()           BFvgGetFilterFormatPremultiplied( BFVG_CONTEXT )
#define ovgGetFilterChannelMask()                   BFvgGetFilterChannelMask( BFVG_CONTEXT )

#define ovgGetMaxScissorRects()                     BFvgGetMaxScissorRects( BFVG_CONTEXT )
#define ovgGetMaxDashCount()                        BFvgGetMaxDashCount( BFVG_CONTEXT )
#define ovgGetMaxKernelSize()                       BFvgGetMaxKernelSize( BFVG_CONTEXT )
#define ovgGetMaxSeparableKernelSize()              BFvgGetMaxSeparableKernelSize( BFVG_CONTEXT )
#define ovgGetMaxColorRampStops()                   BFvgGetMaxColorRampStops( BFVG_CONTEXT )
#define ovgGetMaxImageWidth()                       BFvgGetMaxImageWidth( BFVG_CONTEXT )
#define ovgGetMaxImageHeight()                      BFvgGetMaxImageHeight( BFVG_CONTEXT )
#define ovgGetMaxImagePixels()                      BFvgGetMaxImagePixels( BFVG_CONTEXT )
#define ovgGetMaxImageBytes()                       BFvgGetMaxImageBytes( BFVG_CONTEXT )
#define ovgGetMaxFloat()                            BFvgGetMaxFloat( BFVG_CONTEXT )

/* Object parameter setters */

#define ovgSetPaintType( paint, aType )                 BFvgSetPaintType( BFVG_CONTEXT, paint, aType )
#define ovgSetPaintColor( paint, aColor )               BFvgSetPaintColor( BFVG_CONTEXT, paint, aColor, 0 )
#define ovgSetPaintColorRampSpreadMode( paint, aMode )  BFvgSetPaintColorRampSpreadMode( BFVG_CONTEXT, paint, aMode )
#define ovgSetPaintColorRampStops( paint, count, aStops )      BFvgSetPaintColorRampStops( BFVG_CONTEXT, paint, count, aStops, 0 )
#define ovgSetPaintLinearGradient( paint, aGrad )       BFvgSetPaintLinearGradient( BFVG_CONTEXT, paint, aGrad, 0 )
#define ovgSetPaintRadialGradient( paint, aGrad )       BFvgSetPaintRadialGradient( BFVG_CONTEXT, paint, aGrad, 0 )
#define ovgSetPaintPatternTilingMode( paint, aMode )    BFvgSetPaintPatternTilingMode( BFVG_CONTEXT, paint, aMode )

/* Object parameter getters */

#define ovgGetPathFormat( path )        BFvgGetPathFormat( BFVG_CONTEXT, path )
#define ovgGetPathDataType( path )      BFvgGetPathDataType( BFVG_CONTEXT, path )
#define ovgGetPathScale( path )         BFvgGetPathScale( BFVG_CONTEXT, path )
#define ovgGetPathBias( path )          BFvgGetPathBias( BFVG_CONTEXT, path )
#define ovgGetPathNumSegments( path )   BFvgGetPathNumSegments( BFVG_CONTEXT, path )
#define ovgGetPathNumCoords( path )     BFvgGetPathNumCoords( BFVG_CONTEXT, path )

#define ovgGetPaintType( paint )                    BFvgGetPaintType( BFVG_CONTEXT, paint )
#define ovgGetPaintColor( paint, aColor )           BFvgGetPaintColor( BFVG_CONTEXT, paint, aColor, 0 )
#define ovgGetPaintColorRampSpreadMode( paint )     BFvgGetPaintColorRampSpreadMode( BFVG_CONTEXT, paint )
#define ovgGetPaintColorRampStops( paint, count, aStops )  BFvgGetPaintColorRampStops( BFVG_CONTEXT, paint, count, aStops, 0 )
#define ovgGetPaintLinearGradient( paint, aGrad )   BFvgGetPaintLinearGradient( BFVG_CONTEXT, paint, aGrad, 0 )
#define ovgGetPaintRadialGradient( paint, aGrad )   BFvgGetPaintRadialGradient( BFVG_CONTEXT, paint, aGrad, 0 )
#define ovgGetPaintPatternTilingMode( paint )       BFvgGetPaintPatternTilingMode( BFVG_CONTEXT, paint )

#define ovgGetImageFormat( image )  BFvgGetImageFormat( BFVG_CONTEXT, image )
#define ovgGetImageWidth( image )   BFvgGetImageWidth( BFVG_CONTEXT, image )
#define ovgGetImageHeight( image )  BFvgGetImageHeight( BFVG_CONTEXT, image )

#else

/* Context setters */
#define ovgSetMatrixMode( aMode )                   vgSeti( VG_MATRIX_MODE, aMode )
#define ovgSetFillRule( aFillRule )                 vgSeti( VG_FILL_RULE,  aFillRule )
#define ovgSetImageQuality( aImageQuality )         vgSeti( VG_IMAGE_QUALITY, aImageQuality )
#define ovgSetRenderingQuality( aRenderingQuality ) vgSeti( VG_RENDERING_QUALITY, aRenderingQuality )
#define ovgSetBlendMode( aBlendMode )               vgSeti( VG_BLEND_MODE, aBlendMode )
#define ovgSetImageMode( aImageMode )               vgSeti( VG_IMAGE_MODE, aImageMode )
#define ovgSetScissorRects( count, aScissorRects )  vgSetiv( VG_SCISSOR_RECTS, count, aScissorRects )
#define ovgSetStrokeLineWidth( aStrokeLineWidth )   vgSetf( VG_STROKE_LINE_WIDTH, aStrokeLineWidth )
#define ovgSetCapStyle( aCapStyle )                 vgSeti( VG_STROKE_CAP_STYLE, aCapStyle )
#define ovgSetJoinStyle( aJoinStyle )               vgSeti( VG_STROKE_JOIN_STYLE, aJoinStyle )
#define ovgSetMiterLimit( aMiterLimit )             vgSetf( VG_STROKE_MITER_LIMIT, aMiterLimit )
#define ovgSetDashPattern( count, aDashPattern )    vgSetfv( VG_STROKE_DASH_PATTERN, count, aDashPattern )
#define ovgSetDashPhase( aDashPhase )               vgSetf( VG_STROKE_DASH_PHASE, aDashPhase )
#define ovgSetTileFillColor( aTileFillColor )       vgSetfv( VG_TILE_FILL_COLOR, 4, aTileFillColor )
#define ovgSetClearColor( aClearColor )             vgSetfv( VG_CLEAR_COLOR, 4, aClearColor )
#define ovgSetMasking( aMasking )                   vgSeti( VG_MASKING, aMasking )
#define ovgSetScissoring( aScissoring )             vgSeti( VG_SCISSORING, aScissoring )
#define ovgSetPixelLayout( aPixelLayout )           vgSeti( VG_PIXEL_LAYOUT, aPixelLayout )

#define ovgSetFilterFormatLinear( aFilterFormatLinear )                 vgSeti( VG_FILTER_FORMAT_LINEAR, aFilterFormatLinear )
#define ovgSetFilterFormatPremultiplied( aFilterFormatPremultiplied )   vgSeti( VG_FILTER_FORMAT_PREMULTIPLIED, aFilterFormatPremultiplied )
#define ovgSetFilterChannelMask( aFilterChannelMask )                   vgSeti( VG_FILTER_CHANNEL_MASK, aFilterChannelMask )

/* Context getters */
#define ovgGetMatrixMode()                          vgGeti( VG_MATRIX_MODE )
#define ovgGetFillRule()                            vgGeti( VG_FILL_RULE )
#define ovgGetImageQuality()                        vgGeti( VG_IMAGE_QUALITY )
#define ovgGetRenderingQuality()                    vgGeti( VG_RENDERING_QUALITY )
#define ovgGetBlendMode()                           vgGeti( VG_BLEND_MODE )
#define ovgGetImageMode()                           vgGeti( VG_IMAGE_MODE )
#define ovgGetScissorRects( count, aScissorRects )  vgGetiv( VG_SCISSOR_RECTS, count, aScissorRects )
#define ovgGetStrokeLineWidth()                     vgGetf( VG_STROKE_LINE_WIDTH )
#define ovgGetCapStyle()                            vgGeti( VG_STROKE_CAP_STYLE )
#define ovgGetJoinStyle()                           vgGeti( VG_STROKE_JOIN_STYLE )
#define ovgGetMiterLimit()                          vgGetf( VG_STROKE_MITER_LIMIT )
#define ovgGetDashPattern( count, aDashPattern )    vgGetfv( VG_STROKE_DASH_PATTERN, count, aDashPattern )
#define ovgGetDashPhase()                           vgGetf( VG_STROKE_DASH_PHASE )
#define ovgGetTileFillColor( aTileFillColor )       vgGetfv( VG_TILE_FILL_COLOR, 4, aTileFillColor )
#define ovgGetClearColor( aClearColor )             vgGetfv( VG_CLEAR_COLOR, 4, aClearColor )
#define ovgGetMasking()                             vgGeti( VG_MASKING )
#define ovgGetScissoring()                          vgGeti( VG_SCISSORING )
#define ovgGetPixelLayout()                         vgGeti( VG_PIXEL_LAYOUT )

#define ovgGetFilterFormatLinear( aFilterFormatLinear )                 vgGeti( VG_FILTER_FORMAT_LINEAR )
#define ovgGetFilterFormatPremultiplied( aFilterFormatPremultiplied )   vgGeti( VG_FILTER_FORMAT_PREMULTIPLIED )
#define ovgGetFilterChannelMask( aFilterChannelMask )                   vgGeti( VG_FILTER_CHANNEL_MASK )

#define ovgGetMaxScissorRects()         vgGeti( VG_MAX_SCISSOR_RECTS )
#define ovgGetMaxDashCount()            vgGeti( VG_MAX_DASH_COUNT )
#define ovgGetMaxKernelSize()           vgGeti( VG_MAX_KERNEL_SIZE )
#define ovgGetMaxSeparableKernelSize()  vgGeti( VG_MAX_SEPARABLE_KERNEL_SIZE )
#define ovgGetMaxColorRampStops()       vgGeti( VG_MAX_COLOR_RAMP_STOPS )
#define ovgGetMaxImageWidth()           vgGeti( VG_MAX_IMAGE_WIDTH )
#define ovgGetMaxImageHeight()          vgGeti( VG_MAX_IMAGE_HEIGHT )
#define ovgGetMaxImagePixels()          vgGeti( VG_MAX_IMAGE_PIXELS )
#define ovgGetMaxImageBytes()           vgGeti( VG_MAX_IMAGE_BYTES )
#define ovgGetMaxFloat()                vgGetf( VG_MAX_FLOAT )

/* Object parameter setters */

#define ovgSetPaintType( paint, aType )                 vgSetParameteri( paint, VG_PAINT_TYPE, aType )
#define ovgSetPaintColor( paint, aColor )               vgSetParameterfv( paint, VG_PAINT_COLOR, 4, aColor )
#define ovgSetPaintColorRampSpreadMode( paint, aMode )  vgSetParameteri( paint, VG_PAINT_COLOR_RAMP_SPREAD_MODE, aMode )
#define ovgSetPaintColorRampStops( paint, count, aStops )      vgSetParameterfv( paint, VG_PAINT_COLOR_RAMP_STOPS, count, aStops )
#define ovgSetPaintLinearGradient( paint, aGrad )       vgSetParameterfv( paint, VG_PAINT_LINEAR_GRADIENT, 4, aGrad )
#define ovgSetPaintRadialGradient( paint, aGrad )       vgSetParameterfv( paint, VG_PAINT_RADIAL_GRADIENT, 5, aGrad )
#define ovgSetPaintPatternTilingMode( paint, aMode )    vgSetParameteri( paint, VG_PAINT_PATTERN_TILING_MODE, aMode )

/* Object parameter getters */

#define ovgGetPathFormat( path )        vgGetParameteri( path, VG_PATH_FORMAT )
#define ovgGetPathDataType( path )      vgGetParameteri( path, VG_PATH_DATATYPE )
#define ovgGetPathScale( path )         vgGetParameterf( path, VG_PATH_SCALE )
#define ovgGetPathBias( path )          vgGetParameterf( path, VG_PATH_BIAS )
#define ovgGetPathNumSegments( path )   vgGetParameteri( path, VG_PATH_NUM_SEGMENTS )
#define ovgGetPathNumCoords( path )     vgGetParameteri( path, VG_PATH_NUM_COORDS )

#define ovgGetPaintType( paint )                    vgGetParameteri( paint, VG_PAINT_TYPE )
#define ovgGetPaintColor( paint, aColor )           vgGetParameterfv( paint, VG_PAINT_COLOR, 4, aColor )
#define ovgGetPaintColorRampSpreadMode( paint )     vgGetParameteri( paint, VG_PAINT_COLOR_RAMP_SPREAD_MODE )
#define ovgGetPaintColorRampStops( paint, count, aStops )  vgGetParameterfv( paint, VG_PAINT_COLOR_RAMP_STOPS, count, aStops )
#define ovgGetPaintLinearGradient( paint, aGrad )   vgGetParameterfv( paint, VG_PAINT_LINEAR_GRADIENT, 4, aGrad )
#define ovgGetPaintRadialGradient( paint, aGrad )   vgGetParameterfv( paint, VG_PAINT_RADIAL_GRADIENT, 5, aGrad )
#define ovgGetPaintPatternTilingMode( paint )       vgGetParameteri( paint, VG_PAINT_PATTERN_TILING_MODE )

#define ovgGetImageFormat( image )  vgGetParameteri( image, VG_IMAGE_FORMAT )
#define ovgGetImageWidth( image )   vgGetParameteri( image, VG_IMAGE_WIDTH )
#define ovgGetImageHeight( image )  vgGetParameteri( image, VG_IMAGE_HEIGHT )

#endif


#if !defined( BF_CFG_USE_EXTERNAL_OPENVG ) || !defined( BF_CFG_USE_EXTERNAL_OPENVG_READPATH_EXT )

/* natural getters and setters, specific to our software OVG, used when OVG compliance is not enforced */
void BFvgSetMatrixMode( BFOpenVG pCX, VGMatrixMode aMatrixMode );
void BFvgSetFillRule( BFOpenVG pCX, VGFillRule aFillRule );
void BFvgSetImageQuality( BFOpenVG pCX, VGImageQuality aImageQuality );
void BFvgSetRenderingQuality( BFOpenVG pCX, VGRenderingQuality aRenderingQuality );
void BFvgSetBlendMode( BFOpenVG pCX, VGBlendMode aBlendMode );
void BFvgSetImageMode( BFOpenVG pCX, VGImageMode aImageMode );
void BFvgSetScissorRects( BFOpenVG pCX, VGint aCount, const VGfloat *afScissorRects, const VGint *aiScissorRects );
void BFvgSetStrokeLineWidth( BFOpenVG pCX, VGfloat aStrokeLineWidth );
void BFvgSetCapStyle( BFOpenVG pCX, VGCapStyle aCapStyle );
void BFvgSetJoinStyle( BFOpenVG pCX, VGJoinStyle aJoinStyle );
void BFvgSetMiterLimit( BFOpenVG pCX, VGfloat aMiterLimit );
void BFvgSetDashPattern( BFOpenVG pCX, VGint aCount, const VGfloat *afDashPattern, const VGint *aiDashPattern );
void BFvgSetDashPhase( BFOpenVG pCX, VGfloat aDashPhase );
void BFvgSetTileFillColor( BFOpenVG pCX, const VGfloat *afTileFillColor, const VGint *aiTileFillColor );
void BFvgSetClearColor( BFOpenVG pCX, const VGfloat *afClearColor, const VGint *aiClearColor );
void BFvgSetMasking( BFOpenVG pCX, VGboolean aMasking );
void BFvgSetScissoring( BFOpenVG pCX, VGboolean aScissoring );
void BFvgSetPixelLayout( BFOpenVG pCX, VGPixelLayout aPixelLayout );
void BFvgSetFilterFormatLinear( BFOpenVG pCX, VGboolean aFilterFormatLinear );
void BFvgSetFilterFormatPremultiplied( BFOpenVG pCX, VGboolean aFilterFormatPremultiplied );
void BFvgSetFilterChannelMask( BFOpenVG pCX, VGbitfield aFilterChannelMask );

VGMatrixMode BFvgGetMatrixMode(BFOpenVG pCX);
VGFillRule BFvgGetFillRule(BFOpenVG pCX);
VGImageQuality BFvgGetImageQuality(BFOpenVG pCX);
VGRenderingQuality BFvgGetRenderingQuality(BFOpenVG pCX);
VGBlendMode BFvgGetBlendMode(BFOpenVG pCX);
VGImageMode BFvgGetImageMode(BFOpenVG pCX);
void BFvgGetScissorRects( BFOpenVG pCX, VGint aCount, VGfloat *afScissorRects, VGint *aiScissorRects );
VGfloat BFvgGetStrokeLineWidth(BFOpenVG pCX);
VGCapStyle BFvgGetCapStyle(BFOpenVG pCX);
VGJoinStyle BFvgGetJoinStyle(BFOpenVG pCX);
VGfloat BFvgGetMiterLimit(BFOpenVG pCX);
void BFvgGetDashPattern( BFOpenVG pCX, VGint aCount, VGfloat *afDashPattern, VGint *aiDashPattern );
VGfloat BFvgGetDashPhase(BFOpenVG pCX);
void BFvgGetTileFillColor( BFOpenVG pCX, VGfloat *afTileFillColor, VGint *aiTileFillColor );
void BFvgGetClearColor( BFOpenVG pCX, VGfloat *afClearColor, VGint *aiClearColor );
VGboolean BFvgGetMasking(BFOpenVG pCX);
VGboolean BFvgGetScissoring(BFOpenVG pCX);
VGPixelLayout BFvgGetPixelLayout(BFOpenVG pCX);
VGboolean BFvgGetFilterFormatLinear(BFOpenVG pCX);
VGboolean BFvgGetFilterFormatPremultiplied(BFOpenVG pCX);
VGbitfield BFvgGetFilterChannelMask(BFOpenVG pCX);

VGint BFvgGetMaxScissorRects(BFOpenVG pCX);
VGint BFvgGetMaxDashCount(BFOpenVG pCX);
VGint BFvgGetMaxKernelSize(BFOpenVG pCX);
VGint BFvgGetMaxSeparableKernelSize(BFOpenVG pCX);
VGint BFvgGetMaxColorRampStops(BFOpenVG pCX);
VGint BFvgGetMaxImageWidth(BFOpenVG pCX);
VGint BFvgGetMaxImageHeight(BFOpenVG pCX);
VGint BFvgGetMaxImagePixels(BFOpenVG pCX);
VGint BFvgGetMaxImageBytes(BFOpenVG pCX);
VGfloat BFvgGetMaxFloat(BFOpenVG pCX);

void BFvgSetPaintType( BFOpenVG pCX, VGPaint paint, VGPaintType aType );
void BFvgSetPaintColor( BFOpenVG pCX, VGPaint paint, const VGfloat *fcolors, const VGint *icolors );
void BFvgSetPaintColorRampSpreadMode( BFOpenVG pCX, VGPaint paint, VGColorRampSpreadMode aColorRampSpreadMode );
void BFvgSetPaintColorRampStops( BFOpenVG pCX, VGPaint paint, VGint count, const VGfloat *fstops, const VGint *istops );
void BFvgSetPaintLinearGradient( BFOpenVG pCX, VGPaint paint, const VGfloat *fgradient, const VGint *igradient );
void BFvgSetPaintRadialGradient( BFOpenVG pCX, VGPaint paint, const VGfloat *fgradient, const VGint *igradient );
void BFvgSetPaintPatternTilingMode( BFOpenVG pCX, VGPaint paint, VGTilingMode aTilingMode );

VGPaintType BFvgGetPaintType( BFOpenVG pCX, VGPaint paint );
void BFvgGetPaintColor( BFOpenVG pCX, VGPaint paint, VGfloat *fcolors, VGint *icolors );
VGColorRampSpreadMode BFvgGetPaintColorRampSpreadMode( BFOpenVG pCX, VGPaint paint );
void BFvgGetPaintColorRampStops( BFOpenVG pCX, VGPaint paint, VGint count, VGfloat *fstops, VGint *istops );
void BFvgGetPaintLinearGradient( BFOpenVG pCX, VGPaint paint, VGfloat *fgradient, VGint *igradient );
void BFvgGetPaintRadialGradient( BFOpenVG pCX, VGPaint paint, VGfloat *fgradient, VGint *igradient );
VGTilingMode BFvgGetPaintPatternTilingMode( BFOpenVG pCX, VGPaint paint );


VGint BFvgGetPathFormat( BFOpenVG pCX, VGPath path );
VGPathDatatype BFvgGetPathDataType( BFOpenVG pCX, VGPath path );
VGfloat BFvgGetPathScale( BFOpenVG pCX, VGPath path );
VGfloat BFvgGetPathBias( BFOpenVG pCX, VGPath path );
VGint BFvgGetPathNumSegments( BFOpenVG pCX, VGPath path );
VGint BFvgGetPathNumCoords( BFOpenVG pCX, VGPath path );

VGImageFormat BFvgGetImageFormat( BFOpenVG pCX, VGImage image );
VGint BFvgGetImageWidth( BFOpenVG pCX, VGImage image );
VGint BFvgGetImageHeight( BFOpenVG pCX, VGImage image );

/** \endcond */ /* ends OVG_INTERNAL_SHOW_DOXYGEN conditional section */

/** \file BFOpenVG.h
 *
 * Functionality to handle OpenVG contexts. This functionality is not part of the OpenVG spec, and is specific to
 * the BitFlash internal OpenVG implementation. When configured to use the internal OpenVG implementation the SVG
 * Player invokes these functions to create an OpenVG context, as well as to support extensions required by the
 * internal OpenVG implementation. These functions are not built or used as part of the SVG Player if an external
 * OpenVG implementation is used.
 */

/** return value for BFOPENVG_AttachRenderingBuffer() */
typedef enum
{
    BFOPENVG_BUFFER_CREATE, /**< the VGimage used as the render target is (re)created */
    BFOPENVG_BUFFER_ATTACH, /**< the VGimage used as the render target is maintained but has a new data buffer associated with it */
    BFOPENVG_BUFFER_FAIL    /**< (memory allocation) failure */
} BFOPENVGattachStatus;

/** Creates and returns a pointer to an OpenVG context. This function must be called before making any OpenVG calls.
 * \param pMemCtx       pointer to an existing memory context that will be used for all normal memory allocations within the
 *                      BF internal OpenVG implementation.
 * \param pFastMemCtx   pointer to an existing memory context that will be used for all fast memory allocations within the
 *                      BF internal OpenVG implementation.
 */
BF_API_CALL BFOpenVG BFOPENVG_Create( BFHeapContext *pMemCtx, BFHeapContext *pFastMemCtx );

/** Creates or modifies a VGImage that is used as the default render target of the BF internal OpenVG implementation.
 *  If one is specified, a memory buffer is attached to this VGImage render target. This function must be called to
 *  at least define the dimensions of the render target for the specified OpenVG context.
 * \param pVG           the OpenVG context to set the render target for
 * \param uWidth        the width in pixels of the render target
 * \param uHeight       the height in pixels of the render target
 * \param iLinePadding  the number of (filler) bytes between the last pixel in one row and the first in the next. This allows
 *                      subimages to be supported as render targets. For example the render target could be directly in the
 *                      screen memory but restricted to a subregion of it by proper offset to pBuffer (see below) and line padding.
 * \param pBuffer       pointer to the buffer to be attached to the VGImage render target associated with the pVG OpenVG context.
 *                      This buffer is assumed to have dimensions given by uWidth and uHeight and line padding given by
 *                      iLinePadding. If pBuffer is 0 then a buffer of dimensions uWidth and uHeight and line padding of
 *                      iLinePadding is created and associated with the VGImage used as the default render target. Note that there
 *                      is no benefit of specifying a non-zero line padding value in this case.
 * \param pfnClearCB    pointer to a callback function that performs clearing of the render target. Anytime a BF_COMMAND_VIEW_REDRAW
 *                      command is processed the render target is first cleared. If pfnClearCB is not NULL then the callback is
 *                      invoked by the Player to perform the clearing. This allows the render target to be cleared with an arbitrary "background".
 * \param pClearUserData    a pointer that is passed to pfnClearCB whenever it is invoked by the Player.
 * \return              a BFOPENVGattachStatus value indicating the success status.
 */
BF_API_CALL BFOPENVGattachStatus BFOPENVG_AttachRenderingBuffer( BFOpenVG pVG, BF_UINT32 uWidth, BF_UINT32 uHeight,
        BF_INT16 iLinePadding, void *pBuffer, BFClearCallback pfnClearCB, BF_INT_VOID pClearUserData );
/** ensures that no pixels in the render target are written to under any circumstances
 * \param vg            the OpenVG context to disable/enable rendering to
 * \param bDisable      disables rendering if VG_TRUE, enables rendering if VG_FALSE
 */
BF_API_CALL void BFOPENVG_DisableRendering( BFOpenVG vg, VGboolean bDisable );

/** frees all resources associated with the OpenVG context and destroys it. The resources freed does not include any paths, paints,
 * images or fonts created for this context, unless BF_CFG_OVG_COMPLIANCE is defined.
 */
BF_API_CALL void BFOPENVG_Destroy( BFOpenVG vg );

/** sets the gamma table to apply when performing anti-aliasing. This function supports
 *  the legacy feature of tweaking the anti-aliasing for the BF stroke text rendering.
 *  \param pCX          the current OpenVG context
 *  \param aTable       BF_GAMMA_SMOOTH or BF_GAMMA_CRISP
 */
BF_API_CALL void BFOPENVG_SetGammaTable( BFOpenVG pCX, BF_INT32 aTable );

/** sets the internal status flag of the OpenVG context. The only flag supported is BFVG_RENDER_OVERFLOW
 *  which indicates that a fixed-point numeric overflow occured. This function is primarily used to
 *  simply clear the flag.
 *  \param pCX          the current OpenVG context
 *  \param aFlag        value of the flag to set
 */
BF_API_CALL void BFOPENVG_SetStatusFlag( BFOpenVG pCX, BF_UINT8 aFlag );

/** gets the internal status flag of the OpenVG context. The only flag supported is BFVG_RENDER_OVERFLOW
 *  which indicates that a fixed-point numeric overflow occured. This function is primarily used to
 *  determine if an overflow occured.
 *  \param pCX          the current OpenVG context
 *  \return             current value of the status flag in the OpenVG context
 */
BF_API_CALL BF_UINT8 BFOPENVG_GetStatusFlag( BFOpenVG pCX );
#endif

#ifdef __cplusplus
BF_END_EXTERN_C
#endif

#endif /* _OPENVG_H */

