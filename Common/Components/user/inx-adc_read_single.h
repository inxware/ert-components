#ifndef INXWARE_adc_read_single
#define INXWARE_adc_read_single
#include "inx-component.h"

/*****************************************************
 * Read ADC value when the sample port is triggered. It will output error when the ADC unit is not initialised or configured in a different mode
 *****************************************************/
#define INXWARE_FB_ID_adc_read_single 0xB63C
#define INXWARE_FB_NAME_adc_read_single "adc_read_single"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(adc_read_single); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(adc_read_single); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(adc_read_single); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(adc_read_single_sample); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(adc_read_single)

#endif /* INXWARE_adc_read_single */
