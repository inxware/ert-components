#ifndef INXWARE_pwm
#define INXWARE_pwm
#include "inx-component.h"

/*****************************************************
 * rtc
 *****************************************************/
#define INXWARE_FB_pwm "pwm"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(pwm); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(pwm); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(pwm); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(pwm_config); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(pwm_enable); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(pwm_duty); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(pwm)

#endif /* INXWARE_rtc */
