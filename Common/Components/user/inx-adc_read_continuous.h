#ifndef INXWARE_adc_read_continuous
#define INXWARE_adc_read_continuous
#include "inx-component.h"

/*****************************************************
 * Read value continuously as mean and mean squared values. The port and data are triggered only if the data is ready.
 *****************************************************/
#define INXWARE_FB_ID_adc_read_continuous 0xB20D
#define INXWARE_FB_NAME_adc_read_continuous "adc_read_continuous"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(adc_read_continuous); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(adc_read_continuous); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(adc_read_continuous); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(adc_read_continuous_enable); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(adc_read_continuous_disable); //a function executed by a start port
EHS_GLOBAL EHS_MEMORY_ATTRIB EHS_FB_RUN_FUNCTION(adc_read_continuous_convert); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(adc_read_continuous_set_bias); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(adc_read_continuous)

#endif /* INXWARE_adc_read_continuous */
