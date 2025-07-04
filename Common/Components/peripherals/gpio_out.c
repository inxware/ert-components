/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/
#include "target.h"
#include "inx-parameters.h"
#include "gpio_out.h"
#if (EHS_PERIPHERALS_GPIO_SUPPORT == EHS_PERIPHERALS_GPIO_TYPE_STUBBED) || (EHS_PERIPHERALS_GPIO_SUPPORT == EHS_PERIPHERALS_GPIO_TYPE_GUI)
#include "hal_gpio.h"
#ifndef EHS_MSVC
#warning "Using Stubbed/GUI GPIO"
#endif
#else
#include "target_gpio.h"
#endif

EHS_FB_FUNCTIONS_START(gpio_out)

EHS_FB_FUNCTION_ENTRY("write", 0x01, gpio_out_write)
EHS_FB_FUNCTIONS_END

#define INX_gpio_out_ARG_write_value 1


EHS_FB_IDENTIFY_FUNCTION(gpio_out)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(ehs_gpio_out_state_type);
}

EHS_FB_INIT_FUNCTION(gpio_out)
{
    //this is the reference to the object data for this instance of the function block
    ehs_gpio_out_state_type* gpio_out_state = (ehs_gpio_out_state_type*)EHS_FB_INIT_CONTEXT;
    unsigned short int initial_state = 0;
    unsigned short int open_drain_mode = 0;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %hu %hu",&gpio_out_state->pin_id, &initial_state, &open_drain_mode);
    gpio_out_state->initial_state = (ehs_uint8) initial_state;
    gpio_out_state->open_drain_mode = (ehs_bool) open_drain_mode;
    if (gpio_out_state->initial_state == 0) 
        gpio_out_state->pin_value = EHS_FALSE;
    else if (gpio_out_state->initial_state == 1)
        gpio_out_state->pin_value = EHS_TRUE;
    else  gpio_out_state->pin_value = EHS_FALSE;

    if (gpio_out_state->initial_state != 2)
        gpio_out_state->error_state = EhsInitOutputGPIO(gpio_out_state);
    // else we will do this as soon as we get a value
    return EHS_TRUE; /* Note we don't want to return an error here as this crashes out the whole of the SODL and we ant to handle errros in the app */
}

EHS_FB_RUN_FUNCTION(gpio_out_write)
{
    ehs_gpio_out_state_type* gpio_out_state = (ehs_gpio_out_state_type*)EHS_FB_RUN_CONTEXT;
    gpio_out_state->pin_value = EHS_FB_IN_B_API2(INX_gpio_out_ARG_write_value);
    if (gpio_out_state->initial_state == 2) { // LAte config if this was to be left in high state until set.
        gpio_out_state->error_state = EhsInitOutputGPIO(gpio_out_state);
        gpio_out_state->initial_state = 255; /* mark that the intialisation has now been done */
    }
    EhsWriteOutputGPIO(gpio_out_state); /* We will write this again in all cases in case the intialiser doesn't support intial values*/
}

EHS_FB_DESTROY_FUNCTION(gpio_out)
{
    ehs_gpio_out_state_type* gpio_out_state = (ehs_gpio_out_state_type*)EHS_FB_DESTROY_CONTEXT;
    return EhsDestroyOutputGPIO(gpio_out_state);
}
