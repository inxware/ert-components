/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file widget.h
 * Declarations for the widget functions.
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section widget_h
 * @anchor widget_h
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on 2007-10-12
 * Last modified on $Date$
 *
 * This file contained the following derogations to the MISRA standard.
 * - Rule 18.4 Unions shall not be used. Acceptable derogation to implement variant records provided
 * common field differentiates variant.
 * - Rule 19.1 Only preprocessor statements and comments before #include. Derogation necessary to allow declaration
 * of a type that is defined in this file.
 */

#ifndef EHS_WIDGET_H
#define EHS_WIDGET_H

#ifndef xEHS_GUI_SUPPORT

/*****************************************************************************/
/* Included files */

/**
 * Pre-use declaration of the Widget class
 */
typedef struct EhsWidgetStruct EhsWidgetClass; /*lint !e961 Only preprocessor statements and comments before #include.
											    Derogation necessary to allow declaration of a type that is defined in this file. */

#include "globals.h"
#include "hal_viewport.h"
#include "graphics.h"
#include "widget_textbox.h"
#include "widget_image.h"
#include "widget_patch.h"
#include "widget_viewport.h"
#include "widget_ui.h"
//#include "widget_video_port.h"
#include "ehs_fb_types.h"

#if defined(EHS_GUI_SUPPORT_MODE_B_QT)
#include "ertqt.h"
#endif

/*****************************************************************************/
/* Define macros  @ todo put a description of the semantics of this bit array*/
#define EHS_WIDGET_STATE_EMPTY		0x00 /**< No image is ready */
#define EHS_WIDGET_STATE_INIT		0x80 /**< Image has been initialised */

/**
 * Init value occurs if top bit is set, second bit isn't set. This means
 * we don't fall foul of Windows uninitialized value 0xcccc
 */
#define EHS_WIDGET_STATE_INIT_MASK	0xc0 /** @todo Used for what ??*/
#define EHS_WIDGET_STATE_CREATED	0x01 /**< Image has been created */
#define EHS_WIDGET_STATE_SHOW		0x02 /**< Image has been shown */

/** test if the widget has been initialized */
#define EHS_WIDGET_STATE_INITIALIZED(x) (((x) & EHS_WIDGET_STATE_INIT) == EHS_WIDGET_STATE_INIT)

/** test if the widget is being shown */
/* @todo is this correct that init and created state widgets should be counted as shown? It might be to pre-empt them being shown if this needs to be prevented?*/
#define EHS_WIDGET_STATE_SHOWN(x) (((x) & (EHS_WIDGET_STATE_INIT | EHS_WIDGET_STATE_CREATED | EHS_WIDGET_STATE_SHOW)) \
	== (EHS_WIDGET_STATE_INIT | EHS_WIDGET_STATE_CREATED | EHS_WIDGET_STATE_SHOW))

/*****************************************************************************/
/* Define types */
/**
 * Type of widget defined in the widget structure
 */
typedef enum { EHS_WIDGET_KIND_IMAGE,
               EHS_WIDGET_KIND_TEXTBOX,
               EHS_WIDGET_KIND_PATCH,
               EHS_WIDGET_KIND_VIEWPORT,
               EHS_WIDGET_KIND_VIDEO_PORT,
               EHS_WIDGET_KIND_UI
             } EhsWidgetKindEnum;

/**
 * Type of widget defined in the widget structure
 * todo 2023 - this doesn't seem to be used anywhere
 */
typedef enum { ARGB8888, ARGB1888, RGB888 } EhsBlitMethodEnum;

/* Some call-back functions */

typedef ehs_bool (*fadeFunc_t) (EhsWidgetClass* pWidget, ehs_uint8 nOpacity)  ;


/**
 * Generic widget structure. This represents the superclass of all widget types.
 * EhsWidgetImageType "inherit" and EhsWidgetTextboxType "inherit" from this class.
 * In C terms, this means that functions that don't know what type of widget they're
 * dealing with will simply call EhsWidget_ functions, and it will sort out what
 * specific functions to call. Currently this only applies to the draw function.
 *
 * As we will see in the definition of EhsWidget_Image_ and EhsWidget_Textbox_ functions,
 * they take EhsWidgetClass pointers as parameters, not EhsWidget_ImageType or EhsWidget_TextboxType
 */
