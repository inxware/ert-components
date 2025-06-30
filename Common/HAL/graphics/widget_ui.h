/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file widget_ui.h
 * Declarations for the ui widget functions. Intended to be included from widget.h. Don't include
 * this file directly.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_WIDGETUI_H
#define EHS_WIDGETUI_H

#include "ehs_types.h"
#include "globals.h"

/*
 * Event IDs
 */
#define EHS_WIDGET_UI_EVENT_MOUSE_DOWN                  0x0001
#define EHS_WIDGET_UI_EVENT_MOUSE_CLICKED               0x0002
#define EHS_WIDGET_UI_EVENT_DATA_UPDATED                0x0004
#define EHS_WIDGET_UI_EVENT_LABEL_UPDATED               0x0008
#define EHS_WIDGET_UI_EVENT_DATA_CHANGED                0x0010
#define EHS_WIDGET_UI_EVENT_LABEL_CHANGED               0x0020

/*
 * UI State IDs
 */
#define EHS_WIDGET_UI_STATE_EMPTY                       0x0000
#define EHS_WIDGET_UI_STATE_CREATE                      0x0001
#define EHS_WIDGET_UI_STATE_DESTROY                     0x0002
#define EHS_WIDGET_UI_STATE_UPDATE                      0x0004
#define EHS_WIDGET_UI_STATE_SHOW                        0x0008

// enums must be defined in this order, as they represent iGB set IDs
typedef enum {
    EHS_STRING_UI_WIDGET = 0,
    EHS_STRING_UI_WIDGET_TEXT_FIELD,
    EHS_STRING_UI_WIDGET_KEYPAD,
    EHS_STRING_UI_WIDGET_LABEL_LIST,
    /* next string widget id */
    EHS_STRING_UI_WIDGET_COUNT
} EhsStringTypeUiWigets;

typedef enum {
    EHS_BOOL_UI_WIDGET = EHS_STRING_UI_WIDGET_COUNT,
    EHS_BOOL_UI_WIDGET_BUTTON,
    EHS_BOOL_UI_WIDGET_TOGGLE_BUTTON,
    EHS_BOOL_UI_WIDGET_CHECK_BOX,
    /* next bool widget id */
    EHS_BOOL_UI_WIDGET_COUNT
} EhsBoolTypeUiWigets;

typedef enum {
    EHS_INT_UI_WIDGET = EHS_BOOL_UI_WIDGET_COUNT,
    EHS_INT_UI_WIDGET_SLIDER,
    EHS_INT_UI_WIDGET_PROGRESS_BAR,
    EHS_INT_UI_WIDGET_GAUGE,
    EHS_INT_UI_WIDGET_DROP_DOWN_LIST,
    /* next int widget id */
    EHS_INT_UI_WIDGET_COUNT
} EhsIntTypeUiWigets;

typedef enum {
    EHS_FLOAT_UI_WIDGET = EHS_INT_UI_WIDGET_COUNT,
    EHS_FLOAT_UI_WIDGET_SLIDER,
    EHS_FLOAT_UI_WIDGET_PROGRESS_BAR,
    EHS_FLOAT_UI_WIDGET_GAUGE,
    EHS_FLOAT_UI_WIDGET_ROLLER,
    EHS_FLOAT_UI_WIDGET_NUMPAD,
    /* next float widget id */
    EHS_FLOAT_UI_WIDGET_COUNT
} EhsFloatTypeUiWigets;

typedef enum {
    EHS_OTHER_UI_WIDGET = EHS_FLOAT_UI_WIDGET_COUNT,
    EHS_OTHER_UI_WIDGET_PANEL,
    EHS_OTHER_UI_WIDGET_IMAGE,
    EHS_OTHER_UI_WIDGET_SPINNER,
    EHS_OTHER_UI_WIDGET_CHART,
    EHS_OTHER_UI_WIDGET_LIST,
    EHS_OTHER_UI_WIDGET_VIEWPORT,
    /* next other widget id */
    EHS_OTHER_UI_WIDGET_COUNT
} EhsOtherTypeUiWigets;

typedef enum {
    EHS_SPECIAL_UI_WIDGET = EHS_OTHER_UI_WIDGET_COUNT,
    EHS_SPECIAL_UI_GPIO_IN,
    EHS_SPECIAL_UI_GPIO_OUT,
    EHS_SPECIAL_UI_GPIO_PANEL,
    /* next other widget id */
    EHS_SPECIAL_UI_WIDGET_COUNT
} EhsSpecialTypeUiWigets;

/* Other enums */
typedef enum {
    EHS_UI_WIDGET_LIST_IDLE = 0,
    EHS_UI_WIDGET_LIST_APPEND,
    EHS_UI_WIDGET_LIST_CLEAR
} EhsWidgetUiListState;

/*
 * UI Widget specific structures
 */

// EHS_WIDGET_UI_BASIC
typedef struct
{
    void* label;
    void* data;
} EhsWidgetUi;

// EHS_WIDGET_UI_IMAGE
typedef struct
{
    ehs_bool bLoadImageFromAppDir;
    ehs_bool bDynamicFilename;
    ehs_char szFilename[EHS_MAXPATHLENGTH];
} EhsWidgetUiImage;

// EHS_WIDGET_UI_SPINNER
typedef struct
{
    ehs_uint32 time; /**< spinner speed */
    ehs_uint32 arc_length; /**< spinner arc length */
} EhsWidgetUiSpinner;

