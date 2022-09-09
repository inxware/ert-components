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
#include "keypress.h" /* we take some direct keyboard functions in here */

#ifndef EHS_MINGW
#include <X11/Xlib.h>
#include <gdk/gdkx.h>
#else
/*
_WIN32_WINNT should be define in target_config.h, but just in case
*/
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
//#include <windows.h>
#include <gdk/gdkwin32.h>
#include <gdk/gdk.h>
//#define _WIN32_WINNT
#ifndef WINVER
#define WINVER 0x0500
#endif
#include <windows.h>
WINUSERAPI BOOL WINAPI SetLayeredWindowAttributes(HWND,COLORREF,BYTE,DWORD);
#endif
#include <gtk/gtk.h>

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
//#undef TRACE_VIEWPORT
#ifdef TRACE_VIEWPORT
int EhsTraceLevel = 0;
static char* currentFunc;
#define INDENT(x) ((x==0)?"":((x==1)?">":((x==2)?">>":((x==3)?">>>":(">..>")))))
#define ENTER(x) {ehs_uint32 ticks = EhsTgtTimer_tickTous(EhsTgtTimer_now()); EhsTraceLevel++;currentFunc = #x;}
#define LEAVE(x) {ehs_uint32 ticks = EhsTgtTimer_tickTous(EhsTgtTimer_now());--EhsTraceLevel;
#else
#define ENTER(x)
#define LEAVE(x)
#endif


GtkWidget * EhsTMain_GTK_Window_Handle;

/**
 * Define the target viewport type. Contains target specific data required
 * by the viewport functions.
 */
struct EhsTVStruct
{
    GtkWidget* pMainWindow;				/**< Contains the toplevel window for the viewport */
    cairo_t* cr; /*Cairo context for viewport*/
    EhsGraphicsRectangleClass xClipRect;	/**< Clipping rectangle - used when drawing all images */
    EhsTVSurfaceClass* pAllocSurface; /**< List of allocated surfaces - used for deallocation purposes */
    ehs_bool bViewportChanged;			/**< The pixbuf has changed, we need to copy it into pPixmap */
    ehs_uint8 transparency; // transparency of window
};

/*****************************************************************************/
/* Declare prototypes of local functions */


/* When the window is given the "delete_event" signal (this is given
 * by the window manager, usually by the "close" option, or on the
 * titlebar), we ask it to call the delete_event () function
 * as defined above. The data passed to the callback
 * function is NULL and is ignored in the callback function. */
EHS_LOCAL gboolean EhsL_event_deleteWindow(GtkWidget* pWidget, GdkEventExpose* pEvent);

/* Here we connect the "destroy" event to a signal handler.
 * This event occurs when we call gtk_widget_destroy() on the window,
 * or if we return FALSE in the "delete_event" callback. */
EHS_LOCAL gboolean EhsL_event_destroyWindow(GtkWidget* pWidget, GdkEventExpose* pEvent);

/**
 * Window has been assigned a new size and/or position
 * @param[in] pWidget The Widget that this event applies to
 * @param[in] pAllocation position and size to be allocated to widget
 */
EHS_LOCAL gboolean EhsL_size_allocate(GtkWidget* pWidget, GtkAllocation* pAllocation);

/**
 * Update the main drawing area. This function is called as an event
 * @param[in] pWidget The Widget that this event applies to
 * @param[in] pEvent The event that triggered this function
 */
EHS_LOCAL gboolean EhsL_event_expose(GtkWidget* pWidget, GdkEventExpose* pEvent);

#ifdef EHS_MINGW
EHS_LOCAL gboolean EhsL_event_show(GtkWidget* pWidget, GdkEventExpose* pEvent); //for the first showing - calls some win32 config stuff that GDK cant do
#endif
/**
 * Create a pixmap for the main drawing area. This function is called
 * as an event.
 * @param[in] pWidget Widget that this event applies to
 * @param[in] pEvent Event that triggered this function.
 */
EHS_LOCAL gboolean EhsL_event_configure(GtkWidget* pWidget, GdkEventConfigure* pEvent);

/**
 * Handle key presses
 * @param[in] pWidget The Widget that this event applies to
 * @param[in] pEvent The event that triggered this function
 */

EHS_LOCAL gboolean EhsL_event_key_release(GtkWidget* pWidget, GdkEventKey* pEvent);
EHS_LOCAL gboolean EhsL_event_key_press(GtkWidget* pWidget, GdkEventKey* pEvent);


EHS_LOCAL gboolean EhsL_event_button_press(GtkWidget* pWidget, GdkEventButton* pButton);
EHS_LOCAL gboolean EhsL_event_button_release(GtkWidget* pWidget, GdkEventButton* pButton);
EHS_LOCAL gboolean EhsL_event_motion_notify(GtkWidget* pWidget, GdkEventMotion* pMotion);


/* Handle window Resizing */
EHS_LOCAL void EhsTV_MaximiseViewport(GtkWidget* pWidget) ;
EHS_LOCAL void EhsTV_MinimiseViewport (GtkWidget* pWidget) ;

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

/*Used for redrawing screen at GTK level
 * @todo we don't really want a timer here - we should call this on updates only.
 * - especially for actual blits as we might just get tearing etc.
 * We should see if this function can be prompted only after rendering - or by calls from GTK if the window is obscured.
 * */
