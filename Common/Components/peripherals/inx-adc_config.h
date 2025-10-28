#ifndef INXWARE_adc_config
#define INXWARE_adc_config
#include "inx-component.h"

/*****************************************************
 * Configure the ADC unit either from parameters or JSON string
 *****************************************************/
#define INXWARE_FB_ID_adc_config 0x566F
#define INXWARE_FB_NAME_adc_config "adc_config"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(adc_config); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(adc_config); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(adc_config); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(adc_config_config); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(adc_config)

#endif /* INXWARE_adc_config */
