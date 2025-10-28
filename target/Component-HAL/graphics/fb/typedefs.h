/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/


/** @file typedefs.h
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TYPEDEFS_H
#define EHS_TYPEDEFS_H

#include "globals.h"

/*lint -efile(751, typedefs.h) */
/*lint -efile(750, typedefs.h) */
/*lint -efile(753, typedefs.h) */
/*lint -efile(749, typedefs.h) */
typedef enum STGXOBJ_ColorType_e
{
    STGXOBJ_COLTYP_ARGB8888,
    STGXOBJ_COLTYP_RGB888,
    STGXOBJ_COLTYP_ARGB8565,
    STGXOBJ_COLTYP_RGB565,
    STGXOBJ_COLTYP_ARGB1555,
    STGXOBJ_COLTYP_ARGB4444,

    STGXOBJ_COLTYP_CLUT8,
    STGXOBJ_COLTYP_CLUT4,
    STGXOBJ_COLTYP_CLUT2,
    STGXOBJ_COLTYP_CLUT1,
    STGXOBJ_COLTYP_ACLUT88,
    STGXOBJ_COLTYP_ACLUT44,

    STGXOBJ_COLTYP_S_YCBCR888_444,
    STGXOBJ_COLTYP_U_YCBCR888_444,
    STGXOBJ_COLTYP_S_YCBCR888_422,
    STGXOBJ_COLTYP_U_YCBCR888_422,
    STGXOBJ_COLTYP_S_YCBCR888_420,
    STGXOBJ_COLTYP_U_YCBCR888_420,
    STGXOBJ_COLTYP_U_AYCBCR6888_444,
    STGXOBJ_COLTYP_S_AYCBCR8888,
    STGXOBJ_COLTYP_U_AYCBCR8888,

    STGXOBJ_COLTYP_ALPHA1,
    STGXOBJ_COLTYP_ALPHA4,
    STGXOBJ_COLTYP_ALPHA8,
    STGXOBJ_COLTYP_BYTE,

    STGXOBJ_COLTYP_ARGB8888_255,
    STGXOBJ_COLTYP_ARGB8565_255,
    STGXOBJ_COLTYP_ACLUT88_255,
    STGXOBJ_COLTYP_ALPHA8_255

} STGXOBJ_ColorType_t;

typedef enum STGXOBJ_BitmapType_e
{
    STGXOBJ_BITMAP_TYPE_RASTER_PROGRESSIVE,
    STGXOBJ_BITMAP_TYPE_RASTER_TOP_BOTTOM,
    STGXOBJ_BITMAP_TYPE_MB,
    STGXOBJ_BITMAP_TYPE_MB_HDPIP,
    STGXOBJ_BITMAP_TYPE_MB_TOP_BOTTOM,
    STGXOBJ_BITMAP_TYPE_MB_RANGE_MAP
} STGXOBJ_BitmapType_t;

typedef enum STGXOBJ_ColorSpaceConversionMode_e
{
    STGXOBJ_ITU_R_BT601,
    STGXOBJ_ITU_R_BT709,
    STGXOBJ_ITU_R_BT470_2_M,
    STGXOBJ_ITU_R_BT470_2_BG,
    STGXOBJ_SMPTE_170M,
    STGXOBJ_SMPTE_240M,
    STGXOBJ_FCC,
    STGXOBJ_CONVERSION_MODE_UNKNOWN
} STGXOBJ_ColorSpaceConversionMode_t;

typedef enum STGXOBJ_AspectRatio_e
{
    STGXOBJ_ASPECT_RATIO_16TO9,
    STGXOBJ_ASPECT_RATIO_4TO3,
    STGXOBJ_ASPECT_RATIO_221TO1,
    STGXOBJ_ASPECT_RATIO_SQUARE,
    STGXOBJ_ASPECT_RATIO_14TO9
} STGXOBJ_AspectRatio_t;

typedef enum STGXOBJ_SubByteFormat_e
{
    STGXOBJ_SUBBYTE_FORMAT_RPIX_MSB = 0,
    STGXOBJ_SUBBYTE_FORMAT_RPIX_LSB = 1
} STGXOBJ_SubByteFormat_t;

typedef enum YUV_Scaling_s
{
    YUV_NO_RESCALE = 1,   /* default value to be used in standard cases*/
    YUV_HALF_VALUE,
    YUV_RANGE_MAP_0,
    YUV_RANGE_MAP_1,
    YUV_RANGE_MAP_2,
    YUV_RANGE_MAP_3,
    YUV_RANGE_MAP_4,
    YUV_RANGE_MAP_5,
    YUV_RANGE_MAP_6,
    YUV_RANGE_MAP_7

} YUV_Scaling_t;

typedef struct YUV_ScalingFactor_s
{
    YUV_Scaling_t ScalingFactorY;
    YUV_Scaling_t ScalingFactorUV;

} YUV_ScalingFactor_t;

