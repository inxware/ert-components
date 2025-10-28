/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/* Provides GPIO simulation in the available UI when GFX are available
   e.g. used for desktop use to develop apps with GPIO before HW is available
*/

#include "globals.h"
#include "hal_logger.h"
#include "hal_gpio.h"
#include "guiparams.h"
#include "widget.h"

#ifdef EHS_GUI_SUPPORT_MODE_B

#include "target_viewport_modeB.h"

/* Position of the first GPIO widget */
#define GPIO_UI_X_POS 10
#define GPIO_UI_Y_POS 10
/* GPIO single widget size */
#define GPIO_UI_WIDTH 40
#define GPIO_UI_HEIGHT 40

#define GPIO_UI_SEP 1.1f

#else

/* Position of the first GPIO widget */
#define GPIO_UI_X_POS 50
#define GPIO_UI_Y_POS 150
/* GPIO single widget size */
#define GPIO_UI_WIDTH 50
#define GPIO_UI_HEIGHT 100

#define GPIO_UI_SEP 1.1f

#endif


/* Global GPIO counter used for positioning */
static ehs_uint16 g_GpioCount = 0;

typedef struct
{
#ifndef EHS_GUI_SUPPORT_MODE_B
    EhsWidgetClass* pWidgetBackground;
    EhsWidgetClass* pWidgetLabel;
#endif
    EhsWidgetClass* pWidgetLed;
} gpio_out_gui_widget;

typedef struct
{
#ifndef EHS_GUI_SUPPORT_MODE_B
    EhsWidgetClass* pWidgetBackground;
    EhsWidgetClass* pWidgetLabel;
    EhsWidgetClass* pWidgetSwitchBackground;
#endif
    EhsWidgetClass* pWidgetSwitch;
    ehs_bool inputValue;
} gpio_in_gui_widget;

#ifdef EHS_GUI_SUPPORT_MODE_B

typedef struct
{
    EhsWidgetClass* pBackground;
} gpio_panel_gui_widget;

gpio_panel_gui_widget* pGpioPanel = NULL;

EhsWidgetClass* create_ui_widget(ehs_uint16 id, ehs_sint32 x, ehs_sint32 y, ehs_sint32 w, ehs_sint32 h, ehs_uint32 z,
                                 ehs_uint16 iL, ehs_uint16 iT, ehs_uint16 iR, ehs_uint16 iB,
                                 ehs_uint8 r, ehs_uint8 g, ehs_uint8 b, ehs_uint8 a,
                                 ehs_uint8 fr, ehs_uint8 fg, ehs_uint8 fb, ehs_uint8 fa) 
{
    EhsWidgetClass* pWidget = NULL;
    EhsGuiParamsType xParams = { 0 };
    xParams.xRect.nLeft = x;
    xParams.xRect.nTop = y;
    xParams.xRect.nWidth = w;
    xParams.xRect.nHeight = h;
    xParams.nZorder = z;
    xParams.uClass.xPatch.sComp.nRed = r;
    xParams.uClass.xPatch.sComp.nGreen = g;
    xParams.uClass.xPatch.sComp.nBlue = b;
    xParams.uClass.xPatch.sComp.nAlpha = a;
    xParams.uClass.xTextbox.xBgColour = xParams.uClass.xPatch;
    xParams.uClass.xTextbox.xFgColour.sComp.nRed = fr;
    xParams.uClass.xTextbox.xFgColour.sComp.nGreen = fg;
    xParams.uClass.xTextbox.xFgColour.sComp.nBlue = fb;
    xParams.uClass.xTextbox.xFgColour.sComp.nAlpha = fa;
    xParams.uClass.xTextbox.nIndentL = iL;
    xParams.uClass.xTextbox.nIndentT = iT;
    xParams.uClass.xTextbox.nIndentR = iR;
    xParams.uClass.xTextbox.nIndentB = iB;
    xParams.uClass.xTextbox.nLineSep = 14;
    xParams.eClass = EHS_WIDGET_CLASS_TEXTBOX;
    EhsGraphicsFontClass* pFont = NULL;
    pWidget = EhsWidgetUI_init(id, 0, 0, 0,
                                &(xParams.xRect),xParams.nZorder,
                                xParams.uClass.xTextbox.nIndentL,
                                xParams.uClass.xTextbox.nIndentT,
                                xParams.uClass.xTextbox.nIndentR,
                                xParams.uClass.xTextbox.nIndentB,
                                xParams.uClass.xTextbox.nLineSep,
                                xParams.uClass.xTextbox.xFgColour,
                                xParams.uClass.xTextbox.xBgColour,
                                /*pFont*/NULL);
    return pWidget;
}

