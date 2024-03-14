/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_viewport.c
 * This file provides the definitions for EhsTargetViewportClass, which
 * drives the target's graphic device
 *
 * STAPI Notes
 * The STGXOBJ_Bitmap_t type has the following gotchas:
 * # If the field BigNotLittle must be set to TRUE to copy STGXOBJ_ColorARGB_t values (i.e. 32 bit
 * values directly into the Data1_p area. Otherwise alpha and blue (and red/green) values are transposed.
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section target_viewport
 * @anchor target_viewport
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on 2007-10-12
 * Last modified on $Date$
 *
 * This file contained no derogations to the MISRA standard.
 *
 * ??Note it is necessary to replace <sys/types.h> with <types.h> to lint this file successfully.
 */


/**
 * Provides access to the target-specific declarations of header files
 */
#define EHS_TARGET_CODE

/*****************************************************************************/
/* Included files */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "hal_string.h"
#include "hal_viewport.h"
#include "hal_mem.h"
#include "widget.h"
//#include "typedefs.h"
#include <stblit.h>
#include <stlayer.h>
#include <stvtg.h>
#include "messages.h"


/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/**
 * Default alpha value for the layer (i.e. used to blend against background)
 * Range is 0 (transparent) .. 128 (opaque)
 */
#define EHSL_VIEWPORT_LAYER_ALPHA 128u

/**
 * Check for any errors and report them if appropriate
 */
#define TRACE_VIEWPORT
#undef TRACE_VIEWPORT
#ifdef TRACE_VIEWPORT
static int level = 0;
static char* currentFunc;
#define INDENT(x) ((x==0)?"":((x==1)?">":((x==2)?">>":((x==3)?">>>":(">..>")))))
#define ENTER(x) printf("%sEnter %s\n",INDENT(level),#x);level++;currentFunc = #x;
#define LEAVE(x) --level;printf("%sLeave %s\n",INDENT(level),#x)
#define EHSL_REPORT_ERRORS(err,func) if ((err) != ST_NO_ERROR) { EHSH_LOG_ERROR(EHS_MSG_TGT_STAPI(#func,GetErrorText(err))); }
#else
#define EHSL_REPORT_ERRORS(err,func) if ((err) != ST_NO_ERROR) { EHSH_LOG_ERROR(EHS_MSG_TGT_STAPI(#func,GetErrorText(err))); }
#define ENTER(x)
#define LEAVE(x)
#endif
/**
 * Attempting to use full screen width/height results in two errors (which come from
 * LAYER_OpenVP):
 * - LAYER_OpenViewPort(2, 280,72,1280,720)=ST_ERROR_FEATURE_NOT_SUPPORTED
 * - LAYER_EnableViewPort(2)=ST_ERROR_INVALID_HANDLE
 * Consequently we need to set screen height/width smaller than full screen.
 */
#define EHS_TV_WIDTH 500;//720
#define EHS_TV_HEIGHT 500;//576

/**
 * Define the target viewport type. Contains target specific data required
 * by the viewport functions.
 */
struct EhsTVStruct
{
    STGXOBJ_Bitmap_t xOsdRegion;		/**< Bitmap that represents the OSD region */
    EhsTVSurfaceClass* pDblBuff;			/**< Surface that represents the double buffer */
    STBLIT_Handle_t xBlitHandle;		/**< Handle to the blitter */
    EhsGraphicsRectangleClass clipRect;	/**< Rectangle currently used to clip what is being displayed */
    EhsGraphicsRectangleClass viewRect; /**< Specifies the size of the viewport */
    EhsTVSurfaceClass* pAllocSurface; /**< List of allocated surfaces - used for deallocation purposes */
    STBLIT_BlitContext_t pBlitContext;	/**< Required by blitting operation - must be declared global */
};

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

/*****************************************************************************/
/* Declare prototypes of local functions */

void GFXUTILS_Deallocate( void * Address_p);

/**
 * Convert EHS colour format into the STAPI colour format
 *
 * @param[in] pEhsColour Pointer to the EHS colour to convertfrom
 * @param[out] pStapiColour Pointer to the STAPI colour to convert to
 */
EHS_LOCAL void EhsL_convertEhsColor(const EhsGraphicsColourClass* pEhsColour, STGXOBJ_Color_t* pStapiColour);


/*****************************************************************************/
/* Variables defined with file-scope */

/**
 * STAPI requires global memory, rather than stack memory for certain data
 * included in this is the palette.
 */
EHS_LOCAL STGXOBJ_Palette_t EhsL_palette;

/**
 * STAPI requires global memory, rather than stack memory for certain data
 * included in this is the blitDestination.
 */
EHS_LOCAL	STBLIT_Destination_t EhsL_blitDestination;

/**
 * STAPI requires global memory, rather than stack memory for certain data
 * included in this is the blitSource.
 */
EHS_LOCAL	STBLIT_Source_t EhsL_blitSource;

/**
 * STAPI requires global memory, rather than stack memory for certain data
 * included in this is the colour.
 */
EHS_LOCAL STGXOBJ_Color_t EhsL_drawColour;

/* This has been added during a blinf merge - remove if problems! */
EHS_LOCAL STBLIT_FlickerFilterMode_t s_FlickerFilterMode;