gboolean timer_exe(GtkWidget * window)
{
    if (!EhsGtkTimerActive)
        return EHS_TRUE; /* return true or else timer call backs stop for ever */
    if (!EhsTPMutex_widgetTable)
        return EHS_TRUE;/* This can be made null on application term, while a timer event is still pending */
    /* No need to mutex the widget table as we don't call anything directly from here */
    if (hideViewport)
    {
        hideViewport = 0;
        gtk_widget_hide(window);
    }
    if (showViewport) // refresh this perhaps..
    {
        showViewport = 0;
        if (nZOrder == 2)
        {
            gtk_window_set_keep_above(GTK_WINDOW(window), TRUE); // We may want this to be true for embedded - dispate the app?
            gtk_window_set_keep_below(GTK_WINDOW(window), FALSE); //ideallly this should be below the video plane for windows//
        }
        else if (nZOrder == 1)
        {
            gtk_window_set_keep_above(GTK_WINDOW(window), FALSE);
            gtk_window_set_keep_below(GTK_WINDOW(window), FALSE);
        }
        else
        {
            gtk_window_set_keep_above(GTK_WINDOW(window), FALSE); // We may want this to be true for embedded - dispate the app?
            gtk_window_set_keep_below(GTK_WINDOW(window), TRUE); //ideallly this should be below the video plane for windows//
        }
        gtk_widget_show(window);
    }
    if (windowMoved)
    {
        if (repositionWindow)
        {
            if (bIsFirstTime || !windowHasFrame)
            {
                gtk_window_move(GTK_WINDOW(window), SetX + newX, SetY + newY);
                gtk_widget_modify_bg(window, GTK_STATE_NORMAL, (GdkColor*)&viewColour);
            }
            else
            {
                int nCurrX;
                int nCurrY;
                gtk_window_get_position(GTK_WINDOW(window), &nCurrX, &nCurrY);
                gtk_window_move(GTK_WINDOW(window), nCurrX + newX,
                                nCurrY + newY);
            }
        }
        gtk_window_resize(GTK_WINDOW(window), SetWidth + newWidth,SetHeight + newHeight);

        windowMoved = 0;
        repositionWindow = 0;
        bIsFirstTime = EHS_FALSE;
    }
    if (update) //update the entire screen..
    {
        gdk_window_process_updates(gtk_widget_get_window (window), EHS_FALSE); // Sends one or more expose events to window - forces repaint of dirty rectangle
        gtk_widget_queue_draw_area(window, globalRect.nLeft, globalRect.nTop,
                                   globalRect.nWidth, globalRect.nHeight);
        update = 0;//@todo temo test
    }
    return TRUE;
}
#ifdef EHS_MANUAL_ALIGNMENT
ehs_uint32 stride_value(ehs_uint32 width, ehs_uint32 bpp)
{
    return ((((bpp*width)+7)/8) + (sizeof(ehs_uint32)-1)) & ~(sizeof(ehs_uint32)-1);
}
#endif

/* non use paramter initialisation - this is called before any SODL is read*/
/* We keep the GTK thread going between app function calls - we may want graphics in-between..*/

ehs_bool EhsTV_init(EhsTVClass* pViewport)
{
    ehs_bool bInitialised = EHS_TRUE; /* has initialisation been successful? assume it has */
    /* All GTK initialisation is done in the gtk main (loop) thread */

    //@todo need to review this code for all targets, viewRectangle controls the area of the viewport that widgets can be painted in and is only set here - it is not updated if a viewport widget exists
    viewRectangle.nTop = 0;
    viewRectangle.nLeft = 0;
    viewRectangle.nHeight = EHS_CONFIG_DISPLAY_HEIGHT;//Use the default values in case the viewport block is not used.
    viewRectangle.nWidth = EHS_CONFIG_DISPLAY_WIDTH;

    viewColour.sComp.nAlpha = 255;
    viewColour.sComp.nRed = 0;
    viewColour.sComp.nGreen = 0;
    viewColour.sComp.nBlue = 0;
    g_thread_init(NULL); // PPP: Needs glib for linking

    g_thread_create((GThreadFunc)EhsL_graphicsThreadFunc, (gpointer)pViewport,FALSE,NULL); //@todo this does not have the EHS thread policy??

    while (!go)
    {
        g_usleep(10000);   //Wait for gtk init to have completed (except the GTK Main part).
    }
    //go2 = 1; //flag back to EhsL_graphicsThreadFunc (gtk thread) that we have started


    return bInitialised;
}

/**
 * Run the graphics handling in a thread.
 * For windows all GTK calls must be made from this thread
 *
 */