void create_gpio_panel_widget()
{
    if(pGpioPanel == NULL){

    }
}

void update_gpio_panel_widget()
{
    if(pGpioPanel != NULL){

    }
}

void destroy_gpio_panel_widget()
{
    if(pGpioPanel != NULL){

    }
}

#else
/* these are gui widgets for mode A */

EhsWidgetClass* create_patch_widget(ehs_sint32 x, ehs_sint32 y, ehs_sint32 w, ehs_sint32 h, ehs_uint32 z,
                                    ehs_uint8 r, ehs_uint8 g, ehs_uint8 b, ehs_uint8 a) 
{
    EhsWidgetClass* pWidget = NULL;
    EhsGuiParamsType xParams = { 0 };
    xParams.eClass = EHS_WIDGET_CLASS_PATCH;
    xParams.xRect.nLeft = x;
    xParams.xRect.nTop = y;
    xParams.xRect.nWidth = w;
    xParams.xRect.nHeight = h;
    xParams.nZorder = z;
    xParams.uClass.xPatch.sComp.nRed = r;
    xParams.uClass.xPatch.sComp.nGreen = g;
    xParams.uClass.xPatch.sComp.nBlue = b;
    xParams.uClass.xPatch.sComp.nAlpha = a;
    pWidget = EhsWidgetPatch_init(&xParams.xRect, xParams.nZorder, xParams.uClass.xPatch);
    return pWidget;
}

EhsWidgetClass* create_text_widget(ehs_sint32 x, ehs_sint32 y, ehs_sint32 w, ehs_sint32 h, ehs_uint32 z,
                                   ehs_uint16 iL, ehs_uint16 iT, ehs_uint16 iR, ehs_uint16 iB,
                                   ehs_uint8 r, ehs_uint8 g, ehs_uint8 b, ehs_uint8 a)
{
    EhsWidgetClass* pWidget = NULL;
    EhsGuiParamsType xParams = { 0 };
    xParams.xRect.nLeft = x;
    xParams.xRect.nTop = y;
    xParams.xRect.nWidth = w;
    xParams.xRect.nHeight = h;
    xParams.nZorder = z;
    xParams.uClass.xPatch.sComp.nRed = r;
    xParams.uClass.xPatch.sComp.nGreen = g;
    xParams.uClass.xPatch.sComp.nBlue = b;
    xParams.uClass.xPatch.sComp.nAlpha = a;
    xParams.uClass.xTextbox.xBgColour = xParams.uClass.xPatch;
    xParams.uClass.xTextbox.xFgColour.sComp.nRed = 255;
    xParams.uClass.xTextbox.xFgColour.sComp.nGreen = 255;
    xParams.uClass.xTextbox.xFgColour.sComp.nBlue = 255;
    xParams.uClass.xTextbox.xFgColour.sComp.nAlpha = 255;
    xParams.uClass.xTextbox.nIndentL = iL;
    xParams.uClass.xTextbox.nIndentT = iT;
    xParams.uClass.xTextbox.nIndentR = iR;
    xParams.uClass.xTextbox.nIndentB = iB;
    xParams.uClass.xTextbox.nLineSep = 14;
    xParams.eClass = EHS_WIDGET_CLASS_TEXTBOX;
    EhsGraphicsFontClass* pFont = NULL;
#ifndef EHS_DONT_USE_BASIC_FONTS
    //pFont = EhsGraphicsFont_load(xParams.uClass.xTextbox.szFontName);
    pFont = EhsGraphicsFont_load("f0000000.bdf"); // @TODO - assign font properly
#endif 
    pWidget = EhsWidgetTextbox_init(&(xParams.xRect), xParams.nZorder,
                                    xParams.uClass.xTextbox.nIndentL,
                                    xParams.uClass.xTextbox.nIndentT,
                                    xParams.uClass.xTextbox.nIndentR,
                                    xParams.uClass.xTextbox.nIndentB,
                                    xParams.uClass.xTextbox.nLineSep,
                                    xParams.uClass.xTextbox.xFgColour,
                                    xParams.uClass.xTextbox.xBgColour,
                                    pFont);
    return pWidget;
}