/*****************************************************************************/
/* Variables defined with global-scope */

extern STBLIT_Handle_t BLIT_Handle;	/**< Used to provide access to the blitter */
extern STLAYER_Handle_t LAYER_Handle[NUM_LAYERS];
extern STLAYER_ViewPortHandle_t LAYER_ViewPortHandle[NUM_VPORTS];
extern STVTG_Handle_t VTG_Handle[VTG_USED];



/**
 * Define the target viewport. Only one viewport is defined at this
 * time.
 * ASSUMPTIONS:
 * This variable should not be accessed directly in functions.
 * A pointer reference to it (passed in the arguement) shouild be used in stead
 * The init function could dymanicall create this to make this code module re-rentrant - i.e. for devices with more than one view port, or display.
 * If this is malloced this global variable could remove this.
 */
EhsTVClass EhsTV;



/*****************************************************************************/
/* Function definitions */

/**
 * Convert EHS colour format into the STAPI colour format
 * @param[in] pEhsColour Pointer to the EHS colour to convertfrom
 * @param[out] pStapiColour Pointer to the STAPI colour to convert to
 */
void EhsL_convertEhsColor(const EhsGraphicsColourClass* pEhsColour, STGXOBJ_Color_t* pStapiColour)
{
    pStapiColour->Type = STGXOBJ_COLOR_TYPE_ARGB8888;
    pStapiColour->Value.ARGB8888.Alpha = EHS_TV_ALPHA8_SCALE(pEhsColour->sComp.nAlpha);
    pStapiColour->Value.ARGB8888.R = pEhsColour->sComp.nRed;
    pStapiColour->Value.ARGB8888.G = pEhsColour->sComp.nGreen;
    pStapiColour->Value.ARGB8888.B = pEhsColour->sComp.nBlue;
}

/**
 * Initialise the target viewport. This function is called
 * once in the life of the EHS application at system initialization time.
 * ASSUMPTION - we only support colour type ARGB8888
 *
 * @return true if successful
 */
