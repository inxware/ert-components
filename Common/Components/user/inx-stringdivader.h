#ifndef INXWARE_StringDivader
#define INXWARE_StringDivader
#include "inx-component.h"

/*****************************************************
 * String Divader
 *****************************************************/
#define INXWARE_FB_ID_StringDivader 0xC795
#define INXWARE_FB_NAME_StringDivader "StringDivader"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(StringDivader); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(StringDivader); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(StringDivader); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(StringDivader_split); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(StringDivader)

#endif /* INXWARE_StringDivader */
