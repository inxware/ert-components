/** @file widget.h
 * Declarations for the widget functions.
 *
 * @author: inx limited
 * @version: $Revision: 3998 $
 * @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
 *
 * Copyright (c) inx limited, 2006. All rights reserved.
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

#include "hal_viewport.h"
#include "target_config.h"
#include "graphics.h"
#include "widget_textbox.h"
#include "widget_image.h"
#include "widget_patch.h"
#include "widget_viewport.h"
//#include "widget_video_port.h"
#include "ehs_fb_types.h"

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
typedef enum { EHS_WIDGET_KIND_IMAGE, EHS_WIDGET_KIND_TEXTBOX, EHS_WIDGET_KIND_PATCH, EHS_WIDGET_KIND_VIEWPORT, EHS_WIDGET_KIND_VIDEO_PORT } EhsWidgetKindEnum;

/**
 * Type of widget defined in the widget structure
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
struct EhsWidgetStruct {
	EhsGraphicsRectangleClass xDesignRect;	/**< widget size as specified at design time (i.e. by LGB-based on original image size*/
	EhsGraphicsRectangleClass xOrigRect;	/**< Initial bounding rectangle (as defined in LAB's properties file - Used to distinguish relative sizes and viewport - Otherwise the same as DesignRectangle*/
	EhsGraphicsRectangleClass xCurRect;	/**< current bounding rectangle for the widget */
	EhsGraphicsRectangleClass MediaRect;	/**< pixel dimensions of the original media */
	EhsGraphicsRectangleClass UpdatedOffsettRect;	/**< This is the last updated offset in case we need to re-apply it to new media. */
	ehs_uint8 nAlpha; 						/* Stored value of alpha - if changed */

	ehs_bool bMaintainAspectRatio;		/** Maintain the aspect ratio by only processing changes in width and setting height accordingly */
	ehs_bool bRelativeCoordinates;		/** The widget's parameters and input coordinates are in % screen width and these are converted to absolute pixels when updated (but not the screen width)*/
	ehs_uint16 nZ;  /**< Z order */
	ehs_bool bOptimiseForSpeed;		/**< Do we want this widget to be time-, or memory-efficient? */
	ehs_bool bContentChanged;	/* this flag is set if the content (text box only) is changed so that renderers such as text don't need to reblit such as in the case for OpenGL textures */
	EhsWidgetKindEnum eWidgetKind; /**< Type of graphic object contained within this widget */
//	EhsBlitMethodEnum eBlitMethod; /**< Blit method used to draw widget */
	/*lint -e960 18.4 Unions shall not be used. Acceptable derogation to use variants - eWidgetKind shows which union member to use */
	union {
		EhsWidgetImageSubclass image;		/**< Image specific attributes */
		EhsWidgetTextboxSubclass textbox; 	/**< textbox specific attributes */
		EhsWidgetPatchSubclass patch; 		/**< Patch specific attributes */
		//EhsWidgetVideoPortSubclass video_port; /*Video port specific attributes */
	} specificWidgetType;
	/*line +e960 */
	ehs_uint8 nState; /**< Widget state defined by EHS_WIDGET_STATE_ macros */
	void (*pfDrawFunc)(EhsWidgetClass* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass *pClipRect); /**< Implementation of method "draw" */
	ehs_bool (*pfCreateFunc)(EhsWidgetClass* pWidget); /**< Implementation of method "create" */
	void (*pfDestroyFunc)(EhsWidgetClass* pWidget); /**< Implementation of method "destroy" */
	ehs_bool (*pfFadeFunc) (EhsWidgetClass* pWidget, ehs_uint8 nOpacity);	/**< Implementation of method "fade" */

	EhsFunctionInstanceDataType* pFIData; /*Needed so gtk thread can fire mouse click events @todo this could be a call back structure*/
	//@todo the following should be removed when a call back function (with a known port number) is used to fire the port
	ehs_sint8 mouseClickPortNumber; /*Needed so gtk knows which finish port to fire*/
	ehs_sint8 mouseDownPortNumber; /*Needed so gtk knows which finish port to fire*/
	ehs_sint8 mouseUpPortNumber; /*Needed so gtk knows which finish port to fire*/
	ehs_sint8 mouseDragPortNumber; /*Needed so gtk knows which finish port to fire*/
	ehs_sint8 mouseUpDownAbsXPortNumber; /*Needed so gtk knows which finish port to fire*/
	ehs_sint8 mouseUpDownAbsYPortNumber; /*Needed so gtk knows which finish port to fire*/
	ehs_sint8 mouseDragOffsetXPortNumber; /*Needed so gtk knows which finish port to fire*/
	ehs_sint8 mouseDragOffsetYPortNumber; /*Needed so gtk knows which finish port to fire*/
	ehs_bool bCaptureClicksIgnoringZOrder;		/** capture clicks on this widget regardless of its zorder */
	ehs_bool bRegisteredMouseDown;		/** records a mouse down event in this widget */
	ehs_uint32 nMouseDownX;
	ehs_uint32 nMouseDownY;
};

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


#endif /* #ifdef EHS_GUI_SUPPORT */

#endif /* EHS_WIDGET_H */

