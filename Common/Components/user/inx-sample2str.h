#ifndef INXWARE_sample2str
#define INXWARE_sample2str
#include "inx-component.h"

/*****************************************************
 * Storing real value samples in a string buffer.
 *****************************************************/
#define INXWARE_FB_ID_sample2str 0xE2FC
#define INXWARE_FB_NAME_sample2str "sample2str"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(sample2str); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(sample2str); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(sample2str); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(sample2str_Run_Input); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(sample2str_Clear); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(sample2str)

#endif /* INXWARE_sample2str */
