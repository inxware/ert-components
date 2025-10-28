#ifndef INXWARE_json_parser_str
#define INXWARE_json_parser_str
#include "inx-component.h"

/*****************************************************
 * Parse the string value from json string input or file.
 *****************************************************/
#define INXWARE_FB_ID_json_parser_str 0xE643
#define INXWARE_FB_NAME_json_parser_str "json_parser_str"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(json_parser_str); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(json_parser_str); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(json_parser_str); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(json_parser_str_parse); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(json_parser_str)

#endif /* INXWARE_json_parser_str */
