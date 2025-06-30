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
 * ??Note it is necessary to replace <sys/types.h> with <types.h> to lint this file successfully.
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
//#include "typedefs.h"
#include "hal-api.h"

#include "messages.h"
//#include "keypress.h" /* we take some direct keyboard functions in here */


/* INlcude OpenGL headers here */
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android_native_app_glue.h>
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
//#define TRACE_VIEWPORT
#undef TRACE_VIEWPORT
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


struct engine
{
    struct android_app* app;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
};


struct EhsTVStruct
{

    /* Probable need some OpenGLhandle to put in this struct */

    EhsGraphicsRectangleClass xClipRect;	/**< Clipping rectangle - used when drawing all images */
    EhsTVSurfaceClass* pAllocSurface; /**< List of allocated surfaces - used for deallocation purposes */
    //ehs_bool bViewportChanged;			/**< The pixbuf has changed, we need to copy it into pPixmap */
    ehs_uint8 transparency; // transparency of window
    struct engine* engine;
};

#ifdf EHS_OPENGLE_TEST_CODE

GLfloat vertices[] = {0,0,0, 0,1,0, 1,0,0, 0,1,0, 1,1,0, 1,0,0,
                      -1,0,0, 0,0,0, -1,-1,0, -1,-1,0, 0,-1,0, 0,0,0,
                      0,0,0, -1,0,0, -1,1,0, 0,0,0, -1,1,0, 0,1,0,
                      0,0,0, 1,0,0, 1,-1,0, 0,0,0, 0,-1,0, 1,-1,0
                      };

GLfloat colours[] = {1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0,
                     1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0,
                     1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0,
                     1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0
                    };

GLfloat texCoord[] = {0,0, 0,1, 1,1, 1,0};

unsigned char redChar[4];
unsigned char greenChar[4];
unsigned char blueChar[4];

int textures[4];



void generateTextures(int n, int *tex)
{

    redChar[0]=255;
    greenChar[0]=0;
    blueChar[0]=0;
    redChar[1]=0;
    greenChar[1]=255;
    blueChar[1]=0;
    redChar[2]=0;
    greenChar[2]=0;
    blueChar[2]=255;
    redChar[3]=255;
    greenChar[3]=255;
    blueChar[3]=0;

    char bitmap[64][64][4];
    char *array = &bitmap[0][0][0];

    ////generate IDs
    glGenTextures(n, tex);

    int i = 0;
    for(i=0; i<n; i++)
    {

        ///load bitmap into memory
        int j=0;
        int k=0;
        for(j=0; j<64; j++)
        {
            for(k=0; k<64; k++)
            {
                bitmap[j][k][0] = redChar[i];
                bitmap[j][k][1] = greenChar[i];
                bitmap[j][k][2] = blueChar[i];
                bitmap[j][k][3] = 255;
            }
        }

        ///bind bitmap
        glBindTexture(GL_TEXTURE_2D, tex[i]);

        ///set up texture parameters
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        ///create textures
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, array);

    }
}



static int engine_init_display(struct engine* engine)
{

    /*
    	const EGLint attribs[] = {
               EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
               EGL_BLUE_SIZE, 8,
               EGL_GREEN_SIZE, 8,
               EGL_RED_SIZE, 8,
               EGL_NONE
        };
    */
    EGLint w = 0;
    EGLint h = 0;
    EGLint dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface = 0;
    EGLContext context = 0;

    /*  EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
      eglInitialize(display, 0, 0);
      eglChooseConfig(display, attribs, &config, 1, &numConfigs);
      eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
      ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);
      surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
      context = eglCreateContext(display, config, NULL, NULL);
    */
    /*  if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
          LOGW("Unable to eglMakeCurrent");
          return -1;
      }*/

    /*  eglQuerySurface(display, surface, EGL_WIDTH, &w);
      eglQuerySurface(display, surface, EGL_HEIGHT, &h);
    */
    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    //generateTextures(4, textures);

    return 0;
}

