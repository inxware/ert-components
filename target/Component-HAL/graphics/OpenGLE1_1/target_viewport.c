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
 */


/**
 * Provides access to the target-specific declarations of header files
 */
#define EHS_TARGET_CODE

/*****************************************************************************/
/* Included files */


//#include "target_viewport.h"
#include "globals.h"
#include "hal_string.h"
#include "hal_viewport.h"
#include "hal_process.h"
#include "widget.h"
#include "graphics.h"
#include "hal-api.h"
#include "messages.h"


/* Include OpenGL headers here */
#include "hal_viewport.h"
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
//#undef TRACE_VIEWPORT
#ifdef TRACE_VIEWPORT
int EhsTraceLevel = 0;
static char* currentFunc;
#define INDENT(x) ((x==0)?"":((x==1)?">":((x==2)?">>":((x==3)?">>>":(">..>")))))
#define ENTER(x) {ehs_uint32 ticks = EhsTgtTimer_tickTous(EhsTgtTimer_now());EhsTraceLevel++;currentFunc = #x;}
#define LEAVE(x) {ehs_uint32 ticks = EhsTgtTimer_tickTous(EhsTgtTimer_now());--EhsTraceLevel;}
#else
#define ENTER(x)
#define LEAVE(x)
#endif

/* Some internal utility functions */

/* find a sensible pow2 value less than 2^15
 * return -1 otherwise
 */
int nNextPow2(int in)
{
    ehs_sint32 pow2=1;
    ehs_bool found=EHS_FALSE;
    for (ehs_uint32 i=1; i<1024*32; i++)
    {
        pow2=pow2*2;
        if (pow2>in)
        {
            found =EHS_TRUE;
            break;
        }
    }
    if (found) return pow2;
    else return -1;
}

/* todo remove - some constant textures used for fills */


int engine_init_display(struct engine* engine/*, EhsTVClass* pViewport*/)
{
#define WANT32BITS
#ifdef WANT32BITS
    const EGLint attribs[] =
    {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT|EGL_PBUFFER_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
        EGL_COLOR_BUFFER_TYPE,EGL_RGB_BUFFER,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_NONE
    };
#else
    const EGLint attribs[] =
    {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
        EGL_BLUE_SIZE, 5,
        EGL_GREEN_SIZE, 6,
        EGL_RED_SIZE, 5,
        EGL_NONE
    };
#endif
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLint egl_major_version, egl_minor_version;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, &egl_major_version, &egl_minor_version);
    EHSH_LOG_INFO("EGL version: %d.%d",egl_major_version, egl_minor_version );

    if(!eglChooseConfig(display, attribs, &config, 1, &numConfigs))
    {
        LOGI("eglChooseConfig failed\n");
    }

    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    EHSH_LOG_INFO("EGL format = %d",format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);//format);

    context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);
    if (context == 0) LOGE("Error code: %x\n", eglGetError());

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    if (surface == 0) LOGE("Error code: %x\n", eglGetError());

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
    {
        EHSH_LOG_ERROR("EHS Unable to eglMakeCurrent");
        return -1;
    }
    //LOGI("EHS Set up EGL");
    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    EHSH_LOG_INFO("Setting display width as %dx%d",w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glTranslated(-x, -y, 0);
    glOrthof (0, 1, 0, 1,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    /* Some Open GL preferences */
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /* temp create a texture for solid colour rendering - @todo replace this with a solif fill gl command*/
    //generateTextures(4, textures);
    /* temporary - for fills */
    //glBindTexture(GL_TEXTURE_2D, textures[0]); // just does the red tecture !!

    /* REMOVE TEMP SPLASH */
    GLfloat rectVert[] =
    {
        0.1,0.1,
        0.2,0.1,
        0.1,0.2,
        0.2,0.1,
        0.1,0.2,
        0.2,0.2
    };	//6 pairs vertices, x,y, representing 2 triangles that make the rect

    /*
     * OpenGL commands to draw rectangle from vertices
     *
     * TODO note that colour comes from texture at the moment - can't we use a vertex shader instead
     */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_TEXTURE_2D);/* todo should be able to remove this - it's already done */
    glVertexPointer(/*3*/2, GL_FLOAT, 0, /*vertices*/rectVert);
    glColor4f(0.0,1.0,0.0,1.0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);;
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    eglSwapBuffers(engine->display, engine->surface);

    return 0;
}


/*****************************************************************************/
/* Variables defined with global-scope */


/**
 * Define the target viewport. Only one viewport is defined at this
 * time.
 * ASSUMPTIONS:
 * This variable should not be accessed directly in functions.
 * A pointer reference to it (passed in the arguement) shouild be used in stead
 * The init function could dymanicall create this to make this code module re-rentrant - i.e. for devices with more than one view port, or display.
 * If this is malloced this global variable could remove this.
 */


/**
 * Define the target viewport. Only one viewport is defined at this
 * time.
 * ASSUMPTIONS:
 * This variable should not be accessed directly in functions.
 * A pointer reference to it (passed in the arguement) shouild be used in stead
 * The init function could dymanicall create this to make this code module re-rentrant - i.e. for devices with more than one view port, or display.
 * If this is malloced this global variable could remove this.
 */