typedef struct STGXOBJ_Bitmap_s
{
    STGXOBJ_ColorType_t                   ColorType;
    STGXOBJ_BitmapType_t                  BitmapType;
    ehs_bool                              PreMultipliedColor;
    STGXOBJ_ColorSpaceConversionMode_t    ColorSpaceConversion;
    STGXOBJ_AspectRatio_t                 AspectRatio;
    ehs_uint32                            Width;
    ehs_uint32                            Height;
    ehs_uint32                            Pitch;
    ehs_uint32                            Offset;
    void*                                 Data1_p;
    ehs_uint32                            Size1;
    void*                                 Data2_p;
    ehs_uint32                            Size2;
    STGXOBJ_SubByteFormat_t               SubByteFormat;
    ehs_bool	                            BigNotLittle;
    ehs_uint32                            Pitch2;
    ehs_uint32                            Offset2;
    YUV_ScalingFactor_t                   YUVScaling;
} STGXOBJ_Bitmap_t;

typedef enum STGFX_FontType_e
{
    STGFX_BITMAPPED_FONT,
    STGFX_OUTLINE_FONT,
    STGFX_TXT_FONT,
    STGFX_USE_CHAR_MAPPING = 256
} STGFX_FontType_t;

typedef enum STGFX_ShadowDirection_e
{
    STGFX_NORTH,
    STGFX_SOUTH,
    STGFX_EAST,
    STGFX_WEST,
    STGFX_NORTH_EAST,
    STGFX_SOUTH_EAST,
    STGFX_SOUTH_WEST,
    STGFX_NORTH_WEST,
    STGFX_NORTH_SOUTH,
    STGFX_EAST_WEST,
    STGFX_NORTH_SOUTH_EAST_WEST
} STGFX_ShadowDirection_t;

typedef enum STGFX_TxtFontEffect_e
{
    STGFX_NO_EFFECT,
    STGFX_FRINGING,
    STGFX_ROUNDING,
    STGFX_ROUNDING_FRINGING,
    STGFX_SHADOW
} STGFX_TxtFontEffect_t;

typedef struct STGFX_TxtFont_s
{
    ehs_uint8     NbColor;
    ehs_uint16*   IsForeground_p;
    ehs_uint16           FontWidth;
    ehs_uint16           FontHeight;
    ehs_uint16           NumOfChars;
    ehs_uint8*           GlyphData_p;
} STGFX_TxtFont_t;

typedef struct STGFX_TxtFontAttributes_s
{
    ehs_uint8*                       Color_p;
    STGFX_TxtFontEffect_t     Effect;
    ehs_uint8                        ZoomWidth;
    ehs_uint8                        ZoomHeight;
    ehs_uint8                        UnderlineHeight;
    ehs_uint8                        UnderlineColor;
    ehs_uint8                        OverlineHeight;
    ehs_uint8                        OverlineColor;
    ehs_uint8                        FringeColor;
    ehs_uint8                        RoundingColor;
    ehs_uint8                        ShadowWidth;
    ehs_uint8                        ShadowColor;
    STGFX_ShadowDirection_t   ShadowDirection;
} STGFX_TxtFontAttributes_t;

typedef struct STGFX_BitmappedGlyph_s
{
    ehs_uint8   GlyphIndex;
    ehs_uint16  BoundingBoxW;
    ehs_uint16  BoundingBoxH;
    ehs_sint16  BoundingBoxX;
    ehs_sint16  BoundingBoxY;
    ehs_uint16  Width;
    ehs_uint16  Height;
    ehs_uint8*  GlyphData_p;
} STGFX_BitmappedGlyph_t;

typedef struct STGFX_BitmappedFont_s
{
    ehs_sint16                       FontAscent;
    ehs_sint16                       FontDescent;
    ehs_uint16                       DefaultChar;
    ehs_uint16                       NumOfGlyphs;
    STGFX_BitmappedGlyph_t*   Glyph_p;
} STGFX_BitmappedFont_t;

typedef struct STGFX_OutlineGlyph_s
{
    ehs_uint32                       GlyphOffset;
    ehs_sint16                       Ascent;
    ehs_sint16                       Descent;
    ehs_sint16                       BoundingBoxX;
    ehs_uint16                       Width;
} STGFX_OutlineGlyph_t;

typedef struct STGFX_OutlineFont_s
{
    ehs_uint32                       GlyphSectionSize;
    ehs_uint8*                       GlyphSection_p;
    ehs_uint16                       OutlineResolution;
    ehs_sint16                       Ascent;
    ehs_sint16                       Descent;
    ehs_sint16                       Baseline;
    ehs_uint16                       NumCharacters;
    STGFX_OutlineGlyph_t*     Glyph_p;
} STGFX_OutlineFont_t;

