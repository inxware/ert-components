/*
 * Qt Graphics HAL - EHS Widget to Qt QObject Bridge
 */

#define EHSL_MODULE_ID (EHSH_LOG_MODULE_GRAPHICS)
#define EHS_TARGET_CODE

#include <stdio.h>
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

// Define viewport structure
struct EhsTVStruct {
    EhsTVSurfaceClass* pAllocSurface; /**< List of allocated surfaces - used for deallocation purposes */
};

// Global viewport instance
EhsTVClass EhsTV;

// Initialisation flag
static ehs_bool g_qt_initialised = EHS_FALSE;

//=============================================================================
// Qt Signal Handlers → EHS Events
//=============================================================================

// Button pressed (mouse down)
static void qt_on_button_pressed(void * user_data)
{
    EhsWidgetClass * pWidget = (EhsWidgetClass *)user_data;

    if (!pWidget)
    {
        return;
    }

    EHSH_LOG_INFO("Qt button pressed\n");

    // Trigger EHS mouse_down event via the widget's event callback
    if (EHS_WIDGET_UI(pWidget).event_callback)
    {
        ehs_bool value = EHS_TRUE;
        EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_MOUSE_DOWN | EHS_WIDGET_UI_EVENT_DATA_CHANGED, NULL, &value);
    }
}

// Button released (mouse up) - EHS treats this as the actual "click"
static void qt_on_button_released(void * user_data)
{
    EhsWidgetClass * pWidget = (EhsWidgetClass *)user_data;

    if (!pWidget)
    {
        return;
    }

    EHSH_LOG_INFO("Qt button released (click)\n");

    // Trigger EHS click event via the widget's event callback
    // EHS treats button release as the actual click event
    if (EHS_WIDGET_UI(pWidget).event_callback)
    {
        ehs_bool value = EHS_FALSE;
        EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_MOUSE_CLICKED | EHS_WIDGET_UI_EVENT_DATA_CHANGED, NULL, &value);
    }
}

// Text changed
static void qt_on_text_changed(const char * utf8_text, void * user_data)
{
    EhsWidgetClass * pWidget = (EhsWidgetClass *)user_data;

    if (!pWidget)
    {
        return;
    }

    EHSH_LOG_INFO("Qt text changed: '%s'\n", utf8_text ? utf8_text : "(null)");

    // Trigger EHS data changed event via the widget's event callback
    if (EHS_WIDGET_UI(pWidget).event_callback)
    {
        EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_DATA_CHANGED, NULL, (void *)utf8_text);
    }
}

// Checkbox clicked
static void qt_on_checkbox_clicked(void * user_data)
{
    EhsWidgetClass * pWidget = (EhsWidgetClass *)user_data;
    bool checked = false;

    if (!pWidget)
    {
        return;
    }

    // Read checkbox state from Qt
    ertqt_checkbox_get_checked(pWidget->qt_handle, &checked);

    EHSH_LOG_INFO("Qt checkbox clicked: checked = %d\n", checked);

    // Trigger EHS data changed event via the widget's event callback
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

    // Determine widget type from UI subclass ID
    // For PoC, register all possible signals - Qt will only connect the ones that exist

    // Register button pressed/released signals (EHS treats release as the actual click)
    ertqt_bind_pressed(h, qt_on_button_pressed, pWidget);
    ertqt_bind_released(h, qt_on_button_released, pWidget);

    // Register text field change signal
    ertqt_bind_text_changed(h, qt_on_text_changed, pWidget);

    // Register checkbox clicked signal
    ertqt_checkbox_on_clicked(h, qt_on_checkbox_clicked, pWidget);

    EHSH_LOG_INFO("Registered Qt signals for handle %ld\n", h);
}

//=============================================================================
// Widget UI Creation/Destruction (Mode B)
//=============================================================================

// Create Qt widget and register event handlers
// Called by EhsWidgetUi_create() -> this is where Qt widgets get connected to EHS
void EhsTargetWidgetUi_create(EhsWidgetClass * pWidget, EhsTVClass * pViewport)
{
    if (!pWidget)
    {
        EHSH_LOG_ERROR("NULL widget pointer in EhsTargetWidgetUi_create\n");
        return;
    }

    // Look up Qt object by name
    char * name_hack = "user_interface";
    ertqt_object_handle h = ertqt_get_object_by_name(name_hack);    // @TODO: get the widget name string from `pWidget`

    if (h < 0)
    {
        EHSH_LOG_WARNING("Failed to find Qt object '%s', trying default names\n", name_hack);
        // @TODO: Try alternate naming strategies or create widget dynamically
        return;
    }

    // Store Qt handle in widget structure
    pWidget->qt_handle = h;

    // Register Qt signal handlers
    register_qt_signals(pWidget);

    EHSH_LOG_INFO("Created Qt widget UI for handle %ld\n", h);
}

