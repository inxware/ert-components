/*---------------------------------------------------------------
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
 *---------------------------------------------------------------*/

/**
 * @file target_gpio.c
 * Zephyr GPIO HAL implementation for eRT.
 *
 * Uses the Zephyr GPIO devicetree API.  Board overlays must define the
 * DTS aliases inx-gpio0 and inx-gpio1 pointing to the relevant GPIO
 * controllers (see boards/<board>.overlay).
 *
 * Pin ID encoding (matches target_gpio.h comment):
 *   bits [4:0]  — pin number within the controller (0-31)
 *   bits [N:5]  — port index (0 → inx-gpio0, 1 → inx-gpio1)
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include "globals.h"
#include "ehs_hal_gpio.h"

/* --------------------------------------------------------------------------
 * DTS-sourced GPIO device handles
 * -------------------------------------------------------------------------- */
/* TODO INX_GPIO_PORT_COUNT needs to be configurable in the config.mk for a given board and should be checked against the Zephyr board's DTS.*/
#define INX_GPIO_PORT_COUNT  2

static const struct device *gpio_dev[INX_GPIO_PORT_COUNT];

static void gpio_init_devices(void)
{
    static ehs_bool initialised = EHS_FALSE;
    if (initialised) return;

#if DT_NODE_HAS_STATUS(DT_ALIAS(inx_gpio0), okay)
    gpio_dev[0] = DEVICE_DT_GET(DT_ALIAS(inx_gpio0));
#endif
#if DT_NODE_HAS_STATUS(DT_ALIAS(inx_gpio1), okay)
    gpio_dev[1] = DEVICE_DT_GET(DT_ALIAS(inx_gpio1));
#endif

    initialised = EHS_TRUE;
}

/* Decode port index and pin number from the eRT pin_id convention. */
static inline int pin_to_port(ehs_sint32 pin_id) { return (int)(pin_id >> 5) & 0x01; }
static inline int pin_to_num(ehs_sint32 pin_id)  { return (int)(pin_id & 0x1F); }

/* --------------------------------------------------------------------------
 * Output GPIO
 * -------------------------------------------------------------------------- */

EHS_GLOBAL ehs_bool EhsInitOutputGPIO(ehs_gpio_out_state_type *pGPIO)
{
    gpio_init_devices();
    int port = pin_to_port(pGPIO->pin_id);
    int pin  = pin_to_num(pGPIO->pin_id);

    if (port >= INX_GPIO_PORT_COUNT || gpio_dev[port] == NULL ||
        !device_is_ready(gpio_dev[port]))
    {
        pGPIO->error_state = EHS_TRUE;
        return EHS_FALSE;
    }

    int flags = GPIO_OUTPUT;
    if (pGPIO->open_drain_mode) {
        flags |= GPIO_OPEN_DRAIN;
    }

    int rc = gpio_pin_configure(gpio_dev[port], (gpio_pin_t)pin, flags);
    if (rc != 0) {
        pGPIO->error_state = EHS_TRUE;
        return EHS_FALSE;
    }
    pGPIO->error_state = EHS_FALSE;
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsWriteOutputGPIO(ehs_gpio_out_state_type *pGPIO)
{
    int port = pin_to_port(pGPIO->pin_id);
    int pin  = pin_to_num(pGPIO->pin_id);

    if (port >= INX_GPIO_PORT_COUNT || gpio_dev[port] == NULL) {
        return EHS_FALSE;
    }

    int rc = gpio_pin_set(gpio_dev[port], (gpio_pin_t)pin,
                          pGPIO->pin_value ? 1 : 0);
    return (rc == 0) ? EHS_TRUE : EHS_FALSE;
}

EHS_GLOBAL ehs_bool EhsDestroyOutputGPIO(ehs_gpio_out_state_type *pGPIO)
{
    int port = pin_to_port(pGPIO->pin_id);
    int pin  = pin_to_num(pGPIO->pin_id);

    if (port >= INX_GPIO_PORT_COUNT || gpio_dev[port] == NULL) {
        return EHS_TRUE; /* nothing to destroy */
    }

    gpio_pin_configure(gpio_dev[port], (gpio_pin_t)pin, GPIO_DISCONNECTED);
    return EHS_TRUE;
}

/* --------------------------------------------------------------------------
 * Input GPIO
 * -------------------------------------------------------------------------- */

EHS_GLOBAL ehs_bool EhsInitInputGPIO(ehs_gpio_in_state_type *pGPIO)
{
    gpio_init_devices();
    int port = pin_to_port(pGPIO->pin_id);
    int pin  = pin_to_num(pGPIO->pin_id);

    if (port >= INX_GPIO_PORT_COUNT || gpio_dev[port] == NULL ||
        !device_is_ready(gpio_dev[port]))
    {
        return EHS_FALSE;
    }

    int rc = gpio_pin_configure(gpio_dev[port], (gpio_pin_t)pin,
                                GPIO_INPUT | GPIO_PULL_UP);
    return (rc == 0) ? EHS_TRUE : EHS_FALSE;
}

EHS_GLOBAL ehs_bool EhsReadInputGPIO(ehs_gpio_in_state_type *pGPIO)
{
    int port = pin_to_port(pGPIO->pin_id);
    int pin  = pin_to_num(pGPIO->pin_id);

    if (port >= INX_GPIO_PORT_COUNT || gpio_dev[port] == NULL) {
        pGPIO->pin_value = EHS_FALSE;
        return EHS_FALSE;
    }

    int val = gpio_pin_get(gpio_dev[port], (gpio_pin_t)pin);
    pGPIO->pin_value = (val > 0) ? EHS_TRUE : EHS_FALSE;
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsDestroyInputGPIO(ehs_gpio_in_state_type *pGPIO)
{
    int port = pin_to_port(pGPIO->pin_id);
    int pin  = pin_to_num(pGPIO->pin_id);

    if (port >= INX_GPIO_PORT_COUNT || gpio_dev[port] == NULL) {
        return EHS_TRUE;
    }

    gpio_pin_configure(gpio_dev[port], (gpio_pin_t)pin, GPIO_DISCONNECTED);
    return EHS_TRUE;
}
