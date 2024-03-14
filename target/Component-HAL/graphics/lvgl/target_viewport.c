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

#include <stdio.h>
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
#include "keypress.h" 

#if EHS_PERIPHERALS_GPIO_SUPPORT == EHS_PERIPHERALS_GPIO_TYPE_GUI
#include "hal_gpio.h"
#endif

#include "lvgl/lvgl.h"
#include "target_lvgl_bridge.h"

/* Include Driver headers here */
#include "hal_viewport.h"
#include "target_viewport_modeB.h"

#ifdef EHS_RUNTIME_LOGGER_ENABLED
#define DEBUG_ESP32_TOUCH_EVENT 0
#if DEBUG_ESP32_TOUCH_EVENT == 1
#include "esp_log.h"
#endif
#endif

/* This should be general and let platform target code handle the target-specific driver and configurations */
#include "target_graphics.h"
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
#define INDENT(x) ((x==0)?"":((x==1)?">":((x==:EhsTargetWidget_hide(lv_obj_t *obj))?">>":((x==3)?">>>":(">..>")))))
#define ENTER(x) {ehs_uint32 ticks = EhsTgtTimer_tickTous(EhsTgtTimer_now());EhsTraceLevel++;currentFunc = #x;}
#define LEAVE(x) {ehs_uint32 ticks = EhsTgtTimer_tickTous(EhsTgtTimer_now());--EhsTraceLevel;}
#else
#define ENTER(x)
#define LEAVE(x)
#endif

#define EHS_LVGL_STATE_IDLE                0x0000
#define EHS_LVGL_STATE_INIT                0x0001
#define EHS_LVGL_STATE_UPDATE_WIDGET       0x0002
#define EHS_LVGL_STATE_UPDATE_BACKGROUND   0x0004
#define EHS_LVGL_STATE_UPDATE_WINDOW       0x0008
#define EHS_LVGL_STATE_CLEAN               0x0010
#define EHS_LVGL_STATE_DEINIT              0x0020

#define EHS_LVGL_FONT_TABLE_SIZE 25
#define EHS_LVGL_FONT_MIN_SIZE (8 / 2)

#define EHS_ROLLER_OPTION_SIZE 256
#define EHS_ROLLER_ENTRY_SIZE 32

struct EhsTVStruct {

    EhsTVSurfaceClass* pAllocSurface; /**< List of allocated surfaces - used for deallocation purposes */
};

/*****************************************************************************/
/* Variables defined with global-scope */

EhsTVClass EhsTV; // Global handle of viewport strcuture

lv_color_t* buf1_1 = NULL;
lv_color_t* buf1_2 = NULL;

static lv_color_t g_bg_color;
static lv_opa_t g_bg_alpha = 255;

static ehs_bool gEhsTVExit = EHS_FALSE;
static ehs_bool gEhsTVtickExit = EHS_FALSE;
static ehs_bool gEhsTVhandleExit = EHS_FALSE;

static ehs_uint16 gEhsLvglState = EHS_LVGL_STATE_IDLE;

static const lv_font_t* gLvglFontTable[EHS_LVGL_FONT_TABLE_SIZE] = {    // 21 available font sizes in total
    LV_FONT_DEFAULT,                        // font size 0, default
    LV_FONT_DEFAULT,                        // font size 2, default
    LV_FONT_DEFAULT,                        // font size 4, default
    LV_FONT_DEFAULT,                        // font size 6, default
    #if LV_FONT_MONTSERRAT_8 == 1
    &lv_font_montserrat_8,                  // font size 8
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_10 == 1
    &lv_font_montserrat_10,                 // font size 10
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_12 == 1
    &lv_font_montserrat_12,                 // font size 12
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_14 == 1
    &lv_font_montserrat_14,                 // font size 14
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_16 == 1
    &lv_font_montserrat_16,                 // font size 16
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_18 == 1
    &lv_font_montserrat_18,                 // font size 18
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_20 == 1
    &lv_font_montserrat_20,                 // font size 20
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_22 == 1
    &lv_font_montserrat_22,                 // font size 22
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_24 == 1
    &lv_font_montserrat_24,                 // font size 24
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_26 == 1
    &lv_font_montserrat_26,                 // font size 26
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_28 == 1
    &lv_font_montserrat_28,                 // font size 28
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_30 == 1
    &lv_font_montserrat_30,                 // font size 30
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_32 == 1
    &lv_font_montserrat_32,                 // font size 32
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_34 == 1
    &lv_font_montserrat_34,                 // font size 34
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_36 == 1
    &lv_font_montserrat_36,                 // font size 36
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_38 == 1
    &lv_font_montserrat_38,                 // font size 38
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_40 == 1
    &lv_font_montserrat_40,                 // font size 40
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_42 == 1
    &lv_font_montserrat_42,                 // font size 42
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_44 == 1
    &lv_font_montserrat_44,                 // font size 44
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_46 == 1
    &lv_font_montserrat_46,                 // font size 46
    #else
    NULL,
    #endif
    #if LV_FONT_MONTSERRAT_48 == 1
    &lv_font_montserrat_48                  // font size 48
    #else
    NULL
    #endif
};

/*****************************************************************************/
/* Function prototypes */
void EhsTargetWidgetUi_init_lvgl();
void EhsTargetWidgetUi_create_lvgl(struct EhsWidgetStruct* pWidget);
void EhsTargetWidgetUi_draw_lvgl(struct EhsWidgetStruct* pWidget);
void EhsTargetWidgetUi_style(lv_obj_t* obj, struct EhsWidgetStruct* pWidget);
void EhsTargetWidget_hide(lv_obj_t *obj);
void EhsTargetWidget_unhide(lv_obj_t *obj);

#if EHS_PERIPHERALS_GPIO_SUPPORT == EHS_PERIPHERALS_GPIO_TYPE_GUI
lv_obj_t* EhsTargetWidget_create_gpio(struct EhsWidgetStruct* pWidget);
void EhsTargetWidget_update_gpio(struct EhsWidgetStruct* pWidget);
#endif

static const lv_font_t * Ehs_LVGL_find_font(ehs_uint16 size)
{
    ehs_uint16 Lindex = size / 2, isOdd = size % 2, i;
    ehs_uint16 contn = 0;
    ehs_uint16 Lindex_store, dec0_inc1 = 0;
    lv_font_t * result = NULL;

    if (Lindex >= EHS_LVGL_FONT_TABLE_SIZE) Lindex = EHS_LVGL_FONT_TABLE_SIZE - 1;
    if (Lindex <= EHS_LVGL_FONT_MIN_SIZE) Lindex = EHS_LVGL_FONT_MIN_SIZE;

    // Check the font size availability around the given size
    result = gLvglFontTable[Lindex];
    if (result == NULL)
    {
        if ((isOdd == 1 && Lindex == EHS_LVGL_FONT_TABLE_SIZE - 1) || isOdd == 0) contn = 1;
        else if (isOdd == 1 && Lindex < EHS_LVGL_FONT_TABLE_SIZE - 1)
        {
            result = gLvglFontTable[++Lindex];
            if (result == NULL) contn = 1;
        }
    }

    Lindex_store = Lindex;
    
    while (contn == 1)
    {
        if (Lindex <= EHS_LVGL_FONT_MIN_SIZE) {dec0_inc1 = 1; Lindex = Lindex_store;}
        if (Lindex >= EHS_LVGL_FONT_TABLE_SIZE - 1) {dec0_inc1 = 0; Lindex = Lindex_store;}
        Lindex = dec0_inc1 == 1 ? Lindex + 1 : Lindex - 1;
        result = gLvglFontTable[Lindex];
        if (result != NULL) contn = 0;
    }

    return (const lv_font_t *) result;
}

/* waits until state completes */
void EhsTargetWidget_lvgl_state_wait(ehs_uint16 state)
{
    while (gEhsLvglState & state)
    {
        EhsSleep(EHS_TIME_ms(1));
    }
}

static void EhsTV_LVGL_handle_ui_calls(lv_timer_t* timer)
{
    if(gEhsLvglState & EHS_LVGL_STATE_UPDATE_BACKGROUND)
    {
        lv_obj_set_style_bg_color(lv_scr_act(), g_bg_color, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(lv_scr_act(), g_bg_alpha, LV_PART_MAIN);
        gEhsLvglState = gEhsLvglState & (~EHS_LVGL_STATE_UPDATE_BACKGROUND);
    }

    if(gEhsLvglState & EHS_LVGL_STATE_UPDATE_WIDGET)
    {
        EhsTPMutex_lock(EhsTPMutex_widgetTable);
        if (EhsWidgetTable.initialised == EHS_MAGIC_NUMBER) // check if table is initalised 
        {
            for(int i = 0; i < EhsWidgetTable.nSize; i++)
            {
                struct EhsWidgetStruct* pWidget = EhsWidgetTable.pZOrderedWidget[i];
                if(pWidget)
                {
                    /* Check flag for destroying widget */
                    if(EHS_WIDGET_UI(pWidget).nUiState & EHS_WIDGET_UI_STATE_DESTROY)
                    {
                        if (EHS_WIDGET_UI(pWidget).pUiObject != NULL) 
                        {
                            lv_obj_del((lv_obj_t*) EHS_WIDGET_UI(pWidget).pUiObject);
                            EHS_WIDGET_UI(pWidget).pUiObject = NULL;
                        }
                        EHS_WIDGET_UI(pWidget).nUiState = EHS_WIDGET_UI(pWidget).nUiState & (~EHS_WIDGET_UI_STATE_DESTROY);
                    }
                    /* Check flag for creating widget */
                    if(EHS_WIDGET_UI(pWidget).nUiState & EHS_WIDGET_UI_STATE_CREATE)
                    {
                        EhsTargetWidgetUi_create_lvgl(pWidget);
                        EHS_WIDGET_UI(pWidget).nUiState = EHS_WIDGET_UI(pWidget).nUiState & (~EHS_WIDGET_UI_STATE_CREATE);
                    }
                    /* Check flag for chaning show state of widget */
                    if(EHS_WIDGET_UI(pWidget).nUiState & EHS_WIDGET_UI_STATE_SHOW)
                    {
                        if(EHS_WIDGET_UI(pWidget).pUiObject){
                            if(EHS_WIDGET_STATE_SHOWN(pWidget->nState)){
                                EhsTargetWidget_unhide(EHS_WIDGET_UI(pWidget).pUiObject);
                            }else{
                                EhsTargetWidget_hide(EHS_WIDGET_UI(pWidget).pUiObject);
                            }
                        }
                        EHS_WIDGET_UI(pWidget).nUiState = EHS_WIDGET_UI(pWidget).nUiState & (~EHS_WIDGET_UI_STATE_SHOW);
                    }
                    /* Check flag for updating content of widget */
                    if(EHS_WIDGET_UI(pWidget).nUiState & EHS_WIDGET_UI_STATE_UPDATE)
                    {
                        EhsTargetWidgetUi_draw_lvgl(pWidget);
                        // event notifies about ui changes in a different callback
                        EHS_WIDGET_UI(pWidget).nUiState = EHS_WIDGET_UI(pWidget).nUiState & (~EHS_WIDGET_UI_STATE_UPDATE);
                    }
                }
            }
        }
        EhsTPMutex_unlock(EhsTPMutex_widgetTable);
        gEhsLvglState = gEhsLvglState & (~EHS_LVGL_STATE_UPDATE_WIDGET);
    }

    if(gEhsLvglState & EHS_LVGL_STATE_CLEAN)
    {
        lv_obj_clean(lv_scr_act());
        gEhsLvglState = gEhsLvglState & (~EHS_LVGL_STATE_CLEAN);
    }
}

void EhsTV_LVGL_tick_thread(void)
{
    //(void) data;
    while (1)
    {
        if (gEhsTVExit == EHS_TRUE) break;
        lv_tick_inc(1);
        EhsSleep(EHS_TIME_ms(1));
    }
    gEhsTVtickExit = EHS_TRUE;
}

void EhsTV_LVGL_gui_thread(void)
{
    // initalise lvgl library
    if(gEhsLvglState == EHS_LVGL_STATE_INIT)
    {
        EhsTargetWidgetUi_init_lvgl();
        const ehs_sint32 stackSize = 128; // tick thread shouldn't need much stack //todo2024 we may want to include the tick function in a more generic "fast loop" thread
        EhsHThread_execute(EhsTV_LVGL_tick_thread, NULL, -5, stackSize);
        
        gEhsLvglState = EHS_LVGL_STATE_IDLE;
    }
    else
    {
        // starting thread without initalisation
        // @TODO - report error
        gEhsLvglState = EHS_LVGL_STATE_IDLE;
        return;
    }

    lv_timer_create((lv_timer_cb_t)EhsTV_LVGL_handle_ui_calls, 30, NULL);

    while (1)
    {
        if (gEhsTVExit == EHS_TRUE) break;
        
        EhsTPMutex_lock(EhsTPMutex_viewport);
        lv_timer_handler();
        EhsTPMutex_unlock(EhsTPMutex_viewport);

        EhsSleep(EHS_TIME_ms(10));
    }
    gEhsTVhandleExit = EHS_TRUE;
}

void on_pointer_feedback_cb(struct _lv_indev_drv_t * drv, uint8_t event_id)
{
    // implement any handler needed for interacting with the view
#if DEBUG_ESP32_TOUCH_EVENT == 1
    if(event_id == LV_INDEV_STATE_PRESSED){
        lv_indev_data_t data = { 0 };
        drv->read_cb(drv, &data);
        int mem = esp_get_free_heap_size();
        ESP_LOGI("esp32_touch", "(%d %d) mem=%d", data.point.x, data.point.y, mem);
    }
#endif
}

#if TARGET_LVGL_KEYBOARD == 1

void on_keyboard_pressed_handler(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    if(!indev_drv || indev_drv->type != LV_INDEV_TYPE_KEYPAD){
        return;
    }
    if(data){
        if(data->state == LV_INDEV_STATE_PRESSED && data->continue_reading){
            ehs_uint32 nCharacter = data->key;
            switch(nCharacter){
                case LV_KEY_UP:
                    nCharacter = 0xFF52;
                    break;
                case LV_KEY_DOWN:
                    nCharacter = 0xFF54;
                    break;
                case LV_KEY_RIGHT:
                    nCharacter = 0xFF53;
                    break;
                case LV_KEY_LEFT:
                    nCharacter = 0xFF51;
                    break;
                case LV_KEY_ESC:
                    nCharacter = 0xFF1B;
                    break;
                case LV_KEY_DEL:
                    //nCharacter = 0; // not handled by eRT keypress.c
                    break;
                case LV_KEY_BACKSPACE:
                    nCharacter = 0xFF08;
                    break;
                case LV_KEY_ENTER:
                    nCharacter = 0xFF0D;
                    break;
                case LV_KEY_NEXT: // PAGEDOWN
                    nCharacter = 0xFF56;
                    break;
                case LV_KEY_PREV: // PAGEUP
                    nCharacter = 0xFF55;
                    break;
                case LV_KEY_HOME:
                    nCharacter = 0xFF50;
                    break;
                case LV_KEY_END:
                    nCharacter = 0xFF57;
                    break;
                // @TODO - add the rest of keys from keypress.c
                default:
                    break;
            };
            EhsGUIKbHit(nCharacter);
        }
    }
}

void lvgl_keyboard_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    TARGET_LVGL_KEYBOARD_CALLBACK_FUNCTION(indev_drv, data);

    on_keyboard_pressed_handler(indev_drv, data);
}

