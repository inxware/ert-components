/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

#include "stlayer.h"
#include "stblit.h"
#include "stgxobj.h"
#include "stvtg.h"

int main_shared_init()
{
}

int main_shared_exec(const char* p)
{
}

int main_shared_load()
{

}
int main_shared_args(int argc, char**argv)
{
}

typedef enum
{
    LAYER_VIDEO1,
    LAYER_GDP1,
    LAYER_GDP2,
    LAYER_ALPHA,
    LAYER_CURSOR,
    LAYER_VIDEO2,
    NUM_LAYERS
} LAYER_Used_t;

enum
{
    VPORT_MENU,
    VPORT_STILL,
    VPORT_OSD,
    VPORT_CURSOR,
    VPORT_IMAGE,
    VPORT_ALPHA,
    NUM_VPORTS
} Layer_ViewPort_t;

typedef enum
{
    MAIN_VTG,
    AUX_VTG,
    VTG_USED
} VTG_Used_t;

STBLIT_Handle_t BLIT_Handle;	/**< Used to provide access to the blitter */
STLAYER_Handle_t LAYER_Handle[NUM_LAYERS];
STLAYER_ViewPortHandle_t LAYER_ViewPortHandle[NUM_VPORTS];
STVTG_Handle_t VTG_Handle[VTG_USED];

char* GetErrorText(ST_ErrorCode_t ST_ErrorCode) { }


ST_ErrorCode_t GFXUTILS_Allocate(U8 Layer, U32 Size, U32 Alignment, void** Address_p) { }

void GFXUTILS_Deallocate( void * Address_p) {} //@todo this needs implementing - it is also repeated in the graphics version of this file

ST_ErrorCode_t LAYER_OpenVP(U8 Layer, U8 Vport, STGXOBJ_Bitmap_t* Bitmap_p, STGXOBJ_Palette_t* Palette_p) { }

ST_ErrorCode_t STBLIT_Blit(
    STBLIT_Handle_t Handle,
    STBLIT_Source_t* Src1_p,
    STBLIT_Source_t* Src2_p,
    STBLIT_Destination_t* Dst_p,
    STBLIT_BlitContext_t* BlitContext_p
) {}

ST_ErrorCode_t STBLIT_CopyRectangle(
    STBLIT_Handle_t Handle,
    STGXOBJ_Bitmap_t* SrcBitmap_p,
    STGXOBJ_Rectangle_t* SrcRectangle_p,
    STGXOBJ_Bitmap_t* DstBitmap_p,
    S32 DstPositionX,
    S32 DstPositionY,
    STBLIT_BlitContext_t* BlitContext_p
) { }


ST_ErrorCode_t STBLIT_FillRectangle(
    STBLIT_Handle_t Handle,
    STGXOBJ_Bitmap_t* Bitmap_p,
    STGXOBJ_Rectangle_t* Rectangle_p,
    STGXOBJ_Color_t* Color_p,
    STBLIT_BlitContext_t*  BlitContext_p
) { }


ST_ErrorCode_t STGXOBJ_GetBitmapAllocParams(STGXOBJ_Bitmap_t* Bitmap_p,
        STGXOBJ_HardUse_t HardUse,
        STGXOBJ_BitmapAllocParams_t* Params1_p,
        STGXOBJ_BitmapAllocParams_t* Params2_p ) { }

ST_ErrorCode_t STGXOBJ_GetPaletteAllocParams(STGXOBJ_Palette_t* Palette_p,
        STGXOBJ_HardUse_t HardUse,
        STGXOBJ_PaletteAllocParams_t* Params1_p) { }

ST_ErrorCode_t STGXOBJ_SetPaletteColor(STGXOBJ_Palette_t* pPalette, U8 nIndx, STGXOBJ_Color_t* pEntry) {}

ST_ErrorCode_t STLAYER_GetBitmapAllocParams(
    STLAYER_Handle_t LayerHandle,
    STGXOBJ_Bitmap_t* Bitmap_p,
    STGXOBJ_BitmapAllocParams_t* Params1_p,
    STGXOBJ_BitmapAllocParams_t* Params2_p
) { }
ST_ErrorCode_t STLAYER_GetLayerParams(
    STLAYER_Handle_t Handle,
    STLAYER_LayerParams_t* Parameters_p
) { }

ST_ErrorCode_t STLAYER_SetViewPortAlpha(
    STLAYER_ViewPortHandle_t Handle,
    STLAYER_GlobalAlpha_t * Alpha_p
) { }

ST_ErrorCode_t STLAYER_DisableViewPort(STLAYER_ViewPortHandle_t VPHandle) { }