#endif

/************************************** GPIO OUTPUT **************************************/

gpio_out_gui_widget* init_gpio_out_gui_widget(ehs_gpio_out_state_type* pGPIO)
{
    gpio_out_gui_widget* pWidget = NULL;
    if (pGPIO) {
        pWidget = (gpio_out_gui_widget*)malloc(sizeof(gpio_out_gui_widget));
        if (pWidget) {
            ehs_uint8 r, g, b, a;
            ehs_sint32 width = GPIO_UI_WIDTH, height = GPIO_UI_HEIGHT;
            ehs_sint32 x = GPIO_UI_X_POS + (GPIO_UI_SEP*width * g_GpioCount), y = GPIO_UI_Y_POS;
#ifdef EHS_GUI_SUPPORT_MODE_B
            // create a led widget
            r = 255; g = 0; b = 0; a = 255;
            ehs_uint8 fr = 200, fg = 200, fb = 200, fa = 255;
            pWidget->pWidgetLed = create_ui_widget(EHS_SPECIAL_UI_GPIO_OUT, x, y, width, height, 0, 0, 0, 0, 0, r, g, b, a, fr, fg, fb, fa);
#else
            int width_offset = (width - width*0.9f);
            int height_offset = (height - height*0.95f);
            // create a background patch
            r = 118; g = 140; b = 176; a = 255;
            pWidget->pWidgetBackground = create_patch_widget(x, y, width, height, 1, r, g, b, a);
            // create a label textbox
            r = 72; g = 84; b = 102; a = 255;
            pWidget->pWidgetLabel = create_text_widget(x + width_offset, y + height_offset, width*0.9f - width_offset, height*0.25, 1, 3, -8, 0, 0, r, g, b, a);
            // create a led patch
            r = 255; g = 0; b = 0; a = 255;
            pWidget->pWidgetLed = create_patch_widget(x + width_offset, y + height*0.25 + 2*height_offset, width*0.9f - width_offset, height*0.75 - 3*height_offset, 1, r, g, b, a);
#endif
        }
    }
    return pWidget;
}

void create_gpio_out_gui_widget(gpio_out_gui_widget* pWidget, const ehs_gpio_out_state_type* pGPIO) {
    if (pWidget) {
#ifdef EHS_GUI_SUPPORT_MODE_B
        if (pWidget->pWidgetLed) {
            EHS_WIDGET_UI(pWidget->pWidgetLed).data = (void*) pGPIO;
            EHS_WIDGET_UI(pWidget->pWidgetLed).properties = (pGPIO->pin_id < 0) ? 0 : pGPIO->pin_id;
            EhsWidget_create(pWidget->pWidgetLed);
            EhsWidget_show(pWidget->pWidgetLed);
        }
#else
        // show background widget
        if (pWidget->pWidgetBackground) {
            EhsWidget_create(pWidget->pWidgetBackground);
            EhsWidget_show(pWidget->pWidgetBackground);
        }
        if (pWidget->pWidgetLabel) {
            EhsWidget_create(pWidget->pWidgetLabel);
            EhsWidget_show(pWidget->pWidgetLabel);
            // display text
            Ehs_widget_position_update(pWidget->pWidgetLabel, EHS_FALSE, 255, EHS_FALSE, 0, EHS_FALSE, 0, EHS_FALSE, 0, EHS_FALSE, 0);
            char str[EHS_STRING_LENGTH_MAX];
            EhsSprintf(str, "%d", pGPIO->pin_id);
            EhsWidgetTextbox_write(pWidget->pWidgetLabel, str);
        }
        if (pWidget->pWidgetLed) {
            EhsWidget_create(pWidget->pWidgetLed);
            EhsWidget_show(pWidget->pWidgetLed);
        }
#endif
    }
}