struct EhsWidgetStruct
{
    /* ── Geometry rectangles (shared: Mode A and Mode B) ───────────────── */

    EhsGraphicsRectangleClass xDesignRect;	/**< Widget size as specified at design time by the LGB tool,
                                                 based on original image/media dimensions. Used as a
                                                 reference when scaling or repositioning. (Shared) */
    EhsGraphicsRectangleClass xOrigRect;	/**< Initial bounding rectangle from LAB's properties file.
                                                 Used to distinguish relative sizes from viewport-absolute
                                                 sizes. Usually identical to xDesignRect unless the widget
                                                 uses relative coordinates. (Shared) */
    EhsGraphicsRectangleClass xCurRect;	    /**< Current bounding rectangle - the live position and size
                                                 of the widget after any runtime moves or resizes. This is
                                                 what the draw/render path uses. (Shared) */
    EhsGraphicsRectangleClass UpdatedOffsettRect;	/**< Last applied position offset. Cached so it can be
                                                         re-applied when new media is loaded into the widget
                                                         (e.g. image swap). (Shared) */
#if defined(EHS_GUI_SUPPORT_MODE_B) 
    EhsGraphicsSizeClass MediaRect;	/**< Pixel dimensions (width, height) of the original media
                                         content. Mode B only stores size (no x,y) because the
                                         external library owns positioning. (Mode B / Mode B Qt) */
#else
    EhsGraphicsRectangleClass MediaRect; /**< Full rectangle (x, y, width, height) of the original
                                              media. Mode A needs position for coordinate-based
                                              hit-testing and blitting. (Mode A only) */
#endif

    /* ── Z-order (shared) ──────────────────────────────────────────────── */

    ehs_uint16 nZ;                          /**< Z-order value controlling draw/overlap priority.
                                                 Higher values are drawn on top. In Mode A this
                                                 determines hit-test priority for mouse events.
                                                 In Mode B the external library may also use its own
                                                 z-ordering, but this value is still tracked. (Shared) */

    /* ── Virtual method table (shared) ─────────────────────────────────── */

    ehs_bool (*pfFadeFunc) (EhsWidgetClass* pWidget, ehs_uint8 nOpacity);	/**< Virtual method: fade/opacity change.
                                                                                 Mode A: manipulates pixel alpha.
                                                                                 Mode B: pushes opacity to external library. (Shared) */
    ehs_bool (*pfCreateFunc)(EhsWidgetClass* pWidget); /**< Virtual method: widget creation/initialisation.
                                                            Called once when the widget is first instantiated.
                                                            Mode A: allocates pixel buffers, loads images.
                                                            Mode B: creates library widget (LVGL obj / Qt QObject). (Shared) */
    void (*pfDestroyFunc)(EhsWidgetClass* pWidget); /**< Virtual method: widget teardown/cleanup.
                                                         Mode A: frees pixel buffers.
                                                         Mode B: destroys the external library widget. (Shared) */
    void (*pfDrawFunc)(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass *pClipRect); /**< Virtual method: draw/render.
                                                         Mode A: blits pixels to the viewport framebuffer.
                                                         Mode B: pushes changed properties (position, text,
                                                         colour) to the external library using the
                                                         bContentUpdated/bPositionUpdated/bColourUpdated flags. (Shared) */

    /* ── Widget type discriminator (shared) ────────────────────────────── */

    EhsWidgetKindEnum eWidgetKind; /**< Discriminator for the specificWidgetType union below.
                                        Values: image, textbox, patch, ui, etc. (Shared) */