#endif

void EhsTargetWidgetUi_init_lvgl()
{
    lv_init();

    TARGET_LVGL_DRIVER_INIT;

    static lv_disp_draw_buf_t disp_buf1;
    #if TARGET_MEMORY_DYNAMIC == 1
    buf1_1 = TARGET_LVGL_MALLOC(TARGET_DISPLAY_BUFFER_SIZE * sizeof(lv_color_t));
    assert(buf1_1 != NULL);
    #else
    static lv_color_t buf1_1[TARGET_DISPLAY_BUFFER_SIZE];
    #endif
    #if TARGET_MEMORY_DYNAMIC == 1
    buf1_2 = TARGET_LVGL_MALLOC(TARGET_DISPLAY_BUFFER_SIZE * sizeof(lv_color_t));
    assert(buf1_2 != NULL);
    #else
    static lv_color_t buf1_2[TARGET_DISPLAY_BUFFER_SIZE];
    #endif
    lv_disp_draw_buf_init(&disp_buf1, buf1_1, buf1_2, TARGET_DISPLAY_BUFFER_SIZE);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf1;
    disp_drv.flush_cb = TARGET_LVGL_DISPLAY_FLUSH_FUNCTION;
    disp_drv.hor_res = TARGET_LVGL_HORIZONTAL;
    disp_drv.ver_res = TARGET_LVGL_VERTICAL;
    disp_drv.antialiasing = 1;

    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);

    lv_theme_t * th = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, th);

    lv_group_t * g = lv_group_create();
    lv_group_set_default(g);

    #if TARGET_LVGL_POINTER == 1
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1);
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;
    indev_drv_1.read_cb = TARGET_LVGL_POINTER_CALLBACK_FUNCTION;
    indev_drv_1.feedback_cb = on_pointer_feedback_cb;
    lv_indev_t * mouse_indev = lv_indev_drv_register(&indev_drv_1);
    #endif

    #if TARGET_LVGL_KEYBOARD == 1
    static lv_indev_drv_t indev_drv_2;
    lv_indev_drv_init(&indev_drv_2);
    indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv_2.read_cb = lvgl_keyboard_read;
    lv_indev_t * kb_indev = lv_indev_drv_register(&indev_drv_2);
    lv_indev_set_group(kb_indev, g);
    #endif

    #if TARGET_LVGL_ENCODER == 1
    static lv_indev_drv_t indev_drv_3;
    lv_indev_drv_init(&indev_drv_3);
    indev_drv_3.type = LV_INDEV_TYPE_ENCODER;
    indev_drv_3.read_cb = TARGET_LVGL_ENCODER_CALLBACK_FUNCTION;
    lv_indev_t * enc_indev = lv_indev_drv_register(&indev_drv_3);
    lv_indev_set_group(enc_indev, g);
    #endif
}

/* non user dependent parameter initialisation - this is called before any SODL is read*/
ehs_bool EhsTV_init(EhsTVClass* pViewport)
{
    // printf("EhsTV_init\n");
    ehs_bool bInitialised = EHS_TRUE; /* has initialisation been successful? assume it has */

    gEhsTVExit = EHS_FALSE;

    g_bg_color = lv_color_make(255, 255, 255);
    g_bg_alpha = 255;

    gEhsLvglState = EHS_LVGL_STATE_INIT;

    EhsHThread_execute(EhsTV_LVGL_gui_thread, NULL, -99, EHS_THREAD_USE_DEFAULT_STACK_SIZE);

    EhsTargetWidget_lvgl_state_wait(EHS_LVGL_STATE_INIT);

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
    // clear widget table
    EhsTPMutex_lock(EhsTPMutex_widgetTable);
    EhsWidgetTable_init(&EhsWidgetTable);
    pViewport->pAllocSurface=NULL;//Should this go to the hal?
    EhsTPMutex_unlock(EhsTPMutex_widgetTable);

    // clear viewport
    EhsTPMutex_lock(EhsTPMutex_viewport);
    EhsGUIKbClearCBs(); // clear keyboard
    gEhsLvglState |= EHS_LVGL_STATE_CLEAN;
    EhsTPMutex_unlock(EhsTPMutex_viewport);

    // wait for lvgl library to be cleared
    EhsTargetWidget_lvgl_state_wait(EHS_LVGL_STATE_CLEAN);
}

/**
 * Called after widgets are created
 * These are tasks that are delayed until after the widgets are created
 * This is specifically so that the viewport widget attributes can be applied to the window before it is shown
 */
void EhsTV_show(EhsTVClass* pViewport)
{
}

/**
 *  Shutdown the target viewport. This function
 * releases resources occupied by the viewport.
 */
void EhsTV_term(EhsTVClass* pViewport)
{
    gEhsTVExit = EHS_TRUE;
    while (gEhsTVtickExit != EHS_TRUE || gEhsTVhandleExit != EHS_TRUE);
    if (buf1_1 != NULL) free(buf1_1);
    buf1_1 = NULL;
    if (buf1_2 != NULL) free(buf1_2);
    buf1_2 = NULL;
    EhsTPMutex_lock(EhsTPMutex_viewport);
    lv_deinit();
    EhsTPMutex_unlock(EhsTPMutex_viewport);

    gEhsLvglState |= EHS_LVGL_STATE_DEINIT;

    // @todo - wait for lvgl library to be de-initalised
}

/**
 * Clear the target viewport
 */
void EhsTV_clear(EhsTVClass* pViewport)
{
    EhsTPMutex_lock(EhsTPMutex_viewport);

    gEhsLvglState |= EHS_LVGL_STATE_CLEAN;

    EhsTPMutex_unlock(EhsTPMutex_viewport);

    // wait for lvgl library to be cleared
    EhsTargetWidget_lvgl_state_wait(EHS_LVGL_STATE_CLEAN);
}

/*Change size and position of viewport*/
void EhsTV_move(EhsTVClass* pViewport, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nDeltaWid, EhsDataflowIntType nDeltaHt)
{

}


/*Change size and position of viewport*/
void EhsTV_setwindow(EhsTVClass* pViewport, ehs_bool bIsView, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nWid, EhsDataflowIntType nHt)
{
 //   EhsTPMutex_lock(EhsTPMutex_viewport);

  //  lv_obj_set_pos(lv_scr_act(), nX, nY);
  //  lv_obj_set_size(lv_scr_act(), nWid, nHt);

//    EhsTPMutex_unlock(EhsTPMutex_viewport);
}

/*Change alpha value of viewport*/
void EhsTV_fade(EhsTVClass* pViewport, EhsGraphicsColourClass nColour)
{
    EhsTPMutex_lock(EhsTPMutex_viewport);

    g_bg_color = lv_color_make(nColour.sComp.nRed, nColour.sComp.nGreen, nColour.sComp.nBlue);
    g_bg_alpha = nColour.sComp.nAlpha;

    gEhsLvglState |= EHS_LVGL_STATE_UPDATE_BACKGROUND;

    EhsTPMutex_unlock(EhsTPMutex_viewport);
}


/* show a frame on the viewport */
void EhsTV_showFrame(EhsTVClass* pViewport, ehs_bool bAddFrame)
{

}

/**
 * Indicate to the target that the entire viewport needs updating.
 *
 */

void EhsTV_update(EhsTVClass* pViewport)
{

}

/**
 * Redraw a specified part of the OSD layer.global
 *
 */
void EhsTV_updateRect(EhsTVClass* pViewport, ehs_sint32 nX, ehs_sint32 nY, ehs_sint32 nWidth, ehs_sint32 nHeight)
{

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

}

/**
 * Provide access to pixels representing the surface, create them if they don't exist @todo check crating is wise
 */
void* EhsTVSurface_pixels(EhsTVSurfaceClass* pSurface)
{
    void* pRet = NULL;

    return pRet;
}


void EhsTVSurface_finalisePixels (EhsTVSurfaceClass* pSurface)
{

}

ehs_uint16 EhsTVSurface_height(EhsTVSurfaceClass* pSurface)
{
    ehs_uint16 nRet = 0u;
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

    return nRet;
}

/**
 * Return the width of the bitmap
 */
ehs_uint16 EhsTVSurface_width(EhsTVSurfaceClass* pSurface)
{
    ehs_uint16 nRet = 0u;

    return nRet;
}


void EhsTV_hideViewport()
{

}

/**
 * show viewport and set zorder
 * viewport bottommost: zorder=0
 * viewport normal zorder: zorder=1
 * viewport topmost: zorder=2
 */
