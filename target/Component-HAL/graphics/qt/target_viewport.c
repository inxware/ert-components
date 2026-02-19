/***************************************************************
 * Copyright (C) 2025 inx limited, UK - All Rights Reserved.
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
 * with this file. If not, please visit:
 *  <https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * Qt Graphics HAL - EHS Widget to Qt QObject Bridge
 */

#define EHSL_MODULE_ID (EHSH_LOG_MODULE_GRAPHICS)
#define EHS_TARGET_CODE

#include <stdio.h>
#include <inttypes.h>

#include "globals.h"
#include "hal_viewport.h"  // this in turn includes `target_viewport.h`
#include "hal_process.h"
#include "widget.h"
#include "widget_ui.h"
#include "hal_logger.h"
#include "messages.h"

#include "ertqt.h"
#include "ertqt_button.h"
#include "ertqt_label.h"
#include "ertqt_text_field.h"
#include "ertqt_checkbox.h"

/* Define viewport structure */
struct EhsTVStruct {
    EhsTVSurfaceClass * pAllocSurface; /**< List of allocated surfaces - used for deallocation purposes */
};

/* Global viewport instance */
EhsTVClass EhsTV;

/* Initialisation flag */
static ehs_bool g_qt_initialised = EHS_FALSE;

//=============================================================================
// Qt Signal Handlers -> EHS Events
//=============================================================================

/** Button pressed (mouse down) */
static void qt_on_button_pressed(void * user_data)
{
    EhsWidgetClass * pWidget = (EhsWidgetClass *)user_data;

    if (!pWidget)
    {
        EHSH_LOG_WARNING("qt_on_button_pressed: pWidget is NULL");
        return;
    }
    EHSH_LOG_INFO("qt_on_button_pressed: pWidget=%p, event_callback=%p, pFIData=%p",
           (void *)pWidget, (void *)EHS_WIDGET_UI(pWidget).event_callback, (void *)pWidget->pFIData);

    /* Trigger EHS mouse_down event via the widget's event callback */
    if (EHS_WIDGET_UI(pWidget).event_callback)
    {
        EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_MOUSE_DOWN, NULL, NULL);
    }
    else
    {
        EHSH_LOG_WARNING("qt_on_button_pressed: event_callback is NULL");
    }
}

/* Button released (mouse up) - EHS treats this as the actual "click" */
static void qt_on_button_released(void * user_data)
{
    EhsWidgetClass * pWidget = (EhsWidgetClass *)user_data;

    if (!pWidget)
    {
        EHSH_LOG_WARNING("qt_on_button_released: pWidget is NULL");
        return;
    }

    EHSH_LOG_INFO("qt_on_button_released: pWidget=%p, event_callback=%p, pFIData=%p",
           (void *)pWidget, (void *)EHS_WIDGET_UI(pWidget).event_callback, (void *)pWidget->pFIData);

    /* Trigger EHS click event via the widget's event callback */
    if (EHS_WIDGET_UI(pWidget).event_callback)
    {
        EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_MOUSE_CLICKED, NULL, NULL);
    }
    else
    {
        EHSH_LOG_WARNING("qt_on_button_released: event_callback is NULL");
    }
}

/* Text changed */
static void qt_on_text_changed(const char * utf8_text, void * user_data)
{
    EhsWidgetClass * pWidget = (EhsWidgetClass *)user_data;
    if (!pWidget)
    {
        return;
    }

    EHSH_LOG_INFO("Qt text changed: '%s'", utf8_text ? utf8_text : "(null)");

    /* Trigger EHS data changed event via the widget's event callback */
    if (EHS_WIDGET_UI(pWidget).event_callback)
    {
        EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_DATA_CHANGED, NULL, (void *)utf8_text);
    }
}