EhsThreadFuncReturnType EhsL_graphicsThreadFunc(EhsTVClass* pViewport)
{
//EhsTCreateViewPort(EhsTVClass* pViewport){
    GdkScreen *screen; /*Used for getting colour map of screen*/
#if GTK_MAJOR_VERSION == 2
    GdkColormap *colormap;/*Used for getting colour map of screen*/
#endif
    EhsGraphicsRectangleClass rect;

    /* handle gtk events as they occur */
    int argc = 0;
    char** argv = NULL;
    //GdkColor 	black = {0x50, 0x50, 0x50, 0x50};
//	gdk_threads_enter();
    /*Set update rectangle to whole viewport to force full redraw of screen at start. 23/07/2008 PB*/
    globalRect.nTop = 0; //pViewport->xCurRect.nTop;
    globalRect.nLeft = 0; //pViewport->xCurRect.nLeft;
    globalRect.nHeight = EHS_CONFIG_DISPLAY_HEIGHT; //pViewport->xCurRect.nHeight; //!!! we set to system defaults in case a viewport widget is not defined.
    globalRect.nWidth = EHS_CONFIG_DISPLAY_WIDTH;//pViewport->xCurRect.nWidth; //EHS_CONFIG_DISPLAY_WIDTH;
    EHS_TRACE_MESSAGE("GTK Viewport: Initialising gtk\n");
    gtk_init(&argc, &argv);
    EHS_TRACE_MESSAGE("GTK Viewport: Initialised gtk\n");

    /************************ main window set up *************************/
    //pViewport->pMainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
#ifdef EHS_MINGW
    pViewport->pMainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
#else
    pViewport->pMainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
#endif

    EHS_TRACE_MESSAGE("GTK Viewport: Created Window\n");
#ifdef EHS_DEBUG
    if (pViewport->pMainWindow->window) gdk_keyboard_grab(pViewport->pMainWindow->window, FALSE, GDK_CURRENT_TIME);
#endif
    /* get RGBA colormap */
    //gdk_keyboard_grab(pViewport->pMainWindow->window, TRUE, GDK_CURRENT_TIME);
    screen = gdk_screen_get_default();
    EHSH_LOG_INFO("GTK Viewport: Got Screen = %d\n",screen);
#if GTK_MAJOR_VERSION == 2
    colormap = gdk_screen_get_rgba_colormap(screen);
    if (colormap == NULL)
    {
        //gdk_window_set_opacity ((GdkScreen)screen,1.0); // 12-01-13 R set a default value - for screen really? would this work?
#endif
        gtk_widget_set_default_colormap(gdk_colormap_get_system ());
#if GTK_MAJOR_VERSION == 2

    }
    else
    {
        //gdk_window_set_composited(screen,1);
        gtk_widget_set_default_colormap(colormap);
    }
#endif

    EHS_TRACE_MESSAGE("GTK Viewport: Carrying on\n",screen);
    /*Set window gravity to top left so that 0,0 is the top left of the screen*/
    gtk_window_set_gravity(GTK_WINDOW(pViewport->pMainWindow),GDK_GRAVITY_STATIC); /*This should be a GdkGravity enumeration but I can't get that working yet 22/07/2008 PB*/
    /* @todo do we need double buffered? */
    //gtk_widget_set_double_buffered(pViewport->pMainWindow, TRUE);
    gtk_widget_set_app_paintable(pViewport->pMainWindow, TRUE);
    /* undecorated window, with no border around the main screen */
    EHS_TRACE_MESSAGE("GTK Viewport: Set window to paintable\n");

    gtk_container_set_border_width(GTK_CONTAINER(pViewport->pMainWindow),0);
    gtk_window_set_decorated(GTK_WINDOW(pViewport->pMainWindow),FALSE);
    /*
     * - removed as paints grey rectangle before app is ready
     * gtk_window_set_default_size(GTK_WINDOW(pViewport->pMainWindow),viewRectangle.nWidth,viewRectangle.nHeight );//pWidget->xCurRect.nWidth, pWidget->xCurRect.nHeight);
     */
    gtk_window_set_default_size(GTK_WINDOW(pViewport->pMainWindow),viewRectangle.nWidth,viewRectangle.nHeight );//pWidget->xCurRect.nWidth, pWidget->xCurRect.nHeight);

    EHS_TRACE_MESSAGE("GTK Viewport: Set w,h size paramters %d,%d\n",viewRectangle.nWidth,viewRectangle.nHeight);


    // We should have access to the z-order information in here to select the following..
#ifdef EHS_MINGW // It doesn't seem to make much difference with mingw...
    gtk_window_set_keep_above(GTK_WINDOW(pViewport->pMainWindow),FALSE); // We may want this to be true for embedded - dispate the app?
    gtk_window_set_keep_below(GTK_WINDOW(pViewport->pMainWindow),TRUE); //ideallly this should be below the video plane for windows//
#else

    if (nZOrder == 2)
    {
        gtk_window_set_keep_above(GTK_WINDOW(pViewport->pMainWindow),TRUE); // We may want this to be true for embedded - dispate the app?
        gtk_window_set_keep_below(GTK_WINDOW(pViewport->pMainWindow),FALSE); //ideallly this should be below the video plane for windows//
    }
    else if (nZOrder == 1)
    {
        gtk_window_set_keep_above(GTK_WINDOW(pViewport->pMainWindow),FALSE);
        gtk_window_set_keep_below(GTK_WINDOW(pViewport->pMainWindow),FALSE);
    }
    else
    {
        gtk_window_set_keep_above(GTK_WINDOW(pViewport->pMainWindow),FALSE); // We may want this to be true for embedded - dispate the app?
        gtk_window_set_keep_below(GTK_WINDOW(pViewport->pMainWindow),TRUE); //ideallly this should be below the video plane for windows//
    }

#endif

    //gtk_widget_modify_bg(pViewport->pMainWindow, GTK_STATE_NORMAL, &black );
    gtk_widget_set_events(pViewport->pMainWindow, GDK_BUTTON_MOTION_MASK|GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_PRESS_MASK|GDK_KEY_PRESS_MASK|GDK_KEY_RELEASE_MASK);//|GDK_FOCUS_CHANGE_MASK|GDK_EXPOSURE_MASK);
    //gtk_widget_set_events(pViewport->pMainWindow, GDK_BUTTON_PRESS_MASK);
    EHS_TRACE_MESSAGE("GTK Viewport: Set events\n");
    /*Connect GTK events to our functions*/

    /* When the window is given the "delete_event" signal (this is given
     * by the window manager, usually by the "close" option, or on the
     * titlebar), we ask it to call the delete_event () function
     * as defined above. The data passed to the callback
     * function is NULL and is ignored in the callback function. */
    g_signal_connect(G_OBJECT(pViewport->pMainWindow), "delete_event", G_CALLBACK(EhsL_event_deleteWindow), NULL);

    /* Here we connect the "destroy" event to a signal handler.
     * This event occurs when we call gtk_widget_destroy() on the window,
     * or if we return FALSE in the "delete_event" callback. */
    g_signal_connect(G_OBJECT(pViewport->pMainWindow), "destroy", G_CALLBACK(EhsL_event_destroyWindow), NULL);

    g_signal_connect(G_OBJECT(pViewport->pMainWindow), "expose_event", G_CALLBACK(EhsL_event_expose), NULL);
#ifdef EHS_MINGW
    g_signal_connect(G_OBJECT(pViewport->pMainWindow), "show", G_CALLBACK(EhsL_event_show), NULL);
#endif
    // This stops top level windows working.. g_signal_connect(G_OBJECT(pViewport->pMainWindow), "configure_event", G_CALLBACK(EhsL_event_configure), NULL);
    g_signal_connect(G_OBJECT(pViewport->pMainWindow), "size_allocate", G_CALLBACK(EhsL_size_allocate), NULL);


#ifdef EHS_MINGW // doesnt work propoerly for win32 (can't get focs back adter showin video)

    g_signal_connect(G_OBJECT(pViewport->pMainWindow), "key_release_event", G_CALLBACK(EhsL_event_key_release), NULL );
    g_signal_connect(G_OBJECT(pViewport->pMainWindow), "key_press_event", G_CALLBACK(EhsL_event_key_press), NULL );
#else
    g_signal_connect(G_OBJECT(pViewport->pMainWindow), "key_release_event", G_CALLBACK(EhsL_event_key_release), NULL );
    g_signal_connect(G_OBJECT(pViewport->pMainWindow), "key_press_event", G_CALLBACK(EhsL_event_key_press), NULL );
#endif
    /*Catch mouse button events*/
    g_signal_connect(G_OBJECT(pViewport->pMainWindow), "button_press_event", G_CALLBACK(EhsL_event_button_press), NULL); //use with GDK_BUTTON_PRESS_MASK
    g_signal_connect(G_OBJECT(pViewport->pMainWindow), "button_release_event", G_CALLBACK(EhsL_event_button_release), NULL); //use with GDK_BUTTON_RELEASE_MASK
    /*Catch mouse move events*/
    g_signal_connect(G_OBJECT(pViewport->pMainWindow), "motion_notify_event", G_CALLBACK(EhsL_event_motion_notify), NULL); //use with GDK_BUTTON_MOTION_MASK

    /*Set the window to resizeable, move it to the top left and then show it*/
    gtk_window_set_resizable(GTK_WINDOW(pViewport->pMainWindow), TRUE);
    gtk_window_move(GTK_WINDOW(pViewport->pMainWindow), viewRectangle.nLeft, viewRectangle.nTop);
    EHS_TRACE_MESSAGE("GTK Viewport: Moved window to %d,%d\n",viewRectangle.nLeft, viewRectangle.nTop);
#if GTK_MAJOR_VERSION == 2
    GTK_WIDGET_SET_FLAGS (pViewport->pMainWindow, GTK_CAN_FOCUS);
    GTK_WIDGET_SET_FLAGS (pViewport->pMainWindow, GTK_CAN_DEFAULT);
#endif
#ifndef EHS_MINGW
    gdk_keyboard_grab(gtk_widget_get_window(GTK_WIDGET(pViewport->pMainWindow)), EHS_FALSE, GDK_CURRENT_TIME); // 'f' grabs the keybaord for this window
    gtk_widget_grab_focus (pViewport->pMainWindow);
#endif
    /* Desperate attempts to keep keyboard focus in win32.


    gtk_widget_grab_default (pViewport->pMainWindow);
    gtk_widget_grab_focus (pViewport->pMainWindow); //These may put the gui on top when we don;t want to.
    */
    ///////gtk_window_set_transient_for(GTK_WINDOW(pViewport->pMainWindow), );
    //gdk_keyboard_grab(pViewport->pMainWindow->window, TRUE, GDK_CURRENT_TIME);
    /*
     * - don't show it yet
     * gtk_widget_show(EhsTV.pMainWindow);
     */
#ifdef EHS_MINGW
    gtk_widget_show(EhsTV.pMainWindow);
#endif
    ///////GUIViewPortWidget=&(EhsTV.pMainWindow); // we do this for other Screen users that don't understand the EheTv.pMainWinow struct type.
    /* This isn't ready yet, but we'll do it here for legacy reasons @todo should remove this next test cycle */
    if (gtk_widget_get_window(pViewport->pMainWindow)) gdk_keyboard_grab(gtk_widget_get_window(pViewport->pMainWindow), TRUE, GDK_CURRENT_TIME);

    /*Launch thread that will fire expose events for us*/
    /*@todo review the need for the timer */
#define EHS_NEED_A_TIMER_FOR_GRAPHICS
#ifdef EHS_NEED_A_TIMER_FOR_GRAPHICS
    (void)g_timeout_add(EHS_TIME_BETWEEN_FRAMES, (GSourceFunc)timer_exe, EhsTV.pMainWindow);
#endif
    //gdk_threads_leave();
    EHSH_LOG_INFO(" Starting GTK");
    gtk_main_iteration(); //don't start till some thing has been done to avoid the X-server Errors.
    go=1;
    while (1)
    {
        gtk_main_iteration_do(0);    //this seems less crashy than the gtk_main() blocker!
        g_usleep(50000);
    }
    EHSH_LOG_WARNING("gtk_main - exited!");
    EhsHThread_exit();
}