void destroy_gpio_out_gui_widget(gpio_out_gui_widget* pWidget) 
{
    if (pWidget) {
#ifdef EHS_GUI_SUPPORT_MODE_B
        if (pWidget->pWidgetLed) {
            EhsWidget_destroy(pWidget->pWidgetLed);
        }
#else
        if (pWidget->pWidgetBackground) {
            EhsWidget_destroy(pWidget->pWidgetBackground);
        }
        if (pWidget->pWidgetLabel) {
            EhsWidget_destroy(pWidget->pWidgetLabel);
        }
        if (pWidget->pWidgetLed) {
            EhsWidget_destroy(pWidget->pWidgetLed);
        }
#endif
        free(pWidget);
    }
}

void set_gpio_out_value(gpio_out_gui_widget* pWidget, ehs_bool value)
{
#ifdef EHS_GUI_SUPPORT_MODE_B
    EhsWidgetUI_update(pWidget->pWidgetLed);
    Ehs_widget_commit(pWidget->pWidgetLed);
#else
    if (value == EHS_TRUE) {
        EhsWidget_fade(pWidget->pWidgetLed, (ehs_uint8)255);
    } else {
        EhsWidget_fade(pWidget->pWidgetLed, (ehs_uint8)50);
    }
#endif
}

EHS_GLOBAL ehs_bool EhsInitOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    if (!pGPIO) {
        return EHS_FALSE;
    }

    pGPIO->target_data = NULL;

    gpio_out_gui_widget* pWidget = init_gpio_out_gui_widget(pGPIO);
    if (pWidget) {
        EHSH_LOG_INFO("Initialised GPIO output pin (%d)", pGPIO->pin_id);
        g_GpioCount++;
        create_gpio_out_gui_widget(pWidget, pGPIO);
        set_gpio_out_value(pWidget, pGPIO->initial_state);
        pGPIO->target_data = pWidget;

        return EHS_TRUE;

    } else {
        EHSH_LOG_ERROR("Failed to initialise GPIO output pin (%d)", pGPIO->pin_id);
    }

    return EHS_FALSE;
}

EHS_GLOBAL ehs_bool EhsWriteOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    gpio_out_gui_widget* pWidget = (gpio_out_gui_widget*)pGPIO->target_data;
    if (pWidget) {
        set_gpio_out_value(pWidget, pGPIO->pin_value);
    }
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsDestroyOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    g_GpioCount = 0;
    gpio_out_gui_widget* pWidget = (gpio_out_gui_widget*)pGPIO->target_data;
    if (pWidget) {
        destroy_gpio_out_gui_widget(pWidget);
        pGPIO->target_data = NULL;
    }
    return EHS_TRUE;
}

/************************************** GPIO INPUT **************************************/

#ifdef EHS_GUI_SUPPORT_MODE_B
/* Widget update callback */
static void gui_widget_event_callback(struct EhsWidgetStruct* pWidgetSwitch, ehs_uint16 event_id, const char* label, void* data)
{
	if(pWidgetSwitch && data){
		gpio_in_gui_widget* pGPIO = (ehs_gpio_in_state_type*)EHS_WIDGET_UI(pWidgetSwitch).data;
        ehs_bool* pValue = (ehs_bool*)data;
        if(pGPIO && pValue){
            pGPIO->inputValue = *pValue;
        }
	}
}
#else
void OnMouseDownEvent(EhsWidgetClass* pWidget)
{
    if (pWidget && pWidget->pMouseDownEventData) {
        gpio_in_gui_widget* pGPIOWidget = (gpio_in_gui_widget*)pWidget->pMouseDownEventData;
        pGPIOWidget->inputValue = !pGPIOWidget->inputValue;

        Ehs_widget_position_update(pGPIOWidget->pWidgetSwitchBackground, EHS_FALSE, 255, EHS_FALSE, 0, EHS_FALSE, 0, EHS_FALSE, 0, EHS_FALSE, 0);
        if (pGPIOWidget->inputValue == EHS_TRUE) {
            EhsWidgetTextbox_write(pGPIOWidget->pWidgetSwitchBackground, "1");
            Ehs_widget_position_update(pGPIOWidget->pWidgetSwitch, EHS_FALSE, 255, EHS_FALSE, 0, EHS_TRUE, 50, EHS_FALSE, 0, EHS_FALSE, 0);
        }
        else 
        {
            EhsWidgetTextbox_write(pGPIOWidget->pWidgetSwitchBackground, "0");
            Ehs_widget_position_update(pGPIOWidget->pWidgetSwitch, EHS_FALSE, 255, EHS_FALSE, 0, EHS_TRUE, 0, EHS_FALSE, 0, EHS_FALSE, 0);
        }
    }
}
#endif

