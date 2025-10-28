#ifndef INXWARE_dac
#define INXWARE_dac
#include "inx-component.h"

/*****************************************************
 * Digital-to-Analog Converter
 *****************************************************/
#define INXWARE_FB_ID_dac 0x6628
#define INXWARE_FB_NAME_dac "dac"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(dac); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(dac); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(dac); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(dac_set); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(dac)

#endif /* INXWARE_dac */