/* Value changed (for float/int display widgets — sliders, dials, spinboxes) */
static void qt_on_value_changed(double value, void * user_data)
{
    EhsWidgetClass * pWidget = (EhsWidgetClass *)user_data;
    if (!pWidget)
    {
        return;
    }

    EHSH_LOG_INFO("Qt value changed: %f", value);

    if (EHS_WIDGET_UI(pWidget).event_callback)
    {
        if (EhsWidgetUI_is_float_type(pWidget))
        {
            float fvalue = (float)value;
            EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_DATA_CHANGED, NULL, &fvalue);
        }
        else if (EhsWidgetUI_is_int_type(pWidget))
        {
            ehs_sint32 ivalue = (ehs_sint32)value;
            EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_DATA_CHANGED, NULL, &ivalue);
        }
    }
}

/* Checkbox clicked */
static void qt_on_checkbox_clicked(void * user_data)
{
    EhsWidgetClass * pWidget = (EhsWidgetClass *)user_data;
    bool checked = false;

    if (!pWidget)
    {
        return;
    }

    /* Read checkbox state from Qt */
    ertqt_checkbox_get_checked(pWidget->qt_handle, &checked);

    EHSH_LOG_INFO("Qt checkbox clicked: checked = %d", checked);

    /* Trigger EHS data changed event via the widget's event callback */
    if (EHS_WIDGET_UI(pWidget).event_callback)
    {
        ehs_bool value = checked ? EHS_TRUE : EHS_FALSE;
        EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_DATA_CHANGED, NULL, &value);
    }
}

//=============================================================================
// Helper: Register Qt callbacks based on widget UI type
//=============================================================================

static void register_qt_signals(EhsWidgetClass * pWidget)
{
    ertqt_object_handle h = pWidget->qt_handle;

    EHSH_LOG_INFO("register_qt_signals: handle=%"PRIdPTR", pWidget=%p", (intptr_t)h, (void *)pWidget);

    /* Register button pressed/released signals (EHS treats release as the actual click) */
    ertqt_bind_pressed(h, qt_on_button_pressed, pWidget);
    ertqt_bind_released(h, qt_on_button_released, pWidget);

    /* Register text field change signal */
    ertqt_bind_text_changed(h, qt_on_text_changed, pWidget);

    /* Register value changed signal (for float/int widgets: sliders, dials, spinboxes) */
    ertqt_bind_value_changed(h, qt_on_value_changed, pWidget);

    /* Register checkbox clicked signal */
    ertqt_checkbox_on_clicked(h, qt_on_checkbox_clicked, pWidget);
}

//=============================================================================
// Widget UI Creation/Destruction (Mode B)
//=============================================================================

/**
 * Create Qt widget and register event handlers.
 *
 * Called by Common/HAL/graphics/widget_ui.c:EhsWidgetUi_create() -> this is where Qt widgets
 * get connected to EHS.
 */
void EhsTargetWidgetUi_create(EhsWidgetClass * pWidget, EhsTVClass * pViewport)
{
    if (!pWidget)
    {
        EHSH_LOG_WARNING("EhsTargetWidgetUi_create: pWidget is NULL");
        return;
    }

    /* Look up Qt object by name */
    const char * widget_name = pWidget->szWidgetName[0] != '\0' ? pWidget->szWidgetName : "unknown_widget";
    printf("[ERTQT-STATE] EhsTargetWidgetUi_create: looking up '%s' (app_state=%d)\n",
           widget_name, (int)ertqt_get_app_state());
    fflush(stdout);
    EHSH_LOG_INFO("EhsTargetWidgetUi_create: looking up Qt object '%s'", widget_name);

    ertqt_object_handle h = ertqt_get_object_by_name(widget_name);

    if (h < 0)
    {
        /* Object not found — may have been created dynamically (Loader, Repeater,
         * Component.createObject).  Refresh the object table and retry once. */
        EHSH_LOG_INFO("EhsTargetWidgetUi_create: '%s' not found, refreshing object table", widget_name);
        //ertqt_refresh_objects();
        //rebuild_object_table();
        h = ertqt_get_object_by_name(widget_name);
    }

    if (h < 0)
    {
        EHSH_LOG_WARNING("EhsTargetWidgetUi_create: FAILED to find Qt object '%s'", widget_name);
        return;
    }

    EHSH_LOG_INFO("EhsTargetWidgetUi_create: ertqt_get_object_by_name('%s') -> handle=%"PRIdPTR, widget_name, (intptr_t)h);

    /* Store Qt handle in widget structure */
    pWidget->qt_handle = h;

    /* Register Qt signal handlers */
    register_qt_signals(pWidget);

    EHSH_LOG_INFO("EhsTargetWidgetUi_create: DONE, qt_handle=%"PRIdPTR", event_callback=%p",
           (intptr_t)h, (void *)EHS_WIDGET_UI(pWidget).event_callback);
}

