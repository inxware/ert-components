#ifndef INXWARE_json_parser_bool
#define INXWARE_json_parser_bool
#include "inx-component.h"

/*****************************************************
 * Parse the boolean values from json string input or file.
 *****************************************************/
#define INXWARE_FB_ID_json_parser_bool 0xB7DB
#define INXWARE_FB_NAME_json_parser_bool "json_parser_bool"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(json_parser_bool); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(json_parser_bool); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(json_parser_bool); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(json_parser_bool_parse); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(json_parser_bool)

#endif /* INXWARE_json_parser_bool */
