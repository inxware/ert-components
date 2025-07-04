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
#include "target.h"
#include "target_gpio.h"
#include "target_config.h"
#include "inxware_hrx_config.h"
#include "fsl_port.h"
#include "inx_buttons.h"
#include "hal_logger.h"

static GPIO_Type* get_base(const ehs_sint32 gpio)
{
    switch(gpio)
    {
    case 0:
    case 1:
    case 2:
    case 3:
        return CONFIG_GPIO_PROCBOARDLEDS_BASE;
    case 4:
        return CONFIG_GPIO_BUTTON_SET_BASE;
#if INX_NXP_KENETIS_BOARD_VARIANT == INX_HR_HRc
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
}

static uint32_t get_pin(const ehs_sint32 gpio)
{
    switch(gpio)
    {
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
#if INX_NXP_KENETIS_BOARD_VARIANT == INX_HR_HRc
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

/* todo Should this be commented out?? */

static ehs_bool set_output_GPIO(ehs_sint32 gpio, ehs_bool intialValue, ehs_bool opeDrain)
{
    ehs_bool ret = EHS_TRUE;
    //PORT_Type* pBase=get_port(gpio);
    //const uint32_t pin=get_pin(gpio);
    //PORT_SetPinMux(pBase, pin, kPORT_MuxAsGpio);
    return ret;
}

static ehs_bool set_input_GPIO(ehs_sint32 gpio)
{
    ehs_bool ret = EHS_TRUE;
    //PORT_Type* pBase=get_port(gpio);
    //const uint32_t pin=get_pin(gpio);
    //PORT_SetPinMux(pBase, pin, kPORT_MuxAsGpio);
    return ret;
}

static ehs_bool set_value_GPIO(ehs_sint32 gpio, ehs_bool value, ehs_bool openDrain)
{
    //todo:2023 - add opendrain logic if not done by the init.
    ehs_bool ret = EHS_TRUE;
    GPIO_Type* base=get_base(gpio);
    uint32_t pin=get_pin(gpio);
    if(base==NULL)
    {
        ret=EHS_FALSE;
    }
    else
    {
        if(value==EHS_TRUE)
        {
            inxGPIOOn(base,pin);
        }
        else
        {
            inxGPIOOff(base,pin);
        }
    }
    return ret;
}

static ehs_bool get_value_GPIO(ehs_sint32 gpio, ehs_bool* value)
{
    if(value == NULL)
    {
        return EHS_FALSE;
    }
    ehs_bool ret = EHS_TRUE;
    const GPIO_Type* pBase=get_base(gpio);
    if(pBase==NULL)
    {
        ret=EHS_FALSE;
    }
    else
    {
        const uint32_t pin=get_pin(gpio);
        *value=inxGPIORead(pBase,pin);
    }
    return ret;
}


EHS_GLOBAL ehs_bool EhsInitOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    ehs_bool ret = set_output_GPIO(pGPIO->pin_id, pGPIO->initial_state, pGPIO->open_drain_mode);
    if(ret == EHS_TRUE)
    {
        EHSH_LOG_INFO("Initialise GPIO output pin (%d)", pGPIO->pin_id);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to initialise GPIO output pin (%d)", pGPIO->pin_id);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsWriteOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    ehs_bool ret = set_value_GPIO(pGPIO->pin_id, pGPIO->pin_value, pGPIO->open_drain_mode);
    if(ret == EHS_TRUE)
    {
        //EHSH_LOG_INFO("Write GPIO (%d,%d).",pin_id,value);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to write GPIO (%d,%d).", pGPIO->pin_id, pGPIO->pin_value);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsDestroyOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    EHSH_LOG_INFO("Destroy GPIO output pin (%d)", pGPIO->pin_id);
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsInitInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    ehs_bool ret = set_input_GPIO(pGPIO->pin_id);
    if(ret == EHS_TRUE)
    {
        EHSH_LOG_INFO("Initialise GPIO input pin (%d)",pGPIO->pin_id);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to initialise GPIO input pin (%d)",pGPIO->pin_id);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsReadInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    ehs_bool ret = get_value_GPIO(pGPIO->pin_id, &pGPIO->pin_value);
    if(ret == EHS_TRUE)
    {
        //EHSH_LOG_INFO("Read GPIO (%d,%d).",pin_id,value);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to read GPIO (%d).", pGPIO->pin_id);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsDestroyInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    EHSH_LOG_INFO("Destroy GPIO input pin (%d)", pGPIO->pin_id);
    return EHS_TRUE;
}
