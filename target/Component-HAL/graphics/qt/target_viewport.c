/*
 * Qt Graphics HAL - EHS Widget to Qt QObject Bridge
 */

#define EHS_TARGET_CODE
#include <stdio.h>
#include "globals.h"
#include "hal_viewport.h"
#include "hal_process.h"
#include "widget.h"
#include "hal_logger.h"
#include "messages.h"

#include "ertqt.h"
#include "ertqt_button.h"
#include "ertqt_label.h"
#include "ertqt_text_field.h"
#include "ertqt_checkbox.h"

// Global viewport instance
EhsTVClass EhsTV;

// Initialization flag
static ehs_bool g_qt_initialized = EHS_FALSE;

//=============================================================================
// Qt Signal Handlers → EHS Events
//=============================================================================

// Button clicked
static void qt_on_button_clicked(void * user_data)
{
    EhsWidgetClass * pWidget = (EhsWidgetClass *)user_data;

    if (!pWidget || !pWidget->pFIData)
    {
        return;
    }

    EHSH_LOG_DEBUG("Qt button clicked: %s\n", pWidget->name);

    // Fire EHS component click event
    ehs_bool value = EHS_FALSE;

    // Update component output
    if (EHS_FB_OUT_CONNECTED_API2(INX_gui_widget_ARG_create_data_out))
    {
        EHS_FB_OUT_B_API2(INX_gui_widget_ARG_create_data_out) = value;
    }

    // Fire event
    EhsFunctionInstanceData_triggerEvent(pWidget->pFIData, INX_gui_widget_ARG_create_click);
}

// Text changed
static void qt_on_text_changed(const char * utf8_text, void * user_data)
{
    EhsWidgetClass * pWidget = (EhsWidgetClass *)user_data;

    if (!pWidget || !pWidget->pFIData)
    {
        return;
    }

    EHSH_LOG_DEBUG("Qt text changed: %s = '%s'\n", pWidget->name, utf8_text);

    // Update component string output
    if (EHS_FB_OUT_CONNECTED_API2(INX_gui_widget_ARG_create_data_out))
    {
        static const ehs_uint32 MAX_LEN = EHS_STRING_LENGTH_MAX - 1;
        (EHS_FB_OUT_S_API2(INX_gui_widget_ARG_create_data_out))[MAX_LEN] = '\0';
        EhsStrncpy(EHS_FB_OUT_S_API2(INX_gui_widget_ARG_create_data_out), utf8_text, MAX_LEN);
    }

    // Fire change event
    EhsFunctionInstanceData_triggerEvent(pWidget->pFIData, INX_gui_widget_ARG_create_change);
}

// Checkbox clicked
static void qt_on_checkbox_clicked(void * user_data)
{
    EhsWidgetClass * pWidget = (EhsWidgetClass *)user_data;

    if (!pWidget || !pWidget->pFIData)
    {
        return;
    }

    // Read checkbox state
    bool checked = false;
    ertqt_checkbox_get_checked(((EhsWidgetQtData *)pWidget->qt_data)->qt_handle, &checked);

    EHSH_LOG_DEBUG("Qt checkbox clicked: %s = %d\n", pWidget->name, checked);

    // Update component boolean output
    if (EHS_FB_OUT_CONNECTED_API2(INX_gui_widget_ARG_create_data_out))
    {
        EHS_FB_OUT_B_API2(INX_gui_widget_ARG_create_data_out) = checked ? EHS_TRUE : EHS_FALSE;
    }

    // Fire change event
    EhsFunctionInstanceData_triggerEvent(pWidget->pFIData, INX_gui_widget_ARG_create_change);
}

//=============================================================================
// Helper: Register Qt callbacks based on widget type
//=============================================================================

