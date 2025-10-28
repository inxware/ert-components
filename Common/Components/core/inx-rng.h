#ifndef INXWARE_rng
#define INXWARE_rng
#include "inx-component.h"

/*****************************************************
 * random number generator
 *****************************************************/
#define INXWARE_FB_ID_rng 0x91CD
#define INXWARE_FB_NAME_rng "rng"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(rng); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(rng); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(rng); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(rng_run); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(rng)

#endif /* INXWARE_rng */
