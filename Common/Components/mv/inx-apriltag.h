#ifndef INXWARE_apriltag
#define INXWARE_apriltag
#include "inx-component.h"

/*****************************************************
 * Detect all apriltags with their IDs and coordinates of 4 corners in the image.
 *****************************************************/
#define INXWARE_FB_ID_apriltag 0xC8D5
#define INXWARE_FB_NAME_apriltag "apriltag"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(apriltag); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(apriltag); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(apriltag); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(apriltag_config); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(apriltag_detect); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(apriltag)

#endif /* INXWARE_apriltag */