EhsTVClass EhsTV; // Global handle of viewport strcuture

static volatile char go=0;

EhsGraphicsRectangleClass globalRect; /*The area of screen that currently wants updating*/
EhsGraphicsRectangleClass intersectViewClip; /*Intersection of viewport and area that is currently being updated*/
EhsGraphicsRectangleClass viewRectangle; /*The view ports dimensions*/
EhsGraphicsColourClass viewColour;


/* @todo The following Need to be added to a viewport structure ?*/

/* global flag to identify when graphics is properly initialised */
extern int EHS_All_inited;

/* viewport paramters */
int update = 0;
int newX = 0;
int newY = 0;
int newWidth = 0;
int newHeight = 0;
int SetX = 0;
int SetY = 0;
int SetWidth =  500;
int SetHeight = 500;
int windowMoved = 0;
int repositionWindow = 0;
int windowHasFrame = 0;
int hideViewport = 0;
int showViewport = 0;
int nZOrder = 0;
ehs_bool viewport_grab=EHS_TRUE; //default to grab the focus.
//GtkWidget * GUIViewPortWidget;
ehs_bool EhsGtkTimerActive=EHS_FALSE;
ehs_bool bIsViewportDefined = EHS_FALSE;
ehs_bool bIsFirstTime = EHS_TRUE;


/*****************************************************************************/
/* Function definitions */

/* returns the stride value for a general bitmap surface */
ehs_uint32 stride_value(ehs_uint32 width, ehs_uint32 bpp)
{
    return ((((bpp*width)+7)/8) + (sizeof(ehs_uint32)-1)) & ~(sizeof(ehs_uint32)-1); /* what is this sizeof bitwise and for??? */
}


/* non user dependent parameter initialisation - this is called before any SODL is read*/
/* We keep the GTK thread going between app function calls - we may want graphics in-between..*/

ehs_bool EhsTV_init(EhsTVClass* pViewport)
{
    ehs_bool bInitialised = EHS_TRUE; /* has initialisation been successful? assume it has */
    /* All GTK initialisation is done in the gtk main (loop) thread */

    //@todo need to review this code for all targets, viewRectangle controls the area of the viewport that widgets can be painted in and is only set here - it is not updated if a viewport widget exists
    viewRectangle.nTop = 0;
    viewRectangle.nLeft = 0;
    viewRectangle.nHeight = EHS_CONFIG_DISPLAY_HEIGHT;//Use the defauly values in case the viewport block is not used.
    viewRectangle.nWidth = EHS_CONFIG_DISPLAY_WIDTH;

    viewColour.sComp.nAlpha = 255;
    viewColour.sComp.nRed = 0;
    viewColour.sComp.nGreen = 0;
    viewColour.sComp.nBlue = 0;

    /* Initialise Open GL here */
    /* setup EGL window and context */
    engine_init_display(pViewport->engine);
#ifdef EHS_ANDROID
    EhsWidgetTable_dirty(&EhsWidgetTable);
#endif
    EhsTV_update(pViewport);

    return bInitialised;
}


/* Some generic functions copied from other graphics code */

/* todo - It seems most of the following should be in the common code area

/**
 *  Reset the use of the target viewport. This function
 * releases resources occupied by the viewport.
 */
void EhsTV_reset(EhsTVClass* pViewport)
{
    ENTER(EhsTV_reset);
    EhsTPMutex_lock(EhsTPMutex_widgetTable);

#ifdef EHS_PERIPHERAL_DEVICE_SUPPORT
    EhsGUIKbClearCBs();//@todo this should be in the keypress.c Destroy function (Aswell!).
#endif

    /* todo - the following is moved to the widget.c module
     */

    while (pViewport->pAllocSurface)
    {
        EhsTVSurface_destroy(pViewport, pViewport->pAllocSurface); //@todo check this isn't leaving gtk stuff behind - should it unref surfaces?
    }
    pViewport->pAllocSurface=NULL;//Should this go to the hal?
    EhsWidgetTable_init(&EhsWidgetTable);
    EhsTV_clear(pViewport);
    EhsTPMutex_unlock(EhsTPMutex_widgetTable);
    //todo - Should we stop the timer here also?
    //EhsGtkTimerActive=EHS_TRUE;
    LEAVE(EhsTV_reset);
}

/**
 * Called after widgets are created
 * These are tasks that are delayed until after the widgets are created
 * This is specifically so that the viewport widget attributes can be applied to the window before it is shown
 */
void EhsTV_show(EhsTVClass* pViewport)
{
    // - best to do tests here as it is the last thing done before the app is shown

    // if there is a viewport defined, always reset to viewport init coords and dimensions
    if (bIsFirstTime || bIsViewportDefined)
    {
        windowMoved = 1;
        repositionWindow = 1;
    }

    if (windowHasFrame)
    {
        //gtk_window_set_decorated(GTK_WINDOW(pViewport->pMainWindow),TRUE);
    }
    else
    {
        //gtk_window_set_decorated(GTK_WINDOW(pViewport->pMainWindow),Fttml ALSE);
    }
    //todo review this - don;t like delays - check other targets.
    //EhsSleep(EHS_TIME_us(50000)); // a sleep period of about 10ms allows window to be positioned before it is shown in the timer thread
    showViewport = 1;
}