/**
 * Draw Qt widget.
 *
 * Called by EhsWidgetUi_draw() and Ehs_widget_commit().
 */
void EhsTargetWidgetUi_draw(EhsWidgetClass * pWidget)
{
    if (!pWidget)
    {
        return;
    }

    /* Lazy binding: if widget creation happened before the QML was loaded,
       qt_handle will be 0.  Try to bind now that the object table may have
       been populated by a later scan. */
    if (pWidget->qt_handle == 0)
    {
        const char * name = pWidget->szWidgetName;
        if (name[0] != '\0')
        {
            ertqt_object_handle h = ertqt_get_object_by_name(name);
            if (h >= 0)
            {
                printf("[ERTQT-STATE] lazy bind: '%s' -> handle=%"PRIdPTR"\n", name, (intptr_t)h);
                fflush(stdout);
                pWidget->qt_handle = h;
                register_qt_signals(pWidget);
            }
            else
            {
                return; /* still not available, try again next draw */
            }
        }
        else
        {
            return;
        }
    }
    ertqt_object_handle h = pWidget->qt_handle;

    /* If content was updated, push the new data to the Qt widget */
    if (pWidget->bContentUpdated)
    {
        EHSH_LOG_INFO("Draw widget %"PRIdPTR" - content updated, syncing to Qt", h);
        /* EHS_WIDGET_UI(pWidget).data points to an EhsWidgetUi structure */
        /* The actual widget data (string/bool/int/float) is in the .data field of that structure */
        EhsWidgetUi * gui = (EhsWidgetUi *)EHS_WIDGET_UI(pWidget).data;
        if (!gui)
        {
            EHSH_LOG_WARNING("  gui pointer is NULL!");
            pWidget->bContentUpdated = EHS_FALSE;
            return;
        }

        /* For string widgets (TextBox), set the text property */
        if (EhsWidgetUI_is_string_type(pWidget))
        {
            const char * text = (const char *)gui->data;
            EHSH_LOG_INFO("  gui->data=%p, text='%s'", gui->data, text ? text : "(null)");
            if (text && text[0] != '\0')
            {
                EHSH_LOG_INFO("  Setting Qt 'text' property to: '%s'", text);
                ertqt_status status = ertqt_set_property_string(h, "text", text);
                if (status != ERTQT_OK)
                {
                    EHSH_LOG_WARNING("  Failed to set Qt text property (status %d)", status);
                }
            }
        }
        /* For boolean widgets (Checkbox), set the checked property */
        else if (EhsWidgetUI_is_bool_type(pWidget))
        {
            ehs_bool * value = (ehs_bool *)gui->data;
            if (value)
            {
                EHSH_LOG_INFO("  Setting Qt 'checked' property to: %d", *value);
                ertqt_status status = ertqt_set_property_bool(h, "checked", *value);
                if (status != ERTQT_OK)
                {
                    EHSH_LOG_WARNING("  Failed to set Qt checked property (status %d)", status);
                }
            }
        }
        /* For float widgets, set the value property */
        else if (EhsWidgetUI_is_float_type(pWidget))
        {
            float * value = (float *)gui->data;
            if (value)
            {
                EHSH_LOG_INFO("  Setting Qt 'value' property to: %f", (double)*value);
                ertqt_status status = ertqt_set_property_double(h, "value", (double)*value);
                if (status != ERTQT_OK)
                {
                    EHSH_LOG_WARNING("  Failed to set Qt value property (status %d)", status);
                }
            }
        }
        /* For integer widgets, set the value property */
        else if (EhsWidgetUI_is_int_type(pWidget))
        {
            ehs_sint32 * value = (ehs_sint32 *)gui->data;
            if (value)
            {
                EHSH_LOG_INFO("  Setting Qt 'value' property to: %d", *value);
                ertqt_status status = ertqt_set_property_int(h, "value", (int)*value);
                if (status != ERTQT_OK)
                {
                    EHSH_LOG_WARNING("  Failed to set Qt value property (status %d)", status);
                }
            }
        }

        /* Clear the updated flag */
        pWidget->bContentUpdated = EHS_FALSE;
    }
    else
    {
        EHSH_LOG_INFO("Draw widget %"PRIdPTR" - no content update", h);
    }

    /* Request Qt to update/repaint the widget */
    ertqt_status status = ertqt_update_widget(h);
    if (status != ERTQT_OK && status != ERTQT_ERR_BACKEND_FAILURE)
    {
        /* Only warn if it's not a backend failure (some objects don't support update()) */
        EHSH_LOG_WARNING("Failed to update Qt widget (status %d)", status);
    }
}

