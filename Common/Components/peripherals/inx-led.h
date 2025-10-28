#ifndef INXWARE_led
#define INXWARE_led
#include "inx-component.h"

/*****************************************************
 * LED
 *****************************************************/
#define INXWARE_FB_ID_led 0xA6EA
#define INXWARE_FB_NAME_led "led"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(led); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(led); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(led); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(led_EnableFunc); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(led_DisableFunc); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(led_SetFunc); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(led)

#endif /* INXWARE_led */
