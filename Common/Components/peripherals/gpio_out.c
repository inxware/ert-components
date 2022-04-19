
#include "target.h"
#include "inx-parameters.h"
#include "gpio_out.h"
#include "target_gpio.h"

EHS_FB_FUNCTIONS_START(gpio_out)
EHS_FB_FUNCTION_ENTRY("write", gpio_out_write)
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