// Draw Qt widget
// Called by EhsWidgetUi_draw() and Ehs_widget_commit()
void EhsTargetWidgetUi_draw(EhsWidgetClass * pWidget)
{
    if (!pWidget || pWidget->qt_handle == 0)
    {
        return;
    }

    // Request Qt to update/repaint the widget
    // Note: In most cases Qt automatically updates when properties change,
    // but this ensures updates happen when explicitly requested
    ertqt_status status = ertqt_update_widget(pWidget->qt_handle);

    if (status != ERTQT_OK && status != ERTQT_ERR_BACKEND_FAILURE)
    {
        // Only warn if it's not a backend failure (some objects don't support update())
        EHSH_LOG_WARNING("Failed to update Qt widget (status %d)\n", status);
    }
}

// Destroy Qt widget
// Called by EhsWidgetUi_destroy()
void EhsTargetWidgetUi_destroy(EhsWidgetClass * pWidget)
{
    if (!pWidget)
    {
        return;
    }

    // In Mode B Qt, widgets are owned by QML - we don't destroy them
    // Just clear our handle
    pWidget->qt_handle = 0;

    EHSH_LOG_INFO("Destroyed Qt widget UI\n");
}

// Show/hide Qt widget based on EHS widget state
// Called by EhsWidget_setState() when visibility changes
void EhsTargetWidget_show(EhsWidgetClass * pWidget, ehs_uint8 nState)
{
    if (!pWidget || pWidget->qt_handle == 0)
    {
        return;
    }

    // Check if widget should be visible based on state flags
    ehs_bool bVisible = EHS_WIDGET_STATE_SHOWN(nState);

    // Set Qt "visible" property via abstraction layer
    ertqt_status status = ertqt_set_property_bool(pWidget->qt_handle, "visible", bVisible);

    if (status != ERTQT_OK)
    {
        EHSH_LOG_WARNING("Failed to set Qt widget visibility (status %d)\n", status);
    }
    else
    {
        EHSH_LOG_INFO("Qt widget visibility set to %d\n", bVisible);
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

    EHSH_LOG_INFO("Qt Graphics HAL initialised\n");

    g_qt_initialised = EHS_TRUE;
    return EHS_TRUE;
}

void EhsTV_term(EhsTVClass * pViewport)
{
    g_qt_initialised = EHS_FALSE;
}

void EhsTV_reset(EhsTVClass * pViewport)
{
    // Clear widget table
    extern EhsWidgetTableClass EhsWidgetTable;
    EhsTPMutex_lock(EhsTPMutex_widgetTable);
    EhsWidgetTable_init(&EhsWidgetTable);
    pViewport->pAllocSurface = NULL;
    EhsTPMutex_unlock(EhsTPMutex_widgetTable);

    // In Qt, no additional viewport clearing needed - widgets managed by QML
    EHSH_LOG_INFO("Qt viewport reset\n");
}

// Viewport update functions (Qt renders automatically)
void EhsTV_update(EhsTVClass * pViewport)
{
    // Qt updates automatically, nothing needed
}

void EhsTV_clear(EhsTVClass * pViewport)
{
    // No-op for Qt
}

void EhsTV_show(EhsTVClass * pViewport)
{
    // Window already shown by ertqt_init()
}

// Other required HAL functions (stubs for PoC)
void EhsTV_blit(EhsTVClass * pViewport,
                const EhsTVSurfaceClass * pSurface,
                const EhsGraphicsRectangleClass * prDst,
                const EhsGraphicsRectangleClass * prSrc,
                ehs_uint8 nAlpha)
{
    // Qt renders everything, blitting not used
}

void EhsTV_fillRect(EhsTVClass * pViewport, const EhsGraphicsRectangleClass * pRect, const EhsGraphicsColourClass * pColour)
{
    // Qt handles rendering
}

// Surface allocation (minimal implementation for PoC)
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

// Move/resize viewport - stub for Mode B Qt (window managed by QML)
void EhsTV_move(EhsTVClass * pViewport, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nDeltaWid, EhsDataflowIntType nDeltaHt)
{
    // Stub: In Mode B, viewport (Qt window) is managed by QML, not programmatically.
    // Future: Could potentially call ertqt_set_property_int() on root object if needed.
}

// Set viewport window position and size - stub for Mode B Qt (window managed by QML)
void EhsTV_setwindow(EhsTVClass * pViewport, ehs_bool bIsView, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nWid, EhsDataflowIntType nHt)
{
    // Stub: In Mode B, viewport (Qt window) is managed by QML, not programmatically.
    // Future: Could potentially adjust window geometry via Qt properties if needed.
}

// Set background colour/fade for viewport
void EhsTV_fade(EhsTVClass * pViewport, EhsGraphicsColourClass nColour)
{
    // Future implementation: Could set QML window background colour via:
    //   ertqt_object_handle root = ertqt_get_object_by_name("windowRoot");
    //   ertqt_set_property_string(root, "colour", "#RRGGBB");
    // For now, stub - background colour managed by QML.
}

// Show/hide window frame - stub (window chrome managed by OS/QML)
void EhsTV_showFrame(EhsTVClass * pViewport, ehs_bool bAddFrame)
{
    // Stub: Window frame/chrome is managed by the OS and QML window flags.
    // Not applicable for Mode B Qt PoC.
}

// Hide viewport - stub for Mode B Qt
void EhsTV_hideViewport(void)
{
    // Future implementation: Could call:
    //   ertqt_object_handle root = ertqt_get_object_by_name("windowRoot");
    //   ertqt_set_property_bool(root, "visible", false);
    // For now, stub - window visibility managed by QML/OS.
}

// Show viewport with Z-order - stub for Mode B Qt
void EhsTV_showViewport(ehs_uint16 zorder)
{
    // Stub: Window visibility and stacking order managed by OS.
    // Z-order concept doesn't apply to top-level Qt windows in typical usage.
}

// Set viewport Z-order - stub for Mode B Qt
void EhsTV_setZOrder(ehs_uint16 zorder)
{
    // Stub: Z-order is not applicable to top-level Qt windows.
    // Widget z-order within the window is managed by QML.
}

// Get screen dimensions - actually implemented via ertqt API
ehs_bool EhsTV_getScreenSize(ehs_sint32 * nScreenWidth, ehs_sint32 * nScreenHeight)
{
    int width = 0;
    int height = 0;

    if (!nScreenWidth || !nScreenHeight)
    {
        return EHS_FALSE;
    }

    // Query Qt window size via ertqt abstraction layer
    if (ertqt_get_window_size(&width, &height) != ERTQT_OK)
    {
        EHSH_LOG_WARNING("Failed to get Qt window size, using defaults\n");
        // Default fallback values
        *nScreenWidth = 800;
        *nScreenHeight = 600;
        return EHS_TRUE;
    }

    *nScreenWidth = (ehs_sint32)width;
    *nScreenHeight = (ehs_sint32)height;

    EHSH_LOG_INFO("Screen size: %dx%d\n", *nScreenWidth, *nScreenHeight);

    return EHS_TRUE;
}

// Get pointer to surface pixel buffer - stub (Qt handles all rendering)
void * EhsTVSurface_pixels(EhsTVSurfaceClass * pSurface)
{
    // Stub: In Mode B Qt, surfaces are not used - Qt manages all pixel buffers.
    // GIF images and other media are handled by Qt's internal rendering.
    return NULL;
}

// Destroy surface with viewport context
void EhsTVSurface_destroy(EhsTVClass * pViewport, EhsTVSurfaceClass * pSurface)
{
    // Stub: Surfaces not used in Mode B Qt.
    // Note: EhsTV_destroySurface() handles the simpler case already defined above.
}

// Update specific rectangle of viewport - stub (Qt auto-updates)
void EhsTV_updateRect(EhsTVClass * pViewport, ehs_sint32 nX, ehs_sint32 nY, ehs_sint32 nWidth, ehs_sint32 nHeight)
{
    // Stub: Qt automatically updates changed regions.
    // In Mode A (framebuffer), this would mark a dirty region for refresh.
}

// Create surface for image loading - stub (Qt handles image buffers internally)
EhsTVSurfaceClass * EhsTVSurface_create(EhsTVClass * pViewport, ehs_uint16 nWidth, ehs_uint16 nHeight, EhsGraphicsColourFormatEnum eFormat, EhsGraphicsColourClass * pPalette, ehs_uint16 nPaletteSize)
{
    // Stub: In Mode B Qt, surfaces aren't used - images are loaded directly by Qt.
    // PNG/JPG/GIF widgets would use Qt's image loading instead of pixel manipulation.
    // Return NULL to indicate surface creation not supported.
    return NULL;
}

// Finalize pixel buffer access
void EhsTVSurface_finalisePixels(EhsTVSurfaceClass * pSurface)
{
    // Stub: No pixel buffers in Mode B Qt.
}

// Get surface pitch (bytes per row)
ehs_uint16 EhsTVSurface_pitch(EhsTVSurfaceClass * pSurface)
{
    // Stub: Surfaces not used in Mode B Qt.
    return 0;
}

// Get surface width
ehs_uint16 EhsTVSurface_width(EhsTVSurfaceClass * pSurface)
{
    // Stub: Surfaces not used in Mode B Qt.
    return 0;
}

// Get surface height
ehs_uint16 EhsTVSurface_height(EhsTVSurfaceClass * pSurface)
{
    // Stub: Surfaces not used in Mode B Qt.
    return 0;
}