// EHS_WIDGET_UI_CHART
#define UI_CHART_TITLE_MAX_SIZE 20
typedef struct
{
    ehs_uint16 data_size;   /**< chart point count */
    ehs_sint16* data1; /**< chart data1 */
    ehs_sint16* data2; /**< chart data2 */
    ehs_sint16* data3; /**< chart data3 */
    ehs_sint32 xmin;
    ehs_sint32 xmax;
    ehs_sint32 ymin;
    ehs_sint32 ymax;
    ehs_sint32 hdiv;
    ehs_sint32 vdiv;
    ehs_sint32 nan_value; /* value used for NaN */
    ehs_bool sett_changed; /* flag set to true when chart settings changed */
    ehs_bool enable_data1;
    ehs_bool enable_data2;
    ehs_bool enable_data3;
} EhsWidgetUiChart;

typedef struct
{
    ehs_char item[EHS_STRING_LENGTH_MAX];
    ehs_uint16 max_size;   /**< max size of the list */
    ehs_bool reversed;
    ehs_uint32 rgb;
    EhsWidgetUiListState state;
} EhsWidgetUiList;

/**
 * Obtain the ui specific properties of EhsWidgetUI
 */
#define EHS_WIDGET_UI(pWidget) ((pWidget)->specificWidgetType.ui)
 
/**
 * Widget ui subclass. This contains ui-specific properties.
 * This is always accessed from a widget
 * class using the EHS_WIDGET_UI(pWidget)
 * macro
 */
typedef struct
{
    void* pUiObject; /* pointer to target specific graphics object */
    const void* data; /**< data associated with this ui widget */
    void (*event_callback)(struct EhsWidgetStruct* pWidget, ehs_uint16 event_id, const char* label, void* data);  /**< event callback associated with ui widget response */
    //EhsGraphicsFontClass* pFont; /**< The font to display on the object */ // not used at the moment, so disable to save memory
    EhsGraphicsColourClass xFgColour; /**< Foreground colour */
    EhsGraphicsColourClass xBgColour; /**< Background colour */
    ehs_uint16 id; /**< ui type id */
    ehs_uint16 properties; /**< any custom properties id associated with the widget */
    ehs_uint16 curvature; /**< roundness of the object */
    ehs_uint16 parent_id; /**< the widget parent id */
    ehs_uint16 nIndentLeft, nIndentRight, nIndentTop, nIndentBottom;
    /**< Records the indents for the text box */
    ehs_uint16 nLineSep;		/**< Distance between the tops of two lines */
    ehs_uint8 nFgBaseAlpha; /**< The starting alpha value for the foreground colour - this can be modified by the fade function */
    ehs_uint8 nBgBaseAlpha; /**< The starting alpha value for the background colour - this can be modified by the fade function */
    ehs_uint8 nUiState; /* used for controlling widget state in the rendering loop */
    ehs_uint8 nNoOfDecPlaces; /* number of decimal places */
} EhsWidgetUiSubclass;

/**
 * Initialise the widget with the general widget specific properties including
 * position, dimensions, z-ordering and colour.
 *
 * @param[in] id ui type ID
 * @param[in] properties any custom properties id associated with the widget
 * @param[in] curvature defines roundess of the widget
 * @param[in] parent_id id of the parent widget (@todo)
 * @param[in] xBounds Position and size of the overall widget
 * @param[in] nZ Z order for widget
 * @param[in] nIndentL indent for left hand side
 * @param[in] nIndentT indent for top side
 * @param[in] nIndentR indent for right hand side
 * @param[in] nIndentB indent for bottom
 * @param[in] nLineSep Separation between lines (distance from top of character
 * 		on one line to top of character on next line down)
 * @param[in] xFgColour Widget's foreground colour
 * @param[in] xBgColour Widget's background colour
 * @param[in] pFont Pointer to the font to use for this widget
 * @return pointer to initialised widget, or NULL
 */
EHS_GLOBAL EhsWidgetClass* EhsWidgetUI_init(ehs_uint16 id, ehs_uint16 properties, ehs_uint16 curvature, ehs_uint16 parent_id,
                                            const EhsGraphicsRectangleClass* xBounds, ehs_uint16 nZ,
                                            ehs_uint16 nIndentL, ehs_uint16 nIndentT, ehs_uint16 nIndentR, ehs_uint16 nIndentB, 
                                            ehs_uint16 nLineSep,EhsGraphicsColourClass xFgColour, EhsGraphicsColourClass xBgColour, 
                                            EhsGraphicsFontClass* pFont);

/**
 * Set the data to be displayed onto a specific ui. This function can be called
 * directly by any functions
 *
 * @param pWidget UI to be updated
 */
EHS_GLOBAL void EhsWidgetUI_update(struct EhsWidgetStruct* pWidget);

/**
 * @param pWidget UI to be checked
 * @return returns true if expected widget type
 */
EHS_GLOBAL ehs_bool EhsWidgetUI_is_string_type(struct EhsWidgetStruct* pWidget);

/**
 * @param pWidget UI to be checked
 * @return returns true if expected widget type
 */
EHS_GLOBAL ehs_bool EhsWidgetUI_is_bool_type(struct EhsWidgetStruct* pWidget);

/**
 * @param pWidget UI to be checked
 * @return returns true if expected widget type
 */
EHS_GLOBAL ehs_bool EhsWidgetUI_is_int_type(struct EhsWidgetStruct* pWidget);

/**
 * @param pWidget UI to be checked
 * @return returns true if expected widget type
 */
EHS_GLOBAL ehs_bool EhsWidgetUI_is_float_type(struct EhsWidgetStruct* pWidget);

/**
 * @param pWidget UI to be checked
 * @return returns true if expected widget type
 */
EHS_GLOBAL ehs_bool EhsWidgetUI_is_other_type(struct EhsWidgetStruct* pWidget);


#endif /* EHS_WIDGETUI_H */

