/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * target_gpio.c
 */

#include "driver/gpio.h"
#include "esp_log.h"
#define TAG "target_gpio"

#include "target_gpio.h"
#include "target.h"
#include "target_config.h"
#include "hal_logger.h"


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

static ehs_bool set_output_GPIO(ehs_sint32 gpio, ehs_bool initialValue, ehs_bool openDrain)
{
    if (gpio <= 48)
    {
        //ESP_LOGI(TAG, "set_output_GPIO %d", gpio);
        ehs_bool ret = EHS_TRUE;
        gpio_reset_pin(gpio);
        ret = gpio_set_direction(gpio, GPIO_MODE_OUTPUT);
        if (ret == ESP_OK) ret = gpio_set_level(gpio, initialValue);
        return (ret == ESP_OK);
    }
}

static ehs_bool set_input_GPIO(ehs_sint32 gpio)
{
    //ESP_LOGI(TAG, "set_input_GPIO %d", gpio);
    ehs_bool ret = EHS_TRUE;
    gpio_reset_pin(gpio);
    ret = gpio_set_direction(gpio, GPIO_MODE_INPUT);
    return (ret == ESP_OK);
}

static ehs_bool set_value_GPIO(ehs_sint32 gpio, ehs_bool value, ehs_bool openDrain)
{
    ehs_bool ret = EHS_TRUE;
    ret = gpio_set_level(gpio, value);
    //ESP_LOGI(TAG, "set_value_GPIO %d to value %d", gpio, value);
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
EHS_GLOBAL ehs_bool EhsInitOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    ehs_bool ret = set_output_GPIO(pGPIO->pin_id, pGPIO->initial_state, pGPIO->open_drain_mode);
    ret = ret && set_value_GPIO(pGPIO->pin_id, pGPIO->initial_state, pGPIO->open_drain_mode); // @todo - needed? the above function already sets default value.
    if (ret == EHS_TRUE)
    {
        //ESP_LOGI(TAG, "Initialise GPIO output pin (%d)", pGPIO->pin_id);
    }
    else
    {
        //ESP_LOGI(TAG, "Failed to initialise GPIO output pin (%d)", pGPIO->pin_id);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsWriteOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    ehs_bool ret = set_value_GPIO(pGPIO->pin_id, pGPIO->pin_value, pGPIO->open_drain_mode);
    if (ret == EHS_TRUE)
    {
        // ESP_LOGI(TAG, "Write GPIO (%d,%d).",pin_id,value);
    }
    else
    {
        //EHSH_LOG_ERROR("Failed to write GPIO (%d,%d).", pGPIO->pin_id, pGPIO->pin_value);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsDestroyOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    //ESP_LOGI(TAG, "Destroy GPIO output pin (%d)", pGPIO->pin_id);
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsInitInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    ehs_bool ret = set_input_GPIO(pGPIO->pin_id);
    if (ret == EHS_TRUE)
    {
        //ESP_LOGI(TAG, "Initialise GPIO input pin (%d)", pGPIO->pin_id);
    }
    else
    {
        //EHSH_LOG_ERROR("Failed to initialise GPIO input pin (%d)", pGPIO->pin_id);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsReadInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    ehs_bool ret = get_value_GPIO(pGPIO->pin_id, &pGPIO->pin_value);
    if (ret == EHS_TRUE)
    {
        // ESP_LOGI(TAG, "Read GPIO (%d,%d).",pin_id,value);
    }
    else
    {
        // EHSH_LOG_ERROR("Failed to read GPIO (%d).", pGPIO->pin_id);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsDestroyInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    //ESP_LOGI(TAG, "Destroy GPIO input pin (%d)", pGPIO->pin_id);
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