    /* ── Mode A mouse event handling ───────────────────────────────────── */

#if defined(EHS_GUI_SUPPORT_MODE_A) 
    /* Mode A only: eRT owns pixel buffers and performs coordinate-based
     * hit-testing. These fields track mouse state per widget. In Mode B the
     * external library (LVGL/Qt) handles hit-testing and fires events via
     * the event_callback in EhsWidgetUiSubclass instead. */
    void (*pfMouseDownEventFunc)(EhsWidgetClass* pWidget); /**< Callback for mouse-down on widgets with no
                                                                pFIData (e.g. GPIO widget). (Mode A only) */
    void* pMouseDownEventData;  /**< Opaque data pointer passed to pfMouseDownEventFunc. (Mode A only) */
    ehs_uint32 nMouseDownX;     /**< X coordinate of last mouse-down event, used for drag
                                     offset calculation. (Mode A only) */
    ehs_uint32 nMouseDownY;     /**< Y coordinate of last mouse-down event. (Mode A only) */

//	EhsBlitMethodEnum eBlitMethod; /**< Blit method used to draw widget */
#endif

    /* ── Function instance back-pointer (shared) ───────────────────────── */

    EhsFunctionInstanceDataType* pFIData; /**< Back-pointer to the owning function block instance.
                                               Allows the graphics/event layer to fire EHS finish ports
                                               when user interaction occurs (clicks, drags, etc.).
                                               NULL for widgets not owned by a function block (e.g. GPIO).
                                               (Shared - used by both Mode A and Mode B) */

    /* ── Type-specific subclass union (discriminated by eWidgetKind) ──── */

    /*lint -e960 18.4 Unions shall not be used. Acceptable derogation to use variants - eWidgetKind shows which union member to use */
    union
    {
#if defined(EHS_GUI_SUPPORT_MODE_A) 
        /* Mode A only: eRT manages image/textbox rendering directly.
         * In Mode B the external library owns these widget types and
         * they are represented by EhsWidgetUiSubclass instead. */
        EhsWidgetImageSubclass image;		/**< Image-specific attributes (Mode A only) */
        EhsWidgetTextboxSubclass textbox; 	/**< Textbox-specific attributes (Mode A only) */
#endif
        EhsWidgetPatchSubclass patch; 		/**< 9-patch specific attributes (Shared) */
        EhsWidgetUiSubclass ui;             /**< UI widget attributes for Mode B.
                                                 Contains event_callback for receiving
                                                 click/press/release events from the
                                                 external library. (Mode B / Mode B Qt) */
        //EhsWidgetVideoPortSubclass video_port; /*Video port specific attributes */
    } specificWidgetType;
    /*line +e960 */

    /* ── Appearance state (shared) ─────────────────────────────────────── */

    ehs_uint8 nAlpha; 						/**< Current alpha/opacity value (0-255). Tracked here so
                                                 the render path can detect changes. (Shared) */
    ehs_bool bMaintainAspectRatio;		    /**< When true, width changes auto-calculate height to
                                                 preserve the original aspect ratio. (Shared) */
    //ehs_bool bRelativeCoordinates;		    /** The widget's parameters and input coordinates are in % screen width and these are converted to absolute pixels when updated (but not the screen width)*/
    ehs_bool bContentChanged;	/**< Set when text/image content changes. Allows renderers
                                     (e.g. OpenGL texture-based text) to skip re-blitting when
                                     only position changed. (Shared, primarily Mode A) */

    /* ── Mode B dirty flags ────────────────────────────────────────────── */

    /*************************************************************************************************************************************/
    /* MODE B widget rendering changed flags - used to pass new position,
     * metadata and colour info to the external widget library. Tested by
     * pfDrawFunc (target implementation) to minimise unnecessary updates.
     * Typically set by a function block's update port handler. */
    ehs_bool bContentUpdated ;    /**< Content changed (e.g. new text string). (Mode B) */
    ehs_bool bPositionUpdated;   /**< Position or size changed. (Mode B) */
    ehs_bool bColourUpdated;     /**< Colour or alpha changed. (Mode B) */
    /************************************************************************************************************************************/

    /* ── Widget lifecycle state (shared) ───────────────────────────────── */

    ehs_uint8 nState; /**< Widget lifecycle state, defined by EHS_WIDGET_STATE_ macros
                           (e.g. CREATED, VISIBLE, HIDDEN). (Shared) */