gpio_in_gui_widget* init_gpio_in_gui_widget(ehs_gpio_in_state_type* pGPIO)
{
    gpio_in_gui_widget* pWidget = NULL;
    if (pGPIO) {
        pWidget = (gpio_in_gui_widget*)malloc(sizeof(gpio_in_gui_widget));
        if (pWidget) {
            pWidget->inputValue = pGPIO->pin_value;
            pGPIO->target_data = pWidget;
            ehs_sint32 width = GPIO_UI_WIDTH, height = GPIO_UI_HEIGHT;
            ehs_sint32 x = GPIO_UI_X_POS + (GPIO_UI_SEP*width * g_GpioCount), y = GPIO_UI_Y_POS;
            ehs_uint8 r, g, b, a;
#ifdef EHS_GUI_SUPPORT_MODE_B
            // create a led widget
            r = 255; g = 0; b = 0; a = 255;
            ehs_uint8 fr = 100, fg = 100, fb = 100, fa = 255;
            pWidget->pWidgetSwitch = create_ui_widget(EHS_SPECIAL_UI_GPIO_IN, x, y, width, height, 0, 0, 0, 0, 0, r, g, b, a, fr, fg, fb, fa);
#else
            int width_offset = (width - width * 0.9f);
            int height_offset = (height - height * 0.95f);
            // create a background patch
            r = 118; g = 140; b = 176; a = 255;
            pWidget->pWidgetBackground = create_patch_widget(x, y, width, height, 1, r, g, b, a);
            if (pWidget->pWidgetBackground) { // set mouse down callback
                pWidget->pWidgetBackground->pfMouseDownEventFunc = OnMouseDownEvent;
                pWidget->pWidgetBackground->pMouseDownEventData = pWidget;
            }
            // create a label textbox
            r = 72; g = 84; b = 102; a = 255;
            pWidget->pWidgetLabel = create_text_widget(x + width_offset, y + height_offset, width * 0.9f - width_offset, height * 0.25, 1, 3, -8, 0, 0, r, g, b, a);
            
            // create switch background textbox
            r = 100; g = 100; b = 100; a = 255;
            pWidget->pWidgetSwitchBackground = create_text_widget(x + width_offset, y + height * 0.25 + 2 * height_offset, width * 0.9f - width_offset, height * 0.75 - 3 * height_offset, 1, 15, 8, -5, 0, r, g, b, a);

            // create switch item patch
            r = 50; g = 50; b = 50; a = 255;
            pWidget->pWidgetSwitch = create_patch_widget(x + width_offset, y + height * 0.25 + 2 * height_offset, width * 0.9f - width_offset, height * 0.25 - 3 * height_offset, 1, r, g, b, a);
#endif
        }
    }
    return pWidget;
}