void EhsTV_showViewport(ehs_uint16 zorder)
{

}

/**
 * set zorder
 * viewport bottommost: zorder=0
 * viewport normal zorder: zorder=1
 * viewport topmost: zorder=2
 */
void EhsTV_setZOrder(ehs_uint16 zorder)
{

}

void EhsTVSurface_destroy(EhsTVClass* pViewport, EhsTVSurfaceClass *pSurface)
{
    /* stub */
}

ehs_bool EhsTV_getScreenSize(ehs_sint32* nScreenWidth, ehs_sint32* nScreenHeight)
{
    if(nScreenWidth != NULL && nScreenHeight != NULL){
        *nScreenWidth = TARGET_LVGL_HORIZONTAL;
        *nScreenHeight = TARGET_LVGL_VERTICAL;
        return EHS_TRUE;
    }
    return EHS_FALSE;
}

void EhsTargetWidget_hide(lv_obj_t *obj)
{
    if(!lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN)){
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
}

void EhsTargetWidget_unhide(lv_obj_t *obj)
{
    if(lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN)){
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// text gui callbacks /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static void EhsTargetWidgetUi_gui_click_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        struct EhsWidgetStruct* pWidget = e->user_data;
        if(pWidget != NULL){
            EhsTPMutex_lock(EhsTPMutex_viewport);
            EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_MOUSE_CLICKED, NULL, NULL);
            EhsTPMutex_unlock(EhsTPMutex_viewport);
        }
    }
}

static void EhsTargetWidgetUi_gui_pressed_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_PRESSED){
        struct EhsWidgetStruct* pWidget = e->user_data;
        if(pWidget != NULL){
            EhsTPMutex_lock(EhsTPMutex_viewport);
            EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_MOUSE_DOWN, NULL, NULL);
            EhsTPMutex_unlock(EhsTPMutex_viewport);
        }
    }
}

static void EhsTargetWidgetUi_gui_data_value_event_handler(struct EhsWidgetStruct* pWidget, ehs_uint32 event_id, const char* label, void* data)
{
    if(pWidget != NULL && event_id != 0){
        EHS_WIDGET_UI(pWidget).event_callback(pWidget, event_id, label, data);
    }
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// button callbacks ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static void EhsTargetWidgetUi_button_clicked_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        struct EhsWidgetStruct* pWidget = e->user_data;
        if(pWidget != NULL){
            ehs_bool value = EHS_FALSE;
            EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_MOUSE_CLICKED|EHS_WIDGET_UI_EVENT_DATA_CHANGED, NULL, &value);
        }
    }
}

static void EhsTargetWidgetUi_button_pressed_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_PRESSED){
        struct EhsWidgetStruct* pWidget = e->user_data;
        if(pWidget != NULL){
            ehs_bool value = EHS_TRUE;
            EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_MOUSE_DOWN|EHS_WIDGET_UI_EVENT_DATA_CHANGED, NULL, &value);
        }
    }
}

static void EhsTargetWidgetUi_button_toggle_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_VALUE_CHANGED){
        struct EhsWidgetStruct* pWidget = e->user_data;
        if(pWidget != NULL){
            lv_obj_t * button = lv_event_get_target(e);
            ehs_bool value = (lv_obj_get_state(button) & LV_STATE_CHECKED) ? EHS_TRUE : EHS_FALSE;
            EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_MOUSE_CLICKED|EHS_WIDGET_UI_EVENT_DATA_CHANGED, NULL, &value);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// slider callbacks ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static void EhsTargetWidgetUi_slider_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_VALUE_CHANGED){
        struct EhsWidgetStruct* pWidget = e->user_data;
        if(pWidget != NULL){
            lv_obj_t * slider = lv_event_get_target(e);
            int value = (int)lv_slider_get_value(slider);
            if(EhsWidgetUI_is_float_type(pWidget)){
                float fvalue = (float)value;
                EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_DATA_CHANGED, NULL, &fvalue);
            }else{
                EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_DATA_CHANGED, NULL, &value);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// ui list callbacks //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//TODO

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// chart callbacks ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static void chart_draw_begin_event_cb(lv_event_t * e)
{
    lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(e);
    if(dsc != NULL){
        if( dsc->part == LV_PART_MAIN && (dsc->type == LV_CHART_DRAW_PART_DIV_LINE_HOR || LV_CHART_DRAW_PART_DIV_LINE_VER) ) {
            // draw dashed lines in the background
            if(dsc->line_dsc == NULL || dsc->p1 == NULL || dsc->p2 == NULL) return;
            if(dsc->id == 0) {
                // do not draw the first line when pad lv_style_set_pad_all = 0 
                dsc->line_dsc->width = 0;
            }else{
                dsc->line_dsc->width = 1;
                dsc->line_dsc->dash_gap  = 6;
                dsc->line_dsc->dash_width  = 2;
            }
        }else if(lv_obj_draw_part_check_type(dsc, &lv_chart_class, LV_CHART_DRAW_PART_TICK_LABEL)){
            // adjust chart tick labels
            if(dsc->id == LV_CHART_AXIS_PRIMARY_X){
                if(dsc->label_dsc){
                    dsc->label_dsc->ofs_y = -10;
                }
                if(dsc->line_dsc){
                    dsc->line_dsc->dash_width = 0;
                    dsc->line_dsc->width = 0;
                }
            }else if(dsc->id == LV_CHART_AXIS_PRIMARY_Y){
                if(dsc->label_dsc){
                    dsc->label_dsc->ofs_x = 10;
                }
                if(dsc->line_dsc){
                    dsc->line_dsc->dash_width = 0;
                    dsc->line_dsc->width = 0;
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////// drop down list callbacks /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define EHS_DROP_DOWN_LIST_ITEM_MAX_SIZE 64

static void EhsTargetWidgetUi_drop_down_list_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        struct EhsWidgetStruct* pWidget = e->user_data;
        if(pWidget != NULL){
            lv_obj_t * obj = lv_event_get_target(e);
            char buf[EHS_DROP_DOWN_LIST_ITEM_MAX_SIZE];
            lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
            int value = (int)lv_dropdown_get_option_index(obj, buf);
            EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_MOUSE_CLICKED|EHS_WIDGET_UI_EVENT_DATA_CHANGED|EHS_WIDGET_UI_EVENT_LABEL_CHANGED, buf, &value);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// text field callbacks ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static void EhsTargetWidgetUi_text_field_event_handler(lv_event_t * e)
{
    // TODO
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) {
        /*Focus on the clicked text area*/
        //if(kb != NULL) lv_keyboard_set_textarea(kb, ta);
    }
    else if(code == LV_EVENT_READY) {
       // LV_LOG_USER("Ready, current text: %s", lv_textarea_get_text(ta));
    }
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// roller callbacks /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static void EhsTargetWidgetUi_numeric_roller_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        struct EhsWidgetStruct* pWidget = e->user_data;
        if(pWidget != NULL){
            lv_obj_t * obj = EHS_WIDGET_UI(pWidget).pUiObject;
            char buf[EHS_ROLLER_ENTRY_SIZE] = {'\0'};
            float value = 0;
            uint32_t buf_pos = 0;
            uint32_t roller_count = lv_obj_get_child_cnt(obj);
            for(uint32_t i = 0; i < roller_count && buf_pos < EHS_ROLLER_ENTRY_SIZE; i++){
                char roller_value[EHS_ROLLER_ENTRY_SIZE] = {0};
                lv_obj_t * roller = lv_obj_get_child(obj, i);
                lv_roller_get_selected_str(roller, roller_value, EHS_ROLLER_ENTRY_SIZE);
                strcat(&buf[buf_pos], roller_value);
                buf_pos += strlen(roller_value);
                if(EHS_ROLLER_ENTRY_SIZE <= buf_pos){
                    break;
                }
            }
            value = atof(buf);
            EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_DATA_CHANGED, NULL, &value);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const char* EhsTargetWidgetUi_roller_numeric_next_option(const char* options, char* buff, int buff_size){
    if(options == NULL || *options == '\0' || buff == NULL){
        return NULL;
    }
    const char *end = options;
    int i = 0;
    // Find the end of the substring
    while (*end != ';' && *end != '\0') {
        if(i >= buff_size){
            // option buffer too small
            return NULL;
        }
        buff[i++] = *end;
        end++;
    }
    if(i < buff_size){
        buff[i] = '\0';
    }
    if (*end == ';') {
        end++;
    }
    return end;
}

static const char* EhsTargetWidgetUi_float_format_str(struct EhsWidgetStruct* pWidget){
    const char* fmt_str = "%.2f";
    switch(EHS_WIDGET_UI(pWidget).nNoOfDecPlaces){
        case 0:{
            fmt_str = "%.0f";
            break;
        }
        case 1:{
            fmt_str = "%.1f";
            break;
        }
        case 2:{
            fmt_str = "%.2f";
            break;
        }
        case 3:{
            fmt_str = "%.3f";
            break;
        }
        case 4:{
            fmt_str = "%.4f";
            break;
        }
        case 5:{
            fmt_str = "%.5f";
            break;
        }
        case 6:{
            fmt_str = "%.6f";
            break;
        }
        default:
            // use 2 dec places by default 
            break;
    }
    return fmt_str;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////// EhsTargetWidgetUi_factory ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

lv_obj_t* EhsTargetWidgetUi_create_label(struct EhsWidgetStruct* pWidget){

    lv_obj_t* obj = lv_label_create(lv_scr_act());
    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE); // make this label clickable
    lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_click_event_handler, LV_EVENT_CLICKED, pWidget);
    lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_pressed_event_handler, LV_EVENT_PRESSED, pWidget);
    
    lv_color_t bg_color = lv_color_make(EHS_WIDGET_UI(pWidget).xBgColour.sComp.nRed, EHS_WIDGET_UI(pWidget).xBgColour.sComp.nGreen, EHS_WIDGET_UI(pWidget).xBgColour.sComp.nBlue);
    lv_color_t fg_color = lv_color_make(EHS_WIDGET_UI(pWidget).xFgColour.sComp.nRed, EHS_WIDGET_UI(pWidget).xFgColour.sComp.nGreen, EHS_WIDGET_UI(pWidget).xFgColour.sComp.nBlue);

    static lv_font_t * this_font;
    this_font = Ehs_LVGL_find_font(EHS_WIDGET_UI(pWidget).nLineSep);

    lv_obj_set_style_border_width       (obj, 0,                                                           LV_PART_MAIN);
    lv_obj_set_style_border_color       (obj, bg_color,                                                    LV_PART_MAIN);
    lv_obj_set_style_border_opa         (obj, EHS_WIDGET_UI(pWidget).xBgColour.sComp.nAlpha,               LV_PART_MAIN);
    lv_obj_set_style_bg_color           (obj, bg_color,                                                    LV_PART_MAIN);
    lv_obj_set_style_bg_opa             (obj, EHS_WIDGET_UI(pWidget).xBgColour.sComp.nAlpha,               LV_PART_MAIN);

    lv_obj_set_style_pad_top            (obj, EHS_WIDGET_UI(pWidget).nIndentTop,                           LV_PART_MAIN);
    lv_obj_set_style_pad_bottom         (obj, EHS_WIDGET_UI(pWidget).nIndentBottom,                        LV_PART_MAIN);
    lv_obj_set_style_pad_left           (obj, EHS_WIDGET_UI(pWidget).nIndentLeft,                          LV_PART_MAIN);
    lv_obj_set_style_pad_right          (obj, EHS_WIDGET_UI(pWidget).nIndentRight,                         LV_PART_MAIN);

    lv_obj_set_style_text_color         (obj, fg_color,                                                    LV_PART_MAIN);
    lv_obj_set_style_text_opa           (obj, (lv_opa_t) EHS_WIDGET_UI(pWidget).xFgColour.sComp.nAlpha,    LV_PART_MAIN);
    lv_obj_set_style_text_font          (obj, this_font,                                                   LV_PART_MAIN);

    lv_obj_set_style_text_align         (obj, LV_TEXT_ALIGN_LEFT,                                          LV_PART_MAIN);
    lv_obj_set_style_text_line_space    (obj, EHS_WIDGET_UI(pWidget).nLineSep,                             LV_PART_MAIN);

    lv_label_set_text_fmt(obj, "%s", "");

    return obj;
}

lv_obj_t* EhsTargetWidgetUi_create_patch(struct EhsWidgetStruct* pWidget)
{
    lv_obj_t* obj = lv_obj_create(lv_scr_act());
    lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_click_event_handler, LV_EVENT_CLICKED, pWidget);
    lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_pressed_event_handler, LV_EVENT_PRESSED, pWidget);

    lv_obj_set_size(obj, pWidget->xCurRect.nWidth, pWidget->xCurRect.nHeight);
    lv_obj_set_pos(obj, pWidget->xCurRect.nLeft, pWidget->xCurRect.nTop);

    lv_color_t bg_color = lv_color_make(EHS_WIDGET_UI(pWidget).xBgColour.sComp.nRed, EHS_WIDGET_UI(pWidget).xBgColour.sComp.nGreen, EHS_WIDGET_UI(pWidget).xBgColour.sComp.nBlue);

    lv_obj_set_style_border_width       (obj, 0,                                                              LV_PART_MAIN);
    lv_obj_set_style_border_color       (obj, bg_color,                                                       LV_PART_MAIN);
    lv_obj_set_style_border_opa         (obj, (lv_opa_t)EHS_WIDGET_UI(pWidget).xBgColour.sComp.nAlpha,        LV_PART_MAIN);
    lv_obj_set_style_bg_color           (obj, bg_color,                                                       LV_PART_MAIN);
    lv_obj_set_style_bg_opa             (obj, (lv_opa_t)EHS_WIDGET_UI(pWidget).xBgColour.sComp.nAlpha,        LV_PART_MAIN);

    return obj;
}

ehs_bool EhsTargetWidgetUi_set_image_path(EhsWidgetUiImage* ui_image, char* image_path)
{
    ehs_bool bRet = EHS_FALSE;
    if(ui_image == NULL || image_path == NULL){
        return bRet;
    }
    strcpy(image_path, "A:"); // add drive id required by lvgl image decoders (LV_FS_STDIO_LETTER=65 , where 65 is 'A')
    char* szCanonicalFilePath = &image_path[2]; // offset path by drive id length
    if(ui_image->bDynamicFilename == EHS_FALSE){
        ui_image->bLoadImageFromAppDir = EHS_TRUE; 
    }
    if(ui_image->bLoadImageFromAppDir == EHS_TRUE){
        bRet = EhsHMetagetCurrentAppDir(szCanonicalFilePath);
        if (bRet == EHS_TRUE){
            EhsStrcat(szCanonicalFilePath, EHS_TD_FILES_SEPARATOR_STR);
            EhsStrcat(szCanonicalFilePath, ui_image->szFilename);
        }
    }else{
        bRet = EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_USERDATA_DIR, ui_image->szFilename, EHS_TRUE);
    }

    return bRet;
}


lv_obj_t* EhsTargetWidgetUi_create_image(struct EhsWidgetStruct* pWidget)
{
    EhsWidgetUiImage* ui_image = (EhsWidgetUiImage*)EHS_WIDGET_UI(pWidget).data;

    char buffer[EHS_MAXPATHLENGTH];
    memset(buffer, 0, EHS_MAXPATHLENGTH);
    if(EhsTargetWidgetUi_set_image_path(ui_image, buffer) == EHS_FALSE){
        return NULL;
    }

    lv_obj_t * obj = lv_img_create(lv_scr_act());

    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE); // make this label clickable
    lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_click_event_handler, LV_EVENT_CLICKED, pWidget);
    lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_pressed_event_handler, LV_EVENT_PRESSED, pWidget);

    lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
    lv_img_set_src(obj, buffer);
    lv_img_t * img = (lv_img_t *)obj;
    int width = pWidget->xCurRect.nWidth;
    int height = pWidget->xCurRect.nHeight;
    float scale = (img->w > img->h) ? (float)width/(float)img->w : (float)height/(float)img->h;
    lv_img_set_zoom(obj, 256*scale);
    //lv_img_set_antialias(obj, true);
    lv_opa_t value = (lv_opa_t)EHS_WIDGET_UI(pWidget).xBgColour.sComp.nAlpha;
    lv_obj_set_style_img_opa(obj, value, LV_PART_MAIN);

    return obj;
}