ehs_bool EhsTV_init(EhsTVClass* pViewport)
{
    ehs_bool bInitialised = EHS_FALSE; /* has initialisation been successful? assume not */
    STGXOBJ_BitmapAllocParams_t BitmapAllocParams1, BitmapAllocParams2; /* hold information needed to allocate bitmaps */
    STLAYER_LayerParams_t xLayerParams;	/* Contains screen parameters */
    ST_ErrorCode_t ErrCode;
    STVTG_ModeParams_t xVTGModeParams;
    ENTER(EhsTV_init);

    /* Get command line arguments */
    ErrCode = main_shared_args(0,NULL);
    EHSL_REPORT_ERRORS(ErrCode,main_shared_args);

    /* Load device drivers */
    if (ErrCode == ST_NO_ERROR)
    {
        main_shared_load();
        EHSL_REPORT_ERRORS(ErrCode,main_shared_load);
    }

    /* driver initialisation */
    if (ErrCode == ST_NO_ERROR)
    {
        ErrCode = main_shared_init();
        /* don't worry if we're already initialised */
        if (ST_ERROR_ALREADY_INITIALIZED == ErrCode)
            ErrCode = ST_NO_ERROR;
        EHSL_REPORT_ERRORS(ErrCode,main_shared_init);
    }

    /* set up display sizes */
    if (ErrCode == ST_NO_ERROR)
    {
        ErrCode = main_shared_exec("vos_setup \"720p_50\" \"pal\"");
        EHSL_REPORT_ERRORS(ErrCode,main_shared_exec__vos_setup);
    }

    /* set up main display type */
    if (ErrCode == ST_NO_ERROR)
    {
        ErrCode = main_shared_exec("set_avpath \"main\"");
        EHSL_REPORT_ERRORS(ErrCode, main_shared_exec__set_avpath_main);
    }

    /* set playback type */
    if (ErrCode == ST_NO_ERROR)
    {
        ErrCode = main_shared_exec("vid_setup \"mpeg\"");
        EHSL_REPORT_ERRORS(ErrCode, main_shared_exec__vid_setup_mpeg);
    }

    /* get up the parameters for the OSD layer */
    if (ErrCode == ST_NO_ERROR)
    {
        ErrCode=STLAYER_GetLayerParams(LAYER_Handle[LAYER_GDP1],&xLayerParams);
        EHSL_REPORT_ERRORS(ErrCode,STLAYER_GetLayerParams);
    }
#ifdef REMOVE_THIS_TO_USE_VTG_TO_DETERMINE_VIDEO_RESOLUTION
    if (ErrCode == ST_NO_ERROR)
    {
        X
        STVTG_TimingMode_t STVTG_TimingMode;
        ST_ErrorCode = STVTG_GetMode(VTG_Handle[MAIN_VTG], &STVTG_TimingMode, &xVTGModeParams);
        EHSL_REPORT_ERRORS(ErrCode,STVTG_GetMode);
    }

    if (ErrCode == ST_NO_ERROR)
    {
        xLayerParams.Width = xVTGModeParams.ActiveAreaWidth;
        xLayerParams.Height = xVTGModeParams.ActiveAreaHeight;
        /* also use DigitalActiveAreaXStart and DigitalActiveAreaYStart to establish top corner */
#else
    if (ErrCode == ST_NO_ERROR)
    {
        /* allocation of full screen viewport fails - thus we use fixed width/height */
        xLayerParams.Width = EHS_TV_WIDTH;
        xLayerParams.Height = EHS_TV_HEIGHT;
#endif
        pViewport->xBlitHandle = BLIT_Handle;
        /* Determine requirements for bitmap size */

        /* Image Info */
        pViewport->xOsdRegion.ColorType            = STGXOBJ_COLOR_TYPE_ARGB8888;
        pViewport->xOsdRegion.BitmapType           = STGXOBJ_BITMAP_TYPE_RASTER_PROGRESSIVE;
        pViewport->xOsdRegion.ColorSpaceConversion = STGXOBJ_ITU_R_BT601;
        pViewport->xOsdRegion.AspectRatio          = xLayerParams.AspectRatio;
        pViewport->xOsdRegion.Width                = xLayerParams.Width;
        pViewport->xOsdRegion.Height               = xLayerParams.Height;
        /* Unused */
        pViewport->xOsdRegion.PreMultipliedColor   = FALSE;
        pViewport->xOsdRegion.SubByteFormat        = STGXOBJ_SUBBYTE_FORMAT_RPIX_MSB;
        pViewport->xOsdRegion.BigNotLittle         = FALSE;

        /* Get Alloc Parameters */
        ErrCode = STLAYER_GetBitmapAllocParams(LAYER_Handle[LAYER_GDP1], &(pViewport->xOsdRegion), &BitmapAllocParams1, &BitmapAllocParams2);
        EHSL_REPORT_ERRORS(ErrCode,STLAYER_GetBitmapAllocParams);
    }

    /* allocate memory for the OSD region */
    if (ErrCode == ST_NO_ERROR)
    {
        /* allocate display buffer */
        pViewport->xOsdRegion.Pitch   = BitmapAllocParams1.Pitch;
        pViewport->xOsdRegion.Offset  = BitmapAllocParams1.Offset;
        pViewport->xOsdRegion.Data1_p = NULL;
        pViewport->xOsdRegion.Size1   = BitmapAllocParams1.AllocBlockParams.Size;
        pViewport->xOsdRegion.Data2_p = NULL;
        pViewport->xOsdRegion.Size2   = 0;
        /* Allocate Display buffer */
        ErrCode = GFXUTILS_Allocate((U8)LAYER_GDP1, BitmapAllocParams1.AllocBlockParams.Size,BitmapAllocParams1.AllocBlockParams.Alignment, (void **)&(pViewport->xOsdRegion.Data1_p));
        EHSL_REPORT_ERRORS(ErrCode,GFXUTILS_Allocate);
    }

    /* Map the OSD layer to the display memory */
    if (ErrCode == ST_NO_ERROR)
    {
        STGXOBJ_Rectangle_t OutputRectangle;
        /* Create the OSD Viewport */
        OutputRectangle.PositionX = 280;
        OutputRectangle.PositionY = 72;
        OutputRectangle.Height 	 = pViewport->xOsdRegion.Height;
        OutputRectangle.Width 	 = pViewport->xOsdRegion.Width;

        ErrCode= LAYER_OpenVP( LAYER_GDP1,VPORT_OSD,&(pViewport->xOsdRegion),NULL,OutputRectangle );
        EHSL_REPORT_ERRORS(ErrCode,LAYER_OpenVP);
    }

    /* Set the global alpha value for the OSD layer */
    if (ErrCode == ST_NO_ERROR)
    {
        /* Set the alpha value for this layer */
        STLAYER_GlobalAlpha_t Alpha;

        Alpha.A0 = EHSL_VIEWPORT_LAYER_ALPHA;
        Alpha.A1 = EHSL_VIEWPORT_LAYER_ALPHA; /* not actually used for ARGB888 */
        ErrCode = STLAYER_SetViewPortAlpha(LAYER_ViewPortHandle[VPORT_OSD], &Alpha);
        EHSL_REPORT_ERRORS(ErrCode,STLAYER_SetViewPortAlpha);
    }

    /* set up clipping rectangle, blit context and create a double buffer surface */
    if (ErrCode == ST_NO_ERROR)
    {
        /* Initialise clipping & view rectangle */
        pViewport->clipRect.nLeft = 0u;
        pViewport->clipRect.nTop = 0u;
        pViewport->clipRect.nWidth = (ehs_uint16)xLayerParams.Width;
        pViewport->clipRect.nHeight = (ehs_uint16)xLayerParams.Height;
        EhsMemcpy(&(pViewport->viewRect),&(pViewport->clipRect),sizeof(EhsGraphicsRectangleClass));

        pViewport->pBlitContext.ColorKeyCopyMode          = STBLIT_COLOR_KEY_MODE_NONE;
        pViewport->pBlitContext.AluMode                   = STBLIT_ALU_ALPHA_BLEND;
        pViewport->pBlitContext.EnableMaskWord            = FALSE;
        pViewport->pBlitContext.EnableMaskBitmap          = FALSE;
        pViewport->pBlitContext.EnableColorCorrection     = FALSE;
        pViewport->pBlitContext.Trigger.EnableTrigger     = FALSE;
        pViewport->pBlitContext.GlobalAlpha               = 128; // to change
        pViewport->pBlitContext.EnableClipRectangle       = TRUE;

        /* clipping still needs to be turned on to avoid problems if we try
           to raster outside the bitmap (e.g. end of Robustness test) */

        pViewport->pBlitContext.ClipRectangle.PositionX   = pViewport->viewRect.nLeft;
        pViewport->pBlitContext.ClipRectangle.PositionY   = pViewport->viewRect.nTop;
        pViewport->pBlitContext.ClipRectangle.Width       = pViewport->viewRect.nWidth;
        pViewport->pBlitContext.ClipRectangle.Height      = pViewport->viewRect.nHeight;
        pViewport->pBlitContext.WriteInsideClipRectangle  = TRUE;
        pViewport->pBlitContext.EnableFlickerFilter       = FALSE;
        pViewport->pBlitContext.JobHandle                 = STBLIT_NO_JOB_HANDLE;
        pViewport->pBlitContext.UserTag_p                 = NULL; /* set later to mark avmem to free */
        pViewport->pBlitContext.NotifyBlitCompletion      = FALSE;
        pViewport->pBlitContext.EventSubscriberID         = 0;

        /* create a double buffer surface */
        pViewport->pAllocSurface = NULL; /* set chain of allocated memory to null - this doesn't include the double buffer */
        pViewport->pDblBuff = EhsTVSurface_create(pViewport, pViewport->xOsdRegion.Width, pViewport->xOsdRegion.Height, EHS_GRAPHICS_COLOUR_ARGB8888, NULL, 0, EHS_FALSE);
        if (pViewport->pDblBuff)
        {
            bInitialised = EHS_TRUE;
            pViewport->pAllocSurface = NULL; /* set chain of allocated memory to null - this doesn't include the double buffer */
        }
    }

    if (ErrCode == ST_NO_ERROR)
    {
        s_FlickerFilterMode = STBLIT_FLICKER_FILTER_MODE_ADAPTIVE;
        ErrCode = STBLIT_SetFlickerFilterMode(pViewport->xBlitHandle, s_FlickerFilterMode);
        //EHS_TGT_VIEWPORT_REPORT_ERRORS(ErrCode,GFX_LoadFont);
    }
    LEAVE(EhsTV_init);

    return bInitialised;
}

void EhsTV_hideViewport()
{
//   	hideViewport = 1;
}

/*Change size and position of viewport*/
//@todo this is untsted
void EhsTV_move(EhsTVClass* pViewport, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nDeltaWid, EhsDataflowIntType nDeltaHt)
{
    /*
     *
     ENTER(EhsTV_move);
    if((newX != nX) || (newY != nY))
    {
    	newX = nX;
    	newY = nY;
    	windowMoved = 1;
    }

    newWidth = nDeltaWid;
    newHeight = nDeltaHt;

    LEAVE(EhsTV_move);
    */
}

void EhsTV_fade(EhsTVClass* pViewport, EhsGraphicsColourClass nColour)
{
    /*	ENTER(EhsTV_fade);
    	viewColour = nColour;
    	EhsTV_update(pViewport);
    	LEAVE(EhsTV_fade); */
}
void EhsTV_showViewport(ehs_uint16)
{
//    	showViewport = 1;
}


/**
 *  Reset the use of the target viewport. This function
 * releases resources occupied by the viewport.
 */
void EhsTV_reset(EhsTVClass* pViewport)
{

    ENTER(EhsTV_reset);

    /* destroy all of the allocated surfaces */
    while (pViewport->pAllocSurface)
    {
        EhsTVSurface_destroy(pViewport, pViewport->pAllocSurface);
    }
    EhsTV_clear(pViewport);

    LEAVE(EhsTV_reset);
}

/**
 *  Shutdown the target viewport. This function
 * releases resources occupied by the viewport.
 */
void EhsTV_term(EhsTVClass* pViewport)
{
    ST_ErrorCode_t ErrCode = ST_NO_ERROR;

    ENTER(EhsTV_term);

    //EhsTV_reset(pViewport); We do this from common code

    /* I did try to terminate video and other output stage drivers
     * using main_shared_exec("vid_term") at this point, but this
     * caused an exception.
     */

    /* terminate general video drivers */
    if (ErrCode == ST_NO_ERROR)
    {
        ErrCode = main_shared_exec("vos_term");
        EHSL_REPORT_ERRORS(ErrCode,main_shared_exec__vos_term);
    }
    LEAVE(EhsTV_term);
}

/**
 * Indicate to the target that the entire viewport needs updating.
 */
void EhsTV_update(EhsTVClass* pViewport)
{
    ENTER(EhsTV_update);
    EhsTV_updateRect(pViewport, pViewport->viewRect.nLeft, pViewport->viewRect.nTop, pViewport->viewRect.nWidth, pViewport->viewRect.nHeight);
    LEAVE(EhsTV_update);
}

/**
 * Redraw a specified part of the OSD layer.
 *
 */
EHS_GLOBAL
void EhsTV_updateRect(EhsTVClass* pViewport, ehs_sint32 nX, ehs_sint32 nY, ehs_sint32 nWidth, ehs_sint32 nHeight)

//void EhsTV_updateRect(EhsTVClass* pViewport, ehs_uint16 nX, ehs_uint16 nY, ehs_uint16 nWidth, ehs_uint16 nHeight)
{
    EhsGraphicsRectangleClass clip;	/* contains the rectangle that we're drawing into */
    STGXOBJ_Rectangle_t xStapiClip;	/* clipping rectangle according to STAPI format */
    ST_ErrorCode_t ErrCode;
    ENTER(EhsTV_updateRect);
    clip.nLeft = nX;
    clip.nTop = nY;
    clip.nWidth = nWidth;
    clip.nHeight = nHeight;
    /* Make sure we're not out of bounds */
    if (EhsGraphicsRectangle_intersect(&clip,&clip,&(pViewport->viewRect)))
    {
        /* draw the widgets falling within the rectangle directly to the OSD buffer */

        /* clear the redraw area */
        EhsL_drawColour.Value.ARGB8888.Alpha=0;
        EhsL_drawColour.Value.ARGB8888.R=0;
        EhsL_drawColour.Value.ARGB8888.G=0;
        EhsL_drawColour.Value.ARGB8888.B=0;
        xStapiClip.Height = clip.nHeight;
        xStapiClip.Width  = clip.nWidth;
        xStapiClip.PositionX = clip.nLeft;
        xStapiClip.PositionY = clip.nTop;
        pViewport->clipRect = clip;
        pViewport->pBlitContext.AluMode = STBLIT_ALU_COPY ;
        pViewport->pBlitContext.ColorKeyCopyMode = STBLIT_COLOR_KEY_MODE_NONE;
        EhsL_blitSource.Type = STBLIT_SOURCE_TYPE_COLOR;
        EhsL_blitSource.Data.Color_p = &EhsL_drawColour;
        EhsL_blitDestination.Bitmap_p = &(pViewport->pDblBuff->xBitmap);
        EhsL_blitDestination.Rectangle.Height = xStapiClip.Height;
        EhsL_blitDestination.Rectangle.Width = xStapiClip.Width;
        EhsL_blitDestination.Rectangle.PositionX = xStapiClip.PositionX;
        EhsL_blitDestination.Rectangle.PositionY = xStapiClip.PositionY;
        STBLIT_Blit(pViewport->xBlitHandle,&EhsL_blitSource,NULL,&EhsL_blitDestination,&pViewport->pBlitContext);
        //	EHSL_REPORT_ERRORS(ErrCode,STBLIT_Blit);
        // seems to generate ST_UNKNOWN_ERROR - why is that?

        pViewport->pBlitContext.AluMode = STBLIT_ALU_ALPHA_BLEND; // make sure this writes values directly
        EhsWidgetTable_draw(&EhsWidgetTable,pViewport,&clip);

        /* set up blit context */
        pViewport->pBlitContext.AluMode = STBLIT_ALU_COPY ;
        pViewport->pBlitContext.GlobalAlpha = EHSL_VIEWPORT_LAYER_ALPHA;
        /* copy the double buffer to the main screen */
        ErrCode = STBLIT_CopyRectangle(pViewport->xBlitHandle,
                                       &(pViewport->pDblBuff->xBitmap),	/* source bitmap */
                                       &xStapiClip, 						/* source rectangle */
                                       &(pViewport->xOsdRegion),			/* destination bitmap */
                                       xStapiClip.PositionX,				/* Destination position */
                                       xStapiClip.PositionY,
                                       &pViewport->pBlitContext);
        EHSL_REPORT_ERRORS(ErrCode,STBLIT_CopyRectangle);
    }
    LEAVE(EhsTV_updateRect);
}

/**
 * Clear the target viewport
 */
void EhsTV_clear(EhsTVClass* pViewport)
{
    ENTER(EhsTV_clear);
    ehs_uint32 x, y;
    ehs_uint32 *pPixels = (ehs_uint32 *)(pViewport->xOsdRegion.Data1_p);

    for (x = 0; x < pViewport->xOsdRegion.Width; x++)
    {
        for (y = 0u; y < pViewport->xOsdRegion.Height; y++)
        {
            pPixels[x+y*pViewport->xOsdRegion.Width] = 0u;
        }
    }
    LEAVE(EhsTV_clear);
}

/**
 * General blitting function used by the three specific blitting functions.
 *
 * @param[in] pViewport Viewport to blit into
 * @param[in] pImgData The content of the image represented as a 2d array of colours.
 * @param[in] pDst The bounding box for the image
 * @param[in] nAlpha The global alpha level
 * @param[in] bAlpha Does the image require alpha blending (i.e. some pixels are partially see-through)
 * @param[in] bSprite Are some image pixels completely transparent?
 *
 */
void EhsTV_blit(EhsTVClass* pViewport, const EhsTVSurfaceClass* pImgData,
                const EhsGraphicsRectangleClass* pDst, const EhsGraphicsRectangleClass* pSrc, ehs_uint8 nAlpha)
{
    int i,ii;
    char * inpix, *outpix;
    EhsGraphicsRectangleClass blitBounds;		/* This is the area that we are blitting this image into */
    ehs_uint16 nRow, nCol;						/* index into image */
    STGXOBJ_ColorARGB_t DrawColor; /* @todo replace with EhsL_drawColour ? */


    ENTER(EhsTV_blit);
    //pTextSurface->xBitmap.BigNotLittle = EHS_TRUE;
    /* draw background rectangle colour */
    DrawColor.Alpha=150; //@todo is this rubbish to get rid of ...
    DrawColor.R=100;
    DrawColor.G=200;
    DrawColor.B=70;

    /* calculate the parts of the image that we need to update */
    if (EhsGraphicsRectangle_intersect(&blitBounds,pDst,&(pViewport->clipRect)))
    {
        STGXOBJ_Rectangle_t Rectangle;
        ST_ErrorCode_t		  ErrCode;
        /* where to blit to*/
        EhsL_blitDestination.Bitmap_p = &(pViewport->pDblBuff->xBitmap);
        EhsL_blitDestination.Rectangle.PositionX = blitBounds.nLeft;
        EhsL_blitDestination.Rectangle.PositionY = blitBounds.nTop;
        EhsL_blitDestination.Rectangle.Width = blitBounds.nWidth;
        EhsL_blitDestination.Rectangle.Height = blitBounds.nHeight;
        /* where to blit from */
        /* TODO: Possible source of problems here. If blitBounds is smaller than pDst,
         * pSrc is not correctly sized - it needs to be rescaled in line with the
         * changes of blitBounds */
        EhsL_blitSource.Rectangle.PositionX = pSrc->nLeft;
        EhsL_blitSource.Rectangle.PositionY = pSrc->nTop;
        EhsL_blitSource.Rectangle.Width = pSrc->nWidth;
        EhsL_blitSource.Rectangle.Height = pSrc->nHeight;
        /* data to blit */
        EhsL_blitSource.Type=STBLIT_SOURCE_TYPE_BITMAP;
        EhsL_blitSource.Data.Bitmap_p = (STGXOBJ_Bitmap_t *)&(pImgData->xBitmap);
        EhsL_blitSource.Palette_p = (STGXOBJ_Palette_t *)&(pImgData->xPalette);

        pViewport->pBlitContext.GlobalAlpha=nAlpha>>1; /* OSD range is 128 */
        pViewport->pBlitContext.AluMode = STBLIT_ALU_ALPHA_BLEND;
        EhsL_blitSource.Data.Bitmap_p->ColorType = pImgData->xBitmap.ColorType;//STGXOBJ_COLOR_TYPE_ARGB8888;
        EhsL_blitSource.Data.Bitmap_p->ColorSpaceConversion = STGXOBJ_ITU_R_BT601; // Has this got corrupted? investigate why?
        EhsL_blitSource.Data.Bitmap_p->BitmapType = STGXOBJ_BITMAP_TYPE_RASTER_PROGRESSIVE;
        EhsL_blitDestination.Bitmap_p->ColorSpaceConversion = STGXOBJ_ITU_R_BT601;
        EhsL_blitDestination.Bitmap_p->BitmapType = STGXOBJ_BITMAP_TYPE_RASTER_PROGRESSIVE;
        //		xBlitContext.GlobalAlpha               = nAlpha /* ACC: I assume that the range of the global alpha
        //														 * follows the colour type we're using. In this case,
        //														 * we use ARGB8888, so we should have the full 8 bits
        //														 */
        ErrCode = STBLIT_Blit(pViewport->xBlitHandle,&EhsL_blitSource,NULL,&EhsL_blitDestination, &pViewport->pBlitContext);
        /* filter out complaints about blitting outside the allowable range */
        if (ErrCode == ST_ERROR_FEATURE_NOT_SUPPORTED)
            ErrCode = ST_NO_ERROR;
        EHSL_REPORT_ERRORS(ErrCode,STBLIT_Blit);
    }
    LEAVE(EhsTV_blit);//getchar();
}

/**
 * Fill a rectangle with a specified colour in the specified viewport.
 *
 * @param[in] pViewport Viewport to fill rectangle into
 * @param[in] pRect position and size of rectangle to fill
 * @param[in] pColour Colour to use for filling rectangle (includes global alpha value)
 * @todo Update this to use _A1 format surfaces
 */
void EhsTV_fillRect(EhsTVClass* pViewport,
                    const EhsGraphicsRectangleClass* pRect, const EhsGraphicsColourClass* pColour)
{
    EhsGraphicsRectangleClass blitBounds;		/* This is the area that we are blitting this image into */

    ENTER(EhsTV_fillRect);

    /* calculate the parts of the image that we need to update */
    if (EhsGraphicsRectangle_intersect(&blitBounds, pRect, &(pViewport->clipRect)))
    {

        STGXOBJ_Rectangle_t xStapiClip;	/* clipping rectangle according to STAPI format */
        ST_ErrorCode_t ErrCode;

        EhsL_convertEhsColor(pColour,&EhsL_drawColour);
        xStapiClip.Height = blitBounds.nHeight;
        xStapiClip.Width  = blitBounds.nWidth;
        xStapiClip.PositionX = blitBounds.nLeft;
        xStapiClip.PositionY = blitBounds.nTop;
        pViewport->pBlitContext.AluMode = STBLIT_ALU_ALPHA_BLEND ;
        pViewport->pBlitContext.ColorKeyCopyMode = STBLIT_COLOR_KEY_MODE_NONE;
        EhsL_blitSource.Type = STBLIT_SOURCE_TYPE_COLOR;
        EhsL_blitSource.Data.Color_p = &EhsL_drawColour;
        EhsL_blitDestination.Bitmap_p = &(pViewport->pDblBuff->xBitmap);
        EhsL_blitDestination.Rectangle.Height = xStapiClip.Height;
        EhsL_blitDestination.Rectangle.Width = xStapiClip.Width;
        EhsL_blitDestination.Rectangle.PositionX = xStapiClip.PositionX;
        EhsL_blitDestination.Rectangle.PositionY = xStapiClip.PositionY;
        STBLIT_Blit(pViewport->xBlitHandle,&EhsL_blitSource,NULL,&EhsL_blitDestination,&pViewport->pBlitContext);

    }
    LEAVE(EhsTV_fillRect);
}

/**
 * Release memory provided for images.  Assumes that this function is never called for memory
 * allocated when bTemporary = FALSE in Surface_create
 */
void EhsTVSurface_destroy(EhsTVClass* pViewport, EhsTVSurfaceClass* pSurface)
{
    ENTER(EhsTVSurface_destroy);
    if (pSurface)
    {
        EhsTVSurfaceClass* pPrev; /* points to the surface that points to this one */
        /* delete the blit area memory */
        if (pSurface->xBitmap.Data1_p)
        {
            GFXUTILS_Deallocate(pSurface->xBitmap.Data1_p);
            pSurface->xBitmap.Data1_p = NULL;
        }
        /* delete palette area memory */
        if (pSurface->xPalette.Data_p)
        {
            GFXUTILS_Deallocate(pSurface->xPalette.Data_p);
        }
        /* remove this item from the list of surfaces */
        if (pViewport->pAllocSurface == pSurface)
        {
            pViewport->pAllocSurface = pSurface->pNext;
        }
        else
        {
            for (pPrev = pViewport->pAllocSurface; pPrev && (pPrev->pNext != pSurface); pPrev = pPrev->pNext)
                ;
            if (pPrev)
                pPrev->pNext = pSurface->pNext;
        }
        /* delete the structure */
        EhsHMem_tempFree(pSurface);
    }
    LEAVE(EhsTVSurface_destroy);
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
 * @return pointer to the surface, or null if an error occured.
 */
EhsTVSurfaceClass* EhsTVSurface_create(EhsTVClass* pViewport,
                                       ehs_uint16 nWidth, ehs_uint16 nHeight, EhsGraphicsColourFormatEnum eFormat,
                                       EhsGraphicsColourClass* pPalette, ehs_uint16 nPaletteSize, ehs_bool bTemporary)
{
    EhsTVSurfaceClass* pSurface = NULL;
    ENTER(EhsTVSurface_create);

    ST_ErrorCode_t ErrCode = ST_NO_ERROR;

    if (eFormat == EHS_GRAPHICS_COLOUR_ARGB8888 || eFormat == EHS_GRAPHICS_COLOUR_A1)
    {
        if (bTemporary)
        {
            pSurface = EhsHMem_tempAlloc(sizeof(EhsTVSurfaceClass));
        }
        else
        {
            pSurface = EhsHMem_writeableAlloc(sizeof(EhsTVSurfaceClass));
        }

        if (pSurface)
        {
            STGXOBJ_ColorType_t ColorType = (eFormat == EHS_GRAPHICS_COLOUR_ARGB8888)?STGXOBJ_COLOR_TYPE_ARGB8888:STGXOBJ_COLOR_TYPE_CLUT1;
            STGXOBJ_BitmapAllocParams_t BitmapAllocParams1, BitmapAllocParams2;
            STGXOBJ_PaletteAllocParams_t PaletteAllocParams;

            /* set up palette */
            if (eFormat == EHS_GRAPHICS_COLOUR_A1)
            {
                if (nPaletteSize < 1)
                {
                    ErrCode = ST_ERROR_BAD_PARAMETER;
                    EHSH_LOG_ERROR(EHS_MSG_TGT_NO_PALETTE);
                }
                else
                {
                    pSurface->xPalette.ColorType = STGXOBJ_COLOR_TYPE_ARGB8888; /* was ColorType; */
                    pSurface->xPalette.PaletteType = STGXOBJ_PALETTE_TYPE_DEVICE_INDEPENDENT;
                    pSurface->xPalette.ColorDepth = 1; /* was 1 */
                    ErrCode = STGXOBJ_GetPaletteAllocParams(&(pSurface->xPalette), STGXOBJ_NO_HARD, &PaletteAllocParams);
                    EHSL_REPORT_ERRORS(ErrCode,STGXOBJ_GetPaletteAllocParams);
                }
                if (ErrCode == ST_NO_ERROR)
                {
                    ErrCode = GFXUTILS_Allocate((U8)LAYER_GDP1,PaletteAllocParams.AllocBlockParams.Size,PaletteAllocParams.AllocBlockParams.Alignment, (void **)&(EhsL_palette.Data_p));
                    pSurface->xPalette.Data_p = EhsL_palette.Data_p;
                    EHSL_REPORT_ERRORS(ErrCode,GFXUTILS_Allocate__palette);
                }
                if (ErrCode == ST_NO_ERROR)
                {
                    EhsL_convertEhsColor(&(pPalette[0]),&EhsL_drawColour);
                    ErrCode = STGXOBJ_SetPaletteColor(&(pSurface->xPalette),1,&EhsL_drawColour);
                    EhsL_drawColour.Value.ARGB8888.Alpha = 0;
                    EhsL_drawColour.Value.ARGB8888.R = 0;
                    EhsL_drawColour.Value.ARGB8888.G = 0;
                    EhsL_drawColour.Value.ARGB8888.B = 0;
                    ErrCode = STGXOBJ_SetPaletteColor(&(pSurface->xPalette),0,&EhsL_drawColour);

                    EHSL_REPORT_ERRORS(ErrCode,STGXOBJ_SetPaletteColor);
                }
            }
            else
            {
                pSurface->xPalette.Data_p = NULL;
            }
            if (ErrCode == ST_NO_ERROR)
            {
                /* Image Info */
                pSurface->xBitmap.ColorType            = ColorType;
                pSurface->xBitmap.BitmapType           = STGXOBJ_BITMAP_TYPE_RASTER_PROGRESSIVE;
                pSurface->xBitmap.ColorSpaceConversion = STGXOBJ_ITU_R_BT601;
                pSurface->xBitmap.AspectRatio          = STGXOBJ_ASPECT_RATIO_16TO9;//pViewport->xOsdRegion.AspectRatio;//STGXOBJ_ASPECT_RATIO_4TO3;
                pSurface->xBitmap.Width                = nWidth;
                pSurface->xBitmap.Height               = nHeight;
                /* Unused */
                pSurface->xBitmap.PreMultipliedColor   = FALSE;
                pSurface->xBitmap.SubByteFormat        = STGXOBJ_SUBBYTE_FORMAT_RPIX_LSB;
                pSurface->xBitmap.BigNotLittle         = FALSE;
                pSurface->xBitmap.YUVScaling.ScalingFactorUV = YUV_NO_RESCALE;
                pSurface->xBitmap.YUVScaling.ScalingFactorY  = YUV_NO_RESCALE;
                /* Get Alloc Parameters */
                ErrCode = STGXOBJ_GetBitmapAllocParams(&(pSurface->xBitmap), STGXOBJ_NO_HARD, &BitmapAllocParams1, &BitmapAllocParams2);
                EHSL_REPORT_ERRORS(ErrCode,STGXOBJ_GetBitmapAllocParams);
            }
            if ( ErrCode != ST_NO_ERROR )
            {
                EhsTVSurface_destroy(pViewport, pSurface);
                pSurface = NULL;
            }
            else
            {
                pSurface->xBitmap.Pitch   = (eFormat == EHS_GRAPHICS_COLOUR_ARGB8888)?(nWidth*4):((nWidth+7)/8);		/* The allocated bitmap is exactly the same width as requested */
                pSurface->xBitmap.Offset  = 0;			/* The allocated bitmap has no offset */
                pSurface->xBitmap.Data1_p = NULL;
                pSurface->xBitmap.Size1   = BitmapAllocParams1.AllocBlockParams.Size;
                pSurface->xBitmap.Data2_p = NULL;
                pSurface->xBitmap.Size2   = 0;

                /* Allocate Display buffer */

                ErrCode = GFXUTILS_Allocate((U8)LAYER_GDP1,BitmapAllocParams1.AllocBlockParams.Size,BitmapAllocParams1.AllocBlockParams.Alignment, (void **)&(pSurface->xBitmap.Data1_p));
                EHSL_REPORT_ERRORS(ErrCode,GFXUTILS_Allocate);

                if (ErrCode != ST_NO_ERROR)
                {
                    EhsTVSurface_destroy(pViewport, pSurface);
                    pSurface = NULL;
                }
                else
                {
                    /* Add surface to end of linked list - testing Pierre's hypothesis
                     * that GFXUTILS_Alloc/Dealloc needs to be done first-in, last-out
                     */
                    pSurface->pNext = NULL;
                    if (pViewport->pAllocSurface)
                    {
                        EhsTVSurfaceClass* pSearch;
                        for (pSearch = pViewport->pAllocSurface; pSearch->pNext; pSearch = pSearch->pNext)
                            ;
                        pSearch->pNext = pSurface;
                    }
                    else
                    {
                        pViewport->pAllocSurface = pSurface;
                    }
                }
            }

        }
    }
    else
    {
        EHSH_LOG_ERROR(EHS_MSG_TGT_GRAPHICS_UNSUPPORTED_MODE("unrecognised bitmap format"));
        if(pSurface)
            EhsTVSurface_destroy(pViewport, pSurface);
        pSurface = NULL;
    }
    LEAVE(EhsTVSurface_create);
    return pSurface;
}
