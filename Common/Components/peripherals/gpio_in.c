/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/
#include "globals.h"
#include "inx-parameters.h"
#include "gpio_in.h"
#if (EHS_PERIPHERALS_GPIO_SUPPORT == EHS_PERIPHERALS_GPIO_TYPE_STUBBED) || (EHS_PERIPHERALS_GPIO_SUPPORT == EHS_PERIPHERALS_GPIO_TYPE_GUI)
#include "ehs_hal_gpio.h"
#ifndef EHS_MSVC
#warning "Using Stubbed/GUI GPIO"
#endif
#else
#include "target_gpio.h"
#endif


EHS_FB_FUNCTIONS_START(gpio_in)

EHS_FB_FUNCTION_ENTRY("read", 0x01, gpio_in_read)
EHS_FB_FUNCTIONS_END

#define INX_gpio_in_ARG_read_finishevent 1
#define INX_gpio_in_ARG_read_value 1

EHS_FB_IDENTIFY_FUNCTION(gpio_in)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_gpio_in_state_type);
}

EHS_FB_INIT_FUNCTION(gpio_in)
{
    //this is the reference to the object data for this instance of the function block
    ehs_gpio_in_state_type* gpio_in_state = (ehs_gpio_in_state_type*)EHS_FB_INIT_CONTEXT;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d",&gpio_in_state->pin_id);
    gpio_in_state->pin_value = EHS_FALSE;
    ehs_bool ret = EhsInitInputGPIO(gpio_in_state);
    return ret;
}

EHS_FB_RUN_FUNCTION(gpio_in_read)
{
    ehs_gpio_in_state_type* gpio_in_state = (ehs_gpio_in_state_type*)EHS_FB_RUN_CONTEXT;
    ehs_bool ret = EhsReadInputGPIO(gpio_in_state);
    if(ret == EHS_TRUE)
    {
        EHS_FB_OUT_B_API2(INX_gpio_in_ARG_read_value) = gpio_in_state->pin_value;
        EHS_FB_FINISH(INX_gpio_in_ARG_read_finishevent);
    }
}

EHS_FB_DESTROY_FUNCTION(gpio_in)
{
    ehs_gpio_in_state_type* gpio_in_state = (ehs_gpio_in_state_type*)EHS_FB_DESTROY_CONTEXT;
    return EhsDestroyInputGPIO(gpio_in_state);
}