/* Remove this
void render(int n, int *tex, GLfloat *texCoordinates, GLfloat *vertexCoordinates){

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    int i=0;

    for(i=0; i<n; i++){
    	//glBindTexture(GL_TEXTURE_2D, tex[i]);
    	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
    	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    	glVertexPointer(3, GL_FLOAT, 0, vertexCoordinates);
	glColorPointer(4, GL_FLOAT, 0, colours);
    	//glTexCoordPointer(2, GL_FLOAT, 0, texCoordinates);
    	glDrawArrays(GL_TRIANGLES, i*6, 6);
    	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
    	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}

static void engine_draw_frame(struct engine* engine) {
    if (engine->display == NULL) {
        return;
    }

    render(4, textures, texCoord, vertices);
	eglSwapBuffers(engine->display, engine->surface);
}
*/
#endif // end of OPENGLE_TEST_CODE

static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
   
    #error This is not normally called - see android_ALL/target_main.c for the one you should be using
    struct engine* engine = (struct engine*)app->userData;
    switch (cmd)
    {
    case APP_CMD_INIT_WINDOW:
        // The window is being shown, get it ready.
        if (engine->app->window != NULL)
        {
            engine_init_display(engine);
            engine_draw_frame(engine);
        }
        break;
    }
}



/*****************************************************************************/
/* Declare prototypes of local functions */


/* When the window is given the "delete_event" signal (this is given
 * by the window manager, usually by the "close" option, or on the
 * titlebar), we ask it to call the delete_event () function
 * as defined above. The data passed to the callback
 * function is NULL and is ignored in the callback function. */
//EHS_LOCAL gboolean EhsL_event_deleteWindow(GtkWidget* pWidget, GdkEventExpose* pEvent);

/* Here we connect the "destroy" event to a signal handler.
 * This event occurs when we call gtk_widget_destroy() on the window,
 * or if we return FALSE in the "delete_event" callback. */
//EHS_LOCAL gboolean EhsL_event_destroyWindow(GtkWidget* pWidget, GdkEventExpose* pEvent);

/**
 * Window has been assigned a new size and/or position
 * @param[in] pWidget The Widget that this event applies to
 * @param[in] pAllocation position and size to be allocated to widget
 */
//EHS_LOCAL gboolean EhsL_size_allocate(GtkWidget* pWidget, GtkAllocation* pAllocation);

/**
 * Update the main drawing area. This function is called as an event
 * @param[in] pWidget The Widget that this event applies to
 * @param[in] pEvent The event that triggered this function
 */
//EHS_LOCAL gboolean EhsL_event_expose(GtkWidget* pWidget, GdkEventExpose* pEvent);

/**
 * Create a pixmap for the main drawing area. This function is called
 * as an event.
 * @param[in] pWidget Widget that this event applies to
 * @param[in] pEvent Event that triggered this function.
 */
//EHS_LOCAL gboolean EhsL_event_configure(GtkWidget* pWidget, GdkEventConfigure* pEvent);

/**
 * Handle key presses
 * @param[in] pWidget The Widget that this event applies to
 * @param[in] pEvent The event that triggered this function
 */

/* These may not be appropriate for OpenGL - does EGL have any key handling?
EHS_LOCAL gboolean EhsL_event_key_release(GtkWidget* pWidget, GdkEventKey* pEvent);
EHS_LOCAL gboolean EhsL_event_key_press(GtkWidget* pWidget, GdkEventKey* pEvent);


EHS_LOCAL gboolean EhsL_event_button_press(GtkWidget* pWidget, GdkEventButton* pButton);
EHS_LOCAL gboolean EhsL_event_button_release(GtkWidget* pWidget, GdkEventButton* pButton);
EHS_LOCAL gboolean EhsL_event_motion_notify(GtkWidget* pWidget, GdkEventMotion* pMotion);
*/

/* Handle window Resizing */
//EHS_LOCAL void EhsTV_MaximiseViewport(GtkWidget* pWidget) ;
//EHS_LOCAL void EhsTV_MinimiseViewport (GtkWidget* pWidget) ;