lv_obj_t* EhsTargetWidgetUi_create_gauge(struct EhsWidgetStruct* pWidget)
{
    lv_obj_t * meter = NULL;
    #if LV_USE_METER
    // this meter must have equal sides
    pWidget->xCurRect.nHeight = pWidget->xCurRect.nWidth;
    pWidget->xOrigRect.nHeight = pWidget->xOrigRect.nWidth;

    meter = lv_meter_create(lv_scr_act());

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(meter, scale, 8, 4, 15, lv_color_black(), 10);

    lv_meter_indicator_t * indic;

    /*Add a blue arc to the start*/
    indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_meter_set_indicator_start_value(meter, indic, 0);
    lv_meter_set_indicator_end_value(meter, indic, 20);

    /*Make the tick lines blue at the start of the scale*/
    indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_BLUE), false, 0);
    lv_meter_set_indicator_start_value(meter, indic, 0);
    lv_meter_set_indicator_end_value(meter, indic, 20);

    /*Add a red arc to the end*/
    indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_set_indicator_start_value(meter, indic, 80);
    lv_meter_set_indicator_end_value(meter, indic, 100);

    /*Make the tick lines red at the end of the scale*/
    indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
    lv_meter_set_indicator_start_value(meter, indic, 80);
    lv_meter_set_indicator_end_value(meter, indic, 100);

    /*Add a needle line indicator*/
    indic = lv_meter_add_needle_line(meter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);
    meter->user_data = indic;
    #endif
    return meter;
}

#if LV_USE_ROLLER
lv_obj_t* EhsTargetWidgetUi_create_roller_widget(lv_obj_t * roller_view, lv_style_t* style, int selected, const char* text){
    if(roller_view == NULL){
        return NULL;
    }
    lv_obj_t* roller = lv_roller_create(roller_view);
    lv_roller_set_options(roller, text, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 3);
    if(style){
       lv_obj_add_style(roller, style, LV_PART_SELECTED);
    }
    lv_obj_align(roller, LV_ALIGN_CENTER, 0, 0);
    lv_roller_set_selected(roller, selected, LV_ANIM_OFF);
    return roller;
}

void EhsTargetWidgetUi_set_roller_widget_value(lv_obj_t * roller_view, float value){
    if(roller_view == NULL){
        return;
    }
    bool is_negative = (value < 0);
    int integer_part = abs((int)value);
    int digit_count = 1; // At least one digit
    while (integer_part /= 10) {
        digit_count++;
    }

    uint32_t roller_count = lv_obj_get_child_cnt(roller_view);
    
    //lv_obj_t* roller = lv_obj_get_child(list, 0);
    //lv_roller_get_option_cnt();
}
#endif

lv_obj_t* EhsTargetWidgetUi_create_lvgl_label(lv_obj_t* obj, struct EhsWidgetStruct* pWidget, ehs_bool set_text)
{
    lv_obj_t* label_obj = lv_label_create(obj);
    lv_color_t fg_color = lv_color_make(EHS_WIDGET_UI(pWidget).xFgColour.sComp.nRed, EHS_WIDGET_UI(pWidget).xFgColour.sComp.nGreen, EHS_WIDGET_UI(pWidget).xFgColour.sComp.nBlue);
    const lv_font_t* font = Ehs_LVGL_find_font(EHS_WIDGET_UI(pWidget).nLineSep);
    lv_obj_set_style_text_color(label_obj, fg_color, LV_PART_MAIN);
    lv_obj_set_style_text_opa(label_obj, (lv_opa_t) EHS_WIDGET_UI(pWidget).xFgColour.sComp.nAlpha, LV_PART_MAIN);
    lv_obj_set_style_text_font(obj, font, LV_PART_MAIN);
    lv_obj_set_style_text_line_space(obj, EHS_WIDGET_UI(pWidget).nLineSep, LV_PART_MAIN);
    if(set_text == EHS_TRUE){
        const char* label_text = ((EhsWidgetUi*)EHS_WIDGET_UI(pWidget).data)->label;
        if(label_text != NULL){
            lv_label_set_text(label_obj, label_text);
        }else{
            lv_label_set_text(label_obj, "");
        }
    }
    return label_obj;
}

void EhsTargetWidgetUi_set_lvgl_background(lv_obj_t* obj, struct EhsWidgetStruct* pWidget, ehs_uint32 part_id)
{
    if(!obj || !pWidget){
        return;
    }

    lv_color_t bg_color = lv_color_make(EHS_WIDGET_UI(pWidget).xBgColour.sComp.nRed, EHS_WIDGET_UI(pWidget).xBgColour.sComp.nGreen, EHS_WIDGET_UI(pWidget).xBgColour.sComp.nBlue);
    lv_obj_set_style_bg_color(obj, bg_color, part_id);
    lv_obj_set_style_bg_opa(obj, EHS_WIDGET_UI(pWidget).xBgColour.sComp.nAlpha, part_id);
}

void EhsTargetWidgetUi_set_lvgl_obj_text_style(lv_obj_t* obj, struct EhsWidgetStruct* pWidget, ehs_uint32 part_id)
{
    if(!obj || !pWidget){
        return;
    }
    lv_color_t fg_color = lv_color_make(EHS_WIDGET_UI(pWidget).xFgColour.sComp.nRed, EHS_WIDGET_UI(pWidget).xFgColour.sComp.nGreen, EHS_WIDGET_UI(pWidget).xFgColour.sComp.nBlue);
    const lv_font_t* font = Ehs_LVGL_find_font(EHS_WIDGET_UI(pWidget).nLineSep);
    lv_obj_set_style_text_color(obj, fg_color, part_id);
    lv_obj_set_style_text_opa(obj, (lv_opa_t) EHS_WIDGET_UI(pWidget).xFgColour.sComp.nAlpha, part_id);
    lv_obj_set_style_text_font(obj, font, part_id);
    lv_obj_set_style_text_line_space(obj, EHS_WIDGET_UI(pWidget).nLineSep, part_id);
}

const char* EhsTargetWidgetUi_set_second_label(struct EhsWidgetStruct* pWidget, const char* text, int font_size, lv_color_t color){
    if(pWidget == NULL || EHS_WIDGET_UI(pWidget).pUiObject == NULL){
        return NULL;
    }
    lv_obj_t* obj = EHS_WIDGET_UI(pWidget).pUiObject;
    // create second label if doesn't exist
    if(lv_obj_get_child_cnt(obj) == 0){
        lv_obj_t* label = lv_label_create(obj);
        // use style property reserved to define label style
        switch (EHS_WIDGET_UI(pWidget).properties)
        {
        case 7:
            lv_obj_set_align(label, LV_ALIGN_OUT_TOP_LEFT);
            break;
        case 6:
            lv_obj_set_align(label, LV_ALIGN_OUT_BOTTOM_LEFT);
            break;
        case 8:
            lv_obj_set_align(label, LV_ALIGN_TOP_MID);
            break;
        case 9:
            /* heatrod style */
            font_size = 12;
            lv_obj_set_align(label, LV_ALIGN_BOTTOM_LEFT);
            break;
        default:
            lv_obj_center(label);
            break;
        }
        lv_obj_set_style_text_color(label, color, LV_PART_MAIN);
        lv_obj_set_style_text_opa(label, (lv_opa_t) EHS_WIDGET_UI(pWidget).xFgColour.sComp.nAlpha, LV_PART_MAIN);
        const lv_font_t* font = Ehs_LVGL_find_font(font_size);
        lv_obj_set_style_text_font(label, font, LV_PART_MAIN);
        lv_obj_set_style_text_line_space(label, font_size, LV_PART_MAIN);
    }
    lv_obj_t* label = lv_obj_get_child(obj, 0);
    lv_label_set_text_fmt(label, "%s", text);
    return lv_label_get_text(label);
}

