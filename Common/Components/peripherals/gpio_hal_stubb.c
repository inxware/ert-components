/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/*
 * target_gpio.c will be selected for platforms with EHS_PERIPHERALS_GPIO=stubb
 * GPIO operations are physically NOPs in tis mode to allow for application compatability 
 * where GPIO functions are optional or for target specific purposes. 
 */
#include "ehs_types.h"

/* todo2022-Intag-> Delete this block
ehs_char gpio_path_buffer[GPIO_PATH_BUFFER_SIZE];

const ehs_char* get_path_GPIO(ehs_sint32 gpio, const ehs_char* type)
{
    sprintf(gpio_path_buffer, "%s%d%s", DEFAULT_GPIO_PATH, gpio, type);
    return (const ehs_char*)gpio_path_buffer;
}
*/

ehs_bool set_output_GPIO(ehs_sint32 gpio)
{
    return EHS_TRUE;
}

ehs_bool set_input_GPIO(ehs_sint32 gpio)
{
    return EHS_TRUE;
}

ehs_bool set_value_GPIO(ehs_sint32 gpio, ehs_bool value)
{
    return EHS_TRUE;
}

ehs_bool get_value_GPIO(ehs_sint32 gpio, ehs_bool* value)
{
    *value = EHS_FALSE;
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsInitOutputGPIO(ehs_sint32 pin_id)
{
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsWriteOutputGPIO(ehs_sint32 pin_id, ehs_bool value)
{
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsDestroyOutputGPIO(ehs_sint32 pin_id)
{
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsInitInputGPIO(ehs_sint32 pin_id)
{
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsReadInputGPIO(ehs_sint32 pin_id, ehs_bool* value)
{
    *value = EHS_FALSE;
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsDestroyInputGPIO(ehs_sint32 pin_id)
{
    return EHS_TRUE;
}
