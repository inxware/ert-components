#ifndef INXWARE_TFLM
#define INXWARE_TFLM
#include "inx-component.h"

/*****************************************************
 * Run machine learning models with limited resources like a few kilobytes of memory.
 *****************************************************/
#define INXWARE_FB_ID_TFLM 0x1005
#define INXWARE_FB_NAME_TFLM "TFLM"

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(TFLM);         // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(TFLM);             // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(TFLM);          // this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(TFLM_load_model);   // a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(TFLM_do_inference); // a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(TFLM)

#endif /* INXWARE_TFLM */