typedef union STGFX_FontData_u
{
    STGFX_BitmappedFont_t*    BitmappedFontData_p;
    STGFX_OutlineFont_t*      OutlineFontData_p;
    STGFX_TxtFont_t*          TxtFontData_p;
} STGFX_FontData_t;

typedef struct STGFX_CharMapEntry_s
{
    ehs_uint16                       Code;
    ehs_uint16                       Idx;
} STGFX_CharMapEntry_t;

typedef struct STGFX_CharMapping_s
{
    STGFX_CharMapEntry_t*     Entries_p;
    ehs_uint16                       NumEntries;
} STGFX_CharMapping_t;

typedef struct STGFX_Font_s
{
    STGFX_FontType_t          FontType;
    STGFX_FontData_t          FontData;
    STGFX_CharMapping_t       CharMapping;
} STGFX_Font_t;

typedef struct STGFX_GC_s
{
#ifdef EHS_NOT_USED_CODE
    STBLIT_AluMode_t           AluMode;
    STGFX_CapStyle_t           CapStyle;
    STGFX_ClipMode_t           ClipMode;
    STGXOBJ_Rectangle_t        ClipRectangle;
    STGXOBJ_ColorKey_t         ColorKey;
    STBLIT_ColorKeyCopyMode_t  ColorKeyCopyMode;
    STGXOBJ_Color_t            DrawColor;
#endif
    ehs_bool                     EnableFilling;
#ifdef EHS_NOT_USED_CODE
    //STGXOBJ_Color_t            FillColor;
    //STGXOBJ_Bitmap_t*          DrawTexture_p;
    //STGXOBJ_Bitmap_t*          FillTexture_p;
#endif
    STGFX_Font_t*				   Font_p;
    ehs_uint16                   FontSize;
    ehs_uint8                    GlobalAlpha;
#ifdef EHS_NOT_USED_CODE
    STGFX_JoinStyle_t          JoinStyle;
#endif
    ehs_uint8*                   LineStyle_p;
    ehs_uint16                   LineWidth;
#ifdef EHS_NOT_USED_CODE
    STGFX_PolygonFillMode_t    PolygonFillMode;
#endif
    ehs_uint8                    Priority;
#ifdef EHS_NOT_USED_CODE
    STGXOBJ_Color_t            TextForegroundColor;
    STGXOBJ_Color_t            TextBackgroundColor;
    STGFX_TxtFontAttributes_t* TxtFontAttributes_p;
#endif
    ehs_bool                     UseTextBackground;
    ehs_uint16                   XAspectRatio;
    ehs_uint16                   YAspectRatio;
#ifdef EHS_NOT_USED_CODE
    STGXOBJ_Palette_t*         Palette_p;
#endif
} STGFX_GC_t;

#define STGFX_DRIVER_ID    0x80
#define STGFX_DRIVER_BASE  (STGFX_DRIVER_ID << 16)

enum
{
    STGFX_ERROR_INVALID_GC = STGFX_DRIVER_BASE,
    STGFX_ERROR_XYL_POOL_EXCEEDED,
    STGFX_ERROR_INVALID_FONT,
#ifdef EHS_NOT_USED_CODE
    STGFX_FILE_NOT_FOUND,
    STGFX_ERROR_OPENING_FONT_FILE,
#endif
    STGFX_ERROR_NOT_CHARACTER,
#ifdef EHS_NOT_USED_CODE
    STGFX_ERROR_INVALID_RECNUM,
#endif
    STGFX_ERROR_STEVT_DRV,
    STGFX_ERROR_STBLIT_DRV,
    STGFX_ERROR_STBLIT_BLIT,
    STGFX_ERROR_STGXOBJ,
    STGFX_ERROR_STAVMEM
};

enum
{
    GFX_ERROR_OPENING_FONT_FILE = STGFX_DRIVER_BASE + 0x100,
    GFX_ERROR_INVALID_RECNUM
};

#ifdef MODULE
/* Memory Management */
void*   memory_allocate(partition_t *part, size_t size);
void    memory_deallocate(partition_t *part, void* block);
void*   memory_reallocate (partition_t* Partition, void* Block, size_t Requested, size_t OldSize);
void*   memory_allocate_clear(partition_t* Partition, size_t nelem, size_t elsize);

#else   /* MODULE */

#define memory_allocate(partition, amount)                  malloc(amount)
#define memory_deallocate(partition, ptr)                   free(ptr)
#define memory_reallocate(partition, ptr, size)             realloc(ptr, size)
#define memory_allocate_clear(partition, nelem, size)       calloc(nelem, size)
#endif  /* MODULE */

/* defining debug functions */
#define debugopen(x,y)				(long int)fileno(fopen(x,y))
#define debugread(x,y,z)			read((int)x,y,z)
#define debugwrite(x,y,z)			write((int)x,y,z)
#define debugclose(x)				close((int)x)

#endif /* EHS_TYPEDEFS_H */