/**
 * Destroy Qt widget.
 *
 * Called by EhsWidgetUi_destroy().
 */
void EhsTargetWidgetUi_destroy(EhsWidgetClass * pWidget)
{
    if (!pWidget)
    {
        return;
    }

    /* In Mode B Qt, widgets are owned by QML - we don't destroy them: just clear our handle */
    EHSH_LOG_INFO("Destroying Qt widget UI %"PRIdPTR"", pWidget->qt_handle);
    pWidget->qt_handle = 0;

}

/**
 * Viewport cleanup for Mode B Qt.
 *
 * Called by widget_viewport.c when a viewport widget is destroyed.
 * In Qt, viewports are managed by QML so nothing to clean up.
 */
void EhsTargetWidgetUi_viewport_cleanup(EhsWidgetClass * pWidget)
{
    (void)pWidget;
}

/**
 * Show/hide Qt widget based on EHS widget state.
 *
 * Called by EhsWidget_setState() when visibility changes.
 */
void EhsTargetWidget_show(EhsWidgetClass * pWidget, ehs_uint8 nState)
{
    if (!pWidget || pWidget->qt_handle == 0)
    {
        return;
    }

    /* Check if widget should be visible based on state flags */
    ehs_bool bVisible = EHS_WIDGET_STATE_SHOWN(nState);

    ertqt_object_handle h = pWidget->qt_handle;

    /* Set Qt "visible" property via abstraction layer */
    ertqt_status status = ertqt_set_property_bool(h, "visible", bVisible);

    if (status != ERTQT_OK)
    {
        EHSH_LOG_WARNING("Failed to set Qt widget visibility (status %d)", status);
    }
    else
    {
        EHSH_LOG_INFO("Qt widget %"PRIdPTR" visibility set to %d", h, bVisible);
    }
}

//=============================================================================
// Viewport Initialisation
//=============================================================================

ehs_bool EhsTV_init(EhsTVClass * pViewport)
{
    if (g_qt_initialised)
    {
        return EHS_TRUE;
    }

    EHSH_LOG_INFO("Qt Graphics HAL initialised");

    g_qt_initialised = EHS_TRUE;
    return EHS_TRUE;
}

void EhsTV_term(EhsTVClass * pViewport)
{
    g_qt_initialised = EHS_FALSE;
}

void EhsTV_reset(EhsTVClass * pViewport)
{
    /* Clear widget table */
    extern EhsWidgetTableClass EhsWidgetTable;
    EhsTPMutex_lock(EhsTPMutex_widgetTable);
    EhsWidgetTable_init(&EhsWidgetTable);
    pViewport->pAllocSurface = NULL;
    EhsTPMutex_unlock(EhsTPMutex_widgetTable);

    /* In Qt, no additional viewport clearing needed - widgets managed by QML */
    EHSH_LOG_INFO("Qt viewport reset");
}