extern int EHS_All_inited;

/**
 * Create a pixmap for the main drawing area. This function is called
 * as an event.
 * @param[in] pWidget Widget that this event applies to
 * @param[in] pEvent Event that triggered this function.
 */
gboolean EhsL_event_configure(GtkWidget* pWidget, GdkEventConfigure* pEvent)
{
    ENTER(EhsL_event_configure);

    //gdk_keyboard_grab(GTK_WIDGET(pWidget)->window, EHS_FALSE, GDK_CURRENT_TIME);

    LEAVE(EhsL_event_configure);
    return TRUE /* we've handled the event */ ;
}

// The following is to be called in show - this is mostly required for windows hosts where the hwnd needs to be available to do final configuration

#ifndef LWA_ALPHA
#define LWA_ALPHA 0x00000002
#endif

void EhsL_graphics_late_config(GtkWidget* pWidget, ehs_bool doall)
{
#ifdef EHS_MINGW
    if (doall)   //EHSTV_configured) { // for some reason this doesn't work if a viewport widget has not reconfigured it (probably the alpha value is not valid).
    {
        SetWindowLong(GDK_WINDOW_HWND(pWidget->window), GWL_EXSTYLE,GetWindowLong(GDK_WINDOW_HWND(pWidget->window), GWL_EXSTYLE) | WS_EX_LAYERED|WS_EX_TOPMOST);
        SetLayeredWindowAttributes(GDK_WINDOW_HWND(pWidget->window), 0, viewColour.sComp.nAlpha, LWA_ALPHA);

    }
    gtk_window_set_focus (GTK_WINDOW(pWidget),pWidget);
    SetFocus(GDK_WINDOW_HWND(pWidget->window)); // this is to allow keyboard grabbing in windows as GDK doesn't  work here ..
#endif
}

#ifdef EHS_MINGW
ehs_bool horrible_flag_to_prompt_lateconfig_on_new_windows=0;
gboolean EhsL_event_show(GtkWidget* pWidget, GdkEventExpose* pEvent)
{
    ENTER(EhsL_event_expose);
    horrible_flag_to_prompt_lateconfig_on_new_windows=1;
    LEAVE(EhsL_event_expose);
    return TRUE;

}
#endif




/* When the window is given the "delete_event" signal (this is given
 * by the window manager, usually by the "close" option, or on the
 * titlebar), we ask it to call the delete_event () function
 * as defined above. The data passed to the callback
 * function is NULL and is ignored in the callback function. */
gboolean EhsL_event_deleteWindow(GtkWidget* pWidget, GdkEventExpose* pEvent)
{
    ENTER(EhsL_event_deleteWindow);

    EhsHFSMSetInternallyRequestedCommand(EHS_EXIT_EHS);

    LEAVE(EhsL_event_deleteWindow);
    return TRUE;
}

/* Here we connect the "destroy" event to a signal handler.
 * This event occurs when we call gtk_widget_destroy() on the window,
 * or if we return FALSE in the "delete_event" callback. */
gboolean EhsL_event_destroyWindow(GtkWidget* pWidget, GdkEventExpose* pEvent)
{
    ENTER(EhsL_event_destroyWindow);

    EhsHFSMSetInternallyRequestedCommand(EHS_EXIT_EHS);

    LEAVE(EhsL_event_destroyWindow);
    return TRUE;
}

/**
 * Window has been assigned a new size and/or position
 * @param[in] pWidget The Widget that this event applies to
 * @param[in] pAllocation position and size to be allocated to widget
 */
gboolean EhsL_size_allocate(GtkWidget* pWidget, GtkAllocation* pAllocation)
{
    ENTER(EhsL_size_allocate);

    // set viewport rect dimensions after resize gtk window
    //@todo should we set SetWidth,newWidth etc and then call EhsTV_update(&EhsTV); ??
    viewRectangle.nWidth = pAllocation->width;
    viewRectangle.nHeight = pAllocation->height;

    LEAVE(EhsL_size_allocate);
    return TRUE;
}

/**
 * Update the main drawing area. This function is called as an event
 * @param[in] pWidget The Widget that this event applies to
 * @param[in] pEvent The event that triggered this function
 */