void create_gpio_in_gui_widget(gpio_in_gui_widget* pWidget, const ehs_gpio_in_state_type* pGPIO) {
    if (pWidget) {
#ifdef EHS_GUI_SUPPORT_MODE_B
        if (pWidget->pWidgetSwitch) {
            EHS_WIDGET_UI(pWidget->pWidgetSwitch).data = (void*) pWidget;
            EHS_WIDGET_UI(pWidget->pWidgetSwitch).properties = (pGPIO->pin_id < 0) ? 0 : pGPIO->pin_id;
            EHS_WIDGET_UI(pWidget->pWidgetSwitch).event_callback = gui_widget_event_callback;
            EhsWidget_create(pWidget->pWidgetSwitch);
            EhsWidget_show(pWidget->pWidgetSwitch);
        }
#else
        // show background widget
        if (pWidget->pWidgetBackground) {
            EhsWidget_create(pWidget->pWidgetBackground);
            EhsWidget_show(pWidget->pWidgetBackground);
        }
        if (pWidget->pWidgetLabel) {
            EhsWidget_create(pWidget->pWidgetLabel);
            EhsWidget_show(pWidget->pWidgetLabel);
            // display text
            Ehs_widget_position_update(pWidget->pWidgetLabel, EHS_FALSE, 255, EHS_FALSE, 0, EHS_FALSE, 0, EHS_FALSE, 0, EHS_FALSE, 0);
            char str[EHS_STRING_LENGTH_MAX];
            EhsSprintf(str, "%d", pGPIO->pin_id);
            EhsWidgetTextbox_write(pWidget->pWidgetLabel, str);
        }
        if (pWidget->pWidgetSwitchBackground) {
            EhsWidget_create(pWidget->pWidgetSwitchBackground);
            EhsWidget_show(pWidget->pWidgetSwitchBackground);
            // display text
            Ehs_widget_position_update(pWidget->pWidgetSwitchBackground, EHS_FALSE, 255, EHS_FALSE, 0, EHS_FALSE, 0, EHS_FALSE, 0, EHS_FALSE, 0);
            if (pWidget->inputValue == EHS_TRUE) {
                EhsWidgetTextbox_write(pWidget->pWidgetSwitchBackground, "1");
            } else {
                EhsWidgetTextbox_write(pWidget->pWidgetSwitchBackground, "0");
            }   
        }
        if (pWidget->pWidgetSwitch) {
            EhsWidget_create(pWidget->pWidgetSwitch);
            EhsWidget_show(pWidget->pWidgetSwitch);
        }
#endif
    }
}

void destroy_gpio_in_gui_widget(gpio_in_gui_widget* pWidget)
{
    if (pWidget) {
#ifdef EHS_GUI_SUPPORT_MODE_B
        if (pWidget->pWidgetSwitch) {
            EhsWidget_destroy(pWidget->pWidgetSwitch);
        }
#else
        if (pWidget->pWidgetBackground) {
            EhsWidget_destroy(pWidget->pWidgetBackground);
        }
        if (pWidget->pWidgetLabel) {
            EhsWidget_destroy(pWidget->pWidgetLabel);
        }
        if (pWidget->pWidgetSwitchBackground) {
            EhsWidget_destroy(pWidget->pWidgetSwitchBackground);
        }
        if (pWidget->pWidgetSwitch) {
            EhsWidget_destroy(pWidget->pWidgetSwitch);
        }
#endif
        free(pWidget);
    }
}

EHS_GLOBAL ehs_bool EhsInitInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    if (!pGPIO) {
        return EHS_FALSE;
    }

    pGPIO->target_data = NULL;

    gpio_in_gui_widget* pWidget = init_gpio_in_gui_widget(pGPIO);
    if (pWidget) {
        EHSH_LOG_INFO("Initialised GPIO input pin (%d)", pGPIO->pin_id);
        g_GpioCount++;
        create_gpio_in_gui_widget(pWidget, pGPIO);
        pGPIO->target_data = pWidget;

        return EHS_TRUE;
    }
    else {
        EHSH_LOG_ERROR("Failed to initialise GPIO input pin (%d)", pGPIO->pin_id);
    }

    return EHS_FALSE;
}

EHS_GLOBAL ehs_bool EhsReadInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    gpio_in_gui_widget* pWidget = (gpio_in_gui_widget*)pGPIO->target_data;
    if (pWidget) {
        pGPIO->pin_value = pWidget->inputValue;
        return EHS_TRUE;
    }
    return EHS_FALSE;
}

EHS_GLOBAL ehs_bool EhsDestroyInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    g_GpioCount = 0;
    gpio_in_gui_widget* pWidget = (gpio_in_gui_widget*)pGPIO->target_data;
    if (pWidget) {
        destroy_gpio_in_gui_widget(pWidget);
        pGPIO->target_data = NULL;
    }
    return EHS_TRUE;
}