//@todo we need the above functions to work without GTK handles for the general case where the app may change the window focus.
// Best way would be to have some flags that the window expose reads and applies at this point.
/*****************************************************************************/
/* Variables defined with file-scope */


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

/**
 * Initialise the target viewport. This function is called
 * once in the life of the EHS application at system initialization time.
 *
 * @return true if successful
 */
EhsThreadFuncReturnType EhsL_graphicsThreadFunc(EhsTVClass* pViewport); /* prototype for threaded part */


/* returns the stride value for a GTK window */
/*
 *
 ehs_uint32 stride_value(ehs_uint32 width, ehs_uint32 bpp)
{
	return ((((bpp*width)+7)/8) + (sizeof(ehs_uint32)-1)) & ~(sizeof(ehs_uint32)-1);
}
*/


/* non use paramter initialisation - this is called before any SODL is read*/
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
    /* You might need to start a renderign thread here: e.g. EhsL_graphicsThreadFunc() */


    //engine_init_display(pViewport->engine);
    render(2, textures, texCoord, vertices);

    return bInitialised;
}

/**
 *
 * Run the graphics handling in a thread.
 * Probably not needed for OpenGL - there is no event handling with OpenGL.
 */
EhsThreadFuncReturnType EhsL_graphicsThreadFunc(EhsTVClass* pViewport)
{

    EhsGraphicsRectangleClass rect;

    EHSH_LOG_INFO(" Starting GTK");
    go=1;
    //while (1)
    //{
    //   //This may include code fornadling keyboard and mouse events using some other system than OpenGL
    //}  //this seems less crashy than the gtk_main() blocker!
    EhsHThread_exit();
}

/* global flag to identify when graphics is properly initialised */
extern int EHS_All_inited;



/** Something like this might be useful for OpenGL. if a call-back is used to render the scene.
 * Update the main drawing area. This function is called as an event
 * @param[in] pWidget The Widget that this event applies to
 * @param[in] pEvent The event that triggered this function
 */
/*gboolean EhsL_event_expose(GtkWidget* pWidget, GdkEventExpose* pEvent)
{
	ENTER(EhsL_event_expose);

 rendering code here perhaps.

	LEAVE(EhsL_event_expose);
	return TRUE;
}
*/



/* Some generic functions copied from other graphics code */

/* It seems most of the following should be in the common code area

/**
 *  Reset the use of the target viewport. This function
 * releases resources occupied by the viewport.
 */
void EhsTV_reset(EhsTVClass* pViewport)
{
    ENTER(EhsTV_reset);
    EhsTPMutex_lock(EhsTPMutex_widgetTable);
    //EhsGtkTimerActive=EHS_FALSE;
    EhsGUIKbClearCBs();//@todo this should be in the keypress.c Destroy function (Aswell!).
    //windowMoved = 1; // this flag is set by logic in setwindow, don't duplicate it here

    /* the following is moved to the widget.c module
     */
    //We should stop the timer here also
    while (pViewport->pAllocSurface)
    {
        EhsTVSurface_destroy(pViewport, pViewport->pAllocSurface); //@todo check this isn't leaving gtk stuff behind - should it unref surfaces?
    }
    pViewport->pAllocSurface=NULL;//Should this go to the hal?
    EhsWidgetTable_init(&EhsWidgetTable);
    EhsTV_clear(pViewport);
    EhsTPMutex_unlock(EhsTPMutex_widgetTable);
    //EhsGtkTimerActive=EHS_TRUE;
    LEAVE(EhsTV_reset);
}