gboolean EhsL_event_expose(GtkWidget* pWidget, GdkEventExpose* pEvent)
{
    static ehs_uint32 expose_count;
    ENTER(EhsL_event_expose);

//#define EHS_WINDOWING_UI
#ifdef EHS_WINDOWING_UI
    //@todo - better place to set title, needs to be called everytime a new app is loaded
    gtk_window_set_title(GTK_WINDOW(pWidget), EhsHMetaAppGetCurrent());
#endif
    EhsTPMutex_lock(EhsTPMutex_widgetTable);
    /* set up clipping rectangle */
    EhsGraphicsRectangleClass exposeRect;
    exposeRect.nLeft = pEvent->area.x;
    exposeRect.nTop = pEvent->area.y;
    exposeRect.nHeight = pEvent->area.height;
    exposeRect.nWidth = pEvent->area.width;




    if ( (pEvent->area.height == 0 ) || (pEvent->area.width == 0) ) goto skip;
#ifdef EHS_MINGW
    if (horrible_flag_to_prompt_lateconfig_on_new_windows)
    {
        EhsL_graphics_late_config(pWidget,1);
        horrible_flag_to_prompt_lateconfig_on_new_windows=0;

    }
#endif

#ifdef EHS_MINGW
    if (viewport_grab&&((expose_count++)%10==0))
    {
        EhsL_graphics_late_config(pWidget,0); // regain focus every half second or so in case a video has grabbed it.
    }
#endif

    /*Calculate area that wants updating that lies in the viewport. Useful for stuff that is scrolling off the screen*/
    /*intersectViewClip is used by the other blitting functions to calculate how much to draw*/
    if(EhsGraphicsRectangle_intersect(&intersectViewClip,&viewRectangle,&exposeRect))
    {

        //EHS_TRACE_MESSAGE("GTK Viewport:Have a rect todo\n");
        /*
         * Note: Note that due to double-buffering, Cairo contexts created in a GTK+ expose event handler cannot be cached and reused between different expose events.
         * Se we have to create new each expose in GTK
         */
#ifdef EHS_USE_CAIRO_LESS_THAN_322
        EhsTV.cr = gdk_cairo_create(gtk_widget_get_window(pWidget));
#else
        EhsTV.cr = gdk_cairo_create(gtk_widget_get_window(pWidget));
#endif

        //EHS_TRACE_MESSAGE("GTK Viewport:Have gdk_cairo_create\n");
#ifndef NOOOOOO
        //cairo_set_operator(EhsTV.cr, CAIRO_OPERATOR_SOURCE);
        cairo_set_operator(EhsTV.cr, CAIRO_OPERATOR_OVER);
        //EHS_TRACE_MESSAGE("GTK Viewport:Have cairo_set_operator\n");
        // cairo_move_to (cr, 30, 30);
        gdk_cairo_region(EhsTV.cr, pEvent->region);

        //EHS_TRACE_MESSAGE("GTK Viewport:Have gdk_cairo_region\n");
        cairo_clip(EhsTV.cr);
        //EHS_TRACE_MESSAGE("GTK Viewport:Have cairo_clip\n");


        cairo_set_source_rgba (EhsTV.cr, ((double)viewColour.sComp.nRed / 255), ((double)viewColour.sComp.nGreen / 255), ((double)viewColour.sComp.nBlue / 255), ((double)viewColour.sComp.nAlpha / 255));

        //cairo_show_text (EhsTV.cr, "HELLOOOO  WHAT IS GOING ON!");
        // rendering ends up with double alpha for the background for windows, but we don't mind so much about windows
        //EHS_TRACE_MESSAGE("GTK Viewport:Have a cairo_set_source_rgba\n");
        //Paint the background
        cairo_paint(EhsTV.cr);
        //EHS_TRACE_MESSAGE("GTK Viewport:Have a cairo_paint\n");

        //cairo_set_operator(EhsTV.cr, CAIRO_OPERATOR_OVER);
        //EHS_TRACE_MESSAGE("GTK Viewport:Have cairo_set_operator\n");
        if (EhsWidgetTable.initialised != EHS_MAGIC_NUMBER)
        {
            EhsTPMutex_unlock(EhsTPMutex_widgetTable);
            return TRUE; // The mutex many not be initialised either!
        }
        /*Draw all widgets in the exposed region*/
        EhsWidgetTable_draw(&EhsWidgetTable,&EhsTV,&exposeRect);
        //EHS_TRACE_MESSAGE("GTK Viewport:Have EhsWidgetTable_draw\n");
#endif
        cairo_destroy(EhsTV.cr);
        //EHS_TRACE_MESSAGE("GTK Viewport:Have Ecairo_destroy\n");
    }
    else
    {
        //EHSH_LOG_ERROR("EXPOSE DIDNT EXPOSE\n");
    }
skip:
    EhsTPMutex_unlock(EhsTPMutex_widgetTable);
    LEAVE(EhsL_event_expose);
    return TRUE;
}




/**
 * This calls the callback quese for key presses that some objects subscribe to (RCU and key press).
 * @param[in] pWidget The Widget that this event applies to
 * @param[in] pEvent The event that triggered this function
 */
gboolean EhsL_event_key_release(GtkWidget* pWidget, GdkEventKey* pEvent)
{
    /* pEvent->keyval contains the keycode. Values are listed in <gdk/gdkkeysyms.h>
     */

    ENTER(EhsL_event_key_release);
    /* @todo we need a EhsGUIKbRelease() also
    EhsGUIKbHit(pEvent->keyval); // call all the prescribing function blocks
    	if(pEvent->keyval == 27) //'ESCAPE' releases the keyboard from this window
    	{
    		gdk_keyboard_ungrab(GDK_CURRENT_TIME);
    		//@todo It is depriacted to control video from the graphics subsystem: EhsTV_setPosition(0.5);

    	}
    	else if(pEvent->keyval == 103) //'g' grabs it - but of course it is already !!!
    	{
    		gdk_keyboard_grab(pWidget->window, EHS_FALSE, GDK_CURRENT_TIME); // 'f' grabs the keybaord for this window
    	}
    	*/
    LEAVE(EhsL_event_key_release);
    return TRUE;
}

/**
 * Handle Target Specific key press events - just for focus grabbing internally
 * @param[in] pWidget The Widget that this event applies to
 * @param[in] pEvent The event that triggered this function
 */