/**
 *  Shutdown the target viewport. This function
 * releases resources occupied by the viewport.
 */
void EhsTV_term(EhsTVClass* pViewport)
{
    ENTER(EhsTV_term);
    /* Kill any rendering threads here if you need to */
    LEAVE(EhsTV_term);
}

/*Change size and position of viewport*/
void EhsTV_move(EhsTVClass* pViewport, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nDeltaWid, EhsDataflowIntType nDeltaHt)
{
    ENTER(EhsTV_move);
    newX = nX;
    newY = nY;
    newWidth = nDeltaWid;
    newHeight = nDeltaHt;
    windowMoved = 1;
    repositionWindow = 1;
    LEAVE(EhsTV_move);
}


/*Change size and position of viewport*/
void EhsTV_setwindow(EhsTVClass* pViewport, ehs_bool bIsView, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nWid, EhsDataflowIntType nHt)
{
    ENTER(EhsTV_move);

    SetX = nX;
    SetY = nY;
    SetWidth = nWid;
    SetHeight = nHt;
    /* we also need to reset the offset x,y,w,h as this method is called when an app is restarted */
    //@todo might want to have an explicit call to initialise these variables when an app is restarted
    newX = 0;
    newY = 0;
    newWidth = 0;
    newHeight = 0;
    bIsViewportDefined = bIsView;

    LEAVE(EhsTV_move);
}

/*Change alpha value of viewport*/
void EhsTV_fade(EhsTVClass* pViewport, EhsGraphicsColourClass nColour)
{
    ENTER(EhsTV_fade);
    viewColour = nColour;
    EhsTV_update(pViewport);
    LEAVE(EhsTV_fade);
}


/* show a frame on the viewport */
void EhsTV_showFrame(EhsTVClass* pViewport, ehs_bool bAddFrame)
{
    if (bAddFrame)
    {
        windowHasFrame = 1;
    }
    else
    {
        windowHasFrame = 0;
    }
}

/**
 * Indicate to the target that the entire viewport needs updating.
 *
 */

void EhsTV_update(EhsTVClass* pViewport)
{
    ENTER(EhsTV_update);
    if (pViewport->engine == NULL)
    {
        return;
    }
    EhsTV_updateRect(&EhsTV, 0, 0, SetWidth+newWidth,SetHeight+newHeight);
    LEAVE(EhsTV_update);
}

/**
 * Redraw a specified part of the OSD layer.global
 *
 */
void EhsTV_updateRect(EhsTVClass* pViewport, ehs_sint32 nX, ehs_sint32 nY, ehs_sint32 nWidth, ehs_sint32 nHeight)
{
    if (pViewport->engine == NULL)
    {
        EHSH_LOG_WARNING("exit updateRect, too early");
        return;
    }

    EhsGraphicsRectangleClass xRect;
    ENTER(EhsTV_updateRect);
    xRect.nLeft = nX;
    xRect.nTop = nY;
    xRect.nWidth = nWidth;
    xRect.nHeight = nHeight;
    if(!update) // make new dirty rectangle just with this one.
    {
        globalRect = xRect;
        update = 1;
    }
    else // make dirty rectangle from previous
    {
        EhsGraphicsRectangle_union(&globalRect,&globalRect,&xRect);
    }
    pViewport->xClipRect = globalRect;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    EhsWidgetTable_draw(&EhsWidgetTable,pViewport,&globalRect);
    // todo shuld we use this instead?: EhsWidgetTable_draw(&EhsWidgetTable,pViewport,&xRect);
    eglSwapBuffers(pViewport->engine->display, pViewport->engine->surface);/* todo - does this block on vsync? if so does it need to be called from a thread? */

    LEAVE(EhsTV_updateRect);
}

/**
 * Clear the target viewport
 */
void EhsTV_clear(EhsTVClass* pViewport)
{
    ENTER(EhsTV_clear);
    EhsTV_update(pViewport);
    //@todo Any need to implement this for Android?
    LEAVE(EhsTV_clear);
}