lv_obj_t* EhsTargetWidgetUi_factory(struct EhsWidgetStruct* pWidget){
    lv_obj_t* obj = NULL;
    switch (EHS_WIDGET_UI(pWidget).id)
    {
        /* Create a basic label widget */
        case EHS_STRING_UI_WIDGET:
        case EHS_BOOL_UI_WIDGET:
        case EHS_INT_UI_WIDGET:
        case EHS_FLOAT_UI_WIDGET:
        {
            obj = EhsTargetWidgetUi_create_label(pWidget);
            break;
        }
        case EHS_OTHER_UI_WIDGET_PANEL:
        {
            obj = EhsTargetWidgetUi_create_patch(pWidget);
            break;
        }
        case EHS_OTHER_UI_WIDGET_IMAGE:
        {
            obj = EhsTargetWidgetUi_create_image(pWidget);
            break;
        }
        case EHS_BOOL_UI_WIDGET_BUTTON:
        {
            #if LV_USE_BTN
            obj = lv_btn_create(lv_scr_act()); // create button object
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_button_clicked_event_handler, LV_EVENT_CLICKED, pWidget);
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_button_pressed_event_handler, LV_EVENT_PRESSED, pWidget);
            EhsTargetWidgetUi_set_lvgl_background(obj, pWidget, LV_PART_MAIN);
            lv_obj_t* btn_label = EhsTargetWidgetUi_create_lvgl_label(obj, pWidget, EHS_TRUE);
            lv_obj_center(btn_label);
            #endif
            break; 
        }
        case EHS_BOOL_UI_WIDGET_TOGGLE_BUTTON:
        {
            #if LV_USE_BTN
            obj = lv_btn_create(lv_scr_act()); // create button object
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
            lv_obj_clear_state(obj, LV_STATE_CHECKED);
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_button_toggle_event_handler, LV_EVENT_VALUE_CHANGED, pWidget);
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_pressed_event_handler, LV_EVENT_PRESSED, pWidget);
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_click_event_handler, LV_EVENT_CLICKED, pWidget);
            EhsTargetWidgetUi_set_lvgl_background(obj, pWidget, LV_PART_MAIN);
            lv_obj_t* btn_label = EhsTargetWidgetUi_create_lvgl_label(obj, pWidget, EHS_TRUE);
            lv_obj_center(btn_label);
            #endif
            break; 
        }
        case EHS_BOOL_UI_WIDGET_CHECK_BOX:
        {
            #if LV_USE_CHECKBOX
            obj = lv_checkbox_create(lv_scr_act());
            lv_checkbox_set_text(obj, "");
            lv_obj_clear_state(obj, LV_STATE_CHECKED);
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_button_toggle_event_handler, LV_EVENT_VALUE_CHANGED, pWidget);
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_pressed_event_handler, LV_EVENT_PRESSED, pWidget);
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_click_event_handler, LV_EVENT_CLICKED, pWidget);
            EhsTargetWidgetUi_set_lvgl_background(obj, pWidget, LV_PART_INDICATOR);
            lv_color_t fg_color = lv_color_make(EHS_WIDGET_UI(pWidget).xFgColour.sComp.nRed, EHS_WIDGET_UI(pWidget).xFgColour.sComp.nGreen, EHS_WIDGET_UI(pWidget).xFgColour.sComp.nBlue);
            lv_obj_set_style_text_color(obj, fg_color, LV_PART_MAIN);
            lv_obj_set_style_text_opa(obj, (lv_opa_t) EHS_WIDGET_UI(pWidget).xFgColour.sComp.nAlpha, LV_PART_MAIN);
            #endif
            break;
        }
        case EHS_INT_UI_WIDGET_SLIDER:
        case EHS_FLOAT_UI_WIDGET_SLIDER:
        {
            #if LV_USE_SLIDER
            obj = lv_slider_create(lv_scr_act());
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_slider_event_handler, LV_EVENT_VALUE_CHANGED, pWidget);
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_pressed_event_handler, LV_EVENT_PRESSED, pWidget);
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_click_event_handler, LV_EVENT_CLICKED, pWidget);
            EhsTargetWidgetUi_set_lvgl_background(obj, pWidget, LV_PART_INDICATOR);
            lv_obj_t * slider_label = EhsTargetWidgetUi_create_lvgl_label(obj, pWidget, EHS_TRUE);
            lv_obj_center(slider_label);
            #endif
            break;
        }
        case EHS_INT_UI_WIDGET_PROGRESS_BAR:
        case EHS_FLOAT_UI_WIDGET_PROGRESS_BAR:
        {
            #if LV_USE_BAR
            obj = lv_bar_create(lv_scr_act());
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE); // make this widget clickable
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_click_event_handler, LV_EVENT_CLICKED, pWidget);
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_pressed_event_handler, LV_EVENT_PRESSED, pWidget);
            EhsTargetWidgetUi_set_lvgl_background(obj, pWidget, LV_PART_INDICATOR);
            lv_bar_set_value(obj, 0, LV_ANIM_OFF);
            lv_obj_t * bar_label = EhsTargetWidgetUi_create_lvgl_label(obj, pWidget, EHS_TRUE);
            lv_obj_center(bar_label);
            #endif
            break;
        }
        case EHS_INT_UI_WIDGET_GAUGE:
        case EHS_FLOAT_UI_WIDGET_GAUGE:
        {
            #if LV_USE_METER
            obj = EhsTargetWidgetUi_create_gauge(pWidget);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE); // make this widget clickable
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_click_event_handler, LV_EVENT_CLICKED, pWidget);
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_gui_pressed_event_handler, LV_EVENT_PRESSED, pWidget);
            EhsTargetWidgetUi_set_lvgl_background(obj, pWidget, LV_PART_MAIN);
            lv_obj_t * meter_label = EhsTargetWidgetUi_create_lvgl_label(obj, pWidget, EHS_FALSE);
            const char* label = (const char*)EHS_WIDGET_UI(pWidget).data;
            lv_obj_center(meter_label);
            int offset = pWidget->xCurRect.nHeight/3;
            lv_obj_set_y(meter_label, offset);
            if(label){
                lv_label_set_text(meter_label, label);
            }else{
                lv_label_set_text(meter_label, "");
            }
            #endif
            break;
        }
        case EHS_OTHER_UI_WIDGET_SPINNER:
        {
            #if LV_USE_SPINNER
            ehs_uint32 time = 1000;
            ehs_uint32 arc_length = 60;
            EhsWidgetUiSpinner* pSpinner = (EhsWidgetUiSpinner*)EHS_WIDGET_UI(pWidget).data;
            if(pSpinner){
                time = pSpinner->time;
                arc_length = pSpinner->arc_length;
            }
            obj = lv_spinner_create(lv_scr_act(), time, arc_length);
            #endif
            break;
        }
        case EHS_OTHER_UI_WIDGET_CHART:
        {
            #if LV_USE_CHART
            EhsWidgetUiChart* pChart = (EhsWidgetUiChart*)EHS_WIDGET_UI(pWidget).data;
            if(pChart){
                obj = lv_chart_create(lv_scr_act());
                EhsTargetWidgetUi_set_lvgl_background(obj, pWidget, LV_PART_MAIN);

                // add callback for chart begin draw to update style
                lv_obj_add_event_cb(obj, chart_draw_begin_event_cb, LV_EVENT_DRAW_PART_BEGIN, (void*)pWidget);

                // remove border
                lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);

                // create style for the graph division lines
                static lv_style_t chart_ticks_style;
                static ehs_bool ticks_style_initialised = EHS_FALSE;
                if(ticks_style_initialised == EHS_FALSE){
                    lv_style_init(&chart_ticks_style);
                    const lv_font_t* font = Ehs_LVGL_find_font(8);
                    lv_style_set_text_font(&chart_ticks_style, font);
                    ticks_style_initialised = EHS_TRUE;
                }
                // set style for the graph division lines
                lv_obj_add_style(obj, &chart_ticks_style, LV_PART_TICKS | LV_STATE_DEFAULT);

                // create style for the base properties of a graph
                static lv_style_t chart_base_style;
                static ehs_bool chart_base_initialised = EHS_FALSE;
                if(chart_base_initialised == EHS_FALSE){
                    lv_style_init(&chart_base_style);
                    lv_style_set_border_width(&chart_base_style, 1);
                    lv_style_set_pad_all(&chart_base_style, 0);
                    chart_base_initialised = EHS_TRUE;
                }
                // set style for the base properties of a graph
                lv_obj_add_style(obj, &chart_base_style, LV_PART_MAIN | LV_STATE_DEFAULT);

                // @TODO - add line type as parameter
                // set line type as parameter
                lv_chart_set_type(obj, LV_CHART_TYPE_LINE);

                // do not display points on the plot lines
                lv_obj_set_style_size(obj, 0, LV_PART_INDICATOR);

                // set point count for the chart
                ehs_uint16 point_count = 0;
                if(pChart->data1_size > pChart->data2_size){
                    point_count = pChart->data1_size;
                }else{
                    point_count = pChart->data2_size;
                }
                if(point_count > 0){
                    lv_chart_set_point_count(obj, point_count);
                }

                uint8_t hdiv = 5;
                uint8_t vdiv = 9;

                // set number of division lines
                lv_chart_set_div_line_count(obj, hdiv, vdiv);

                /* data1 series */
                if(pChart->data1_size > 0){
                    lv_chart_set_range(obj, LV_CHART_AXIS_PRIMARY_X, pChart->xmin, pChart->xmax);
                    lv_chart_set_range(obj, LV_CHART_AXIS_PRIMARY_Y, pChart->ymin, pChart->ymax);

                    lv_chart_set_axis_tick(obj, LV_CHART_AXIS_PRIMARY_X, 10, 0, vdiv, 1, true, 30);
                    lv_chart_set_axis_tick(obj, LV_CHART_AXIS_PRIMARY_Y, 10, 0, hdiv, 1, true, 30);

                    lv_chart_series_t * ser1 = lv_chart_add_series(obj, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
                    for(int i = 0; i < pChart->data1_size; i++){
                        lv_chart_set_next_value(obj, ser1, 0);
                    }
                    pChart->data1 = (ehs_sint16*)&ser1->y_points[0];
                }
                
                /* data2 series */
                if(pChart->data2_size > 0){
                    // @TODO - range for secondary plot should be different
                    lv_chart_set_range(obj, LV_CHART_AXIS_SECONDARY_X, pChart->xmin, pChart->xmax);
                    lv_chart_set_range(obj, LV_CHART_AXIS_SECONDARY_Y, pChart->ymin, pChart->ymax);
                    lv_chart_series_t * ser2 = lv_chart_add_series(obj, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_SECONDARY_Y);
                    for(int i = 0; i < pChart->data2_size; i++){
                        lv_chart_set_next_value(obj, ser2, 0);
                    }
                    pChart->data2 = (ehs_sint16*)&ser2->y_points[0];
                }                

                // @TODO - add zoom in/out options

                /* title */
                if(pChart->title[0] != '\0'){
                    lv_obj_t * chart_label = lv_label_create(obj);
                    lv_label_set_text(chart_label, pChart->title);
                }

                lv_chart_refresh(obj);
            }
            #endif
            break;
        }
        case EHS_OTHER_UI_WIDGET_LIST:
        {
            #if LV_USE_LIST

            obj = lv_list_create(lv_scr_act());
            EhsTargetWidgetUi_set_lvgl_background(obj, pWidget, LV_PART_MAIN);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);
            lv_obj_set_style_pad_row(obj, 5, 0);
            lv_obj_set_style_pad_left(obj, 5, 0);
            lv_obj_set_style_pad_top(obj, 5, 0);

            EhsWidgetUiList* gui = (EhsWidgetUiList*)EHS_WIDGET_UI(pWidget).data;
            if(gui){
                if(gui->reversed){
                    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN_REVERSE);
                }
            }
            
            #endif
            break;
        }
        case EHS_INT_UI_WIDGET_LIST_BOX:
        {
            #if LV_USE_DROPDOWN

            obj = lv_dropdown_create(lv_scr_act());
            EhsTargetWidgetUi_set_lvgl_background(obj, pWidget, LV_PART_MAIN);
            EhsTargetWidgetUi_set_lvgl_obj_text_style(obj, pWidget, LV_PART_MAIN);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_drop_down_list_event_handler, LV_EVENT_VALUE_CHANGED, (void*)pWidget);
            lv_dropdown_set_options(obj, "Empty" ); // set list to be empty by default

            #endif
            break;
        }
        case EHS_STRING_UI_TEXT_FIELD:
        {
            #if LV_USE_TEXTAREA

            obj = lv_textarea_create(lv_scr_act());
            EhsTargetWidgetUi_set_lvgl_background(obj, pWidget, LV_PART_MAIN);
            EhsTargetWidgetUi_set_lvgl_obj_text_style(obj, pWidget, LV_PART_MAIN);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_text_field_event_handler, LV_EVENT_ALL, (void*)pWidget);
            lv_textarea_set_one_line(obj, true);
            // TODO - use styles to define if it's password
            //lv_textarea_set_password_mode(obj, true);
            
            #endif
            break;
        }
        case EHS_FLOAT_UI_WIDGET_ROLLER:
        {
            #if LV_USE_ROLLER
            // create a background for the roller object
            obj = lv_obj_create(lv_scr_act());
            EhsTargetWidgetUi_set_lvgl_background(obj, pWidget, LV_PART_MAIN);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);
            lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
            lv_obj_set_style_pad_column(obj, 5, 0);
            #endif
            break;
        }
