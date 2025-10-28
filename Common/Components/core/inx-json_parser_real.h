#ifndef INXWARE_json_parser_real
#define INXWARE_json_parser_real
#include "inx-component.h"

/*****************************************************
 * Parse the float numbers from json string input or file.
 *****************************************************/
#define INXWARE_FB_ID_json_parser_real 0x15FB
#define INXWARE_FB_NAME_json_parser_real "json_parser_real"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(json_parser_real); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(json_parser_real); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(json_parser_real); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(json_parser_real_parse); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(json_parser_real)

#endif /* INXWARE_json_parser_real */