/**
 * OpenGL This function should get texture object, bind it to OpenGL then render it.
 *
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
void EhsTV_blit(EhsTVClass* pViewport, const EhsTVSurfaceClass* pImgData, const EhsGraphicsRectangleClass* pDst, const EhsGraphicsRectangleClass* pSrc, ehs_uint8 nAlpha)
{
    EhsGraphicsRectangleClass blitBounds;		/* This is the area that we are blitting this image into */
    ehs_uint16 nRow, nCol;						/* index into image */
    ehs_float dOffsetX, dOffsetY, dScaleX, dScaleY;
    ehs_uint8* pBitmap;
    ehs_uint32 i, x, y, nPixelNum = 0, nOffset, nRowNum = 0;
    ehs_uint32 * pcharPixels;
    EhsGraphicsColourClass* pPixels;
    ehs_uint8 nPixAlpha, nVal;

    ENTER(EhsTV_blit);

    GLfloat srcWidth=(float)pImgData->nWidth/(float)pImgData->nBufferWidth;
    GLfloat srcHeight=(float)pImgData->nHeight/(float)pImgData->nBufferHeight;
    //LOGI("pImgData->nWidth = %f, pImgData->nBufferWidth= %f, src width ratio = %f",(float)pImgData->nWidth,(float)pImgData->nBufferWidth,(float)srcWidth);

    GLfloat textureFrame[] =
    {
        0, srcHeight,
        srcWidth, srcHeight,
        0, 0,
        srcWidth, srcHeight,
        0, 0,
        srcWidth, 0
    };	//todo optimise these calcs into vars . changing these coords doesn't change anything at the moment!

    /* This will always just render the texture */
    /* todo - optimisation is to cache these calculations in the Target specific Viewport & surface struct so they are not repeated each render */
    GLfloat sTop = pViewport->engine->height;
    GLfloat sLeft = pViewport->engine->width;// todo names of these variables need changing to make sense.

    //calc the coords of rect in non-normalised space ie range of x:[-sw,sw],on oney:[-sh,sh] where sw is screen width in pixels and sh is screen height in pixels
    //LOGI("BLIT:rect_IN t[%.2d],l[%.2d],b[%.2d],r[%.2d]",pDst->nTop,pDst->nLeft,pDst->nHeight,pDst->nWidth);
    GLfloat rTop = sTop - pDst->nTop;
    GLfloat rLeft = /*sLeft -*/  pDst->nLeft;
    GLfloat rBottom = rTop -  pDst->nHeight;
    GLfloat rRight = rLeft + pDst->nWidth;
    //now normalise the coords into vertex space where both dimensions have range [-1,1]
    GLfloat nrTop = rTop / sTop;
    GLfloat nrBottom = rBottom / sTop;
    GLfloat nrLeft = rLeft / sLeft;
    GLfloat nrRight = rRight / sLeft;
    /*
     * Rectangle is composed of 2 triangles so need to calc pSurface->fmt.pRGBAPixelsvertices of these two triangles
     */
    GLfloat rectVert[] =
    {
        nrLeft,nrBottom,
        nrRight,nrBottom,
        nrLeft,nrTop,
        nrRight,nrBottom,
        nrLeft,nrTop,
        nrRight,nrTop
    };	//6 pairs vertices, x,y, representing 2 triangles that make the rect

    glDisableClientState(GL_VERTEX_ARRAY);;
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    /* todo The following can be removed - we don't know or care about  color formats for Texture rendering */
    switch (pImgData->eFormat)
    {
    case EHS_GRAPHICS_COLOUR_ARGB8888:
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glColor4f(1.0,1.0,1.0,1.0);
        glEnable(GL_TEXTURE_2D);// todo can remove this?
        glBindTexture(GL_TEXTURE_2D, pImgData->textureName);
        glVertexPointer(/*3*/2, GL_FLOAT, 0, /*vertices*/rectVert);
        glTexCoordPointer(2, GL_FLOAT, 0, /*texCoord*/textureFrame);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //glBindTexture(GL_TEXTURE_2D, pImgData->textureName);
        break;
    case EHS_GRAPHICS_COLOUR_A1:
    {
        EhsGraphicsColourClass pColour=pImgData->fmt.pA1Surface.pColour[0];
        EhsGraphicsColourClass pFrontColour=pImgData->fmt.pA1Surface.pColour[1];
        /* paint a background in the BG colour */
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisable(GL_TEXTURE_2D);// todo should be able to remove this - it's already done
        //glEnable(GL_LINE_SMOOTH);
        //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f((float)pColour.sComp.nRed/255.0,(float)pColour.sComp.nGreen/255.0,(float)pColour.sComp.nBlue/255.0,(float)pColour.sComp.nAlpha/255.0);
        glVertexPointer(2, GL_FLOAT, 0, rectVert);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        /* now overlay */
        //glDisableClientState(GL_VERTEX_ARRAY);
        //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        //glEnableClientState(GL_VERTEX_ARRAY);
        glEnable(GL_TEXTURE_2D);// todo can remove this?
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glColor4f((float)pFrontColour.sComp.nRed/255.0,(float)pFrontColour.sComp.nGreen/255.0,(float)pFrontColour.sComp.nBlue/255.0,(float)pFrontColour.sComp.nAlpha/255.0);
        glBindTexture(GL_TEXTURE_2D, pImgData->textureName);
        glVertexPointer(2, GL_FLOAT, 0, rectVert);
        glTexCoordPointer(2, GL_FLOAT, 0, textureFrame);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    break;
    }
    glDisableClientState(GL_VERTEX_ARRAY);;
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    LEAVE(EhsTV_blit);

}

/**
 * Fill a rectangle with a specified colour in the specified viewport.
 *
 * @param[in] pViewport Viewport to fill rectangle into
 * @param[in] pRect position and size of rectangle to fill
 * @param[in] pColour Colour to use for filling rectangle (includes global alpha value)
 * @todo Update this to use _A1 format surfaces
 */

