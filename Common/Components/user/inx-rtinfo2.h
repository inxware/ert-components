#ifndef INXWARE_RuntimeInfo2
#define INXWARE_RuntimeInfo2
#include "inx-component.h"

/*****************************************************
 * Device ID: MAC address, Network Address: IP addr, SODL Type: SODL format version, SODL name: Name of application,    
 *****************************************************/
#define INXWARE_FB_ID_RuntimeInfo2 0x2E49
#define INXWARE_FB_NAME_RuntimeInfo2 "RuntimeInfo2"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(RuntimeInfo2); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(RuntimeInfo2); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(RuntimeInfo2); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(RuntimeInfo2_getstat); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(RuntimeInfo2_getdyn); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(RuntimeInfo2_reqpairid); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(RuntimeInfo2)

#endif /* INXWARE_RuntimeInfo2 */
