#ifndef INXWARE_JSONObject
#define INXWARE_JSONObject
#include "inx-component.h"

/*****************************************************
 * Holds a memory representation of a JSON object for querying and building.
 *****************************************************/
#define INXWARE_FB_JSONObject "JSONObject"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(JSONObject); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(JSONObject); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(JSONObject); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(JSONObject_reset); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(JSONObject_read); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(JSONObject_query); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(JSONObject)

#endif /* INXWARE_JSONObject */