    /* ── Mode A per-widget port numbers for mouse events ───────────────── */

#if defined(EHS_GUI_SUPPORT_MODE_A) 
    /* Mode A only: In render Mode A, eRT owns the framebuffer and there is
     * no external widget library to define widget GFX. The HAL receives raw
     * mouse/touch coordinates from the OS and must perform its own
     * coordinate-based hit-testing against widget bounding rectangles
     * (xCurRect) and z-order (nZ) to determine which widget was touched.
     *
     * Once hit-testing identifies the target widget, the HAL needs to know
     * which EHS kernel finish port to fire for each type of mouse event.
     * These per-widget port numbers provide that mapping — they are set
     * during widget creation from the CDF/function-block definition and
     * allow the HAL to directly fire the correct port without needing to
     * understand the function block's internal wiring.
     *
     * In render Mode B (LVGL, Qt), none of this is needed because the
     * external widget library owns hit-testing — it knows which of its
     * own widgets was clicked. Events arrive via the event_callback
     * function pointer in EhsWidgetUiSubclass, which carries a generic
     * event ID (e.g. EHS_WIDGET_UI_EVENT_MOUSE_CLICKED). The Mode B
     * event handler then fires the appropriate finish port using the
     * function block's own port definitions, rather than stored port
     * numbers on the widget struct. This is a cleaner separation of
     * concerns: the widget struct doesn't need to know about port wiring. */
    //@todo the following should be removed when a call back function (with a known port number) is used to fire the port
    ehs_sint8 mouseClickPortNumber;         /**< Finish port index fired on mouse click. (Mode A only) */
    ehs_sint8 mouseDownPortNumber;          /**< Finish port index fired on mouse down. (Mode A only) */
    ehs_sint8 mouseUpPortNumber;            /**< Finish port index fired on mouse up. (Mode A only) */
    ehs_sint8 mouseDragPortNumber;          /**< Finish port index fired on mouse drag. (Mode A only) */
    ehs_sint8 mouseUpDownAbsXPortNumber;    /**< Finish port for absolute X on mouse up/down. (Mode A only) */
    ehs_sint8 mouseUpDownAbsYPortNumber;    /**< Finish port for absolute Y on mouse up/down. (Mode A only) */
    ehs_sint8 mouseDragOffsetXPortNumber;   /**< Finish port for drag offset X delta. (Mode A only) */
    ehs_sint8 mouseDragOffsetYPortNumber;   /**< Finish port for drag offset Y delta. (Mode A only) */
    ehs_bool bRegisteredMouseDown;		/**< True while a mouse-down is active on this widget,
                                             used to pair down/up events correctly. (Mode A only) */
    ehs_bool bOptimiseForSpeed;		/**< Hint: prefer speed over memory for this widget's
                                         rendering (e.g. cache decoded images). (Mode A only) */
#endif

    /* ── Qt-specific handle (Mode B Qt only) ───────────────────────────── */

#ifdef EHS_GUI_SUPPORT_MODE_B_QT
    ertqt_object_handle qt_handle;  /**< Opaque handle associating this EHS widget with a Qt
                                         QObject in the QML scene. Used by the Qt HAL to look up
                                         the QObject for property changes and signal binding.
                                         (Mode B Qt only) */
#endif

    /* ── Click capture override (shared) ───────────────────────────────── */

    ehs_bool bCaptureClicksIgnoringZOrder;		/**< When true, this widget captures click events
                                                     regardless of z-order overlap. Used for widgets
                                                     that must always respond to input (e.g. modal
                                                     overlays). (Shared) */
}
#ifdef EHS_OPTIMIZE_WIDGET_MEM
__attribute__((packed))
#endif
;

/**
 * Declare a table of widgets
 */
typedef struct
{
    ehs_uint16 nSize; /**< Number of widgets currently defined */
    ehs_uint16 initialised;
    EhsWidgetClass xWidget[EHS_MAX_WIDGET_INSTANCES]; /**< Array of widgets put in creation order */
    EhsWidgetClass* pZOrderedWidget[EHS_MAX_WIDGET_INSTANCES]; /**< Array of pointers to widgets in Z-order */
} EhsWidgetTableClass;

/*****************************************************************************/
/* Declare global variables */

