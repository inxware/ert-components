
/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/*
 * target_gpio.c will be selected for platforms with EHS_PERIPHERALS_GPIO_SUPPORT=EHS_PERIPHERALS_GPIO_TYPE_STUBBED
 * GPIO operations are physically NOPs in tis mode to allow for application compatability 
 * where GPIO functions are optional or for target specific purposes. 
 */
#include "ehs_types.h"
#include "hal_gpio.h"

/* todo2022-Intag-> Delete this block
ehs_char gpio_path_buffer[GPIO_PATH_BUFFER_SIZE];

const ehs_char* get_path_GPIO(ehs_sint32 gpio, const ehs_char* type)
{
    sprintf(gpio_path_buffer, "%s%d%s", DEFAULT_GPIO_PATH, gpio, type);
    return (const ehs_char*)gpio_path_buffer;
}
*/


ehs_bool get_value_GPIO(ehs_sint32 gpio, ehs_bool* value)
{
    *value = EHS_FALSE;
    return EHS_TRUE;
}


EHS_GLOBAL ehs_bool EhsInitOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsWriteOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsDestroyOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsInitInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsReadInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    pGPIO->pin_value = EHS_FALSE;
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsDestroyInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    return EHS_TRUE;
}
