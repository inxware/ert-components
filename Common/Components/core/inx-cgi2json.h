#ifndef INXWARE_cgi2json
#define INXWARE_cgi2json
#include "inx-component.h"

/*****************************************************
 * Converts CGI (Common Gateway Interface) string to JSON string
 *****************************************************/
#define INXWARE_FB_ID_cgi2json 0x1920
#define INXWARE_FB_NAME_cgi2json "cgi2json"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(cgi2json); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(cgi2json); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(cgi2json); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(cgi2json_convert); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(cgi2json)

#endif /* INXWARE_cgi2json */
