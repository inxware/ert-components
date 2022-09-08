/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/
#include "target.h"
#include "inx-parameters.h"
#include "gpio_in.h"
#include "target_gpio.h"

EHS_FB_FUNCTIONS_START(gpio_in)

EHS_FB_FUNCTION_ENTRY("read", 0x00, gpio_in_read)
EHS_FB_FUNCTIONS_END

#define INX_gpio_in_ARG_read_finishevent 1
#define INX_gpio_in_ARG_read_value 1

typedef struct
{
    ehs_sint32 pin_id;
    ehs_bool pin_value;
} gpio_in_state_type;

EHS_FB_IDENTIFY_FUNCTION(gpio_in)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(gpio_in_state_type);
}

EHS_FB_INIT_FUNCTION(gpio_in)
{
    //this is the reference to the object data for this instance of the function block
    gpio_in_state_type* gpio_in_state = (gpio_in_state_type*)EHS_FB_INIT_CONTEXT;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d",&gpio_in_state->pin_id);
    gpio_in_state->pin_value = EHS_FALSE;
    ehs_bool ret = EhsInitInputGPIO(gpio_in_state->pin_id);
    return ret;
}

EHS_FB_RUN_FUNCTION(gpio_in_read)
{
    gpio_in_state_type* gpio_in_state = (gpio_in_state_type*)EHS_FB_RUN_CONTEXT;
    ehs_bool pin_value;
    ehs_bool ret = EhsReadInputGPIO(gpio_in_state->pin_id, &pin_value);
    if(ret == EHS_TRUE)
    {
        gpio_in_state->pin_value = pin_value;
        EHS_FB_OUT_B_API2(INX_gpio_in_ARG_read_value) = gpio_in_state->pin_value;
        EHS_FB_FINISH(INX_gpio_in_ARG_read_finishevent);
    }
}

EHS_FB_DESTROY_FUNCTION(gpio_in)
{
    gpio_in_state_type* gpio_in_state = (gpio_in_state_type*)EHS_FB_DESTROY_CONTEXT;
    return EhsDestroyInputGPIO(gpio_in_state->pin_id);
}