void EhsTV_fillRect(EhsTVClass* pViewport, const EhsGraphicsRectangleClass* pRect, const EhsGraphicsColourClass* pColour)
{
    EhsGraphicsRectangleClass blitBounds;		/* This is the area that we are blitting this image into */
    ehs_uint32 nColour;
    ENTER(EhsTV_fillRect);
    //LOGI("Enter fillRect");
    if (pViewport == NULL)
    {
        return;
    }
    if (pViewport->engine == NULL)
    {
        return;
    }
    if (pViewport->engine->display == NULL)
    {
        return;
    }

    GLfloat sTop = pViewport->engine->height;
    GLfloat sLeft = pViewport->engine->width;

    GLfloat rTop = sTop - pRect->nTop;
    GLfloat rLeft = /*sLeft -*/  pRect->nLeft;
    GLfloat rBottom = rTop -  pRect->nHeight;
    GLfloat rRight = rLeft + pRect->nWidth;
    //now normalise the coords into vertex space where both dimensions have range [-1,1]
    GLfloat nrTop = rTop / sTop;
    GLfloat nrBottom = rBottom / sTop;
    GLfloat nrLeft = rLeft / sLeft;
    GLfloat nrRight = rRight / sLeft;

    /*
     * Rectangle is composed of 2 triangles so need to calc vertices of these two triangles
     */
    GLfloat rectVert[] =
    {
        nrLeft,nrBottom,
        nrRight,nrBottom,
        nrLeft,nrTop,
        nrRight,nrBottom,
        nrLeft,nrTop,
        nrRight,nrTop
    };	//6 pairs vertices, x,y, representing 2 triangles that make the rect

    /*
     * OpenGL commands to draw rectangle from vertices
     *
     * TODO note that colour comes from texture at the moment - can't we use a vertex shader instead
     */
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_TEXTURE_2D);/* todo should be able to remove this - it's already done */
    glVertexPointer(/*3*/2, GL_FLOAT, 0, /*vertices*/rectVert);
    glColor4f(pColour->sComp.nRed/255.0,pColour->sComp.nGreen/255.0,pColour->sComp.nBlue/255.0,pColour->sComp.nAlpha/255.0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableClientState(GL_VERTEX_ARRAY);;
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    /* calculate the parts of the image that we need to update */
    if (EhsGraphicsRectangle_intersect(&blitBounds, pRect, &intersectViewClip))
    {
        //dk todo delete this code - OpenGL so we're not bothered about finding intersection

        /* OpenGL rendering code for a filled rectangle here */
        //LOGI("EHS:render rectangle");
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
    if(pSurface){
    	if (pSurface->textureName>0) glDeleteTextures(	1, &pSurface->textureName);
    }
    
    //todo might be worth checking if the pixel data is still here and destrying if so
    /* This probably won't do anything as pSurface's should be persistent unless interrupted */
    if (pSurface)
    {
        EhsTVSurfaceClass* pPrev; /* points to the surface that points to this one */
        switch (pSurface->eFormat)
        {
        case EHS_GRAPHICS_COLOUR_ARGB8888:
            /* un-bind and free OpenGL texture binding code here */
            //glBindTexture(GL_TEXTURE_2D, );

            break;

        case EHS_GRAPHICS_COLOUR_A1:
            /* un-bind and free OpenGL texture binding code here */
            break;
        }
        /* remove this item from the list of surfaces */
        if (pViewport->pAllocSurface == pSurface)
        {
            pViewport->pAllocSurface = pSurface->pNext;
        }
        else
        {
            for (pPrev = pViewport->pAllocSurface; pPrev && (pPrev->pNext != pSurface); pPrev = pPrev->pNext)
            {
                ;
            }
            if (pPrev)
            {
                pPrev->pNext = pSurface->pNext;
            }
        }
        /* delete the structure */
        EhsHMem_tempFree(pSurface);
    }
    LEAVE(EhsTVSurface_destroy);
}





/**
 * Allocate OpenGL Textures for images (to subsequently render to screen) - this must be maintained for the off-screen intermediate images
 *
 * @param[in] pViewport Viewport where surface will be used
 * @param[in] nWidth Width of the viewport
 * @param[in] nHeight Height of the viewport
 * @param[in] eFormat format that is to be used for the bitmap
 * @param[in] pPalette array of ARGB8888 colours to use for the palette (NULL if not required)
 * @param[in] nPaletteSize number of entries in the palette
 * @return pointer to the surface, or null if an error occured.
 */
EhsTVSurfaceClass* EhsTVSurface_create(EhsTVClass* pViewport,ehs_uint16 nWidth, ehs_uint16 nHeight, EhsGraphicsColourFormatEnum eFormat,EhsGraphicsColourClass* pPalette, ehs_uint16 nPaletteSize)
{
    ehs_uint32 i;
    EhsTVSurfaceClass* pSurface = NULL;
    ehs_sint32 pow2width, pow2height;


    ENTER(EhsTVSurface_create);
    if (eFormat == EHS_GRAPHICS_COLOUR_ARGB8888 || eFormat == EHS_GRAPHICS_COLOUR_A1)
    {
        pSurface = EhsHMem_tempAlloc(sizeof(EhsTVSurfaceClass));
        pSurface->textureName=0;//need to make this null so destroy doesn'y try to destroy iy
        if (pSurface)
        {
            pSurface->eFormat = eFormat;// or TARGET_SPECIFIC?;// eFormat;  need to know inout format so finalisation code can convert to something OpenGL likes.
            switch (eFormat)
            {
            case EHS_GRAPHICS_COLOUR_ARGB8888:
                /* OpenGL friendly bitmap to paint into here */
                pow2width=nNextPow2(nWidth);
                pow2height=nNextPow2(nHeight);
                if (pow2height == -1 || pow2width == -1)
                {
                    EhsError("Image size too large, ignoring image");
                    pSurface->fmt.pRGBAPixels = NULL;
                }
                else
                {
                    pSurface->fmt.pRGBAPixels = EhsHMem_tempAlloc(pow2width * pow2height * EHS_GRAPHICS_COLOUR_ARGB8888_BYTES );
                    if (pSurface->fmt.pRGBAPixels == NULL )
                    {
                        EhsError("Could not allocate space for image data - ignoring image");
                    }
                    pSurface->nWidth = nWidth;			/**< bitmap width */
                    pSurface->nHeight =nHeight;			/**< bitmap height *//**< Used for A1 images */
                    pSurface->nBufferWidth = pow2width;			/**< This will be next pow2 values of image width */
                    pSurface->nBufferHeight = pow2height;
                }
                //This will be bound later with :
                break;
            case EHS_GRAPHICS_COLOUR_A1:
            {
                ehs_uint32 stridevalue= (nWidth+7)/8;
                pSurface->fmt.pA1Surface.pBitmap = EhsHMem_tempAlloc(stridevalue * nHeight) ; /* make a temporary 1 bit depth/0 stride bitmap OGL can't cope with */
                EhsMemset(pSurface->fmt.pA1Surface.pBitmap,0x0,stridevalue * nHeight);
                if (pSurface->fmt.pA1Surface.pBitmap == NULL )
                {
                    EhsError("Could not allocate space for image data - ignoring image");
                }
                pSurface->nWidth = nWidth;			/**< bitmap width */
                pSurface->nHeight =nHeight;			/**< bitmap height *//**< Used for A1 images */
                //LOGI("pSurface->nBufferWidth=%d",pSurface->nWidth);

                /* note!!! - these buffer sizes are for the A1 image, but OpenGL will blit from the subsequent ARGB8888 image created on finalisation, so (presumabley) the buffersizes should be updated to the ARGB888 at that point*/
                pSurface->nBufferWidth = stridevalue;//nWidth;//stride_value(nWidth,1)*8;// we don't know this until finalised -- pow2width;			/**< This will be next pow2 values of image width */
                pSurface->nBufferHeight = nHeight;// we don't know this until finalised -- pow2height;
                /* also the font renderer doesn't seem to be able to handle non-zero stride offsets - so temp data is immediately transformed to strided on finalisation */
                /* todo it would be good if the font renderer could blit to 8bit pixels maps directly rather only via 1 bit depth */
                /* now set the colour pallette */
                pSurface->fmt.pA1Surface.pColour[0]=pPalette[0];
                pSurface->fmt.pA1Surface.pColour[1]=pPalette[1];
            }
            break;
            }
        }
        /* Create only one OpenGL name for this, so new finalised iamges overwright old ones */
        glGenTextures(1, &pSurface->textureName);

    } 
    /* todo the following code is generic (not target specific) and should go in a common function that calls this in-turn. */
    if (pSurface)
    {
        /* Add surface to end of linked list */
        pSurface->pNext = NULL;
        if (pViewport->pAllocSurface)
        {
            EhsTVSurfaceClass* pSearch;
            for (pSearch = pViewport->pAllocSurface; pSearch->pNext; pSearch = pSearch->pNext)
            {
                ;
            }
            pSearch->pNext = pSurface;
        }
        else /* first one */
        {
            pViewport->pAllocSurface = pSurface;
        }
    }
    LEAVE(EhsTVSurface_create);
    return pSurface;
}

/**
 * Provide access to pixels representing the surface, create them if they don't exist @todo check crating is wise
 */
void* EhsTVSurface_pixels(EhsTVSurfaceClass* pSurface)
{
    void* pRet = NULL;

    switch (pSurface->eFormat)
    {
    case EHS_GRAPHICS_COLOUR_ARGB8888:
        /* probably don't need this in OpenGL */
        if (pSurface->fmt.pRGBAPixels == NULL)
        {
            EHSH_LOG_ERROR("Surface not ceated in EhsTVSurface_pixels");
            //pSurface->fmt.pRGBAPixels = EhsHMem_tempAlloc(pSurface->nBufferWidth * pSurface->nBufferHeight * EHS_GRAPHICS_COLOUR_ARGB8888_BYTES );
        }
        pRet= pSurface->fmt.pRGBAPixels;
        break;
    case EHS_GRAPHICS_COLOUR_A1:
        /* probably don't need this in OpenGL */
        if (pSurface->fmt.pA1Surface.pBitmap == NULL)
        {
            EHSH_LOG_ERROR("Surface not ceated in EhsTVSurface_pixels");
            //pSurface->fmt.pA1Surface.pBitmap = EhsHMem_tempAlloc(pSurface->nBufferWidth * pSurface->nBufferHeight * EHS_GRAPHICS_COLOUR_A1_BYTES );
        }
        pRet=pSurface->fmt.pA1Surface.pBitmap;
        break;
    default:
        EHSH_LOG_ERROR("Couldn't find surface type");
    }
    return pRet;
}


void EhsTVSurface_finalisePixels (EhsTVSurfaceClass* pSurface)
{
    ehs_bool bFailed = EHS_FALSE;
    //LOGI("Entering Finalise");
    if (pSurface->fmt.pRGBAPixels)
    {
        if (pSurface->eFormat == EHS_GRAPHICS_COLOUR_ARGB8888 || pSurface->eFormat == EHS_GRAPHICS_COLOUR_A1)
        {
            //LOGI("Finalising the Blit surface 4");
            switch (pSurface->eFormat)
            {
            case EHS_GRAPHICS_COLOUR_ARGB8888:
                /*try binding in now - todo check if this does imrpove performance with some form of caching */
                glEnable(GL_TEXTURE_2D);// todo can remove this?
                glBindTexture(GL_TEXTURE_2D, pSurface->textureName);
                /* some default parameters */
                glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexImage2D(	GL_TEXTURE_2D,
                                0,
                                GL_RGBA,
                                pSurface->nBufferWidth,
                                pSurface->nBufferHeight,
                                0,
                                GL_RGBA,
                                GL_UNSIGNED_BYTE,
                                pSurface->fmt.pRGBAPixels);
                if (pSurface->fmt.pRGBAPixels) EhsHMem_tempFree(pSurface->fmt.pRGBAPixels);
                break;
            case EHS_GRAPHICS_COLOUR_A1:
            {
                ehs_sint32 pow2width, pow2height;

                /* this will process A1 pixels into OpenGL Texture type */

                /* this will return an A1 bitmap - the finalisation code will need to convert this to an OPENGL format */
                pow2width=nNextPow2(pSurface->nWidth);
                pow2height=nNextPow2(pSurface->nHeight);
                if (pow2height == -1 || pow2width == -1)
                {
                    EhsError("Image size too large, ignoring image");
                    pSurface->fmt.pA1Surface.pBitmap = NULL;
                }
                else
                {
                    /* Make another very temporary bitmap for OpenGL texture */
                    ehs_uint32 i, x, y, nPixelNum = 0, nOffset = 0, nRowNum = 0; /* todo remove un needed */
                    ehs_uint8 nBit;
                    ehs_uint32 nTargetStrideOffset=pow2width-pSurface->nWidth;
                    ehs_uint8 *OGLAlphaPixels = NULL;


                    if (pSurface->nWidth%8 != 0) /* nWidth here is the raw width with no stride - so this is OK for us */
                    {
                        nOffset = 8 - (pSurface->nWidth%8);
                    }
                    else
                    {
                        nOffset = 0;
                    }

                    //nOffset=stride_value(pSurface->nWidth,1)*8;// stride value of input 1 bit bitmap
                    OGLAlphaPixels = EhsHMem_tempAlloc((pow2width)* pow2height );
                    if (!OGLAlphaPixels)
                    {
                        EHSH_LOG_ERROR("Could not create texture pixel memory");
                        return; /* todo handle better */
                    }
                    EhsMemset(OGLAlphaPixels,0,(pow2width) * pow2height);
                    /* Note hopw this is the right thing to do: (update to the ARGB888 buffer size */
                    pSurface->nBufferWidth = pow2width;
                    pSurface->nBufferHeight = pow2height;
                    //LOGI("OFFset = %d - nTargetStrideOffset=%d  pSurface->nWidth*pSurface->nHeight=%d,%d",nOffset,nTargetStrideOffset, pSurface->nWidth,pSurface->nHeight);

                    for (i = 0; i < pSurface->nWidth*pSurface->nHeight; i++)
                    {
                        nBit = (ehs_uint8)(  (ehs_uint8)((ehs_uint8)1u) << (7u - (i % 8u))); //Create a mask for the appropriate bit

                        //nRowNum = i/pSurface->nWidth;
                        //if (nRowNum*nTargetStrideOffset > i) nPixelNum=0; /*todo this is abit of a guess to avoid a crash */
                        //else nPixelNum = i - (nRowNum*(nTargetStrideOffset));
                        if (((ehs_uint8)(pSurface->fmt.pA1Surface.pBitmap[i >> 3]) & nBit) == nBit)
                        {
                            //if (nPixelNum%4 == 0) {
                            OGLAlphaPixels[nPixelNum] = 0xFF;
                            //LOGI("[%d]%x bit going to 0xFF in %d",nBit,i,nPixelNum);
                        }
                        else
                        {
                            OGLAlphaPixels[nPixelNum] = 0x00;
                            //LOGI("[%d]%x bit going to 0x00 in %d",nBit,i,nPixelNum);
                        }
                        //OGLAlphaPixels[nPixelNum]=0x80;
                        if (i>0 && i % (pSurface->nWidth+nOffset) == 0)   //detect when we are the end of the input line
                        {
                            //LOGI("New row @ i = %d, nPixelNum = %d+%d",i,nPixelNum,nTargetStrideOffset-1 );
                            nPixelNum+=nTargetStrideOffset+1-nOffset; /* -2 ?? */
                        }
                        else nPixelNum++;

                    }

                    /* transfer the texture to openGL */

                    /* Create a luminance texture that will be overlayed over a fills */
                    glEnable(GL_TEXTURE_2D);// todo can remove this?
                    glBindTexture(GL_TEXTURE_2D, pSurface->textureName);
                    /* some default parameters */
                    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexImage2D(	GL_TEXTURE_2D,0,GL_ALPHA,pow2width,pow2height,0,GL_ALPHA,GL_UNSIGNED_BYTE,OGLAlphaPixels);
                    if (pSurface->fmt.pA1Surface.pBitmap) EhsHMem_tempFree(pSurface->fmt.pA1Surface.pBitmap);
                    if (OGLAlphaPixels) EhsHMem_tempFree(OGLAlphaPixels);
                    //pSurface->nBufferWidth=0;
                    //pSurface->nBufferHeight=0;

                }
            }
            break;
            }
        }
        //LOGI("leavingd Finalise");
        //EHS_LOG_WARNING("Creating a surface in EhsTVSurface_pixels");
    }
}

ehs_uint16 EhsTVSurface_height(EhsTVSurfaceClass* pSurface)
{
    ehs_uint16 nRet = 0u;
    nRet = pSurface->nHeight;
    return nRet;
}

/**
 * Return the number of pixles (not bytes!!) across (allows direct addressing in the array of pixels -
 * index = x + y * pitch
 *
 */
ehs_uint16 EhsTVSurface_pitch(EhsTVSurfaceClass* pSurface)
{
    ehs_uint16 nRet = 0u;

    nRet = pSurface->nBufferWidth;

    return nRet;
}

/**
 * Return the width of the bitmap
 */
ehs_uint16 EhsTVSurface_width(EhsTVSurfaceClass* pSurface)
{
    ehs_uint16 nRet = 0u;
    nRet = pSurface->nWidth;
    return nRet;
}


void EhsTV_hideViewport()
{
    hideViewport = 1;
}

/**
 * show viewport and set zorder
 * viewport bottommost: zorder=0
 * viewport normal zorder: zorder=1
 * viewport topmost: zorder=2
 */
void EhsTV_showViewport(ehs_uint16 zorder)
{
    showViewport = 1;
    EhsTV_setZOrder(zorder);
}

/**
 * set zorder
 * viewport bottommost: zorder=0
 * viewport normal zorder: zorder=1
 * viewport topmost: zorder=2
 */
void EhsTV_setZOrder(ehs_uint16 zorder)
{
    nZOrder=zorder;
}

/**
 * Callbacks function for handling mouse button press events from the mouse.
 */
void EhsT_android_event_button_press(ehs_uint32 x, ehs_uint32 y)
{

    EhsTPMutex_lock(EhsTPMutex_widgetTable);
    EhsWidgetTable_triggerViewportMouseDown(&EhsWidgetTable,x,y);
    EhsWidgetTable_registerMouseDownOnWidgetMatchCoords(&EhsWidgetTable,x,y);
    EhsTPMutex_unlock(EhsTPMutex_widgetTable);

}


/* todo these should really be moved to an input module - not the viewport module - this would require a fair amount of general refactoring */
/**
 * Callback function for handling mouse button release events from the mouse.
 */
void EhsT_android_event_button_release(ehs_uint32 x, ehs_uint32 y)
{


    EhsTPMutex_lock(EhsTPMutex_widgetTable);
    EhsWidgetTable_triggerViewportMouseUp(&EhsWidgetTable,x,y);
    EhsWidgetTable_registerMouseUpOnWidgetMatchCoords(&EhsWidgetTable,x,y);
    EhsTPMutex_unlock(EhsTPMutex_widgetTable);

}

/**
 * Callback function for handling mouse button hold down and move events from the mouse.
 */
void EhsT_android_event_motion_notify( ehs_uint32 x, ehs_uint32 y)
{

    EhsTPMutex_lock(EhsTPMutex_widgetTable);
    EhsWidgetTable_triggerViewportMouseDrag(&EhsWidgetTable,x,y);
    EhsTPMutex_unlock(EhsTPMutex_widgetTable);

}

extern int EHS_ANDROID_width;
extern int EHS_ANDROID_height;

ehs_bool EhsTV_getScreenSize(ehs_sint32 *nScreenWidth, ehs_sint32 *nScreenHeight)
{

    *nScreenWidth = EHS_ANDROID_width; // in pixels
    *nScreenHeight = EHS_ANDROID_height;
}