/**
 * Declare the global widget table
 */
EHS_GLOBAL EhsWidgetTableClass EhsWidgetTable;

/*****************************************************************************/
/* Declare function prototypes  */

/**
 * Obtain a new widget. The widget comes from EhsWidgetTable
 * and updates the number of allocated widgets.
 *
 * @return Pointer to a new widget, or zero if this fails
 */
EHS_GLOBAL EhsWidgetClass* EhsWidget_new(void);

/**
 * set generic widget parameters before anything else
 *
 */
void EhsWidget_init(EhsWidgetClass* pWidget, const EhsGraphicsRectangleClass *pRect, ehs_uint16 nZ, ehs_uint8 nAlpha);

/**
 * Create the widget. This is a necessary step prior to showing the widget
 */
EHS_GLOBAL void EhsWidget_create(EhsWidgetClass* pWidget);

/**
 * Destroy the widget. Required after the widget has been finished with
 */
EHS_GLOBAL void EhsWidget_destroy(EhsWidgetClass* pWidget);

/**
 * Cause the widget to be displayed (assuming it's already been created). If
 * it hasn't been created, there are no ill effects.
 */
EHS_GLOBAL void EhsWidget_show(EhsWidgetClass* pWidget);

/**
 * Cause the widget to be hidden (assuming it's already been created and shown).
 * No ill effects are caused if this is called before the object is created or shown.
 */
EHS_GLOBAL void EhsWidget_hide(EhsWidgetClass* pWidget);

/**
 * Update the widget X,Y parameters. Indicates whether
 *
 * @param[in,out] pWidget Widget data
 * @param[in] nX new relative X position to move widget to
 * @param[in] nY new relative Y position to move widget to
 * @param[in] nWid Change in width for the widget
 * @param[in] nHt change in height for the widget
 */
EHS_GLOBAL void EhsWidget_move(EhsWidgetClass* pWidget, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nWid, EhsDataflowIntType nHt);

/**
 * Fade the image in or out by scaling its global alpha level by the specified amount
 *
 * @param nOpacity amount of opacity for image. 0 = minimum opacity, 255 maximum opacity.
 */
EHS_GLOBAL void EhsWidget_fade(EhsWidgetClass* pWidget, ehs_uint8 nOpacity);

/**
 * Display the widget onto the viewport (if it's ready to be shown)
 *
 * @param pWidget Widget to display
 * @param pViewport Viewport to display it on
 * @param pClipRect Specifies the bounds for drawing the widget
 */
EHS_GLOBAL void EhsWidget_draw(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect);

/**
 * Initialise the widget table
 */
EHS_GLOBAL void EhsWidgetTable_init (EhsWidgetTableClass* pWidgetTable);
EHS_GLOBAL void EhsWidgetTable_reset(EhsWidgetTableClass* pWidgetTable);

/**
 * Obtain a new widget. The widget comes from EhsWidgetTable
 * and updates the number of allocated widgets.
 *
 * @return Pointer to a new widget, or zero if this fails
 */
EHS_GLOBAL EhsWidgetClass* EhsWidgetTable_new(EhsWidgetTableClass* pWidgetTable);

/**
 * Draw a widget list in response to a draw command from the target viewport
 *
 * @param pWidgetTable Set of widgets to redraw
 * @param pViewport Physical viewport to write to
 * @param pClipRect Specifies the bounds of what is drawn
 */
EHS_GLOBAL void EhsWidgetTable_draw(const EhsWidgetTableClass* pWidgetTable, EhsTVClass *pViewport, EhsGraphicsRectangleClass* pClipRect);

/**
 * All widgets from the widget table are marked for update.
 *
 * @param pWidgetTable Set of widgets to redraw
 */
EHS_GLOBAL void EhsWidgetTable_dirty(const EhsWidgetTableClass* pWidgetTable);

/**
 * Iterate through widget table and when find the viewport widget, fire its mouse down event
 * Assumes there is only one viewport
 */
EHS_GLOBAL void EhsWidgetTable_triggerViewportMouseDown(const EhsWidgetTableClass* pWidgetTable, int x, int y);