#if EHS_PERIPHERALS_GPIO_SUPPORT == EHS_PERIPHERALS_GPIO_TYPE_GUI
        case EHS_SPECIAL_UI_GPIO_PANEL:
        case EHS_SPECIAL_UI_GPIO_OUT:
        case EHS_SPECIAL_UI_GPIO_IN:
        {
            obj = EhsTargetWidget_create_gpio(pWidget);
            break;
        }
#endif
        //case EHS_WIDGET_UI_ ... :
        //{
        //    break;
        //}
        default:
            // unsupported 
            break;
    }
    return obj;
}

void EhsTargetWidgetUi_update(struct EhsWidgetStruct* pWidget)
{
    switch (EHS_WIDGET_UI(pWidget).id)
    {
        case EHS_STRING_UI_WIDGET:
        {
            ehs_uint16 event_id = 0;
            const char* value = NULL; const char* label = NULL;
            EhsWidgetUi* gui = (EhsWidgetUi*)EHS_WIDGET_UI(pWidget).data;
            if(gui){
                if(gui->data){
                    const char* str_value = (const char*)gui->data;
                    lv_label_set_text_fmt(EHS_WIDGET_UI(pWidget).pUiObject, "%s", str_value);
                    value = lv_label_get_text(EHS_WIDGET_UI(pWidget).pUiObject);
                    event_id |= EHS_WIDGET_UI_EVENT_DATA_UPDATED;
                }
                if(gui->label){
                    label = EhsTargetWidgetUi_set_second_label(pWidget, gui->label, 10, lv_color_make(50,50,50));
                    event_id |= EHS_WIDGET_UI_EVENT_LABEL_UPDATED;
                }
                EhsTargetWidgetUi_gui_data_value_event_handler(pWidget, event_id, label, (void*)value);
            }
            break;
        }
        case EHS_BOOL_UI_WIDGET:
        {
            ehs_uint16 event_id = 0;
            ehs_bool value = EHS_FALSE; const char* label = NULL;
            EhsWidgetUi* gui = (EhsWidgetUi*)EHS_WIDGET_UI(pWidget).data;
            if(gui){
                if(gui->data){
                    value = *((ehs_bool*)gui->data);
                    lv_label_set_text_fmt(EHS_WIDGET_UI(pWidget).pUiObject, "%d", value);
                    event_id |= EHS_WIDGET_UI_EVENT_DATA_UPDATED;
                }
                if(gui->label){
                    label = EhsTargetWidgetUi_set_second_label(pWidget, gui->label, 10, lv_color_make(50,50,50));
                    event_id |= EHS_WIDGET_UI_EVENT_LABEL_UPDATED;
                }
                EhsTargetWidgetUi_gui_data_value_event_handler(pWidget, event_id, label, (void*)&value);
            }
            break;
        }
        case EHS_INT_UI_WIDGET:
        {
            ehs_uint16 event_id = 0;
            ehs_sint32 value = EHS_FALSE; const char* label = NULL;
            EhsWidgetUi* gui = (EhsWidgetUi*)EHS_WIDGET_UI(pWidget).data;
            if(gui){
                if(gui->data){
                    value = *((ehs_sint32*)gui->data);
                    lv_label_set_text_fmt(EHS_WIDGET_UI(pWidget).pUiObject, "%d", value);
                    event_id |= EHS_WIDGET_UI_EVENT_DATA_UPDATED;
                }
                if(gui->label){
                    label = EhsTargetWidgetUi_set_second_label(pWidget, gui->label, 10, lv_color_make(50,50,50));
                    event_id |= EHS_WIDGET_UI_EVENT_LABEL_UPDATED;
                }
                EhsTargetWidgetUi_gui_data_value_event_handler(pWidget, event_id, label, (void*)&value);
            }
            break;
        }
        case EHS_FLOAT_UI_WIDGET:
        {
            ehs_uint16 event_id = 0;
            float value = EHS_FALSE; const char* label = NULL;
            EhsWidgetUi* gui = (EhsWidgetUi*)EHS_WIDGET_UI(pWidget).data;
            if(gui){
                if(gui->data){
                    value = (float)*((double*)gui->data);
                    const char* float_format_str = EhsTargetWidgetUi_float_format_str(pWidget);
                    lv_label_set_text_fmt(EHS_WIDGET_UI(pWidget).pUiObject, float_format_str, value);
                    event_id |= EHS_WIDGET_UI_EVENT_DATA_UPDATED;
                }
                if(gui->label){
                    label = EhsTargetWidgetUi_set_second_label(pWidget, gui->label, 10, lv_color_make(50,50,50));
                    event_id |= EHS_WIDGET_UI_EVENT_LABEL_UPDATED;
                }
                EhsTargetWidgetUi_gui_data_value_event_handler(pWidget, event_id, label, (void*)&value);
            }
            break;
        }
        case EHS_OTHER_UI_WIDGET_PANEL:
        {
            // do nothing for now
            break;
        }
        case EHS_OTHER_UI_WIDGET_IMAGE:
        {
            // do nothing for now
            break;
        }
        case EHS_BOOL_UI_WIDGET_BUTTON:
        {
            #if LV_USE_BTN
            EhsWidgetUi* gui = (EhsWidgetUi*)EHS_WIDGET_UI(pWidget).data;
            if(gui){
                lv_obj_t * btn_label = lv_obj_get_child(EHS_WIDGET_UI(pWidget).pUiObject, 0);
                if(gui->label){
                    lv_label_set_text(btn_label, gui->label);
                }else{
                    lv_label_set_text(btn_label, "");
                }
                EhsTargetWidgetUi_gui_data_value_event_handler(pWidget, EHS_WIDGET_UI_EVENT_LABEL_UPDATED, gui->label, NULL);
            }
            #endif
            break; 
        }
        case EHS_BOOL_UI_WIDGET_TOGGLE_BUTTON:
        {
            #if LV_USE_BTN
            EhsWidgetUi* gui = (EhsWidgetUi*)EHS_WIDGET_UI(pWidget).data;
            if(gui){
                lv_obj_t * btn_label = lv_obj_get_child(EHS_WIDGET_UI(pWidget).pUiObject, 0);
                if(gui->label){
                    lv_label_set_text(btn_label, gui->label);
                }else{
                    lv_label_set_text(btn_label, "");
                }
                if(gui->data){
                    ehs_bool button_state = *((ehs_bool*)gui->data);
                    if(button_state == EHS_TRUE){
                        lv_obj_add_state(EHS_WIDGET_UI(pWidget).pUiObject, LV_STATE_CHECKED);
                    }else{
                        lv_obj_clear_state(EHS_WIDGET_UI(pWidget).pUiObject, LV_STATE_CHECKED);
                    }
                }
                EhsTargetWidgetUi_gui_data_value_event_handler(pWidget, EHS_WIDGET_UI_EVENT_DATA_UPDATED|EHS_WIDGET_UI_EVENT_LABEL_UPDATED, gui->label, gui->data);
            }
            #endif
            break; 
        }
        case EHS_BOOL_UI_WIDGET_CHECK_BOX:
        {
            #if LV_USE_CHECKBOX
            EhsWidgetUi* gui = (EhsWidgetUi*)EHS_WIDGET_UI(pWidget).data;
            if(gui){
                lv_obj_t * check_box = EHS_WIDGET_UI(pWidget).pUiObject;
                if(gui->label){
                    lv_checkbox_set_text(check_box, gui->label);
                }else{
                    lv_checkbox_set_text(check_box, "");
                }
                if(gui->data){
                    ehs_bool button_state = *((ehs_bool*)gui->data);
                    if(button_state == EHS_TRUE){
                        lv_obj_add_state(EHS_WIDGET_UI(pWidget).pUiObject, LV_STATE_CHECKED);
                    }else{
                        lv_obj_clear_state(EHS_WIDGET_UI(pWidget).pUiObject, LV_STATE_CHECKED);
                    }
                }
                EhsTargetWidgetUi_gui_data_value_event_handler(pWidget, EHS_WIDGET_UI_EVENT_DATA_UPDATED|EHS_WIDGET_UI_EVENT_LABEL_UPDATED, gui->label, gui->data);
            }
            #endif
            break;
        }
        case EHS_INT_UI_WIDGET_SLIDER:
        case EHS_FLOAT_UI_WIDGET_SLIDER:
        {
            #if LV_USE_SLIDER
            EhsWidgetUi* gui = (EhsWidgetUi*)EHS_WIDGET_UI(pWidget).data;
            if(gui){
                lv_obj_t * slider = EHS_WIDGET_UI(pWidget).pUiObject;
                lv_obj_t * slider_label = lv_obj_get_child(slider, 0);
                if(gui->label){
                    lv_label_set_text(slider_label, gui->label);
                }else{
                    lv_label_set_text(slider_label, "");
                }
                ehs_sint32 int_value = 0; float float_value = 0.0f;
                void* slider_data = NULL;
                if(gui->data){
                    if(EHS_WIDGET_UI(pWidget).id == EHS_FLOAT_UI_WIDGET_SLIDER){
                        float_value = (float)*((double*)gui->data);
                        slider_data = &float_value;
                        int_value = (ehs_sint32)float_value; 
                    }else{
                        int_value = *((ehs_sint32*)gui->data);
                        slider_data = &int_value;
                    }
                    lv_slider_set_value(slider, int_value, LV_ANIM_OFF);
                }
                EhsTargetWidgetUi_gui_data_value_event_handler(pWidget, EHS_WIDGET_UI_EVENT_DATA_UPDATED|EHS_WIDGET_UI_EVENT_LABEL_UPDATED, gui->label, slider_data);
            }
            #endif
            break;
        }
        case EHS_INT_UI_WIDGET_PROGRESS_BAR:
        case EHS_FLOAT_UI_WIDGET_PROGRESS_BAR:
        {
            #if LV_USE_BAR
            EhsWidgetUi* gui = (EhsWidgetUi*)EHS_WIDGET_UI(pWidget).data;
            if(gui){
                lv_obj_t * bar = EHS_WIDGET_UI(pWidget).pUiObject;
                lv_obj_t * bar_label = lv_obj_get_child(bar, 0);
                if(gui->label){
                    lv_label_set_text(bar_label, gui->label);
                }else{
                    lv_label_set_text(bar_label, "");
                }
                ehs_sint32 int_value = 0; float float_value = 0.0f;
                void* bar_data = NULL;
                if(gui->data){
                    if(EHS_WIDGET_UI(pWidget).id == EHS_FLOAT_UI_WIDGET_PROGRESS_BAR){
                        float_value = (float)*((double*)gui->data);
                        bar_data = &float_value;
                        int_value = (ehs_sint32)float_value; 
                    }else{
                        int_value = *((ehs_sint32*)gui->data);
                        bar_data = &int_value;
                    }
                    lv_bar_set_value(bar, int_value, LV_ANIM_OFF);
                }
                EhsTargetWidgetUi_gui_data_value_event_handler(pWidget, EHS_WIDGET_UI_EVENT_DATA_UPDATED|EHS_WIDGET_UI_EVENT_LABEL_UPDATED, gui->label, bar_data);
            }
            #endif
            break;
        }
        case EHS_INT_UI_WIDGET_GAUGE:
        case EHS_FLOAT_UI_WIDGET_GAUGE:
        {
            #if LV_USE_METER
            EhsWidgetUi* gui = (EhsWidgetUi*)EHS_WIDGET_UI(pWidget).data;
            if(gui){
                lv_obj_t * meter = EHS_WIDGET_UI(pWidget).pUiObject;
                lv_obj_t * meter_label = lv_obj_get_child(meter, 0);
                if(gui->label){
                    lv_label_set_text(meter_label, gui->label);
                }else{
                    lv_label_set_text(meter_label, "");
                }
                ehs_sint32 int_value = 0; float float_value = 0.0f;
                void* meter_data = NULL;
                if(gui->data){
                    if(EHS_WIDGET_UI(pWidget).id == EHS_FLOAT_UI_WIDGET_GAUGE){
                        float_value = (float)*((double*)gui->data);
                        meter_data = &float_value;
                        int_value = (ehs_sint32)float_value; 
                    }else{
                        int_value = *((ehs_sint32*)gui->data);
                        meter_data = &int_value;
                    }
                    lv_meter_indicator_t * indic = (lv_meter_indicator_t*)meter->user_data;
                    lv_meter_set_indicator_value(meter, indic, int_value);
                }
                EhsTargetWidgetUi_gui_data_value_event_handler(pWidget, EHS_WIDGET_UI_EVENT_DATA_UPDATED|EHS_WIDGET_UI_EVENT_LABEL_UPDATED, gui->label, meter_data);
            }
            #endif
            break;
        }
        case EHS_OTHER_UI_WIDGET_CHART:
        {
            #if LV_USE_CHART
            lv_chart_refresh(EHS_WIDGET_UI(pWidget).pUiObject);
            #endif
            break;
        }
        case EHS_OTHER_UI_WIDGET_LIST:
        {
            #if LV_USE_LIST
            EhsWidgetUiList* gui = (EhsWidgetUiList*)EHS_WIDGET_UI(pWidget).data;
            if(gui){
                lv_obj_t* list = (lv_obj_t*)EHS_WIDGET_UI(pWidget).pUiObject;
                if(list){
                    if(gui->state == EHS_UI_WIDGET_LIST_APPEND){ /* append object to the list */
                        if(gui->item[0] != '\0' && gui->max_size > 0){
                            uint32_t count = lv_obj_get_child_cnt(list);
                            if(count >= gui->max_size){
                                lv_obj_t* child = lv_obj_get_child(list, 0);
                                if(child){
                                    lv_obj_del(child);
                                }
                            }
                            lv_obj_t* lab = lv_label_create(list);
                            EhsTargetWidgetUi_set_lvgl_background(lab, pWidget, LV_PART_MAIN);
                            const lv_font_t* font = Ehs_LVGL_find_font(EHS_WIDGET_UI(pWidget).nLineSep);
                            lv_obj_set_style_text_font(lab, font, LV_PART_MAIN);
                            lv_obj_set_style_text_line_space(lab, EHS_WIDGET_UI(pWidget).nLineSep, LV_PART_MAIN);
                            ehs_uint32 red   = (gui->rgb & 0x00ff0000) >> 16;
                            ehs_uint32 green = (gui->rgb & 0x0000ff00) >> 8;
                            ehs_uint32 blue  = (gui->rgb & 0x000000ff);
                            lv_color_t fg_color = lv_color_make(red, green, blue);
                            lv_obj_set_style_text_color(lab, fg_color, LV_PART_MAIN);
                            lv_label_set_text_fmt(lab, "%s", gui->item);
                        }
                    }else if(gui->state == EHS_UI_WIDGET_LIST_CLEAR){ /* remove object from the list */
                        lv_obj_clean(list);
                    }
                }
            }
            #endif
            break;
        }
        case EHS_INT_UI_WIDGET_LIST_BOX:
        {
            #if LV_USE_DROPDOWN
            EhsWidgetUi* gui = (EhsWidgetUi*)EHS_WIDGET_UI(pWidget).data;
            if(gui){
                lv_obj_t * ddlist = EHS_WIDGET_UI(pWidget).pUiObject;
                if(gui->label){
                    char* options = (char*)gui->label;
                    int len = EhsStrlen(options);
                    for(int i = 0; i < len; i++){
                        if(options[i] == ',') options[i] = '\n';
                    }
                    lv_dropdown_set_options(ddlist, options );
                }else{
                    // clear list
                    lv_dropdown_set_options(ddlist, "Empty" );
                }
                if(gui->data){
                    ehs_sint32 value = *((ehs_sint32*)gui->data);
                    lv_dropdown_set_selected(ddlist, value);
                    char buf[EHS_DROP_DOWN_LIST_ITEM_MAX_SIZE];
                    lv_dropdown_get_selected_str(ddlist, buf, sizeof(buf));
                    EHS_WIDGET_UI(pWidget).event_callback(pWidget, EHS_WIDGET_UI_EVENT_DATA_UPDATED|EHS_WIDGET_UI_EVENT_LABEL_UPDATED, buf, &value);
                }
            }
            #endif
            break;
        }
        case EHS_STRING_UI_TEXT_FIELD:
        {
            #if LV_USE_TEXTAREA
            EhsWidgetUi* gui = (EhsWidgetUi*)EHS_WIDGET_UI(pWidget).data;
            if(gui){
                ehs_uint16 event_id = 0;
                const char* value = NULL;
                const char* label = NULL;
                lv_obj_t * ta = EHS_WIDGET_UI(pWidget).pUiObject;
                const char* placeholder = (const char*)lv_textarea_get_placeholder_text(ta);
                if(gui->label){
                    label = (const char*)gui->label; // placeholder
                    if(placeholder == NULL || strcmp(label,placeholder)){
                        lv_textarea_set_placeholder_text(ta, label);
                        event_id |= EHS_WIDGET_UI_EVENT_LABEL_UPDATED;
                    }
                }else{
                    if(placeholder != NULL && strlen(placeholder) > 0){
                        lv_textarea_set_placeholder_text(ta, "");
                    }
                }
                if(gui->data){
                    value = gui->data;
                    lv_textarea_set_text(ta, value);
                    event_id |= EHS_WIDGET_UI_EVENT_DATA_UPDATED;
                }
                EhsTargetWidgetUi_gui_data_value_event_handler(pWidget, event_id, label, value);
            }
            #endif
            break;
        }
        case EHS_FLOAT_UI_WIDGET_ROLLER:
        {
            #if LV_USE_ROLLER
            EhsWidgetUi* gui = (EhsWidgetUi*)EHS_WIDGET_UI(pWidget).data;
            if(gui){
                ehs_uint16 event_id = 0;
                float value = 0;
                const char* label = NULL;
                lv_obj_t * roller_view = EHS_WIDGET_UI(pWidget).pUiObject;
                if(roller_view){
                    lv_obj_clean(roller_view);
                }
                if(gui->label){
                    char* options = (char*)gui->label;
                    // replace ',' deliminator with a new line
                    int len = EhsStrlen(options);
                    for(int i = 0; i < len; i++){
                        if(options[i] == ',') options[i] = '\n';
                    }
                    // create all roller objects
                    char option[EHS_ROLLER_OPTION_SIZE] = {0};
                    do{
                        options = (char*)EhsTargetWidgetUi_roller_numeric_next_option(options, option, EHS_ROLLER_OPTION_SIZE);
                        if(option[0] != '\0'){
                            lv_obj_t* roller = EhsTargetWidgetUi_create_roller_widget(roller_view, 0, 0, option);
                            lv_obj_add_event_cb(roller, EhsTargetWidgetUi_numeric_roller_event_handler, LV_EVENT_VALUE_CHANGED, (void*)pWidget);
                            lv_obj_center(roller);
                            //lv_obj_set_width(roller, 50);
                        }
                        option[0] = '\0';
                    }while(options != NULL);
                }
                if(gui->data){
                    // @TODO - compleate this
                    value = (float)*((double*)gui->data);
                    char buf[EHS_ROLLER_ENTRY_SIZE] = {'\0'};
                    EhsSprintf(buf, "%.1f", value);
                    
                }
                EhsTargetWidgetUi_gui_data_value_event_handler(pWidget, event_id, label, (void*)&value);
            }
            #endif
            break;
        }
#if EHS_PERIPHERALS_GPIO_SUPPORT == EHS_PERIPHERALS_GPIO_TYPE_GUI
        case EHS_SPECIAL_UI_GPIO_PANEL:
        case EHS_SPECIAL_UI_GPIO_OUT:
        case EHS_SPECIAL_UI_GPIO_IN:
        {
            EhsTargetWidget_update_gpio(pWidget);
            break;
        }
#endif
        //case EHS_WIDGET_UI_ ... :
        //{
        //    break;
        //}
        default:
            // unsupported 
            break;
    }
}

