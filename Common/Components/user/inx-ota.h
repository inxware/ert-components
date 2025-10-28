#ifndef INXWARE_OTA
#define INXWARE_OTA
#include "inx-component.h"

/*****************************************************
 * This will write to the inactive OTA section with the incoming data. The data could be written in the non-contiguous manner with offset.
 *****************************************************/
#define INXWARE_FB_ID_OTA 0x27CF
#define INXWARE_FB_NAME_OTA "OTA"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(OTA); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(OTA); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(OTA); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(OTA_begin); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(OTA_write); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(OTA_end); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(OTA_abort); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(OTA_write_cb); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(OTA)

#endif /* INXWARE_OTA */