gboolean EhsL_event_key_press(GtkWidget* pWidget, GdkEventKey* pEvent)
{
    ENTER(EhsL_event_key_press);
    EhsGUIKbHit(pEvent->keyval); // call all the prescribing function blocks
#define EHS_TARGETVIEWPORT_DIRECT_CONTROL
#ifdef EHS_TARGETVIEWPORT_DIRECT_CONTROL
    if((pEvent->keyval == 65307) /*esc */) // || (pEvent->keyval ==  65507/*ctrl*/) || (pEvent->keyval ==65481/*F10*/ )) //'F12' or Ctrl releases the keyboard from this window
    {
        gdk_keyboard_ungrab(GDK_CURRENT_TIME);
        viewport_grab=EHS_FALSE;
        //@todo It is depriacted to control video from the graphics subsystem: EhsTV_setPosition(0.5);
        EHSH_LOG_INFO ("Ungrabbing the Keyboard focus <F9> to regrab");
    }
    else if(pEvent->keyval == 65478 /*F9*/) // grabs it - but of course it is already !!!
    {
        /* - disabled
        if (pWidget->window) gdk_keyboard_grab(pWidget->window, EHS_FALSE, GDK_CURRENT_TIME); // 'f' grabs the keybaord for this window
        EHSH_LOG_INFO ("Grabbing the Keyboard focus <esc> to ungrab");
        viewport_grab=EHS_TRUE;
        */
    }
    else if(pEvent->keyval == 65479/*F10*/)   //bring window to front
    {
        EHSH_LOG_INFO("Bringing Window to the front");
        EhsTV_MaximiseViewport(pWidget) ;
        EhsTV_showViewport(2);
    }
    else if(pEvent->keyval == 65480/*F11*/)   // bring window to back
    {

        EhsTV_showViewport(0);
        EHSH_LOG_INFO ("Sending Window to the back");
    }
    else if(pEvent->keyval == 65481/*F1*/)   // Hide Window
    {
        EhsTV_MinimiseViewport (pWidget);
        EHSH_LOG_INFO ("Hiding Window ");
    }
    else if(pEvent->keyval == 65481/*F1*/)   // Hide Window
    {
        EhsTV_MinimiseViewport (pWidget);
        EHSH_LOG_INFO ("Hiding Window ");
    }
//		else if(pEvent->keyval == 104 /*h*/) {
//			if (pEvent->state & GDK_CONTROL_MASK) { /*ctrl-h load home app*/
//			}
//		}
//		else if(pEvent->keyval == 65473/*F4*/) { // load home app
    else if(pEvent->keyval == 65477/*F8*/ || pEvent->keyval == 36/*$*/)   // load home app
    {
        if (pEvent->state & GDK_CONTROL_MASK)
        {

        }
        else
        {
            //@todo - change to use console to a receive request from GTK to load home app, GTK shouldn't be directly calling core EHS stuff
            EhsHMetaSetNextAppToRun(EHS_SYS_APP_DEFAULT_NAME);
            EhsHFSMForceInternallyRequestedCommand(EHS_RELOAD_EHS_FROM_FILE);
        }
    }
#endif
    LEAVE(EhsL_event_key_press);
    return TRUE;


}

/**
 * Callback function for handling mouse button press events from the mouse.
 */
gboolean EhsL_event_button_press(GtkWidget* pWidget, GdkEventButton* pButton)
{
    int x, y;
    gtk_widget_get_pointer(pWidget, &x, &y);

    EhsTPMutex_lock(EhsTPMutex_widgetTable);
    EhsWidgetTable_triggerViewportMouseDown(&EhsWidgetTable,x,y);
    EhsWidgetTable_registerMouseDownOnWidgetMatchCoords(&EhsWidgetTable,x,y);
    EhsTPMutex_unlock(EhsTPMutex_widgetTable);


}

/**
 * Callback function for handling mouse button release events from the mouse.
 */
gboolean EhsL_event_button_release(GtkWidget* pWidget, GdkEventButton* pButton)
{

    int x, y;
    gtk_widget_get_pointer(pWidget, &x, &y);

    EhsTPMutex_lock(EhsTPMutex_widgetTable);
    EhsWidgetTable_triggerViewportMouseUp(&EhsWidgetTable,x,y);
    EhsWidgetTable_registerMouseUpOnWidgetMatchCoords(&EhsWidgetTable,x,y);
    EhsTPMutex_unlock(EhsTPMutex_widgetTable);


}

/**
 * Callback function for handling mouse button hold down and move events from the mouse.
 */
gboolean EhsL_event_motion_notify(GtkWidget* pWidget, GdkEventMotion* pMotion)
{
    int x, y;
    gtk_widget_get_pointer(pWidget, &x, &y);
    EhsTPMutex_lock(EhsTPMutex_widgetTable);
    EhsWidgetTable_triggerViewportMouseDrag(&EhsWidgetTable,x,y);
    EhsTPMutex_unlock(EhsTPMutex_widgetTable);

//	update = 1; //not necessary as update is done on timer

}


/* It seems most of the following should be in the common code area

/**
 *  Reset the use of the target viewport. This function
 * releases resources occupied by the viewport.
 */