void EhsTargetWidgetUi_create_lvgl(struct EhsWidgetStruct* pWidget)
{
    if (EHS_WIDGET_UI(pWidget).pUiObject != NULL) 
    {
        lv_obj_del((lv_obj_t*) EHS_WIDGET_UI(pWidget).pUiObject);
        EHS_WIDGET_UI(pWidget).pUiObject = NULL;
    }

    lv_obj_t * obj = EhsTargetWidgetUi_factory(pWidget);

    if(obj != NULL){
        
        EHS_WIDGET_UI(pWidget).pUiObject = obj;

        lv_obj_set_size(obj, pWidget->xCurRect.nWidth, pWidget->xCurRect.nHeight);
        lv_obj_set_pos(obj, pWidget->xCurRect.nLeft, pWidget->xCurRect.nTop);
        // @todo - add z-order of widgets
        // @todo - make alpha working on ui widgets
        EhsTargetWidgetUi_style(obj, pWidget);
        
        EhsTargetWidget_hide(obj);
    }else{
        // @todo - report error
    }
}

void EhsTargetWidgetUi_create(struct EhsWidgetStruct* pWidget, EhsTVClass* pViewport)
{
    // EhsTPMutex_lock(EhsTPMutex_viewport); // mutex already called in 'EhsWidget_create' function

    EHS_WIDGET_UI(pWidget).nUiState |= EHS_WIDGET_UI_STATE_CREATE; // set flag to create this widget in the lvgl loop
    gEhsLvglState |= EHS_LVGL_STATE_UPDATE_WIDGET; // set flag to notify loop about widget changes
    
    // EhsTPMutex_unlock(EhsTPMutex_viewport); // mutex already called in 'EhsWidget_create' function
}

void EhsTargetWidgetUi_destroy(struct EhsWidgetStruct* pWidget)
{
    // EhsTPMutex_lock(EhsTPMutex_viewport); // mutex already called in 'EhsWidget_create' function

    EHS_WIDGET_UI(pWidget).nUiState |= EHS_WIDGET_UI_STATE_DESTROY; // set flag to destroy this widget in the lvgl loop
    gEhsLvglState |= EHS_LVGL_STATE_UPDATE_WIDGET; // set flag to notify loop about widget changes

    // EhsTPMutex_unlock(EhsTPMutex_viewport); // mutex already called in 'EhsWidget_create' function
}