/* Viewport update functions (Qt renders automatically) */

void EhsTV_update(EhsTVClass * pViewport)
{
    /* Qt updates automatically, nothing needed */
}

void EhsTV_clear(EhsTVClass * pViewport)
{
    /* No-op for Qt */
}

void EhsTV_show(EhsTVClass * pViewport)
{
    /* Window already shown by ertqt_init() */
}

/* Other required HAL functions (stubs for PoC) */

void EhsTV_blit(EhsTVClass * pViewport,
                const EhsTVSurfaceClass * pSurface,
                const EhsGraphicsRectangleClass * prDst,
                const EhsGraphicsRectangleClass * prSrc,
                ehs_uint8 nAlpha)
{
    /* Qt renders everything, blitting not used */
}

void EhsTV_fillRect(EhsTVClass * pViewport, const EhsGraphicsRectangleClass * pRect, const EhsGraphicsColourClass * pColour)
{
    /* Qt handles rendering */
}

/* Surface allocation (minimal implementation for PoC) */

EhsTVSurfaceClass * EhsTV_createSurface(EhsGraphicsColourFormatEnum eFormat, ehs_uint16 nWidth, ehs_uint16 nHeight)
{
    EhsTVSurfaceClass * pSurface = (EhsTVSurfaceClass *)malloc(sizeof(EhsTVSurfaceClass));

    if (pSurface)
    {
        pSurface->eFormat = eFormat;
        pSurface->pNext = NULL;
    }

    return pSurface;
}

void EhsTV_destroySurface(EhsTVSurfaceClass * pSurface)
{
    if (pSurface)
    {
        free(pSurface);
    }
}

//=============================================================================
// Additional Viewport HAL Functions (mostly stubs for Mode B)
//=============================================================================

/* Move/resize viewport - stub for Mode B Qt (window managed by QML) */
void EhsTV_move(EhsTVClass * pViewport, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nDeltaWid, EhsDataflowIntType nDeltaHt)
{
    /* Stub: In Mode B, viewport (Qt window) is managed by QML, not programmatically. */
    /* Future: Could potentially call ertqt_set_property_int() on root object if needed. */
}

/* Set viewport window position and size - stub for Mode B Qt (window managed by QML) */
void EhsTV_setwindow(EhsTVClass * pViewport, ehs_bool bIsView, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nWid, EhsDataflowIntType nHt)
{
    /* Stub: In Mode B, viewport (Qt window) is managed by QML, not programmatically. */
    /* Future: Could potentially adjust window geometry via Qt properties if needed. */
}

/* Set background colour/fade for viewport */
void EhsTV_fade(EhsTVClass * pViewport, EhsGraphicsColourClass nColour)
{
    /* Future implementation: Could set QML window background colour via:
     *   ertqt_object_handle root = ertqt_get_object_by_name("windowRoot");
     *   ertqt_set_property_string(root, "colour", "#RRGGBB");
     * For now, stub - background colour managed by QML.
     */
}

/* Show/hide window frame - stub (window chrome managed by OS/QML) */
void EhsTV_showFrame(EhsTVClass * pViewport, ehs_bool bAddFrame)
{
    /* Stub: Window frame/chrome is managed by the OS and QML window flags.
     * Not applicable for Mode B Qt PoC.
     */
}

/* Hide viewport - stub for Mode B Qt */
void EhsTV_hideViewport(void)
{
    /* Future implementation: Could call:
     *   ertqt_object_handle root = ertqt_get_object_by_name("windowRoot");
     *   ertqt_set_property_bool(root, "visible", false);
     * For now, stub - window visibility managed by QML/OS.
     */
}

