#ifndef INXWARE_pwm_gen
#define INXWARE_pwm_gen
#include "inx-component.h"

/*****************************************************
 * Generate a signal with Pulse Width Modulation. PWM will work on a certain set of channels with assignable pins. 

The frequency, duty and the duty maximum value are configured.
 *****************************************************/
#define INXWARE_FB_ID_pwm_gen 0xFEDF
#define INXWARE_FB_NAME_pwm_gen "pwm_gen"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(pwm_gen); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(pwm_gen); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(pwm_gen); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(pwm_gen_setup); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(pwm_gen_enable); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(pwm_gen_disable); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(pwm_gen_set); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(pwm_gen)

#endif /* INXWARE_pwm_gen */