void EhsTV_reset(EhsTVClass* pViewport)
{
    ENTER(EhsTV_reset);
    EhsTPMutex_lock(EhsTPMutex_widgetTable);
    EhsGtkTimerActive=EHS_FALSE;
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
    EhsGtkTimerActive=EHS_TRUE;
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
        gtk_window_set_decorated(GTK_WINDOW(pViewport->pMainWindow),TRUE);
    }
    else
    {
        gtk_window_set_decorated(GTK_WINDOW(pViewport->pMainWindow),FALSE);
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

// 	EhsTV_reset(pViewport);
#ifndef EHS_MINGW
    gdk_threads_enter();  //PPP:  out these back
#endif
    /* release GTK objects - we need to have stopped rendering at this point (removed the call backs and destroyed all the widgets)*/
    g_object_unref(EhsTV.pMainWindow);
#ifndef EHS_MINGW
    gdk_threads_leave();
#endif
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
#ifdef EHS_MINGW
    horrible_flag_to_prompt_lateconfig_on_new_windows=1; // need to trigger expose to reconfig the window with new fade value
#endif
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
    int cairo_stride = 0;
    EhsGraphicsColourClass* pPixels;
    cairo_pattern_t *pattern;
    cairo_matrix_t   matrix;
    cairo_surface_t* tempSurface;
    ehs_uint8 nPixAlpha, nVal;

    ENTER(EhsTV_blit);


    /* calculate the parts of the image that we need to update */
    if (EhsGraphicsRectangle_intersect(&blitBounds,pDst,&intersectViewClip))
    {
        dScaleX = (double)(pDst->nWidth) / (double)(pSrc->nWidth);
        dScaleY = (double)(pDst->nHeight) / (double)(pSrc->nHeight);
        dOffsetX = (ehs_float)(pDst->nLeft - blitBounds.nLeft) * dScaleX;
        dOffsetY = (ehs_float)(pDst->nTop - blitBounds.nTop) * dScaleY;

        switch (pImgData->eFormat)
        {
        case EHS_GRAPHICS_COLOUR_ARGB8888:
            cairo_translate(EhsTV.cr, pDst->nLeft, pDst->nTop);
            cairo_rectangle(EhsTV.cr, (blitBounds.nLeft - pDst->nLeft), (blitBounds.nTop - pDst->nTop), blitBounds.nWidth, blitBounds.nHeight);

            cairo_clip(EhsTV.cr);
            cairo_new_path (EhsTV.cr);
            cairo_scale(EhsTV.cr, dScaleX, dScaleY);

            cairo_set_source_surface(EhsTV.cr, pImgData->fmt.Cargb8888, 0, 0);
//#define EHS_EST_RENDERING
#ifdef EHS_TEST_RENDERING
            cairo_set_source_rgb(EhsTV.cr, 0.6, 0.9, 0.6);
            cairo_set_line_width(EhsTV.cr, 1);

            cairo_rectangle(EhsTV.cr, 20, 20, 120, 80);
            cairo_rectangle(EhsTV.cr, 180, 20, 80, 80);
            cairo_stroke_preserve(EhsTV.cr);
            cairo_fill(EhsTV.cr);
#endif
            cairo_paint_with_alpha(EhsTV.cr, ((double)nAlpha/255));
            cairo_reset_clip(EhsTV.cr);

            cairo_matrix_init_identity(&matrix);
            cairo_set_matrix(EhsTV.cr, &matrix);
            break;

        case EHS_GRAPHICS_COLOUR_A1:
            /* I can't see another way of using the cairo A1buffer we rendered test into and then blitting this with individual colours
             * and tranpsarency for foreground and background: So create a full 32-bit per pixel image from the A1 image without the help of cairo.
             * Each pixel gets
             * transferred into the larger format, then composited, then the image is thrown away.
             * A faster (but less memory efficient) implementation would allocate the pixbuf during
             * the text writing function.
             * todo the above approach has been done for OpenGL - this should be migrated here too?
             */

            /* Calculate the offset to jump to the next row from the last pixel. its on apImgData->fmt.A1.nWidth byte boundary */
            if (pImgData->fmt.A1.nWidth%8 != 0)
            {
                nOffset = 8 - (pImgData->fmt.A1.nWidth%8);
            }
            else
            {
                nOffset = 0;
            }

            pBitmap = pImgData->fmt.A1.pBitmap;

            cairo_stride = cairo_format_stride_for_width (CAIRO_FORMAT_ARGB32,pImgData->fmt.A1.nWidth);
            pPixels = (EhsGraphicsColourClass*) EhsTMem_alloc((cairo_stride * pImgData->fmt.A1.nHeight));

            tempSurface = cairo_image_surface_create_for_data((unsigned char *)pPixels,CAIRO_FORMAT_ARGB32, pImgData->fmt.A1.nWidth, pImgData->fmt.A1.nHeight,cairo_stride);

            for (i = 0; i < (((pImgData->fmt.A1.nWidth+nOffset)*pImgData->fmt.A1.nHeight)-nOffset); i++)
            {
                /* SDG: Calculate the pixel number taking into account the offset */
                nRowNum = nPixelNum/pImgData->fmt.A1.nWidth;
                if (nRowNum*nOffset > i) nPixelNum=0; /*todo this is abit of a guess to avoid a crash */
                else nPixelNum = i - (nRowNum*nOffset);

                ehs_uint8 nBit = (1u << (7u - (i % 8))); //Create a mask for the appropriate bit

                if ((pBitmap[i/8] & nBit) == nBit)
                {
                    pPixels[nPixelNum] = pImgData->fmt.A1.pColour[1];
                }
                else
                {
                    pPixels[nPixelNum] = pImgData->fmt.A1.pColour[0];
                }
                /*  Premultiply alphas etc prior to per-pixel alphablend
                 */
                nPixAlpha = pPixels[nPixelNum].sComp.nAlpha;

                nVal = pPixels[nPixelNum].sComp.nRed;
                nVal = (nVal * nPixAlpha) / 255;
                pPixels[nPixelNum].sComp.nRed = nVal;

                nVal = pPixels[nPixelNum].sComp.nGreen;
                nVal = (nVal * nPixAlpha) / 255;
                pPixels[nPixelNum].sComp.nGreen = nVal;

                nVal = pPixels[nPixelNum].sComp.nBlue;
                nVal = (nVal * nPixAlpha) / 255;
                pPixels[nPixelNum].sComp.nBlue = nVal;
            }
            cairo_translate(EhsTV.cr, pDst->nLeft, pDst->nTop);
            cairo_rectangle(EhsTV.cr, (blitBounds.nLeft - pDst->nLeft), (blitBounds.nTop - pDst->nTop), blitBounds.nWidth, blitBounds.nHeight);
            cairo_clip(EhsTV.cr);
            cairo_new_path (EhsTV.cr);
            cairo_scale(EhsTV.cr, dScaleX, dScaleY);
            cairo_set_source_surface(EhsTV.cr, tempSurface, 0, 0);
            cairo_paint_with_alpha((EhsTV.cr),((double)nAlpha/255));
            cairo_reset_clip(EhsTV.cr);
            cairo_matrix_init_identity(&matrix);
            cairo_set_matrix(EhsTV.cr, &matrix);

            //cairo_surface_finish(tempSurface); - only needed if we want to release buffers
            cairo_surface_destroy(tempSurface);
            EhsTMem_free(pPixels);
            break;
        }
    }
    LEAVE(EhsTV_blit);
}
#ifdef BLIT
void EhsTV_blit(EhsTVClass* pViewport, const EhsTVSurfaceClass* pImgData, const EhsGraphicsRectangleClass* pDst, const EhsGraphicsRectangleClass* pSrc, ehs_uint8 nAlpha)
{
}
#endif /*BLIT*/
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
        cairo_set_source_rgba(EhsTV.cr, ((double)pColour->sComp.nRed / 255), ((double)pColour->sComp.nGreen / 255), ((double)pColour->sComp.nBlue / 255), ((double)pColour->sComp.nAlpha / 255));
        cairo_rectangle(EhsTV.cr, blitBounds.nLeft, blitBounds.nTop, blitBounds.nWidth, blitBounds.nHeight);
        cairo_fill(EhsTV.cr);
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
    ehs_uint8 * ourbuffer = NULL;
    if (pSurface)
    {
        EhsTVSurfaceClass* pPrev; /* points to the surface that points to this one */

        /*Cairo should now handle freeing the memory for destroyed surfaces*/
        switch (pSurface->eFormat)
        {
        case EHS_GRAPHICS_COLOUR_ARGB8888:
            if (pSurface->fmt.Cargb8888)
            {

                ourbuffer = cairo_image_surface_get_data(pSurface->fmt.Cargb8888);
                cairo_surface_destroy(pSurface->fmt.Cargb8888);
                EhsHMem_tempFree(ourbuffer); /* Free this after destroying cairo bufer object */
                pSurface->fmt.Cargb8888=NULL; //@todo not needed the container is about to be destroyed
            }
            break;

        case EHS_GRAPHICS_COLOUR_A1:
            if (pSurface->fmt.A1.surBitmap)
            {

                ourbuffer = cairo_image_surface_get_data(pSurface->fmt.A1.surBitmap);
                cairo_surface_destroy(pSurface->fmt.A1.surBitmap);
                EhsHMem_tempFree(ourbuffer); /* Seems not all resources are cairo's - need to free this before */
                pSurface->fmt.A1.surBitmap=NULL; //@todo not needed the container is about to be destroyed
            }
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
    int cairo_stride =0;
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
                //pSurface->fmt.Cargb8888 = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, nWidth, nHeight);
#ifdef EHS_MANUAL_ALIGNMENT
#error This is unsafe
                pSurface->fmt.Cargb8888 = cairo_image_surface_create_for_data (EhsHMem_tempAlloc(nWidth*nHeight*4), CAIRO_FORMAT_ARGB32, nWidth, nHeight, stride_value(nWidth,32));
                /* alloc failed, reset pSurface */
#else
                cairo_stride = cairo_format_stride_for_width (CAIRO_FORMAT_ARGB32,nWidth);
                //pSurface->row_stride = cairo_stride;
                pSurface->fmt.Cargb8888 = cairo_image_surface_create_for_data (EhsHMem_tempAlloc(cairo_stride*nHeight*4), CAIRO_FORMAT_ARGB32, nWidth, nHeight, cairo_stride);
#endif
                if (!pSurface->fmt.Cargb8888)
                {
                    bFailed = EHS_TRUE;
                }
                break;
            case EHS_GRAPHICS_COLOUR_A1:
                if (nPaletteSize >= 2u)
                {
                    //pSurface->fmt.A1.surBitmap = cairo_image_surface_create(CAIRO_FORMAT_A1, nWidth, nHeight);
                    cairo_stride = cairo_format_stride_for_width (CAIRO_FORMAT_A1,nWidth);
                    pSurface->fmt.A1.surBitmap = cairo_image_surface_create_for_data(EhsHMem_tempAlloc(cairo_stride*nHeight), CAIRO_FORMAT_A1, nWidth, nHeight, cairo_stride);
                    /* todo - do we need to ue cairo atall when creating a pixel buffer? Seems to be used to create a cairo firiendly strided buffer
                     * but no sign of surBitmap being used anywhere other than just to get his handle back...
                    /* CAIRO_FORMAT_A1 is bitpacked */
                    pSurface->fmt.A1.pBitmap = cairo_image_surface_get_data(pSurface->fmt.A1.surBitmap);
                    for (i=0; i <(cairo_stride*nHeight); i++)
                    {
                        pSurface->fmt.A1.pBitmap[i] = 0x0;
                    }
                    if (pSurface->fmt.A1.pBitmap)
                    {
                        pSurface->fmt.A1.nWidth = nWidth;
                        pSurface->fmt.A1.nHeight = nHeight;
                        EhsMemcpy(pSurface->fmt.A1.pColour, pPalette, 2 * sizeof(EhsGraphicsColourClass));
                    }
                    else
                    {
                        /* alloc failed, reset pSurface */
                        bFailed = EHS_TRUE;
                    }
                }
                else
                {
                    /* invalid parameters - palette too small */
                    bFailed = EHS_TRUE;
                }
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
        EhsError(EHS_MSG_TGT_GRAPHICS_UNSUPPORTED_MODE("unrecognised bitmap format"));
    }

    LEAVE(EhsTVSurface_create);
    return pSurface;
}