/* Show viewport with Z-order - stub for Mode B Qt */
void EhsTV_showViewport(ehs_uint16 zorder)
{
    /* Stub: Window visibility and stacking order managed by OS.
     * Z-order concept doesn't apply to top-level Qt windows in typical usage.
     */
}

/* Set viewport Z-order - stub for Mode B Qt */
void EhsTV_setZOrder(ehs_uint16 zorder)
{
    /* Stub: Z-order is not applicable to top-level Qt windows.
     * Widget z-order within the window is managed by QML.
     */
}

/* Get screen dimensions - actually implemented via ertqt API */
ehs_bool EhsTV_getScreenSize(ehs_sint32 * nScreenWidth, ehs_sint32 * nScreenHeight)
{
    int width = 0;
    int height = 0;

    if (!nScreenWidth || !nScreenHeight)
    {
        return EHS_FALSE;
    }

    /* Query Qt window size via ertqt abstraction layer */
    if (ertqt_get_window_size(&width, &height) != ERTQT_OK)
    {
        EHSH_LOG_WARNING("Failed to get Qt window size, using defaults");
        /* Default fallback values */
        *nScreenWidth = 800;
        *nScreenHeight = 600;
        return EHS_TRUE;
    }

    *nScreenWidth = (ehs_sint32)width;
    *nScreenHeight = (ehs_sint32)height;

    EHSH_LOG_INFO("Screen size: %dx%d", *nScreenWidth, *nScreenHeight);

    return EHS_TRUE;
}

/* Get pointer to surface pixel buffer - stub (Qt handles all rendering) */
void * EhsTVSurface_pixels(EhsTVSurfaceClass * pSurface)
{
    /* Stub: In Mode B Qt, surfaces are not used - Qt manages all pixel buffers.
     * GIF images and other media are handled by Qt's internal rendering.
     */
    return NULL;
}

/* Destroy surface with viewport context */
void EhsTVSurface_destroy(EhsTVClass * pViewport, EhsTVSurfaceClass * pSurface)
{
    /* Stub: Surfaces not used in Mode B Qt.
     * Note: EhsTV_destroySurface() handles the simpler case already defined above.
     */
}

/* Update specific rectangle of viewport - stub (Qt auto-updates) */
void EhsTV_updateRect(EhsTVClass * pViewport, ehs_sint32 nX, ehs_sint32 nY, ehs_sint32 nWidth, ehs_sint32 nHeight)
{
    /* Stub: Qt automatically updates changed regions.
     * In Mode A (framebuffer), this would mark a dirty region for refresh.
     */
}

/* Create surface for image loading - stub (Qt handles image buffers internally) */
EhsTVSurfaceClass * EhsTVSurface_create(EhsTVClass * pViewport, ehs_uint16 nWidth, ehs_uint16 nHeight, EhsGraphicsColourFormatEnum eFormat, EhsGraphicsColourClass * pPalette, ehs_uint16 nPaletteSize)
{
    /* Stub: In Mode B Qt, surfaces aren't used - images are loaded directly by Qt.
     * PNG/JPG/GIF widgets would use Qt's image loading instead of pixel manipulation.
     * Return NULL to indicate surface creation not supported.
     */
    return NULL;
}

/* Finalise pixel buffer access */
void EhsTVSurface_finalisePixels(EhsTVSurfaceClass * pSurface)
{
    /* Stub: No pixel buffers in Mode B Qt. */
}

/* Get surface pitch (bytes per row) */
ehs_uint16 EhsTVSurface_pitch(EhsTVSurfaceClass * pSurface)
{
    /* Stub: Surfaces not used in Mode B Qt. */
    return 0;
}

/* Get surface width */
ehs_uint16 EhsTVSurface_width(EhsTVSurfaceClass * pSurface)
{
    /* Stub: Surfaces not used in Mode B Qt. */
    return 0;
}

/* Get surface height */
ehs_uint16 EhsTVSurface_height(EhsTVSurfaceClass * pSurface)
{
    /* Stub: Surfaces not used in Mode B Qt. */
    return 0;
}