/**
 * called after widgets are created
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
        //gtk_window_set_decorated(GTK_WINDOW(pViewport->pMainWindow),FALSE);
    }
    EhsSleep(EHS_TIME_us(50000)); // a sleep period of about 10ms allows window to be positioned before it is shown in the timer thread
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
    //if((newX != 0) || (newY != nY))
    //{
    newX = nX;
    newY = nY;
    newWidth = nDeltaWid;
    newHeight = nDeltaHt;
    windowMoved = 1;
    repositionWindow = 1;
    //}
    //if((newX != nX) || (newY != nY))


    LEAVE(EhsTV_move);
    //EHSH_LOG_INFO("EhsTV_move:windowMoved=[%i]repositionWindow=[%i]\n",windowMoved,repositionWindow);
}


/*Change size and position of viewport*/
void EhsTV_setwindow(EhsTVClass* pViewport, ehs_bool bIsView, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nWid, EhsDataflowIntType nHt)
{
    ENTER(EhsTV_move);
    SetX = nX;
    SetY = nY;
    SetWidth = nWid;
    SetHeight = nHt;

    // we also need to reset the offset x,y,w,h as this method is called when an app is restarted
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
    EhsTV_updateRect(&EhsTV, 0, 0, SetWidth+newWidth,SetHeight+newHeight);
    LEAVE(EhsTV_update);
}

/**
 * Redraw a specified part of the OSD layer.global
 *
 */
void EhsTV_updateRect(EhsTVClass* pViewport, ehs_sint32 nX, ehs_sint32 nY, ehs_sint32 nWidth, ehs_sint32 nHeight)
{
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
    LEAVE(EhsTV_updateRect);
}

/**
 * Clear the target viewport
 */