/* nothing to do for pixel buffer blitting */
void EhsTVSurface_finalisePixels (EhsTVSurfaceClass* pSurface)
{

}


/**
 * Provide access to pixels representing the surface
 */
void* EhsTVSurface_pixels(EhsTVSurfaceClass* pSurface)
{
    void* pRet = NULL;
    switch (pSurface->eFormat)
    {
    case EHS_GRAPHICS_COLOUR_ARGB8888:
//		gdk_threads_enter();
        //pRet = ((EhsGraphicsColourClass*)(gdk_pixbuf_get_pixels((pSurface)->fmt.Argb8888)));
        pRet = (EhsGraphicsColourClass*)(cairo_image_surface_get_data (pSurface->fmt.Cargb8888));
//		gdk_threads_leave();
        break;
    case EHS_GRAPHICS_COLOUR_A1:
        pRet = pSurface->fmt.A1.pBitmap;
    }

    return pRet;
}

/**
 * Provide access to the bitmap representing the surface
 * @todo this should be merged with the above pixels function.
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
        //gdk_threads_enter();
        //nRet = (gdk_pixbuf_get_rowstride((pSurface)->fmt.Argb8888)); /* This returns bytes*/
        nRet = cairo_image_surface_get_stride (pSurface->fmt.Cargb8888);
        //gdk_threads_leave();
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
        //gdk_threads_enter();
        //nRet = (gdk_pixbuf_get_width((pSurface)->fmt.Argb8888));
        nRet = cairo_image_surface_get_width (pSurface->fmt.Cargb8888);
        //gdk_threads_leave();
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
        //gdk_threads_enter();
        //nRet = (gdk_pixbuf_get_height((pSurface)->fmt.Argb8888));
        nRet = cairo_image_surface_get_height (pSurface->fmt.Cargb8888);
        //gdk_threads_leave();
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

/** @brief Returns the full screen size and populates pWidget as well as setting the input arguments */

ehs_bool EhsTV_getScreenSize(ehs_sint32 *nScreenWidth, ehs_sint32 *nScreenHeight)
{

    GdkScreen * screen = gtk_window_get_screen(GTK_WINDOW(EhsTV.pMainWindow));
    *nScreenWidth = gdk_screen_get_width(screen); // in pixels
    *nScreenHeight = gdk_screen_get_height(screen);
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


/** @brief This uniconifies to normalise - not sull screen */
void EhsTV_MaximiseViewport(GtkWidget* pWidget)
{
    gtk_window_deiconify (GTK_WINDOW(pWidget));
}

/** @brief minimizes to an icon */
void EhsTV_MinimiseViewport (GtkWidget* pWidget)
{
    gtk_window_iconify (GTK_WINDOW(pWidget));
}