void EhsTargetWidgetUi_maintain_size_ratio(struct EhsWidgetStruct* pWidget, lv_obj_t* obj)
{
    if(!pWidget || !obj){
        return;
    }
    // check if widget needs to have it's size ratio maintained, e.g gauge
    switch (EHS_WIDGET_UI(pWidget).id)
    {
        case EHS_INT_UI_WIDGET_GAUGE:
        case EHS_FLOAT_UI_WIDGET_GAUGE:
        {
            pWidget->xCurRect.nHeight = pWidget->xCurRect.nWidth;
            pWidget->xOrigRect.nHeight = pWidget->xOrigRect.nWidth;
            break;
        }
        default:
            break;
    }
}

void EhsTargetWidgetUi_draw_lvgl(struct EhsWidgetStruct* pWidget)
{
    if(pWidget->bContentUpdated == EHS_TRUE)
    {
        if (EHS_WIDGET_UI(pWidget).pUiObject != NULL)
        {
            EhsTargetWidgetUi_update(pWidget);
        }
        pWidget->bContentUpdated = EHS_FALSE;
    }

    if (pWidget->bPositionUpdated == EHS_TRUE)
    {
        if (EHS_WIDGET_UI(pWidget).pUiObject != NULL)
        {
            EhsTargetWidgetUi_maintain_size_ratio(pWidget, EHS_WIDGET_UI(pWidget).pUiObject);
            lv_obj_set_size(EHS_WIDGET_UI(pWidget).pUiObject, pWidget->xCurRect.nWidth, pWidget->xCurRect.nHeight);
            lv_obj_set_pos(EHS_WIDGET_UI(pWidget).pUiObject, pWidget->xCurRect.nLeft, pWidget->xCurRect.nTop);
        }
        pWidget->bPositionUpdated = EHS_FALSE;
    }
    if (pWidget->bColourUpdated == EHS_TRUE)
    {
        if (EHS_WIDGET_UI(pWidget).pUiObject != NULL)
        {
            if(EHS_WIDGET_UI(pWidget).id == EHS_OTHER_UI_WIDGET_IMAGE){
                lv_obj_set_style_img_opa(EHS_WIDGET_UI(pWidget).pUiObject, pWidget->nAlpha, LV_PART_MAIN);
            }else{
                lv_obj_set_style_bg_opa(EHS_WIDGET_UI(pWidget).pUiObject, pWidget->nAlpha, LV_PART_MAIN);
                lv_obj_set_style_text_opa(EHS_WIDGET_UI(pWidget).pUiObject, pWidget->nAlpha, LV_PART_MAIN);
            }
        }
        pWidget->bColourUpdated = EHS_FALSE;
    }
}

void EhsTargetWidgetUi_draw(struct EhsWidgetStruct* pWidget)
{
    EhsTPMutex_lock(EhsTPMutex_viewport);

    EHS_WIDGET_UI(pWidget).nUiState |= EHS_WIDGET_UI_STATE_UPDATE; // set flag to update this widget in the lvgl loop
    gEhsLvglState |= EHS_LVGL_STATE_UPDATE_WIDGET; // set flag to notify loop about widget changes

    EhsTPMutex_unlock(EhsTPMutex_viewport);
}

void EhsTargetWidget_show(struct EhsWidgetStruct* pWidget, ehs_uint8 nState)
{
    // EhsTPMutex_lock(EhsTPMutex_viewport); // mutex already called in 'EhsWidget_create' function

    EHS_WIDGET_UI(pWidget).nUiState |= EHS_WIDGET_UI_STATE_SHOW; // set flag to update shown state if this widget in the lvgl loop
    gEhsLvglState |= EHS_LVGL_STATE_UPDATE_WIDGET; // set flag to notify loop about widget changes

    // EhsTPMutex_unlock(EhsTPMutex_viewport); // mutex already called in 'EhsWidget_create' function

    // move to lvgl loop
    if(EHS_WIDGET_UI(pWidget).pUiObject){
        if(EHS_WIDGET_STATE_SHOWN(nState)){
            EhsTargetWidget_unhide(EHS_WIDGET_UI(pWidget).pUiObject);
        }else{
            EhsTargetWidget_hide(EHS_WIDGET_UI(pWidget).pUiObject);
        }
    }
}

void EhsTargetWidgetUi_curvature(lv_obj_t* obj, struct EhsWidgetStruct* pWidget)
{
    switch (EHS_WIDGET_UI(pWidget).id)
    {
#if EHS_PERIPHERALS_GPIO_SUPPORT == EHS_PERIPHERALS_GPIO_TYPE_GUI
        case EHS_SPECIAL_UI_GPIO_PANEL:
        case EHS_SPECIAL_UI_GPIO_OUT:
        case EHS_SPECIAL_UI_GPIO_IN:
#endif
        case EHS_BOOL_UI_WIDGET_CHECK_BOX:
        case EHS_OTHER_UI_WIDGET_IMAGE:
        case EHS_INT_UI_WIDGET_GAUGE:
        case EHS_FLOAT_UI_WIDGET_GAUGE:
        case EHS_OTHER_UI_WIDGET_SPINNER:
        {
            // unsupported
            break;
        }
        default:
            lv_obj_set_style_radius(obj, EHS_WIDGET_UI(pWidget).curvature, LV_PART_MAIN);
            break;
    };
}

// filter objects where standard styles do not apply
ehs_bool EhsTargetWidgetUi_exclude_styles(struct EhsWidgetStruct* pWidget)
{
    switch (EHS_WIDGET_UI(pWidget).id)
    {
        case EHS_OTHER_UI_WIDGET_IMAGE:
        case EHS_OTHER_UI_WIDGET_CHART:
        case EHS_INT_UI_WIDGET_GAUGE:
        case EHS_FLOAT_UI_WIDGET_GAUGE:
        {
            return EHS_TRUE;
        }
        default:
            // unsupported 
            break;
    };
    return EHS_FALSE;
}

lv_style_t* EhsTargetWidgetUi_style1()
{
    static ehs_bool created = EHS_FALSE;
    static lv_style_t style;
    if(created == EHS_TRUE){
        return &style;
    }
    lv_style_init(&style);

    /*Add a shadow*/
    lv_style_set_shadow_width(&style, 8);
    lv_style_set_shadow_color(&style, lv_palette_lighten(LV_PALETTE_BLUE_GREY, 1));
    lv_style_set_shadow_ofs_x(&style, 10);
    lv_style_set_shadow_ofs_y(&style, 10);

    created = EHS_TRUE;
    return &style;
}

lv_style_t* EhsTargetWidgetUi_style2()
{
    static ehs_bool created = EHS_FALSE;
    static lv_style_t style;
    if(created == EHS_TRUE){
        return &style;
    }
    lv_style_init(&style);
    /*Make a gradient*/
    lv_style_set_bg_grad_color(&style, lv_palette_lighten(LV_PALETTE_GREY, 1));
    lv_style_set_bg_grad_dir(&style, LV_GRAD_DIR_VER);

    /*Shift the gradient to the bottom*/
    lv_style_set_bg_main_stop(&style, 128);
    lv_style_set_bg_grad_stop(&style, 192);

    created = EHS_TRUE;
    return &style;
}

lv_style_t* EhsTargetWidgetUi_style3()
{
    /* to do implement this as the above */
    return NULL;
}

lv_style_t* EhsTargetWidgetUi_style4()
{
    /* to do implement this as the above */
    return NULL;
}


void EhsTargetWidgetUi_style(lv_obj_t* obj, struct EhsWidgetStruct* pWidget)
{
    if(!obj || !pWidget){
        return;
    }
    // apply this to object that can have round corners
    EhsTargetWidgetUi_curvature(obj, pWidget);
    // apply style if not default
    lv_style_t* style = NULL;
    ehs_uint16 style_index = EHS_WIDGET_UI(pWidget).properties;
    switch (style_index)
    {
        case 1:{
            style = EhsTargetWidgetUi_style1();
            break;
        }
        case 2:{
            style = EhsTargetWidgetUi_style2();
            break;
        }
        case 3:{
            style = EhsTargetWidgetUi_style3();
            break;
        }
        case 4:{
            style = EhsTargetWidgetUi_style4();
            break;
        }
        case 7:
        case 6:
        case 8:
        case 9:{
            //////////////////////////////////////////////////////////////
            /// reserved for widget label styles. do not overwrite it! ///
            //////////////////////////////////////////////////////////////
            return;
        }
        default:{
            break;
        }
    }
    if(style){
        lv_obj_add_style(obj, style, LV_PART_MAIN);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// GPIO UI ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#if EHS_PERIPHERALS_GPIO_SUPPORT == EHS_PERIPHERALS_GPIO_TYPE_GUI

lv_obj_t* EhsTargetWidgetUi_create_gpio_label(lv_obj_t* obj, int gpio_num, struct EhsWidgetStruct* pWidget)
{
    lv_obj_t* label_obj = lv_label_create(obj);
    lv_color_t fg_color = lv_color_make(EHS_WIDGET_UI(pWidget).xFgColour.sComp.nRed, EHS_WIDGET_UI(pWidget).xFgColour.sComp.nGreen, EHS_WIDGET_UI(pWidget).xFgColour.sComp.nBlue);
    lv_obj_set_style_text_color(label_obj, fg_color, LV_PART_MAIN);
    lv_obj_set_style_text_opa(label_obj, (lv_opa_t) EHS_WIDGET_UI(pWidget).xFgColour.sComp.nAlpha, LV_PART_MAIN);
    lv_label_set_text_fmt(label_obj, "%d", gpio_num);
    lv_obj_align(label_obj, LV_ALIGN_CENTER, 0, 0);
    return label_obj;
}

lv_obj_t* EhsTargetWidget_create_gpio(struct EhsWidgetStruct* pWidget)
{
    lv_obj_t * obj = NULL;

    switch(EHS_WIDGET_UI(pWidget).id){
        case EHS_SPECIAL_UI_GPIO_PANEL:
        {
            break;
        }
        case EHS_SPECIAL_UI_GPIO_OUT:
        {
            #if LV_USE_LED
            obj  = lv_led_create(lv_scr_act());
            EhsTargetWidgetUi_create_gpio_label(obj, EHS_WIDGET_UI(pWidget).properties, pWidget);
            lv_led_off(obj);
            #endif
            break;
        }
        case EHS_SPECIAL_UI_GPIO_IN:
        {
            #if LV_USE_SWITCH
            obj = lv_switch_create(lv_scr_act());
            EhsTargetWidgetUi_create_gpio_label(obj, EHS_WIDGET_UI(pWidget).properties, pWidget);
            lv_obj_add_event_cb(obj, EhsTargetWidgetUi_button_toggle_event_handler, LV_EVENT_VALUE_CHANGED, (void*)pWidget);
            #endif
            break;
        }
        default:
            break;
    }

    return obj;
}

void EhsTargetWidget_update_gpio(struct EhsWidgetStruct* pWidget)
{
    switch(EHS_WIDGET_UI(pWidget).id){
        case EHS_SPECIAL_UI_GPIO_PANEL:
        {
            break;
        }
        case EHS_SPECIAL_UI_GPIO_OUT:
        {
            #if LV_USE_LED
            ehs_gpio_out_state_type* gpio = (EhsWidgetUi*)EHS_WIDGET_UI(pWidget).data;
            if(EHS_WIDGET_UI(pWidget).pUiObject && gpio){
                if(gpio->pin_value){
                    lv_led_on(EHS_WIDGET_UI(pWidget).pUiObject);
                }else{
                    lv_led_off(EHS_WIDGET_UI(pWidget).pUiObject);
                }
            }
            #endif
            break;
        }
        case EHS_SPECIAL_UI_GPIO_IN:
        {
            break;
        }
        default:
            break;
    }

}

#endif