void EhsTV_clear(EhsTVClass* pViewport)
{
    ENTER(EhsTV_clear);
    EhsTV_update(pViewport);
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

    /* Ypu might not need to do all this:*/
    /* calculate the parts of the image that we need to update */
    if (EhsGraphicsRectangle_intersect(&blitBounds,pDst,&intersectViewClip))
    {
        dScaleX = (ehs_float)(pDst->nWidth) / (ehs_float)(pSrc->nWidth);
        dScaleY = (ehs_float)(pDst->nHeight) / (ehs_float)(pSrc->nHeight);
        dOffsetX = (ehs_float)(pDst->nLeft - blitBounds.nLeft) * dScaleX;
        dOffsetY = (ehs_float)(pDst->nTop - blitBounds.nTop) * dScaleY;

        switch (pImgData->eFormat)
        {
        case EHS_GRAPHICS_COLOUR_ARGB8888:
            /* OpenGL rendering code here */
            break;

        case EHS_GRAPHICS_COLOUR_A1:
            /* I can't see another way of doing this, so I create a full 32-bit per pixel image
             * corresponding to the 1 bit per pixel used as the starting point. Each pixel gets
             * transferred into the larger format, then composited, then the image is thrown away.
             * A faster (but less memory efficient) implementation would allocate the pixbuf during
             * the text writing function.
             */

            /*For all pixels in the image data*/
            // Calculate the row offset. its on apImgData->fmt.A1.nWidth byte boundary

            /* OpenGL rendering for 1 bit data (e.g. defult font renderer) code here */
            break;
        }
    }
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

    /* calculate the parts of the image that we need to update */
    if (EhsGraphicsRectangle_intersect(&blitBounds, pRect, &intersectViewClip))
    {
        /* OpenGL rendering code for a filled rectangle here */
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

        /*Cairo should now handle freeing the memory for destroyed surfaces*/
        switch (pSurface->eFormat)
        {
        case EHS_GRAPHICS_COLOUR_ARGB8888:
            /* un-bind and free OpenGL rendering code here */
            break;

        case EHS_GRAPHICS_COLOUR_A1:
            /* un-bind and free OpenGL rendering code here */
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
        // SDG:@todo Commented out the following line because it causes EHS to crash when new SODL is loaded.
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
                                       EhsGraphicsColourClass* pPalette, ehs_uint16 nPaletteSize) // obsolete : ehs_bool bTemporary)
{
    ehs_uint32 i;
    EhsTVSurfaceClass* pSurface = NULL;
    ehs_bool bFailed = EHS_FALSE; /* flag to indicate failure - need to release allocated memory */
    ENTER(EhsTVSurface_create);

    if (eFormat == EHS_GRAPHICS_COLOUR_ARGB8888 || eFormat == EHS_GRAPHICS_COLOUR_A1)
    {
        /*if (bTemporary) {
        	pSurface = EhsHMem_tempAlloc(sizeof(EhsTVSurfaceClass));
        } else {
        	pSurface = EhsHMem_writeableAlloc(sizeof(EhsTVSurfaceClass));
        }*/
        pSurface = EhsHMem_tempAlloc(sizeof(EhsTVSurfaceClass));
        if (pSurface)
        {
            pSurface->eFormat = eFormat;
            switch (eFormat)
            {
            case EHS_GRAPHICS_COLOUR_ARGB8888:
                /* OpenGL rendering code here */
                break;
            case EHS_GRAPHICS_COLOUR_A1:
                /* OpenGL rendering code here */
                break;
            }
        }
        if (bFailed)
        {
            if (pSurface)
            {
                EhsHMem_tempFree(pSurface);
            }
            pSurface = NULL;
        } 
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
    }
    else
    {
        EHSH_LOG_ERROR(EHS_MSG_TGT_GRAPHICS_UNSUPPORTED_MODE("unrecognised bitmap format"));
    }

    LEAVE(EhsTVSurface_create);
    return pSurface;
}

/**
 * Provide access to pixels representing the surface
 */
EhsGraphicsColourClass* EhsTVSurface_pixels(EhsTVSurfaceClass* pSurface)
{
    EhsGraphicsColourClass* pRet = NULL;
    switch (pSurface->eFormat)
    {
    case EHS_GRAPHICS_COLOUR_ARGB8888:
        /* probably don't need this in OpenGL */
        pRet = NULL;
        break;
    case EHS_GRAPHICS_COLOUR_A1:
        /* probably don't need this in OpenGL */
        pRet = NULL; //@todo PPP: Is this correct?? Shouldn't it return somehting - perhaps not it's not a colour image?
        break;
    }

    return pRet;
}

/**
 * Provide access to the bitmap representing the surface - if there is one
 */
void* EhsTVSurface_bitmap(EhsTVSurfaceClass* pSurface)
{
    EhsGraphicsColourClass* pRet = NULL;
    switch (pSurface->eFormat)
    {
    case EHS_GRAPHICS_COLOUR_ARGB8888:
        pRet = NULL;
        break;
    case EHS_GRAPHICS_COLOUR_A1:
        pRet = ((void*)((pSurface)->fmt.A1.pBitmap));
    }

    return pRet;
}


/**
 * Return the number of bytes across (allows direct addressing in the array of pixels -
 * index = x + y * pitch
 * RETURNS PIXELS - NOT BYTES
 *
 */
ehs_uint16 EhsTVSurface_pitch(EhsTVSurfaceClass* pSurface)
{
    ehs_uint16 nRet = 0u;

    switch (pSurface->eFormat)
    {
    case EHS_GRAPHICS_COLOUR_ARGB8888:
        /* TODO This will be the power of two width check this is right!*/
        nRet = pSurface->nBufferWidth; //nWidth
        //nRet = pSurface->fmt
        break;
    case EHS_GRAPHICS_COLOUR_A1:
        nRet = pSurface->fmt.A1.nWidth;
        break;
    }
    return nRet;
}

/**
 * Return the width of the bitmap
 */
ehs_uint16 EhsTVSurface_width(EhsTVSurfaceClass* pSurface)
{
    ehs_uint16 nRet = 0u;

    switch (pSurface->eFormat)
    {
    case EHS_GRAPHICS_COLOUR_ARGB8888:
        /* possibly not needed for OpenGL? */
        nRet = pSurface->nWidth;
        break;
    case EHS_GRAPHICS_COLOUR_A1:
        nRet = pSurface->fmt.A1.nWidth;
        break;
    }
    return nRet;
}

/**
 * Return the height of the bitmap
 */
ehs_uint16 EhsTVSurface_height(EhsTVSurfaceClass* pSurface)
{
    ehs_uint16 nRet = 0u;

    switch (pSurface->eFormat)
    {
    case EHS_GRAPHICS_COLOUR_ARGB8888:
        /* probably not needed for OpenGL */
        nRet = pSurface->nHeight;
        break;
    case EHS_GRAPHICS_COLOUR_A1:
        nRet = pSurface->fmt.A1.nHeight;
        break;
    }
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


