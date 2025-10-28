#ifndef INXWARE_Calibrate
#define INXWARE_Calibrate
#include "inx-component.h"

/*****************************************************
 * Calibrate
 *****************************************************/
#define INXWARE_FB_ID_Calibrate 0xAD13
#define INXWARE_FB_NAME_Calibrate "Calibrate"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(Calibrate); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(Calibrate); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(Calibrate); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(Calibrate_calibrate); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(Calibrate)

#endif /* INXWARE_Calibrate */