/**
 * Iterate through widget table and when find the viewport widget, fire its mouse up event
 * Assumes there is only one viewport
 */
EHS_GLOBAL void EhsWidgetTable_triggerViewportMouseUp(const EhsWidgetTableClass* pWidgetTable, int x, int y);

/**
 * Iterate through widget table and when find the viewport widget, fire its mouse drag event
 * Assumes there is only one viewport
 */
void EhsWidgetTable_triggerViewportMouseDrag(const EhsWidgetTableClass* pWidgetTable, int x, int y);

/**
 * Iterate through widget table and when find widgets that match coords, registers mouse down on widget
 */
EHS_GLOBAL void EhsWidgetTable_registerMouseDownOnWidgetMatchCoords(const EhsWidgetTableClass* pWidgetTable, int x, int y);

/**
 * Iterate through widget table and when find widgets that match coords, registers mouse up on widget
 * Triggers mouse click event on widget if mouse down was on same widget as mouse up
 */
EHS_GLOBAL void EhsWidgetTable_registerMouseUpOnWidgetMatchCoords(const EhsWidgetTableClass* pWidgetTable, int x, int y);

/**
 * Insert a widget itno the Z-ordered list of widgets. This is called
 * after a widget has been initialized, but before the index to the widget
 * table has been updated. This is called only by subclasses of EhsWidget.
 */
EHS_GLOBAL void EhsWidgetTable_updateZOrder(EhsWidgetTableClass* pWidgetTable, EhsWidgetClass* pWidget);
/**
 * Resize the widget to display the image at its original aspect ration by changing the widget's height
 *
 * @param[in] pWidget Specifies the widget to resize
 * @param[in] prSrc Specifies the proportions that we wish to keep
 */
EHS_GLOBAL void EhsWidget_resizeWidgetToMaintainAspectRatio(EhsWidgetClass* pWidget);//, const EhsGraphicsRectangleClass* prSrc);
/**
 * Reset the widget to its design time dimensions
 *
 * @param[in] pWidget Specifies the widget to resize
 */
EHS_GLOBAL void EhsWidget_resetWidgetSizeToDesignTime(EhsWidgetClass* pWidget);

EHS_GLOBAL void EhsWidget_AdjustCoordinates(EhsWidgetClass* pWidget, ehs_bool bRelative, ehs_sint32 nLeft, ehs_sint32 nWidth, ehs_sint32 nTop, ehs_sint32 nHeight );


/**
 * General purpose update function, contains the common code for all update functions.
 * Saves offset position and size values if they are needed for new media updated
 * Changes the current window position to the new position
 *
 * @param[in] pWidget Widget to update
 * @param[in] szString text to be displayed
 * @param[in] bAlphaConnected true if the alpha value should be used
 * @param[in] nAlpha current alpha level to use for image
 * @param[in] bXConnected true if the x coordinate should be used
 * @param[in] nXoffset current x coordinate to use (value is offset to default image position)
 * @param[in] bYConnected true if the y coordinate should be used
 * @param[in] nYoffset current y coordinate to use (value is offset to default image position)
 * @param[in] bWConnected true if the width value should be used
 * @param[in] nWoffset current width offset to use (value is offset to default image position)
 * @param[in] bHConnected true if the height offset should be used
 * @param[in] nHoffset current height offset to use (value is offset to default image position)
 *
 */
void Ehs_widget_position_update(EhsWidgetClass* pWidget, ehs_bool bAlphaConnected, EhsDataflowIntType nAlpha,
                                ehs_bool bXConnected, EhsDataflowIntType nXoffset,
                                ehs_bool bYConnected, EhsDataflowIntType nYoffset,
                                ehs_bool bWConnected, EhsDataflowIntType nWoffset,
                                ehs_bool bHConnected, EhsDataflowIntType nHoffset);

/**
 * Applies changes to the widget
 *
 * @param[in] pWidget Widget to update
 * */
void Ehs_widget_commit(EhsWidgetClass* pWidget);

#endif /* #ifdef EHS_GUI_SUPPORT */

#endif /* EHS_WIDGET_H */