static void register_qt_signals(EhsWidgetClass * pWidget)
{
    EhsWidgetQtData * qt_data = (EhsWidgetQtData *)pWidget->qt_data;
    ertqt_object_handle h = qt_data->qt_handle;
    const char * type = qt_data->widget_type;

    if (strcmp(type, "button") == 0)
    {
        ertqt_button_on_clicked(h, qt_on_button_clicked, pWidget);

    }
    else if (strcmp(type, "textfield") == 0)
    {
        ertqt_bind_text_changed(h, qt_on_text_changed, pWidget);

    }
    else if (strcmp(type, "checkbox") == 0)
    {
        ertqt_checkbox_on_clicked(h, qt_on_checkbox_clicked, pWidget);

    }
    else if (strcmp(type, "label") == 0)
    {
        // Labels are output-only, no signals to register
    }
    else
    {
        EHSH_LOG_WARNING("Unknown widget type for Qt: %s\n", type);
    }
}

//=============================================================================
// EHS Widget Lifecycle Functions
//=============================================================================

ehs_bool EhsWidget_create(EhsWidgetClass * pWidget)
{
    ertqt_object_handle qt_handle;
    EhsWidgetQtData * qt_data;

    EHSH_LOG_DEBUG("Creating widget: %s\n", pWidget->name);

    // Look up Qt object by name (matches .gui → .qml)
    qt_handle = ertqt_get_object_by_name(pWidget->name);

    if (qt_handle < 0)
    {
        EHSH_LOG_ERROR("Widget '%s' not found in QML!\n", pWidget->name);
        EHSH_LOG_ERROR("Check .gui/.qml name consistency\n");
        return EHS_FALSE;
    }

    // Allocate Qt-specific data
    qt_data = (EhsWidgetQtData *)malloc(sizeof(EhsWidgetQtData));
    if (!qt_data)
    {
        EHSH_LOG_ERROR("Failed to allocate Qt data for widget\n");
        return EHS_FALSE;
    }

    qt_data->qt_handle = qt_handle;
    qt_data->widget_type = pWidget->widget_type_string;  // From .gui

    pWidget->qt_data = qt_data;

    // Register Qt signal handlers
    register_qt_signals(pWidget);

    EHSH_LOG_INFO("Widget created: %s (type=%s, qt_handle=%d)\n", pWidget->name, qt_data->widget_type, qt_handle);

    return EHS_TRUE;
}

void EhsWidget_destroy(EhsWidgetClass * pWidget)
{
    if (pWidget->qt_data)
    {
        free(pWidget->qt_data);
        pWidget->qt_data = NULL;
    }
}

void EhsWidget_draw(EhsWidgetClass * pWidget, EhsTVClass * pViewport, EhsGraphicsRectangleClass * pClipRect)
{
    // Sync EHS component state → Qt properties
    EhsWidgetQtData * qt_data = (EhsWidgetQtData *)pWidget->qt_data;

    if (!qt_data)
    {
        return;
    }

    ertqt_object_handle h = qt_data->qt_handle;
    const char * type = qt_data->widget_type;

    // Update Qt properties based on widget type and EHS state
    if (strcmp(type, "label") == 0)
    {
        // Get text from component
        if (pWidget->text_data)
        {
            ertqt_label_set_text(h, pWidget->text_data);
        }

    }
    else if (strcmp(type, "button") == 0)
    {
        // Update button text if changed
        if (pWidget->text_data)
        {
            ertqt_button_set_text(h, pWidget->text_data);
        }
    }
    else if (strcmp(type, "textfield") == 0)
    {
        // Update text field if programmatically set
        if (pWidget->text_data && pWidget->text_changed_by_component)
        {
            ertqt_text_field_set_text(h, pWidget->text_data);
            pWidget->text_changed_by_component = EHS_FALSE;
        }
    }

    // Visibility, enabled state, etc. can be synced here
}

//=============================================================================
// Viewport Initialisation
//=============================================================================

ehs_bool EhsTV_init(EhsTVClass * pViewport)
{
    if (g_qt_initialized)
    {
        return EHS_TRUE;
    }

    EHSH_LOG_INFO("Qt Graphics HAL initialized\n");

    g_qt_initialized = EHS_TRUE;
    return EHS_TRUE;
}

void EhsTV_term(EhsTVClass * pViewport)
{
    g_qt_initialized = EHS_FALSE;
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

void EhsTV_fillRect(EhsTVClass * pViewport, const EhsGraphicsRectangleClass * prDst, EhsGraphicsColourClass nColour)
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
