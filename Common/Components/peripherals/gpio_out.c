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
#include "gpio_out.h"
#include "target_gpio.h"

EHS_FB_FUNCTIONS_START(gpio_out)

EHS_FB_FUNCTION_ENTRY("write", 0x00, gpio_out_write)
EHS_FB_FUNCTIONS_END

#define INX_gpio_out_ARG_write_value 1

typedef struct
{
    ehs_sint32 pin_id;
    ehs_bool pin_value;
} gpio_out_state_type;

EHS_FB_IDENTIFY_FUNCTION(gpio_out)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(gpio_out_state_type);
}

EHS_FB_INIT_FUNCTION(gpio_out)
{
    //this is the reference to the object data for this instance of the function block
    gpio_out_state_type* gpio_out_state = (gpio_out_state_type*)EHS_FB_INIT_CONTEXT;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d",&gpio_out_state->pin_id);
    gpio_out_state->pin_value = EHS_FALSE;
    ehs_bool ret = EhsInitOutputGPIO(gpio_out_state->pin_id);
    return ret;
}

EHS_FB_RUN_FUNCTION(gpio_out_write)
{
    gpio_out_state_type* gpio_out_state = (gpio_out_state_type*)EHS_FB_RUN_CONTEXT;
    gpio_out_state->pin_value = EHS_FB_IN_B_API2(INX_gpio_out_ARG_write_value);
    EhsWriteOutputGPIO(gpio_out_state->pin_id, gpio_out_state->pin_value);
}

EHS_FB_DESTROY_FUNCTION(gpio_out)
{
    gpio_out_state_type* gpio_out_state = (gpio_out_state_type*)EHS_FB_DESTROY_CONTEXT;
    return EhsDestroyOutputGPIO(gpio_out_state->pin_id);
}
