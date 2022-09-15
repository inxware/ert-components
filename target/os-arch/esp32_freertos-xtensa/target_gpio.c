/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/*
 * target_gpio.c
 */
#include "target_gpio.h"
#include "target.h"
#include "target_config.h"
//#include "heatrod_config.h"
//#include "fsl_port.h"
//#include "inx_buttons.h"

#include "driver/gpio.h"
#include "esp_log.h"
#define TAG "target_gpio"

#if 0
/*
Internal device specific functions
Tis is all NXP GPIO API specific so is nly relevant in principle - not symbols are expected to found in esp32
*/
static GPIO_Type* get_base(const ehs_sint32 gpio)
{
    /*switch(gpio){
        case 0:
        case 1:
        case 2:
        case 3:
            return CONFIG_GPIO_PROCBOARDLEDS_BASE;
        case 4:
            return CONFIG_GPIO_BUTTON_SET_BASE;
    #if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
        case 5:
            return CONFIG_GPIO_BUTTON_MODE_BASE;
    #endif
        case 6:
            return CONFIG_GPIO_BUTTON_DOWN_BASE;
        case 7:
            return CONFIG_GPIO_BUTTON_UP_BASE;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            return CONFIG_GPIO_CONTROL_BASE_OUT;
        case 14:
        case 15:
            return CONFIG_GPIO_SENSE_CONRELAY_BASE;
        default:
            return NULL;
    }
    */
}

static uint32_t get_pin(const ehs_sint32 gpio)
{
    /*switch(gpio){
        case 0:
            return CONFIG_GPIO_PROCBOARDLED1_RED_PIN;
        case 1:
            return CONFIG_GPIO_PROCBOARDLED1_GRN_PIN;
        case 2:
            return CONFIG_GPIO_PROCBOARDLED2_RED_PIN;
        case 3:
            return CONFIG_GPIO_PROCBOARDLED2_GRN_PIN;
        case 4:
            return CONFIG_GPIO_BUTTON_SET_PIN;
    #if INX_HEATROD_IOT_BOARD_VARIANT == INX_HR_HRc
        case 5:
            return CONFIG_GPIO_BUTTON_MODE_PIN;
    #endif
        case 6:
            return CONFIG_GPIO_BUTTON_DOWN_PIN;
        case 7:
            return CONFIG_GPIO_BUTTON_UP_PIN;
        case 8:
            return CONFIG_GPIO_CONTROL_INTERNAL_RELAY_1;
        case 9:
            return CONFIG_GPIO_CONTROL_INTERNAL_RELAY_2;
        case 10:
            return CONFIG_GPIO_CONTROL_INTERNAL_RELAY_3;
        case 11:
            return CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_1;
        case 12:
            return CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_2;
        case 13:
            return CONFIG_GPIO_CONTROL_SSR_EXTERNAL_OUT_3;
        case 14:
            return CONFIG_GPIO_SENSE_CONRELAY_1;
        case 15:
            return CONFIG_GPIO_SENSE_CONRELAY_2;
        default:
            return 0;
    }
    */
}

/*static PORT_Type* get_port(const ehs_sint32 gpio){
    switch(gpio){
        case 1:
        case 2:
            return PORTE;
        case 3:
            return CONFIG_GPIO_BUTTON_DOWN_PORT;
        default:
            return CONFIG_GPIO_BUTTON_SET_PORT;
    }
}*/

#endif

/* The following must be implemented to support the ert components*/

/* reference code from the blink LED test code

#define BLINK_GPIO GPIO_NUM_2

static uint8_t s_led_state = 0;

static void configure_led(void)
{
    gpio_reset_pin(BLINK_GPIO);
    / * Set the GPIO as a push/pull output * /
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

static void blink_led(void)
{
    / * Set the GPIO level according to the state (LOW or HIGH)* /
    gpio_set_level(BLINK_GPIO, s_led_state);
}

see here for full API:
https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html
*/

static ehs_bool set_output_GPIO(ehs_sint32 gpio)
{
    ehs_bool ret = EHS_TRUE;
    gpio_reset_pin(gpio);
    ret = gpio_set_direction(gpio, GPIO_MODE_OUTPUT);
    return (ret == ESP_OK);
}

static ehs_bool set_input_GPIO(ehs_sint32 gpio)
{
    ehs_bool ret = EHS_TRUE;
    gpio_reset_pin(gpio);
    ret = gpio_set_direction(gpio, GPIO_MODE_INPUT);
    return (ret == ESP_OK);
}

static ehs_bool set_value_GPIO(ehs_sint32 gpio, ehs_bool value)
{
    ehs_bool ret = EHS_TRUE;
    ret = gpio_set_level(gpio, value);
    ESP_LOGI(TAG, "set_value_GPIO %d to value %d", gpio, value);
    return (ret == ESP_OK);
}

static ehs_bool get_value_GPIO(ehs_sint32 gpio, ehs_bool *value)
{
    if (value == NULL)
    {
        return EHS_FALSE;
    }
    ehs_bool ret = EHS_TRUE;
    *value = gpio_get_level(gpio);
    return ret;
}

EHS_GLOBAL ehs_bool EhsInitOutputGPIO(ehs_sint32 pin_id)
{
    ehs_bool ret = set_output_GPIO(pin_id);
    if (ret == EHS_TRUE)
    {
        EHSH_LOG_INFO("Initialise GPIO output pin (%d)", pin_id);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to initialise GPIO output pin (%d)", pin_id);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsWriteOutputGPIO(ehs_sint32 pin_id, ehs_bool value)
{
    ehs_bool ret = set_value_GPIO(pin_id, value);
    if (ret == EHS_TRUE)
    {
        // EHSH_LOG_INFO("Write GPIO (%d,%d).",pin_id,value);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to write GPIO (%d,%d).", pin_id, value);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsDestroyOutputGPIO(ehs_sint32 pin_id)
{
    EHSH_LOG_INFO("Destroy GPIO output pin (%d)", pin_id);
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsInitInputGPIO(ehs_sint32 pin_id)
{
    ehs_bool ret = set_input_GPIO(pin_id);
    if (ret == EHS_TRUE)
    {
        EHSH_LOG_INFO("Initialise GPIO input pin (%d)", pin_id);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to initialise GPIO input pin (%d)", pin_id);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsReadInputGPIO(ehs_sint32 pin_id, ehs_bool *value)
{
    ehs_bool ret = get_value_GPIO(pin_id, value);
    if (ret == EHS_TRUE)
    {
        // EHSH_LOG_INFO("Read GPIO (%d,%d).",pin_id,value);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to read GPIO (%d).", pin_id);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsDestroyInputGPIO(ehs_sint32 pin_id)
{
    EHSH_LOG_INFO("Destroy GPIO input pin (%d)", pin_id);
    return EHS_TRUE;
}
/*
#include "target_time.h"
#define BLINK_GPIO GPIO_NUM_2

static uint8_t s_led_state = 0;

static void configure_led(void)
{
    //gpio_reset_pin(BLINK_GPIO);
    // Set the GPIO as a push/pull output
    EhsInitOutputGPIO(BLINK_GPIO);
}

static void blink_led(void)
{
    // Set the GPIO level according to the state (LOW or HIGH)
    EhsWriteOutputGPIO(BLINK_GPIO, s_led_state);
}

void app_main(void)
{

    //Configure the peripheral according to the LED type
    configure_led();
    while (1) {
        printf("Hello from the LED!\n");
        fflush(stdout);
        blink_led();
        //Toggle the LED state
        s_led_state = !s_led_state;
        EhsSleepUs(10000);
    }
}*/
